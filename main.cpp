#include <iostream>
#include <csignal>
#include <csetjmp>
#include <stack>
#include <map>
#include <thread>
#include <chrono>
#include <mutex>
#include <syncstream>
// #include <string>
// #include <array>

#include "test01.hpp"

std::map<std::string, std::stack<__jmp_buf_tag>> gBuffer;
std::mutex m;
//std::osyncstream sync_out(std::cout);

//std::stack<jmp_buf> gBuffer;

//jmp_buf gBuffer;
//jmp_buf gBuffer02;

struct Exception {

  int Signal;
  std::string Code;
  std::string Message;

};

const std::string get_thread_id() {

  auto thread_id = std::this_thread::get_id();
  std::stringstream ss;
  ss << thread_id;
  return ss.str();

}

void catch_signal( int signal, siginfo_t *signalInfo, void *unused )
{

  {

    std::osyncstream sync_out(std::cout);

    sync_out << "Cath signal / Thread id: " << get_thread_id() << "\n";

  }

  // ********** Very Important ************
  // Unblock the signal to allow to OS send another time
  sigset_t sigs;
  sigemptyset( &sigs );
  sigaddset( &sigs, signal );
  sigprocmask( SIG_UNBLOCK, &sigs, NULL );
  // ********** Very Important ************

  //signal(SIGINT, SIG_DFL);          // Switch to default handling
  //signal(SIGINT, catch_signal);       // Reactivate this handler.
  //signal( SIGSEGV, catchSignal );

  m.lock();

  //jmp_buf localGBuffer;

  const std::string thread_id = get_thread_id();

  jmp_buf localGBuffer = { gBuffer[ thread_id ].top() };

  gBuffer[ thread_id ].pop();

  m.unlock();

  longjmp           // Jump back into the normal flow of the program
  (
    localGBuffer,        // using this context to say where to jump to
    signal    // and passing back the value of the signal.
  );

}

void begin_exception_protection() {

  jmp_buf localGBuffer;

  const int signalNumber = setjmp( localGBuffer );

  if ( signalNumber == 0 ) {
  
    const std::string thread_id = get_thread_id();

    //sync_out << "Signal number: " << signalNumber << std::endl;
    gBuffer[ thread_id ].push( localGBuffer[ 0 ] );

    // sync_out << "localGBuffer[ 0 ].__mask_was_saved => " << localGBuffer[ 0 ].__mask_was_saved << std::endl;
    // sync_out << "gBuffer.top() => " << gBuffer.top().__mask_was_saved << std::endl;
    // gBuffer.top().__mask_was_saved = 10;
    // sync_out << "localGBuffer[ 0 ].__mask_was_saved => " << localGBuffer[ 0 ].__mask_was_saved << std::endl;
    // sync_out << "gBuffer.top() => " << gBuffer.top().__mask_was_saved << std::endl;

  }
  else if ( signalNumber ) {

    Exception exceptionToThrow;

    if ( signalNumber == SIGSEGV ) {

      exceptionToThrow.Signal = signalNumber;
      exceptionToThrow.Code = "SEGMENT_FAULT";
      exceptionToThrow.Message = "Invalid memory access";

    }

    throw( exceptionToThrow );
     
  }

} 

void end_exception_protection() {

  const std::string thread_id = get_thread_id();

  gBuffer[ thread_id ].pop();

}

int calamity_01() {

  int result = 0;

  try {

    begin_exception_protection();

    int *badPtr = nullptr; 

    result = *badPtr;

    end_exception_protection();

  }
  catch ( Exception &exception ) {

    std::osyncstream sync_out(std::cout);

    sync_out << "Exception Thread: " << get_thread_id() << ", Signal: " << exception.Signal << ", Code: \"" << exception.Code << "\", Message: \"" << exception.Message << "\", Function: " << __func__  << ", File: " << __FILE__ << ":" << __LINE__ << "\n";

  }

  return result;

}

int calamity_02() {

  int result = 0;

  try {

    begin_exception_protection();

    calamity_01();

    int *badPtr = nullptr; 

    result = *badPtr;

    calamity_01(); //This code never executed, is breaked and send to catch

    end_exception_protection();

  }
  catch ( Exception &exception ) {

    std::osyncstream sync_out(std::cout);

    sync_out << "Exception Thread: " << get_thread_id() << ", Signal: " << exception.Signal << ", Code: \"" << exception.Code << "\", Message: \"" << exception.Message << "\", Function: " << __func__  << ", File: " << __FILE__ << ":" << __LINE__ << "\n";

  }

  return result;

}

void thread01_entry_point( int repeat )
{

	sigset_t mask;
	sigemptyset(&mask); 
  sigaddset(&mask, SIGRTMIN+3); 
                
  pthread_sigmask(SIG_BLOCK, &mask, NULL);

  for ( int i = 0; i < repeat; i++ )
  {

    std::osyncstream sync_out(std::cout);

    //sync_out << "Thread using function pointer as callable\n";
    sync_out << "thread01_entry_point: Calling to calamity02. Round: " << i << std::endl;

    calamity_02();
  
    sync_out << "thread01_entry_point: Sleeping by 2 seconds. Round: " << i << std::endl;

    std::this_thread::sleep_for( std::chrono::seconds( 1 ) );

    //sync_out.flush();

  }

}

void thread02_entry_point( int repeat )
{

  for ( int i = 0; i < repeat; i++ )
  {

    std::osyncstream sync_out(std::cout);

    //sync_out << "Thread using function pointer as callable\n";
    sync_out << "thread02_entry_point: Calling to calamity02. Round: " << i << std::endl;
    
    calamity_02();

    sync_out << "thread02_entry_point: Sleeping by 2 seconds. Round: " << i << std::endl;

    std::this_thread::sleep_for( std::chrono::seconds( 1 ) );

    //sync_out.flush();

  }

}

int main(int argc, char *argv[]) 
{

  Test01::Address address01 { "13550 SW 120th St, Miami", "FL33186", "Miami Dade", "Florida", "USA" };

  Test01::Address address02 { address01 };

  Test01::Address *address03 = new Test01::Address( &address01 );

  Test01::Address *address04 = new Test01::Address( address01 );

  Test01::Person person01 { "7720f532-5741-431e-a67e-f25a06a533c5", "John", "Connor", 43, &address01 };

  {

    std::osyncstream sync_out(std::cout);

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

  if ( sigaction( SIGSEGV, &sa, NULL ) == -1 ) {
   
    //handle_error("sigaction");
    //

  }

  calamity_01();
  calamity_02();

  std::thread thread01( thread01_entry_point, 10 );

  std::thread thread02( thread02_entry_point, 10 );

  std::thread thread03( thread01_entry_point, 10 );

  std::thread thread04( thread02_entry_point, 10 );

  thread01.join();

  thread02.join();

  thread03.join();

  thread04.join();

  // try
  // {
    
  //   test_signal_01();

  // }
  // catch (int &z)
  // {
    
  //   std::cerr << "Caught unexpected exception: " << z << std::endl;

  // }
    
  return 0;

}