#pragma once

#include <map>
#include <stack>

#include <cxxabi.h>
#include <execinfo.h>

#include "CommonUtilities.hpp"

#include "SystemMacros.hpp"
#include "SystemConstants.hpp"

#include "Exception.hpp"
#include "CallStackFrame.hpp"

namespace SystemUtilities {

static std::map<std::string, std::stack<__jmp_buf_tag>> global_jump_buffer {};
static std::mutex global_jump_mutex {};
static std::map<std::string, std::string> global_thread_name {};
static std::mutex global_thread_mutex {};
static std::map<std::string, NSTD::Exception::VectorCallStackFrame> global_thread_call_stack {};

static std::string full_program_path {};

int init( const char *thread_name, const int argc, const char *argv[] ) {

  SystemUtilities::global_thread_mutex.lock();

  if ( SystemUtilities::global_thread_name[ CommonUtilities::get_thread_id() ] == "" ) {

    SystemUtilities::global_thread_name[ CommonUtilities::get_thread_id() ] = thread_name;

  }

  if ( strcmp( thread_name, "thread_main" ) == 0 ) {

    full_program_path = argv[ 0 ];

  }

  SystemUtilities::global_thread_mutex.unlock();

  return 0;

}

int finish( const char *thread_name, const int argc, const char *argv[] ) {

  //

  return 0;

}

inline const NSTD::CallStackFrame parse_addr_to_line_out( std::string address_to_line_out,
                                                          std::string symbol_file,
                                                          std::string symbol_offset ) noexcept {

  NSTD::CallStackFrame result;

  int index_at = address_to_line_out.find( " at " );

  int intex_at_offset = 0;

  std::string mangled_symbol_name { "??" };

  if ( index_at != -1 ) {

    intex_at_offset = 4;

    mangled_symbol_name = address_to_line_out.substr( 0, index_at );

  }
  else {

    index_at = address_to_line_out.find( " " );

    if ( index_at != -1 ) {

      intex_at_offset = 1;

      mangled_symbol_name = address_to_line_out.substr( 0, index_at );

    }

  }

  const int end_line = address_to_line_out.find( "\n" );

  std::string symbol_source_location = address_to_line_out.substr( index_at + intex_at_offset, end_line - ( index_at + intex_at_offset ) );

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
  char* demangled_symbol_name = abi::__cxa_demangle( mangled_symbol_name.c_str(), nullptr, nullptr, &status );

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
    demangled_symbol_name_str.c_str(),
    source_file.c_str(),
    source_line.c_str()
  };

  free( demangled_symbol_name );

  return result;

}

NSTD::Vector<NSTD::CallStackFrame> get_call_stack_entries( int stack_offset )
{

  NSTD::Vector<NSTD::CallStackFrame> result; // = new NSTD::Vector<NSTD::CallStackFrame>();

  void *memory_stack_frames[ MAX_STACK_FRAMES ];

  int trace_size = 0;
  char **messages = nullptr;

  trace_size = backtrace( memory_stack_frames, MAX_STACK_FRAMES ); //get the void pointers for all of the entries. MEMORY RAM ADDRESS ONLY!!!
  messages = backtrace_symbols( memory_stack_frames, trace_size ); //Get stack trace relative to current symbols in file disk (function symbols)

  result.reserve( trace_size );

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

        const std::string addr2line_out = CommonUtilities::addr_to_line( symbol_file.c_str(), symbol_offset.c_str() );

        const NSTD::CallStackFrame call_stack_entry = parse_addr_to_line_out( addr2line_out,
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

        std::string addr2line_out = CommonUtilities::addr_to_line( symbol_file.c_str(), symbol_offset.c_str() );

        const NSTD::CallStackFrame call_stack_frame = parse_addr_to_line_out( addr2line_out,
                                                                           symbol_file,
                                                                          symbol_offset );

        result.push_back( call_stack_frame );

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

[[noreturn]] void catch_signal( int signal = 0, siginfo_t *signalInfo = nullptr, void *extra = nullptr )
{

  // ********** Very Important ************
  // Unblock the signal to allow to OS send again
  sigset_t sigs;

  memset( &sigs, 0 , sizeof( sigset_t ) );

  sigemptyset( &sigs );
  sigaddset( &sigs, signal );
  sigprocmask( SIG_UNBLOCK, &sigs, NULL );
  // ********** Very Important ************

  SystemUtilities::global_jump_mutex.lock();

  const std::string thread_id = CommonUtilities::get_thread_id();

  //{

  // NSTD::Vector<NSTD::CallStackFrame> call_stack_entries = SystemUtilities::get_call_stack_entries( 3 );

  // SystemUtilities::global_thread_call_stack[ thread_id ] = call_stack_entries;

  SystemUtilities::global_thread_call_stack[ thread_id ] = SystemUtilities::get_call_stack_entries( 3 );

  //}

  jmp_buf local_jump_buffer = { SystemUtilities::global_jump_buffer[ thread_id ].top() };

  SystemUtilities::global_jump_buffer[ thread_id ].pop();

  //call_stack_entries.clear();

  SystemUtilities::global_jump_mutex.unlock();

  longjmp               // Jump back into the normal flow of the program
  (
    local_jump_buffer,  // using this context to say where to jump to
    signal              // and passing back the value of the signal.
  );

}

}
