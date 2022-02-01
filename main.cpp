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
//#include <malloc.h>
// #include <string>
// #include <array>

#include "test01.hpp"

static std::map<std::string, std::stack<__jmp_buf_tag>> global_jump_buffer;
static std::mutex global_jump_mutex;
static std::map<std::string, std::string> global_thread_name;
static std::mutex global_thread_mutex;

struct Exception 
{

  std::string thread_id; 
  std::string thread_name;
  int signal;
  std::string code;
  std::string message;
  std::string at;

};

const std::string get_thread_id() noexcept 
{

  auto thread_id = std::this_thread::get_id();
  std::stringstream ss;
  ss << thread_id;
  return ss.str();

}

bool replace( std::string& str, const std::string& from, const std::string& to ) 
{

  size_t start_pos = str.find( from );

  if ( start_pos == std::string::npos )
  {
   
    return false;

  }

  str.replace( start_pos, from.length(), to );

  return true;

}

void replace_all( std::string& str, const std::string& from, const std::string& to ) 
{

  if ( from.empty() ) 
  {
   
    return;

  }

  size_t start_pos = 0;
  
  while ( ( start_pos = str.find( from, start_pos ) ) != std::string::npos ) 
  {

    str.replace( start_pos, from.length(), to );
    start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'

  }

}

#define _DATETIME_FORMAT_01 "%Y-%m-%dT%H-%M-%S"
#define _DATETIME_FORMAT_02 "%Y-%m-%dT%H:%M:%S.%%SSSZ%:z"

const std::string get_local_date_time( std::string format = _DATETIME_FORMAT_01 ) noexcept 
{

  /*
  time_t     now = time(0);
  struct tm  tstruct;
  char       buf[80];
  tstruct = *localtime(&now);

  strftime(buf, sizeof(buf), format.c_str(), &tstruct);

  return buf;
  */

  std::string time_stamp;
  char buffer_time_stamp[ 100 ];
  memset( buffer_time_stamp, 0, sizeof( buffer_time_stamp ) );

  struct timeval tv;
  struct tm *tm;

  gettimeofday( &tv, NULL );

  tm = localtime( &tv.tv_sec );

  strftime( buffer_time_stamp,
            sizeof( buffer_time_stamp ),
            format.c_str(),
            tm );

  // sprintf(acTimestamp, "%04d-%02d-%02dT%02d:%02d:%02d.%03d\n",
  //           tm->tm_year + 1900,
  //           tm->tm_mon + 1,
  //           tm->tm_mday,
  //           tm->tm_hour,
  //           tm->tm_min,
  //           tm->tm_sec,
  //           (int) (tv.tv_usec / 1000)
  //       );

  time_stamp = buffer_time_stamp;

  if ( format.find( "%:z" ) != std::string::npos ) {

    char buffer_time_zone_offset[20];

    memset( buffer_time_zone_offset, 0, sizeof( buffer_time_zone_offset ) );

    strftime( buffer_time_zone_offset,
              sizeof( buffer_time_zone_offset ),
              "%z",
              tm );

    buffer_time_zone_offset[ 5 ] = buffer_time_zone_offset[ 4 ];             
    buffer_time_zone_offset[ 4 ] = buffer_time_zone_offset[ 3 ];             
    buffer_time_zone_offset[ 3 ] = ':';             

    replace( time_stamp,
             std::string( "%:z" ),
             buffer_time_zone_offset );

  }

  replace( time_stamp,
           std::string( "%SSS" ),
           std::to_string( (int) ( tv.tv_usec / 1000 ) ) );

  return time_stamp;

}

void catch_signal( int signal, siginfo_t *signalInfo, void *unused )
{

  {

    std::osyncstream sync_out( std::cout );

    sync_out << "cath_signal => Thread: " << global_thread_name[ get_thread_id() ] << " (" << get_thread_id() << ")\n";

  }

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

  const std::string thread_id = get_thread_id();

  jmp_buf local_jump_buffer = { global_jump_buffer[ thread_id ].top() };

  global_jump_buffer[ thread_id ].pop();

  global_jump_mutex.unlock();

  longjmp           // Jump back into the normal flow of the program
  (
    local_jump_buffer,        // using this context to say where to jump to
    signal    // and passing back the value of the signal.
  );

}

void begin_throw_exception_protection() 
{

  jmp_buf local_jump_buffer;

  const int signal = setjmp( local_jump_buffer );

  if ( signal == 0 ) 
  {
  
    const std::string thread_id = get_thread_id();

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

    exceptionToThrow.thread_id = get_thread_id();
    exceptionToThrow.thread_name = global_thread_name[ get_thread_id() ];
    exceptionToThrow.at = get_local_date_time( _DATETIME_FORMAT_02 );

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

  const std::string thread_id = get_thread_id();

  global_jump_buffer[ thread_id ].pop();

}

void valid_function() {

  try {

    begin_throw_exception_protection();

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
    sync_out << "Function: " << __func__  << "\"" << std::endl;
    sync_out << "File: " << __FILE__ << ":" << __LINE__ << std::endl;
    sync_out << "At: " << exception.at << std::endl;
    sync_out << "***** Exception Report End *****" << std::endl;

  }

}

int invalid_memory_access_01() 
{

  int result = 0;

  try 
  {
  
    Test01::Address address01 { "IMA_Address01", "13550 SW 120th St, Miami", "FL33186", "Miami Dade", "Florida", "USA" }; //Test the stack unwind destructor

    begin_throw_exception_protection();

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
    sync_out << "Function: " << __func__  << "\"" << std::endl;
    sync_out << "File: " << __FILE__ << ":" << __LINE__ << std::endl;
    sync_out << "At: " << exception.at << std::endl;
    sync_out << "***** Exception Report End *****" << std::endl;

  }

  return result;

}

int invalid_memory_access_02() 
{

  int result = 0;

  try 
  {

    begin_throw_exception_protection();

    valid_function();

    invalid_memory_access_01();

    valid_function();

    int *badPtr = nullptr; 

    result = *badPtr;

    invalid_memory_access_01(); //This code never executed, is breaked and send to the catch

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
    sync_out << "Function: " << __func__  << "\"" << std::endl;
    sync_out << "File: " << __FILE__ << ":" << __LINE__ << std::endl;
    sync_out << "At: " << exception.at << std::endl;
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

    begin_throw_exception_protection();

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
    sync_out << "Function: " << __func__  << "\"" << std::endl;
    sync_out << "File: " << __FILE__ << ":" << __LINE__ << std::endl;
    sync_out << "At: " << exception.at << std::endl;
    sync_out << "***** Exception Report End *****" << std::endl;

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

  global_thread_name[ get_thread_id() ] = name;

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

  global_thread_name[ get_thread_id() ] = name;

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

  global_thread_name[ get_thread_id() ] = "thread_main";

  // timeval curTime;
  // gettimeofday(&curTime, NULL);
  // int milli = curTime.tv_usec / 1000;

  // char buffer [80];
  // strftime(buffer, 80, "%Y-%m-%dT%H:%M:%S.%LZ%0z", localtime(&curTime.tv_sec));

  // char currentTime[84] = "";
  // sprintf(currentTime, "%s:%03d", buffer, milli);
  // printf("current time: %s \n", currentTime);

  // //std::cout << sTimestamp << std::endl;

  std::cout << get_local_date_time( _DATETIME_FORMAT_02 ) << std::endl;

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


  invalid_memory_access_01();
  invalid_memory_access_02();

  floating_point_exception_01();

  std::thread thread01( thread01_entry_point, 10, "thread_01" );

  std::thread thread02( thread02_entry_point, 10, "thread_02" );

  std::thread thread03( thread01_entry_point, 10, "thread_03" );

  std::thread thread04( thread02_entry_point, 10, "thread_04" );

  int capture = 0;

  //Lambda test
  std::thread thread05( [&capture]( int repeat, std::string name )
  { 
 
    global_thread_mutex.lock();

    global_thread_name[ get_thread_id() ] = name;

    global_thread_mutex.unlock();

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
  

  },
  10, "thread_04" );

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

  std::cout << "Press any key to finish the program" << std::endl;
  std::cin.ignore();
    
  return 0;

}