#include <iostream>
#include <string>
#include <cstring>
#include <utility>
#include <map>

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
  NSTD::Vector<NSTD::CallStackFrame> call_stack;

};

static std::map<std::string, NSTD::Vector<NSTD::CallStackFrame>> global_thread_call_stack;


int main(int argc, char *argv[])
{

  NSTD::CallStackFrame x1 = { "1", "2", "3" };

  std::cout << "x1 => " << std::endl << x1 << std::endl;

  NSTD::CallStackFrame y1 = { x1 };

  std::cout << "y1 => " << std::endl << y1 << std::endl;

  NSTD::CallStackFrame z1 = std::move( y1 );
  std::cout << "y1 => " << std::endl << y1 << std::endl;
  std::cout << "z1 => " << std::endl << z1 << std::endl;

  y1 = z1; //Call to copy operator=
  std::cout << "y1 => " << std::endl << y1 << std::endl;
  std::cout << "z1 => " << std::endl << z1 << std::endl;

  //Now swap the pointer address
  y1 = std::move( z1 ); //Call to move operator=
  std::cout << "y1 => " << std::endl << y1 << std::endl;
  std::cout << "z1 => " << std::endl << z1 << std::endl;

  //Move operator=
  x1 = { "4", "5", "6" }; //Here {} create a new CallStackEntry {}

  //global_thread_call_stack[ "1" ].push_back( x1 );

  if ( global_thread_call_stack.find( "1" ) == global_thread_call_stack.end() ) {

     global_thread_call_stack[ "1" ].reserve( 20 );

  }

  global_thread_call_stack[ "1" ].emplace_back( std::move( x1 ) );
  global_thread_call_stack[ "1" ].emplace_back( std::move( y1 ) );
  global_thread_call_stack[ "1" ].emplace_back( std::move( z1 ) );

  //std::cout << "x1 => " << std::endl << x1 << std::endl;

  // //x =  std::move( { "4", "5", "6" }  );

  // //std::cout << "File: " << x.get_file() << ", Function: " << x.get_function() << ", Line: " << x.get_line() << std::endl;

}

