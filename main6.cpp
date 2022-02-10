#include <iostream>
#include <string>
#include <cstring>
#include <utility>
#include <map>
#include <vector>

#include "CommonUtilities.hpp"

#include "Vector.hpp"

#include "CallStackFrame.hpp"

#define __DEBUG__     true
#define __DEBUG_OUT__ std::cout

std::map<std::string, KISS::Vector<KISS::CallStackFrame>> global_thread_call_stack; // { {}, {} };

const KISS::CallStackFrame test_return_01()
{

  const KISS::CallStackFrame result = { "1", "2", "3" };

  //result = { "1", "2", "3" };

  //return std::move( result );
  return result;

}

KISS::Vector<KISS::CallStackFrame> test_return_02()
{

  KISS::Vector<KISS::CallStackFrame> result;

  result.reserve( 2 );

  result.push_back( KISS::CallStackFrame( "1", "2", "3" ) );
  result.push_back( KISS::CallStackFrame( "4", "5", "6" ) );
  //result.emplace_back( KISS::CallStackFrame( "1", "2", "3" ) );

  return result;

}

std::vector<KISS::CallStackFrame> test_return_03()
{

  std::vector<KISS::CallStackFrame> result;

  result.reserve( 2 );

  result.push_back( std::move( KISS::CallStackFrame( "1", "2", "3" ) ) );
  //result.push_back( std::move( KISS::CallStackFrame( "4", "5", "6" ) ) );
  //result.emplace_back( KISS::CallStackFrame( "1", "2", "3" ) );

  return result;

}

int main(int argc, char *argv[])
{

  #ifdef __DEBUG__

    __DEBUG_OUT__ << "[begin][--function--][main][u] int main(int argc, char *argv[]) //** **" << std::endl << std::endl;

  #endif

  // KISS::CallStackFrame test_01 = test_return_01();

  // __DEBUG_OUT__ << std::endl;
  // __DEBUG_OUT__ << "*******************" << std::endl;
  // __DEBUG_OUT__ << "Function:" << test_01.get_function() << std::endl;
  // __DEBUG_OUT__ << "File:" << test_01.get_file() << std::endl;
  // __DEBUG_OUT__ << "Line:" << test_01.get_line() << std::endl << std::endl;
  // __DEBUG_OUT__ << "*******************" << std::endl;
  // __DEBUG_OUT__ << std::endl;

  KISS::Vector<KISS::CallStackFrame> test_02 = test_return_02();
  //std::vector<KISS::CallStackFrame> test_02 = test_return_03();

  for ( const KISS::CallStackFrame& call_stack_frame: test_02 ) {

    __DEBUG_OUT__ << call_stack_frame;

    // __DEBUG_OUT__ << std::endl;
    // __DEBUG_OUT__ << __INDENT << "*******************" << std::endl;
    // __DEBUG_OUT__ << __INDENT << "Function:" << call_stack_frame.get_function() << std::endl;
    // __DEBUG_OUT__ << __INDENT << "File:" << call_stack_frame.get_file() << std::endl;
    // __DEBUG_OUT__ << __INDENT << "Line:" << call_stack_frame.get_line() << std::endl << std::endl;
    // __DEBUG_OUT__ << __INDENT << "*******************" << std::endl;

  }

  global_thread_call_stack[ "1111" ] = std::move( test_02 ); //Move the elements
  //global_thread_call_stack[ "1111" ] = test_02; //Copy all elements

  for ( const KISS::CallStackFrame& call_stack_frame: global_thread_call_stack[ "1111" ] ) {

    __DEBUG_OUT__ << call_stack_frame;

    // __DEBUG_OUT__ << std::endl;
    // __DEBUG_OUT__ << __INDENT << "*******************" << std::endl;
    // __DEBUG_OUT__ << __INDENT << "Function:" << call_stack_frame.get_function() << std::endl;
    // __DEBUG_OUT__ << __INDENT << "File:" << call_stack_frame.get_file() << std::endl;
    // __DEBUG_OUT__ << __INDENT << "Line:" << call_stack_frame.get_line() << std::endl << std::endl;
    // __DEBUG_OUT__ << __INDENT << "*******************" << std::endl;

  }

  #ifdef __DEBUG__

    if ( test_02.size() > 0 ) {

      __DEBUG_OUT__ << std::endl;

    }

    __DEBUG_OUT__ << "[end][--function--][main][u] int main(int argc, char *argv[]) //** **" << std::endl << std::endl;

  #endif

  return 0;

}
