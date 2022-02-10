#include <iostream>
#include <csignal>
#include <csetjmp>
#include <stack>
#include <map>
#include <thread>
#include <chrono>
#include <mutex>
#include <syncstream>
#include <iomanip>
#include <malloc.h>
#include <vector>

// #include <stdio.h>
// #include <stdlib.h>
// #include <signal.h>
// #include <execinfo.h>
// #include <unistd.h>
// #include <string.h>

// #include <cassert>
// #include <cxxabi.h> // __cxa_demangle
// #include <elfutils/libdwfl.h> // Dwfl*
// #include <execinfo.h> // backtrace
// #include <unistd.h> // getpid

//#define BACKWARD_HAS_DW 1
//#define BACKWARD_HAS_BFD 1
//#define BACKWARD_HAS_DWARF 1
//#define BACKWARD_HAS_BACKTRACE_SYMBOL 1
//#define BACKWARD_HAS_UNWIND 1
#define BACKWARD_HAS_LIBUNWIND 1

#include "backward.hpp"

#include "test01.hpp"

#include "Vector.hpp"

#include "CommonUtilities.hpp"

#include "CallStackFrame.hpp"

struct Exception
{

  std::string thread_id;
  std::string thread_name;
  int signal;
  std::string code;
  std::string message;
  std::string at;
  KISS::Vector<KISS::CallStackFrame> *call_stack;

};

static std::map<std::string, std::stack<__jmp_buf_tag>> global_jump_buffer;
static std::mutex global_jump_mutex;
static std::map<std::string, std::string> global_thread_name;
static std::mutex global_thread_mutex;
//static std::map<std::string, KISS::Vector<KISS::CallStackFrame>> global_thread_call_stack;
static std::map<std::string, KISS::Vector<KISS::CallStackFrame> *> global_thread_call_stack;

static const char * global_program_name;

inline const KISS::CallStackFrame parse_addr2Line_out( std::string addr2line_out,
                                                       std::string symbol_file,
                                                       std::string symbol_offset ) noexcept {

  KISS::CallStackFrame result;

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
    demangled_symbol_name_str.c_str(),
    source_file.c_str(),
    source_line.c_str()
  };

  free( demangled_symbol_name );

  return result;

}

#define MAX_STACK_FRAMES 64

KISS::Vector<KISS::CallStackFrame>* get_call_stack_entries( int stack_offset )
{

  KISS::Vector<KISS::CallStackFrame> *result = new KISS::Vector<KISS::CallStackFrame>();

  void *memory_stack_frames[ MAX_STACK_FRAMES ];

  int trace_size = 0;
  char **messages = nullptr;

  trace_size = backtrace( memory_stack_frames, MAX_STACK_FRAMES ); //get the void pointers for all of the entries. MEMORY RAM ADDRESS ONLY!!!
  messages = backtrace_symbols( memory_stack_frames, trace_size ); //Get stack trace relative to current symbols in file disk (function symbols)

  result->reserve( trace_size );

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

        const KISS::CallStackFrame call_stack_entry = parse_addr2Line_out( addr2line_out,
                                                                           symbol_file,
                                                                           symbol_offset );

        result->push_back( call_stack_entry );

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

// // https://stackoverflow.com/questions/281818/unmangling-the-result-of-stdtype-infoname
// std::string demangle(const char* name) {
//     int status = -4;
//     std::unique_ptr<char, void(*)(void*)> res {
//         abi::__cxa_demangle(name, NULL, NULL, &status),
//         std::free
//     };
//     return (status==0) ? res.get() : name ;
// }

// std::string debug_info(Dwfl* dwfl, void* ip) {
//     std::string function;
//     int line = -1;
//     char const* file;
//     uintptr_t ip2 = reinterpret_cast<uintptr_t>(ip);
//     Dwfl_Module* module = dwfl_addrmodule(dwfl, ip2);
//     char const* name = dwfl_module_addrname(module, ip2);
//     function = name ? demangle(name) : "<unknown>";
//     if (Dwfl_Line* dwfl_line = dwfl_module_getsrc(module, ip2)) {
//         Dwarf_Addr addr;
//         file = dwfl_lineinfo(dwfl_line, &addr, &line, nullptr, nullptr, nullptr);
//     }
//     std::stringstream ss;
//     ss << ip << ' ' << function;
//     if (file)
//         ss << " at " << file << ':' << line;
//     ss << std::endl;
//     return ss.str();
// }

// std::string get_stack_trace() {
//     // Initialize Dwfl.
//     Dwfl* dwfl = nullptr;
//     {
//         Dwfl_Callbacks callbacks = {};
//         char* debuginfo_path = nullptr;
//         callbacks.find_elf = dwfl_linux_proc_find_elf;
//         callbacks.find_debuginfo = dwfl_standard_find_debuginfo;
//         callbacks.debuginfo_path = &debuginfo_path;
//         dwfl = dwfl_begin(&callbacks);
//         assert(dwfl);
//         int r;
//         r = dwfl_linux_proc_report(dwfl, getpid());
//         assert(!r);
//         r = dwfl_report_end(dwfl, nullptr, nullptr);
//         assert(!r);
//         static_cast<void>(r);
//     }

//     // Loop over stack frames.
//     std::stringstream ss;
//     {
//         void* stack[512];
//         int stack_size = ::backtrace(stack, sizeof stack / sizeof *stack);
//         for (int i = 0; i < stack_size; ++i) {
//             ss << i << ": ";

//             // Works.
//             ss << debug_info(dwfl, stack[i]);

// #if 0
//             // TODO intended to do the same as above, but segfaults,
//             // so possibly UB In above function that does not blow up by chance?
//             void *ip = stack[i];
//             std::string function;
//             int line = -1;
//             char const* file;
//             uintptr_t ip2 = reinterpret_cast<uintptr_t>(ip);
//             Dwfl_Module* module = dwfl_addrmodule(dwfl, ip2);
//             char const* name = dwfl_module_addrname(module, ip2);
//             function = name ? demangle(name) : "<unknown>";
//             // TODO if I comment out this line it does not blow up anymore.
//             if (Dwfl_Line* dwfl_line = dwfl_module_getsrc(module, ip2)) {
//               Dwarf_Addr addr;
//               file = dwfl_lineinfo(dwfl_line, &addr, &line, nullptr, nullptr, nullptr);
//             }
//             ss << ip << ' ' << function;
//             if (file)
//                 ss << " at " << file << ':' << line;
//             ss << std::endl;
// #endif
//         }
//     }
//     dwfl_end(dwfl);
//     return ss.str();
// }


// void print_stacktrace() {

//   unsigned int stack_depth = 16;

//   void* callstack[stack_depth];
//   int frames = backtrace(callstack, stack_depth);
//   char** strs = backtrace_symbols(callstack, frames);
//   for(int i=0; i<frames; i++) {
//     puts(strs[i]);
//   }

//   free(strs);

// }

// int addr2line(char const * const program_name, void const * const addr)
// {
//   char addr2line_cmd[512] = {0};

//   /* have addr2line map the address to the relent line in the code */
//   #ifdef __APPLE__
//     /* apple does things differently... */
//     sprintf(addr2line_cmd,"atos -o %.256s %p", program_name, addr);
//   #else
//     sprintf(addr2line_cmd,"addr2line -f -p -e %.256s %p", program_name, addr);
//   #endif

//   /* This will print a nicely formatted string specifying the
//      function and source line of the address */
//   return system(addr2line_cmd);
// }

// void unwind_print_stack_trace() {

//   unw_cursor_t cursor;
//   unw_context_t context;

//   // Initialize cursor to current frame for local unwinding.
//   unw_getcontext(&context);
//   unw_init_local(&cursor, &context);

//   // Unwind frames one by one, going up the frame stack.
//   while (unw_step(&cursor) > 0) {

//     unw_word_t offset, pc;
//     unw_get_reg(&cursor, UNW_REG_IP, &pc);
//     if (pc == 0) {
//       break;
//     }
//     std::printf("0x%lx:", pc);

//     char sym[256];
//     if (unw_get_proc_name(&cursor, sym, sizeof(sym), &offset) == 0) {

//       char* nameptr = sym;
//       int status;
//       char* demangled = abi::__cxa_demangle(sym, nullptr, nullptr, &status);
//       if (status == 0) {
//         nameptr = demangled;
//       }
//       std::printf(" (%s+0x%lx)\n", nameptr, offset);
//       std::free(demangled);

//     }
//     else {

//       std::printf(" -- error: unable to obtain symbol name for this frame\n");

//     }

//   }

// }

// #define MAX_STACK_FRAMES 64

// static void *stack_traces[MAX_STACK_FRAMES];

// void posix_print_stack_trace()
// {
//   int i, trace_size = 0;
//   char **messages = (char **)NULL;

//   trace_size = backtrace(stack_traces, MAX_STACK_FRAMES);
//   messages = backtrace_symbols(stack_traces, trace_size);

//   /* skip the first couple stack frames (as they are this function and
//      our handler) and also skip the last frame as it's (always?) junk. */
//   // for (i = 3; i < (trace_size - 1); ++i)
//   // we'll use this for now so you can see what's going on
//   for (i = 0; i < trace_size; ++i)
//   {
//     if (addr2line(global_program_name, stack_traces[i]) != 0)
//     {
//       printf("  error determining line # for: %s\n", messages[i]);
//     }

//   }

//   if (messages) { free(messages); }

// }

void catch_signal( int signal, siginfo_t *signalInfo, void *extra )
{

  // {

  //   ucontext_t *ucontext = (ucontext_t *) extra;

  //   //const int x = ucontext->uc_mcontext.gregs[ REG_RIP ];

  //   backward::StackTrace st;
  //   void *error_addr = nullptr;

  //   #ifdef REG_RIP // x86_64
  //     error_addr = reinterpret_cast<void *>( ucontext->uc_mcontext.gregs[REG_RIP] );
  //   #endif

  //   if ( error_addr )
  //   {

  //     st.load_from( error_addr,
  //                   32,
  //                   reinterpret_cast<void *>( ucontext ),
  //                   signalInfo->si_addr );

  //   }
  //   else
  //   {

  //     st.load_here( 32,
  //                   reinterpret_cast<void *>( ucontext ),
  //                   signalInfo->si_addr );

  //   }

  //   std::osyncstream sync_out( std::cout );

  //   sync_out << "cath_signal => Thread: " << global_thread_name[ get_thread_id() ] << " (" << get_thread_id() << ")\n";

  //   backward::Printer printer;
  //   printer.address = true;
  //   //printer.
  //   printer.print( st, sync_out );

  // }


  // ********** Very Important ************
  // Unblock the signal to allow to OS send again
  sigset_t sigs;
  sigemptyset( &sigs );
  sigaddset( &sigs, signal );
  sigprocmask( SIG_UNBLOCK, &sigs, NULL );
  // ********** Very Important ************

  //signal(SIGINT, SIG_DFL);          // Switch to default handling
  //signal(SIGINT, catch_signal);       // Reactivate this handler.
  //signal( SIGSEGV, catchSignal );

  //jmp_buf localGBuffer;

  global_jump_mutex.lock();

  const std::string thread_id = CommonUtilities::get_thread_id();

  {

    KISS::Vector<KISS::CallStackFrame> *call_stack_entries = get_call_stack_entries( 3 );
    //KISS::Vector<std::string> *call_stack_entries = new KISS::Vector<std::string>(); // = get_call_stack_entries( 3 );

    // for ( const KISS::CallStackFrame &call_stack_entry: call_stack_entries ) {

    //   //call_stack_entry.~CallStackFrame();

    //   std::osyncstream sync_out( std::cout );

    //   sync_out << call_stack_entry;


    // }

    global_thread_call_stack[ thread_id ] = call_stack_entries;

    // for ( const KISS::CallStackFrame &call_stack_entry: call_stack_entries ) {

    //   call_stack_entry.~CallStackFrame();

    // }

  }


  //global_thread_call_stack[ thread_id ] = call_stack_entries;

  //global_thread_call_stack[ thread_id ] = call_stack_entries;

  // std::osyncstream sync_out( std::cout );

  // for ( CallStackEntry call_stack_entry: call_stack_entries ) {

  //   std::cout << call_stack_entry.function << " => " << call_stack_entry.file << ":" << call_stack_entry.line << std::endl;
  //   //std::cout << call_stack_entry << std::endl;

  // }

  jmp_buf local_jump_buffer = { global_jump_buffer[ thread_id ].top() };

  global_jump_buffer[ thread_id ].pop();

  //call_stack_entries.clear();

  global_jump_mutex.unlock();

  longjmp           // Jump back into the normal flow of the program
  (
    local_jump_buffer,        // using this context to say where to jump to
    signal    // and passing back the value of the signal.
  );

}

void begin_map_posix_signal_to_cpp_exception()
{

  jmp_buf local_jump_buffer;

  const int signal = setjmp( local_jump_buffer );

  if ( signal == 0 )
  {

    const std::string thread_id = CommonUtilities::get_thread_id();

    //sync_out << "Signal number: " << signalNumber << std::endl;
    global_jump_buffer[ thread_id ].push( local_jump_buffer[ 0 ] );

    // sync_out << "localGBuffer[ 0 ].__mask_was_saved => " << localGBuffer[ 0 ].__mask_was_saved << std::endl;
    // sync_out << "gBuffer.top() => " << gBuffer.top().__mask_was_saved << std::endl;
    // gBuffer.top().__mask_was_saved = 10;
    // sync_out << "localGBuffer[ 0 ].__mask_was_saved => " << localGBuffer[ 0 ].__mask_was_saved << std::endl;
    // sync_out << "gBuffer.top() => " << gBuffer.top().__mask_was_saved << std::endl;

  }
  else if ( signal )
  {

    Exception exceptionToThrow;

    exceptionToThrow.thread_id = CommonUtilities::get_thread_id();
    exceptionToThrow.thread_name = global_thread_name[ exceptionToThrow.thread_id ];
    exceptionToThrow.at = CommonUtilities::get_local_date_time( _DATETIME_FORMAT_02 );
    exceptionToThrow.call_stack = global_thread_call_stack[ exceptionToThrow.thread_id ];
    //global_thread_call_stack[ exceptionToThrow.thread_id ].clear();

    if ( signal == SIGILL )
    {

      exceptionToThrow.signal = signal;
      exceptionToThrow.code = "ILLEGAL_INSTRUCTION";
      exceptionToThrow.message = "Illegal Instruction";

    }
    else if ( signal == SIGFPE )
    {

      exceptionToThrow.signal = signal;
      exceptionToThrow.code = "FLOATING_POINT_EXCEPTION";
      exceptionToThrow.message = "Floating point exception";

    }
    else if ( signal == SIGSEGV )
    {

      exceptionToThrow.signal = signal;
      exceptionToThrow.code = "INVALID_MEMORY_REFERENCE";
      exceptionToThrow.message = "Invalid memory reference";

    }

    throw( exceptionToThrow );

  }

}

void end_throw_exception_protection()
{

  const std::string thread_id = CommonUtilities::get_thread_id();

  global_jump_buffer[ thread_id ].pop();

}

void stack_trace() {

  // backward::StackTrace st;

  // st.load_here();

  // backward::Printer printer;

  // //printer.snippet = false;
  // printer.address = true;
  // printer.print( st, std::cout );

  //std::cout.flush();

  //std::cout << get_stack_trace() << std::endl;

  //unwind_print_stack_trace();

  //posix_print_stack_trace();
  KISS::Vector<KISS::CallStackFrame> *call_stack_entries = get_call_stack_entries( 1 );

  for ( const KISS::CallStackFrame &call_stack_entry: *call_stack_entries ) {

    std::cout << call_stack_entry.get_function() << " => " << call_stack_entry.get_file() << ":" << call_stack_entry.get_line() << std::endl;
    //std::cout << call_stack_entry << std::endl;

  }

  delete call_stack_entries;

  malloc_trim( 0 );

}

void valid_function() {

  try {

    // backward::StackTrace st;

    // st.load_here();

    // backward::Printer printer;
    // printer.print( st, std::cout );

    // print_stacktrace();

    begin_map_posix_signal_to_cpp_exception();

    std::osyncstream sync_out( std::cout );

    sync_out << "Valid function called" << std::endl;

    end_throw_exception_protection(); //This code must be executed

  }
  catch ( const Exception &exception ) {

    std::osyncstream sync_out( std::cout );

    sync_out << "***** Exception Report Begin *****" << std::endl;
    sync_out << "Thread: " << exception.thread_name << " (" << exception.thread_id << ")" << std::endl;
    sync_out << "Signal: " << exception.signal << std::endl;
    sync_out << "Code: \"" << exception.code << "\"" << std::endl;
    sync_out << "Message: \"" << exception.message << "\"" << std::endl;
    sync_out << "Function: \"" << __func__  << "\"" << std::endl;
    sync_out << "File: " << __FILE__ << ":" << __LINE__ << std::endl;
    sync_out << "At: " << exception.at << std::endl;
    sync_out << "StackTrace: " << std::endl;

    if ( exception.call_stack ) {

      for ( const KISS::CallStackFrame& call_stack_entry: *exception.call_stack ) {

        sync_out << call_stack_entry.get_function() << " => " << call_stack_entry.get_file() << ":" << call_stack_entry.get_line() << std::endl;

      }

      delete exception.call_stack;

    }

    //sync_out << get_stack_trace() << std::endl;
    sync_out << "***** Exception Report End *****" << std::endl;


  }

}

int invalid_memory_access_01()
{

  int result = 0;

  try
  {

    Test01::Address address01 { "IMA_Address01", "13550 SW 120th St, Miami", "FL33186", "Miami Dade", "Florida", "USA" }; //Test the stack unwind destructor

    begin_map_posix_signal_to_cpp_exception();

    Test01::Address address02 { "IMA_Address02", "13550 SW 120th St, Miami", "FL33186", "Miami Dade", "Florida", "USA" }; //Test the stack unwind destructor

    valid_function();

    int *badPtr = nullptr;

    result = *badPtr;

    end_throw_exception_protection();

  }
  catch ( const Exception &exception )
  {

    std::osyncstream sync_out( std::cout );

    sync_out << "***** Exception Report Begin *****" << std::endl;
    sync_out << "Thread: " << exception.thread_name << " (" << exception.thread_id << ")" << std::endl;
    sync_out << "Signal: " << exception.signal << std::endl;
    sync_out << "Code: \"" << exception.code << "\"" << std::endl;
    sync_out << "Message: \"" << exception.message << "\"" << std::endl;
    sync_out << "Function: \"" << __func__  << "\"" << std::endl;
    sync_out << "File: " << __FILE__ << ":" << __LINE__ << std::endl;
    sync_out << "At: " << exception.at << std::endl;
    sync_out << "StackTrace: " << std::endl;

    if ( exception.call_stack ) {

      for ( const KISS::CallStackFrame& call_stack_entry: *exception.call_stack ) {

        sync_out << call_stack_entry.get_function() << " => " << call_stack_entry.get_file() << ":" << call_stack_entry.get_line() << std::endl;

      }

      delete exception.call_stack;

    }

    sync_out << "***** Exception Report End *****" << std::endl;

  }

  return result;

}

int invalid_memory_access_02()
{

  int result = 0;

  try
  {

    begin_map_posix_signal_to_cpp_exception();

    valid_function();

    invalid_memory_access_01();

    valid_function();

    int *badPtr = nullptr;

    result = *badPtr;

    invalid_memory_access_01(); //This code never executed, is breaked and send to the catch

    end_throw_exception_protection();

  }
  catch ( const Exception &exception )
  {

    std::osyncstream sync_out( std::cout );

    sync_out << "***** Exception Report Begin *****" << std::endl;
    sync_out << "Thread: " << exception.thread_name << " (" << exception.thread_id << ")" << std::endl;
    sync_out << "Signal: " << exception.signal << std::endl;
    sync_out << "Code: \"" << exception.code << "\"" << std::endl;
    sync_out << "Message: \"" << exception.message << "\"" << std::endl;
    sync_out << "Function: \"" << __func__  << "\"" << std::endl;
    sync_out << "File: " << __FILE__ << ":" << __LINE__ << std::endl;
    sync_out << "At: " << exception.at << std::endl;
    sync_out << "StackTrace: " << std::endl;

    if ( exception.call_stack ) {

      for ( const KISS::CallStackFrame& call_stack_entry: *exception.call_stack ) {

        sync_out << call_stack_entry.get_function() << " => " << call_stack_entry.get_file() << ":" << call_stack_entry.get_line() << std::endl;

      }

      delete exception.call_stack;

    }

    sync_out << "***** Exception Report End *****" << std::endl;

  }

  return result;

}

//FLOATING_POINT_EXCEPTION
int floating_point_exception_01()
{

  int result = 0;

  try
  {

    begin_map_posix_signal_to_cpp_exception();

    int div = 0;

    result = 10 / div;

    end_throw_exception_protection();

  }
  catch ( Exception &exception )
  {

    std::osyncstream sync_out( std::cout );

    sync_out << "***** Exception Report Begin *****" << std::endl;
    sync_out << "Thread: " << exception.thread_name << " (" << exception.thread_id << ")" << std::endl;
    sync_out << "Signal: " << exception.signal << std::endl;
    sync_out << "Code: \"" << exception.code << "\"" << std::endl;
    sync_out << "Message: \"" << exception.message << "\"" << std::endl;
    sync_out << "Function: \"" << __func__  << "\"" << std::endl;
    sync_out << "File: " << __FILE__ << ":" << __LINE__ << std::endl;
    sync_out << "At: " << exception.at << std::endl;
    sync_out << "StackTrace: " << std::endl;

    for ( const KISS::CallStackFrame& call_stack_entry: *exception.call_stack ) {

      sync_out << call_stack_entry.get_function() << " => " << call_stack_entry.get_file() << ":" << call_stack_entry.get_line() << std::endl;

    }

    sync_out << "***** Exception Report End *****" << std::endl;

    delete exception.call_stack;

  }

  return result;

}

void thread01_entry_point( int repeat, std::string name )
{

	// sigset_t mask;
	// sigemptyset(&mask);
  // sigaddset(&mask, SIGRTMIN+3);

  // pthread_sigmask(SIG_BLOCK, &mask, NULL);

  global_thread_mutex.lock();

  global_thread_name[ CommonUtilities::get_thread_id() ] = name;

  global_thread_mutex.unlock();

  for ( int i = 0; i < repeat; i++ )
  {

    std::osyncstream sync_out( std::cout );

    //sync_out << "Thread using function pointer as callable\n";
    sync_out << "[" << name << "]: Calling to invalid_memory_access_01. Round: " << i << std::endl;

    invalid_memory_access_01();

    sync_out << "[" << name << "]: Sleeping by 1 seconds. Round: " << i << std::endl;

    std::this_thread::sleep_for( std::chrono::seconds( 1 ) );

    //sync_out.flush();

  }

}

void thread02_entry_point( int repeat, std::string name )
{

  global_thread_mutex.lock();

  global_thread_name[ CommonUtilities::get_thread_id() ] = name;

  global_thread_mutex.unlock();

  for ( int i = 0; i < repeat; i++ )
  {

    std::osyncstream sync_out( std::cout );

    //sync_out << "Thread using function pointer as callable\n";
    sync_out << "[" << name << "]: Calling to invalid_memory_access_02. Round: " << i << std::endl;

    invalid_memory_access_02();

    sync_out << "[" << name << "]: Sleeping by 1 seconds. Round: " << i << std::endl;

    std::this_thread::sleep_for( std::chrono::seconds( 1 ) );

    //sync_out.flush();

  }

}

int main(int argc, char *argv[])
{

  global_program_name = argv[ 0 ];

  global_thread_name[ CommonUtilities::get_thread_id() ] = "thread_main";

  // timeval curTime;
  // gettimeofday(&curTime, NULL);
  // int milli = curTime.tv_usec / 1000;

  // char buffer [80];
  // strftime(buffer, 80, "%Y-%m-%dT%H:%M:%S.%LZ%0z", localtime(&curTime.tv_sec));

  // char currentTime[84] = "";
  // sprintf(currentTime, "%s:%03d", buffer, milli);
  // printf("current time: %s \n", currentTime);

  // //std::cout << sTimestamp << std::endl;

  std::cout << CommonUtilities::get_local_date_time( _DATETIME_FORMAT_02 ) << std::endl;

  Test01::Address address01 { "Address01", "13550 SW 120th St, Miami", "FL33186", "Miami Dade", "Florida", "USA" };

  Test01::Address address02 { address01 };

  address02.set_name( "Address02" );

  Test01::Address *address03 = new Test01::Address( &address01 );

  address03->set_name( "Address03" );

  Test01::Address *address04 = new Test01::Address( address01 );

  address04->set_name( "Address04" );

  Test01::Person person01 { "7720f532-5741-431e-a67e-f25a06a533c5", "John", "Connor", 43, &address01 };

  {

    std::osyncstream sync_out( std::cout );

    sync_out << person01.id << std::endl;
    sync_out << person01.first_name << std::endl;
    sync_out << person01.last_name << std::endl;
    sync_out << person01.age << std::endl;

    for ( auto fieldName: Test01::Person::_FIELDS ) {

      sync_out << fieldName << std::endl;

    }

    sync_out << "Hello world" << std::endl;

  }

  //sync_out << Test01::Person::_FIELDS[ 0 ] << std::endl;

  //person01.clearId();

  const char *Id = person01.getId();

  Id = nullptr;

  delete address03;
  delete address04;

  //calamity01();

  //signal( SIGINT, catch_signal );
  //signal( SIGSEGV, catchSignal );

  struct sigaction sa;

  sa.sa_flags = SA_SIGINFO;

  sigemptyset(&sa.sa_mask);

  sa.sa_sigaction = catch_signal;

  sigaction( SIGILL, &sa, NULL );
  sigaction( SIGFPE, &sa, NULL );
  sigaction( SIGSEGV, &sa, NULL );

  // if ( sigaction( SIGSEGV, &sa, NULL ) == -1 ) {

  //   //handle_error("sigaction");
  //   //

  // }

  stack_trace();

  //valid_function();

  invalid_memory_access_01();
  // invalid_memory_access_02();

  // floating_point_exception_01();

  std::cout << "Press any key to continue the program" << std::endl;
  std::cin.ignore();

  // std::thread thread01( thread01_entry_point, 10, "thread_01" );

  // std::thread thread02( thread02_entry_point, 10, "thread_02" );

  // std::thread thread03( thread01_entry_point, 10, "thread_03" );

  // std::thread thread04( thread02_entry_point, 10, "thread_04" );

  // int capture = 0;

  // //Lambda test
  // std::thread thread05( [&capture]( int repeat, std::string name )
  // {

  //   global_thread_mutex.lock();

  //   global_thread_name[ CommonUtilities::get_thread_id() ] = name;

  //   global_thread_mutex.unlock();

  //   for ( int i = 0; i < repeat; i++ )
  //   {

  //     capture += 1;

  //     std::osyncstream sync_out( std::cout );

  //     //sync_out << "Thread using function pointer as callable\n";
  //     sync_out << "[" << name << "]: Calling to invalid_memory_access_02. Round: " << i << std::endl;

  //     invalid_memory_access_02();

  //     sync_out << "[" << name << "]: Sleeping by 1 seconds. Round: " << i << std::endl;

  //     std::this_thread::sleep_for( std::chrono::seconds( 1 ) );

  //     //sync_out.flush();

  //   }


  // },
  // 10, "thread_04" );

  // thread01.join();

  // thread02.join();

  // thread03.join();

  // thread04.join();

  // thread05.join();

  // std::cout << "Capture: " << capture << std::endl;

  // try
  // {

  //   test_signal_01();

  // }
  // catch (int &z)
  // {

  //   std::cerr << "Caught unexpected exception: " << z << std::endl;

  // }

  //malloc_trim( 0 );

  std::cout << "Press any key to finish the program" << std::endl;
  std::cin.ignore();

  return 0;

}
