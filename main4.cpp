#include <iostream>
#include <execinfo.h>
#include <signal.h>
#include <cstdlib>
#include <unistd.h>
#include <memory>
#include <cxxabi.h>
#include <vector>
//#include <array>
#include "Vector.hpp"

using namespace std;

static char const *global_program_name;

std::string exec( const char* cmd )
{

  std::string result {};

  std::array<char, 256> buffer;

  std::unique_ptr<FILE, decltype(&pclose)> pipe( popen(cmd, "r"), pclose );

  if ( !pipe )
  {

    throw std::runtime_error( "popen() failed!" );

  }

  while ( fgets( buffer.data(), buffer.size(), pipe.get() ) != nullptr )
  {

    result += buffer.data();

  }

  return result;

}


/* Resolve symbol name and source location given the path to the executable
   and an address */
std::string addr2line( const char *program_name, const char *addr )
{

  char addr2line_cmd[ 512 ] = { 0 };

  /* have addr2line map the address to the relent line in the code */
  #ifdef __APPLE__
    /* apple does things differently... */
    sprintf(addr2line_cmd,"atos -o %.256s %p", program_name, addr);
  #else
    sprintf(addr2line_cmd,"/usr/bin/addr2line -f -p -e \"%.256s\" \"%s\"", program_name, addr );
  #endif

  return exec( addr2line_cmd );

}

//static void *stack_traces[MAX_STACK_FRAMES];

struct CallStackFrame {

  std::string function;
  std::string file;
  std::string line;

};

const CallStackFrame parse_addr2Line_out( std::string addr2line_out,
                                          std::string symbol_file,
                                          std::string symbol_offset ) noexcept {

  CallStackFrame result;

  int index_at = addr2line_out.find( " at " );

  int intex_at_offset = 0;

  std::string mangled_symbol_name { "??" };

  if ( index_at != -1 ) {

    intex_at_offset = 4;

    mangled_symbol_name = addr2line_out.substr( 0, index_at );

  }
  else {

    index_at = addr2line_out.find( " " );

    if ( index_at != -1 ) {

      intex_at_offset = 1;

      mangled_symbol_name = addr2line_out.substr( 0, index_at );

    }

  }

  const int end_line = addr2line_out.find( "\n" );

  std::string symbol_source_location = addr2line_out.substr( index_at + intex_at_offset, end_line - ( index_at + intex_at_offset ) );

  const int semicolon = symbol_source_location.find( ":" );

  std::string source_file = symbol_source_location.substr( 0, semicolon );

  if ( source_file == "??" )
  {

    source_file = symbol_file;

  }

  std::string source_line = symbol_source_location.substr( semicolon + 1 );

  if ( source_line == "?" ||
       source_line == "0" )
  {

    source_line = symbol_offset;

  }

  int status;
  char* demangled_symbol_name = abi::__cxa_demangle(mangled_symbol_name.c_str(), nullptr, nullptr, &status );

  std::string demangled_symbol_name_str; // = { demangled_symbol_name_str };

  if ( demangled_symbol_name )
  {

    demangled_symbol_name_str = demangled_symbol_name;

  }
  else if ( mangled_symbol_name != "??" ) {

    demangled_symbol_name_str = mangled_symbol_name;

  }
  else {

    demangled_symbol_name_str = "unknown_symbol[" + mangled_symbol_name + "]";

  }

  result =
  {
    demangled_symbol_name_str,
    source_file,
    source_line
  };

  free( demangled_symbol_name );

  return result;

}

#define MAX_STACK_FRAMES 64

const NSTD::Vector<CallStackFrame> get_call_stack_entries( int stack_offset )
{

  NSTD::Vector<CallStackFrame> result;

  void *memory_stack_frames[ MAX_STACK_FRAMES ];

  int trace_size = 0;
  char **messages = nullptr;

  trace_size = backtrace( memory_stack_frames, MAX_STACK_FRAMES ); //get the void pointers for all of the entries. MEMORY RAM ADDRESS ONLY!!!
  messages = backtrace_symbols( memory_stack_frames, trace_size ); //Get stack trace relative to current symbols in file disk (function symbols)

  for ( int trace_index = stack_offset; trace_index < trace_size; ++trace_index )
  {

    std::string symbol_line = messages[ trace_index ];

    #ifdef __clang__

      //std::cout << "clang++:: => " << messages[ trace_index ] << std::endl;

      const int left_symbol_offset = symbol_line.find( " [" );

      if ( left_symbol_offset != -1 )
      {

        std::string symbol_file = symbol_line.substr( 0, left_symbol_offset );

        const int left_symbol_internal_offset = symbol_file.find( "(" );

        const int right_symbol_internal_offset = symbol_file.find( ")" );

        std::string symbol_internal_offset {};

        if ( left_symbol_internal_offset != -1 &&
             right_symbol_internal_offset != -1 ) {

          symbol_internal_offset = symbol_file.substr( left_symbol_internal_offset,
                                                       right_symbol_internal_offset - left_symbol_internal_offset + 1 );

          symbol_file = symbol_file.substr( 0, left_symbol_internal_offset );

          const int left_symbol_internal_offset_plus = symbol_internal_offset.find( "+" );

          if ( left_symbol_internal_offset_plus != -1 ) {

            const int right_symbol_internal_offset_plus = symbol_internal_offset.find( ")" );

            symbol_internal_offset = symbol_internal_offset.substr( left_symbol_internal_offset_plus,
                                                                    right_symbol_internal_offset_plus - left_symbol_internal_offset_plus );

          }
          else {

            symbol_internal_offset = "";

          }

        }

        std::string symbol_offset {};

        if ( symbol_internal_offset.length() == 0 ) {

          const int right_symbol_offset = symbol_line.find( "]" );

          symbol_offset = symbol_line.substr( left_symbol_offset + 2, right_symbol_offset - ( left_symbol_offset + 2 ) );

        }
        else {

          symbol_offset = symbol_internal_offset;

        }

        //std::cout << "clang++:: => " << symbol_offset << std::endl;

        const std::string addr2line_out = addr2line( symbol_file.c_str(), symbol_offset.c_str() );

        const CallStackFrame call_stack_entry = parse_addr2Line_out( addr2line_out,
                                                                     symbol_file,
                                                                     symbol_offset );

        result.push_back( call_stack_entry );

        //std::cout << call_stack_entry.function << " => " << call_stack_entry.file << ":" << call_stack_entry.line << std::endl;

      }

    //#endif

    #else //__GNUG__

      const int left_symbol_offset = symbol_line.find( "(+" );

      if ( left_symbol_offset != -1 )
      {

        const std::string symbol_file = symbol_line.substr( 0, left_symbol_offset );

        const int right_symbol_offset = symbol_line.find( ") [" );

        const std::string symbol_offset = symbol_line.substr( left_symbol_offset + 2, right_symbol_offset - ( left_symbol_offset + 2 ) );

        std::string addr2line_out = addr2line( symbol_file.c_str(), symbol_offset.c_str() );

        const CallStackEntry call_stack_entry = parse_addr2Line_out( addr2line_out,
                                                                     symbol_file,
                                                                     symbol_offset );

        result.push_back( call_stack_entry );

        // const int index_at = addr2line_out.find( " at " );

        // std::string mangled_symbol_name = addr2line_out.substr( 0, index_at );

        // const int end_line = addr2line_out.find( "\n" );

        // std::string symbol_source_location = addr2line_out.substr( index_at + 4, end_line - ( index_at + 4 ) );

        // const int semicolon = symbol_source_location.find( ":" );

        // std::string source_file = symbol_source_location.substr( 0, semicolon );

        // if ( source_file == "??" )
        // {

        //   source_file = symbol_file;

        // }

        // std::string source_line = symbol_source_location.substr( semicolon + 1 );

        // if ( source_line == "?" )
        // {

        //   source_line = symbol_offset;

        // }

        // int status;
        // char* demangled_symbol_name = abi::__cxa_demangle(mangled_symbol_name.c_str(), nullptr, nullptr, &status );

        // std::string demangled_symbol_name_str; // = { demangled_symbol_name_str };

        // if ( demangled_symbol_name )
        // {

        //   demangled_symbol_name_str = demangled_symbol_name;

        // }
        // else {

        //   demangled_symbol_name_str = mangled_symbol_name;

        // }

        // CallStackEntry call_stack_entry {
        //   demangled_symbol_name_str,
        //   source_file,
        //   source_line
        // };

        // //CallStackEntry call_stack_entry;

        // //call_stack_entries.push_back( 10 );
        // result.push_back( call_stack_entry );

        // free( demangled_symbol_name );

      }

    #endif

  } // we'll use this for now so you can see what's going on

  if ( messages )
  {

    free( messages );

  }

  //result2 = { result;

  return result;

}

void error_handler(int sig)
{

  { //To avoid memory leak, and force to c++ clean before exit the process. checked with valgrind

    const NSTD::Vector<CallStackFrame> call_stack_entries = get_call_stack_entries( 3 );

    for ( CallStackFrame call_stack_entry: call_stack_entries ) {

      std::cout << call_stack_entry.function << " => " << call_stack_entry.file << ":" << call_stack_entry.line << std::endl;
      //std::cout << call_stack_entry << std::endl;

    }

  }

  //  void *memory_stack_traces[MAX_STACK_FRAMES];

  //  int trace_size = 0;
  //  char **messages = nullptr;

  //  trace_size = backtrace( memory_stack_traces, MAX_STACK_FRAMES ); //get the void pointers for all of the entries. MEMORY RAM ADDRESS
  //  messages = backtrace_symbols( memory_stack_traces, trace_size ); //Get stack trace relative to current symbols in file disk (function symbols)

  //  {

  //  std::vector<int> call_stack_entries;
  //  //call_stack_entries.reserve(20);
  //  //Vector<CallStackEntry> call_stack_entries;
  //  //std::vector<CallStackEntry> call_stack_entries;
  //  //std::array<CallStackEntry,trace_size> call_stack_entries;

  //  int i = 0;

  //  for (i = 0; i < trace_size; ++i)
  //  {

  //     #ifdef __clang__

  //       std::cout << "clang++:: => " << messages[ i ] << std::endl;

  //     #else

  //       std::cout << "g++:: => " << messages[ i ] << std::endl;

  //     #endif


  //     //const char *m = messages[ i ];
  //     std::string symbol_line = messages[ i ];

  //     int left_symbol_offset = symbol_line.find( "(+" );

  //     if ( left_symbol_offset != -1 ) {

  //       const std::string symbol_file = symbol_line.substr( 0, left_symbol_offset );

  //       const int right_symbol_offset = symbol_line.find( ") [" );

  //       const std::string symbol_offset = symbol_line.substr( left_symbol_offset + 2, right_symbol_offset - ( left_symbol_offset + 2 ) );

  //       std::string addr2line_out = addr2line( symbol_file.c_str(), symbol_offset.c_str() );

  //       const int index_at = addr2line_out.find( " at " );

  //       std::string mangled_symbol_name = addr2line_out.substr( 0, index_at );

  //       const int end_line = addr2line_out.find( "\n" );

  //       std::string symbol_source_location = addr2line_out.substr( index_at + 4, end_line - ( index_at + 4 ) );

  //       const int semicolon = symbol_source_location.find( ":" );

  //       std::string source_file = symbol_source_location.substr( 0, semicolon );

  //       if ( source_file == "??" )
  //       {

  //           source_file = symbol_file;

  //       }

  //       std::string source_line = symbol_source_location.substr( semicolon + 1 );

  //       if ( source_line == "?" )
  //       {

  //           source_line = symbol_offset;

  //       }

  //       int status;
  //       char* demangled_symbol_name = abi::__cxa_demangle(mangled_symbol_name.c_str(), nullptr, nullptr, &status );

  //       std::string demangled_symbol_name_str; // = { demangled_symbol_name_str };

  //       if ( demangled_symbol_name )
  //       {

  //           demangled_symbol_name_str = demangled_symbol_name;

  //       }
  //       else {

  //           demangled_symbol_name_str = mangled_symbol_name;

  //       }

  //       const CallStackEntry call_stack_entry {
  //           demangled_symbol_name_str,
  //           source_file,
  //           source_line
  //       };

  //       //CallStackEntry call_stack_entry;

  //       call_stack_entries.push_back( 10 );
  //       //call_stack_entries.push_back( call_stack_entry );

  //       free( demangled_symbol_name );

  //     }

  //  } // we'll use this for now so you can see what's going on

  //  for ( int call_stack_entry: call_stack_entries ) {
  //  //for ( CallStackEntry call_stack_entry: call_stack_entries ) {

  //    //std::cout << call_stack_entry.function << " => " << call_stack_entry.file << ":" << call_stack_entry.line << std::endl;
  //    std::cout << call_stack_entry << std::endl;

  //  }

  //  }

  //  if ( messages )
  //  {

  //    free( messages );

  //  }

   //std::terminate();
  exit( 0 );

}

void invalid_index() {

  int *ptr = (int*) 0;

  std::string s = "Message";

  cout << *ptr << endl; // segmentation error

  std::cout << s << std::endl;

}

void func1() {

  invalid_index();

}

void func2() {

  func1();

  func1();

}

int main(int argc, char **argv) {

  global_program_name = argv[0];

  signal(SIGSEGV, error_handler); // use handler to print the errors

  func2(); // this will call all other function to generate error

  func2();

}


