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

//#include <valgrind/valgrind.h>

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
//#define BACKWARD_HAS_LIBUNWIND 1

//#include "backward.hpp"

#include "test01.hpp"

#include "Vector.hpp"

#include "CommonUtilities.hpp"

#include "SystemUtilities.hpp"

#include "CallStackFrame.hpp"

#include "Exception.hpp"

// Disabled by lot of problems with valgrind
// inline void begin_map_posix_signal_to_cpp_exception()
// {

//   jmp_buf local_jump_buffer {};

//   memset( &local_jump_buffer, 0, sizeof( local_jump_buffer ) );

//   // void *stack = (void *)mmap(0, STACK_SIZE, PROT_READ|PROT_WRITE|PROT_EXEC,
//   //                               MAP_ANONYMOUS|MAP_PRIVATE, -1, 0);

//   //const size_t stack_size = ( 10 * 4096 ); //( 1 << 20 );

//   //#define STACK_SIZE 1 << 20;

//   //void *stack = malloc( stack_size );

//   //int valgrind_stack_id = VALGRIND_STACK_REGISTER( stack, ( ( uint64_t ) stack ) + stack_size );

//   //volatile int signal = 0;

//   //int signal = ;

//   // int signal = 1;

//   if ( setjmp( local_jump_buffer ) == 0 )
//   {

//     const std::string thread_id = CommonUtilities::get_thread_id();

//     //sync_out << "Signal number: " << signalNumber << std::endl;
//     global_jump_buffer[ thread_id ].push( local_jump_buffer[ 0 ] );

//     // // sync_out << "localGBuffer[ 0 ].__mask_was_saved => " << localGBuffer[ 0 ].__mask_was_saved << std::endl;
//     // // sync_out << "gBuffer.top() => " << gBuffer.top().__mask_was_saved << std::endl;
//     // // gBuffer.top().__mask_was_saved = 10;
//     // // sync_out << "localGBuffer[ 0 ].__mask_was_saved => " << localGBuffer[ 0 ].__mask_was_saved << std::endl;
//     // // sync_out << "gBuffer.top() => " << gBuffer.top().__mask_was_saved << std::endl;

//   }
//   else //if ( signal )
//   {

//     //VALGRIND_STACK_DEREGISTER( valgrind_stack_id );

//     // exceptionToThrow.thread_id = CommonUtilities::get_thread_id();
//     // exceptionToThrow.thread_name = global_thread_name[ exceptionToThrow.thread_id ];
//     // exceptionToThrow.at = CommonUtilities::get_local_date_time( _DATETIME_FORMAT_02 );
//     // exceptionToThrow.call_stack = global_thread_call_stack[ exceptionToThrow.thread_id ];
//     //global_thread_call_stack[ exceptionToThrow.thread_id ].clear();

//     const std::string thread_id = CommonUtilities::get_thread_id();
//     const char *code = "";
//     const char *message = "";

//     int signal = SIGSEGV;

//     if ( signal == SIGILL )
//     {

//       //signal = signal;
//       code = "ILLEGAL_INSTRUCTION";
//       message = "Illegal Instruction";

//     }
//     else if ( signal == SIGFPE )
//     {

//       //signal = signal;
//       code = "FLOATING_POINT_EXCEPTION";
//       message = "Floating point exception";

//     }
//     else if ( signal == SIGSEGV )
//     {

//       //signal = signal;
//       code = "INVALID_MEMORY_REFERENCE";
//       message = "Invalid memory reference";

//     }

//     NSTD::Exception exceptionToThrow =
//     {
//       thread_id.c_str(),
//       global_thread_name[ thread_id ].c_str(),
//       signal,
//       code,
//       message,
//       CommonUtilities::get_local_date_time( _DATETIME_FORMAT_02 ).c_str(),
//       &global_thread_call_stack[ thread_id ]
//     };

//     throw exceptionToThrow;

//     //NSTD::Exception exceptionToThrow {};

//     // throw( "Exception" );

//     //raise_exception();

//   }

//   //return result;

// }

// void end_map_posix_signal_to_cpp_exception()
// {

//   const std::string thread_id = CommonUtilities::get_thread_id();

//   global_jump_buffer[ thread_id ].pop();

// }

void stack_trace() {

  NSTD::Vector<NSTD::CallStackFrame> call_stack_entries = SystemUtilities::get_call_stack_entries( 1 );

  for ( const NSTD::CallStackFrame &call_stack_entry: call_stack_entries ) {

    std::cout << call_stack_entry.get_function() << " => " << call_stack_entry.get_file() << ":" << call_stack_entry.get_line() << std::endl;

  }

  malloc_trim( 0 ); //Force to return the memory to Operating system

}

void valid_function() {

  try {

    // backward::StackTrace st;

    // st.load_here();

    // backward::Printer printer;
    // printer.print( st, std::cout );

    // print_stacktrace();

    //begin_map_posix_signal_to_cpp_exception();
    __BEGIN_MAP_POSIX_SIGNAL_TO_CPP_EXCEPTION

    std::osyncstream sync_out( std::cout );

    sync_out << "Valid function called" << std::endl;

    //end_map_posix_signal_to_cpp_exception(); //This code must be executed
    __END_MAP_POSIX_SIGNAL_TO_CPP_EXCEPTION

  }
  catch ( const NSTD::Exception &exception )
  {

    __REPORT_EXCEPTION__

  }

}

int invalid_memory_access_01()
{

  int result = 0;

  try
  {
    //begin_map_posix_signal_to_cpp_exception();
    __BEGIN_MAP_POSIX_SIGNAL_TO_CPP_EXCEPTION

    // Test01::Address address01 { "IMA_Address01", "13550 SW 120th St, Miami", "FL33186", "Miami Dade", "Florida", "USA" }; //Test the stack unwind destructor

    // Test01::Address address02 { "IMA_Address02", "13550 SW 120th St, Miami", "FL33186", "Miami Dade", "Florida", "USA" }; //Test the stack unwind destructor

    valid_function();

    int *badPtr = nullptr;

    result = *badPtr;

    //end_map_posix_signal_to_cpp_exception();
    __END_MAP_POSIX_SIGNAL_TO_CPP_EXCEPTION

  }
  catch ( const NSTD::Exception &exception )
  {

    __REPORT_EXCEPTION__

  }

  return result;

}

int invalid_memory_access_02()
{

  int result = 0;

  try
  {

    //begin_map_posix_signal_to_cpp_exception();
    __BEGIN_MAP_POSIX_SIGNAL_TO_CPP_EXCEPTION

    valid_function();

    invalid_memory_access_01();

    valid_function();

    int *badPtr = nullptr;

    result = *badPtr;

    invalid_memory_access_01(); //This code never executed, is breaked and send to the catch

    //end_map_posix_signal_to_cpp_exception();
    __END_MAP_POSIX_SIGNAL_TO_CPP_EXCEPTION

  }
  catch ( const NSTD::Exception &exception )
  {

    __REPORT_EXCEPTION__

  }

  return result;

}

//FLOATING_POINT_EXCEPTION
int floating_point_exception_01()
{

  int result = 0;

  try
  {

    //begin_map_posix_signal_to_cpp_exception();
    __BEGIN_MAP_POSIX_SIGNAL_TO_CPP_EXCEPTION

    int div = 0;

    result = 10 / div;

    //end_map_posix_signal_to_cpp_exception();
    __END_MAP_POSIX_SIGNAL_TO_CPP_EXCEPTION

  }
  catch ( const NSTD::Exception &exception )
  {

    __REPORT_EXCEPTION__

  }

  return result;

}

void thread01_entry_point( const int repeat, const char *name, const int argc, const char *argv[] )
{

  CommonUtilities::init( name, argc, argv );

  SystemUtilities::init( name, argc, argv );

  // SystemUtilities::global_thread_mutex.lock();

  // SystemUtilities::global_thread_name[ CommonUtilities::get_thread_id() ] = name;

  // SystemUtilities::global_thread_mutex.unlock();

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

  SystemUtilities::finish( name, argc, argv );

  CommonUtilities::finish( name, argc, argv );

}

void thread02_entry_point( const int repeat, const char *name, const int argc, const char *argv[] )
{

  CommonUtilities::init( name, argc, argv );

  SystemUtilities::init( name, argc, argv );

  // SystemUtilities::global_thread_mutex.lock();

  // SystemUtilities::global_thread_name[ CommonUtilities::get_thread_id() ] = name;

  // SystemUtilities::global_thread_mutex.unlock();

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

  SystemUtilities::finish( name, argc, argv );

  CommonUtilities::finish( name, argc, argv );

}

int main( const int argc, const char *argv[] )
{

  //global_program_name = argv[ 0 ];

  CommonUtilities::init( "thread_main", argc, argv );

  SystemUtilities::init( "thread_main", argc, argv );

  std::cout << "Running from the path: " << SystemUtilities::full_program_path << std::endl;

  //SystemUtilities::global_thread_name[ CommonUtilities::get_thread_id() ] = "thread_main";

  // // timeval curTime;
  // // gettimeofday(&curTime, NULL);
  // // int milli = curTime.tv_usec / 1000;

  // // char buffer [80];
  // // strftime(buffer, 80, "%Y-%m-%dT%H:%M:%S.%LZ%0z", localtime(&curTime.tv_sec));

  // // char currentTime[84] = "";
  // // sprintf(currentTime, "%s:%03d", buffer, milli);
  // // printf("current time: %s \n", currentTime);

  // // //std::cout << sTimestamp << std::endl;

  // std::cout << CommonUtilities::get_local_date_time( _DATETIME_FORMAT_02 ) << std::endl;

  // Test01::Address address01 { "Address01", "13550 SW 120th St, Miami", "FL33186", "Miami Dade", "Florida", "USA" };

  // Test01::Address address02 { address01 };

  // address02.set_name( "Address02" );

  // Test01::Address *address03 = new Test01::Address( &address01 );

  // address03->set_name( "Address03" );

  // Test01::Address *address04 = new Test01::Address( address01 );

  // address04->set_name( "Address04" );

  // Test01::Person person01 { "7720f532-5741-431e-a67e-f25a06a533c5", "John", "Connor", 43, &address01 };

  // {

  //   std::osyncstream sync_out( std::cout );

  //   sync_out << person01.id << std::endl;
  //   sync_out << person01.first_name << std::endl;
  //   sync_out << person01.last_name << std::endl;
  //   sync_out << person01.age << std::endl;

  //   for ( auto fieldName: Test01::Person::_FIELDS ) {

  //     sync_out << fieldName << std::endl;

  //   }

  //   sync_out << "Hello world" << std::endl;

  // }

  // //sync_out << Test01::Person::_FIELDS[ 0 ] << std::endl;

  // //person01.clearId();

  // const char *Id = person01.getId();

  // Id = nullptr;

  // delete address03;
  // delete address04;

  //calamity01();

  //signal( SIGINT, catch_signal );
  //signal( SIGSEGV, catchSignal );

  struct sigaction sa;

  sa.sa_flags = SA_SIGINFO;

  sigemptyset(&sa.sa_mask);

  sa.sa_sigaction = (void (*)(int, siginfo_t *, void *)) SystemUtilities::catch_signal;

  sigaction( SIGILL, &sa, NULL );
  sigaction( SIGFPE, &sa, NULL );
  sigaction( SIGSEGV, &sa, NULL );
  sigaction( SIGINT, &sa, NULL );

  //stack_trace();

  //valid_function();

  invalid_memory_access_01();
  invalid_memory_access_02();

  floating_point_exception_01();

  std::cout << "Press any key to continue the program" << std::endl;
  std::cin.ignore();

  std::thread thread01( thread01_entry_point, 10, "thread_01", argc, argv  );

  std::thread thread02( thread02_entry_point, 10, "thread_02", argc, argv );

  std::thread thread03( thread01_entry_point, 10, "thread_03", argc, argv );

  std::thread thread04( thread02_entry_point, 10, "thread_04", argc, argv );

  int capture = 0;

  //Lambda test
  std::thread thread05( [&capture]( const int repeat, const char *name, const int argc, const char *argv[] )
  {

    CommonUtilities::init( name, argc, argv );

    SystemUtilities::init( name, argc, argv );

    // SystemUtilities::global_thread_mutex.lock();

    // SystemUtilities::global_thread_name[ CommonUtilities::get_thread_id() ] = name;

    // SystemUtilities::global_thread_mutex.unlock();

    for ( int i = 0; i < repeat; i++ )
    {

      capture += 1;

      std::osyncstream sync_out( std::cout );

      //sync_out << "Thread using function pointer as callable\n";
      sync_out << "[" << name << "]: Calling to invalid_memory_access_02. Round: " << i << std::endl;

      invalid_memory_access_02();

      sync_out << "[" << name << "]: Sleeping by 1 seconds. Round: " << i << std::endl;

      std::this_thread::sleep_for( std::chrono::seconds( 1 ) );

      //sync_out.flush();

    }

    CommonUtilities::finish( name, argc, argv );

    SystemUtilities::finish( name, argc, argv );

  },
  10,
  "thread_05", argc, argv );

  thread01.join();

  thread02.join();

  thread03.join();

  thread04.join();

  thread05.join();

  std::cout << "Capture: " << capture << std::endl;

  // try
  // {

  //   test_signal_01();

  // }
  // catch (int &z)
  // {

  //   std::cerr << "Caught unexpected exception: " << z << std::endl;

  // }

  //malloc_trim( 0 );

  std::cout << "Press any key to finish the program [key]" << std::endl;
  std::cin.ignore();

  CommonUtilities::finish(  "thread_main", argc, argv );

  return 0;

}
