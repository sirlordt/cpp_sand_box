#include <iostream>
//#include <string>
//#include <cstring>
#include <utility>
#include <map>

//#include "String.hpp"

#include "CommonUtilities.hpp"

#include "Exception.hpp"

#define __DEBUG__     true
#define __DEBUG_OUT__ std::cout

int main( int argc, char *argv[] )
{

  #ifdef __DEBUG__

    __DEBUG_OUT__ << "[begin][--function--][main][u] int main(int argc, char *argv[]) //** **" << std::endl << std::endl;

  #endif

  CommonUtilities::init( argc, argv );

//   __INC_INDENT

// // memset( CommonUtilities::call_left_indent_buffer + CommonUtilities::call_left_indent, \
// //                                        '~', \
// //                                        CommonUtilities::call_left_indent + 2 ); \
// //                                CommonUtilities::call_left_indent += 2;

//   __DEBUG_OUT__ << __INDENT << "Test01" << std::endl;

//   __INC_INDENT
// // memset( CommonUtilities::call_left_indent_buffer + CommonUtilities::call_left_indent, \
// //                                        '~', \
// //                                        2 ); \
// //                                CommonUtilities::call_left_indent += 2;

//   __DEBUG_OUT__ << __INDENT << "Test02" << std::endl;

// // memset( CommonUtilities::call_left_indent_buffer + CommonUtilities::call_left_indent - 2, \
// //                                        0, \
// //                                        CommonUtilities::call_left_indent ); \
// //                                CommonUtilities::call_left_indent -= 2;

//   __DEC_INDENT

//   __DEBUG_OUT__ << __INDENT << "Test03" << std::endl;

  NSTD::Exception exception { "thread_id", "thread_name", 11, "AAAA", "Test01", "Now" };

  //NSTD::String place_holder { 10, ' ' };

  // char buffer[ 150 ];

  // memset( buffer, 0, 150 );

  // memset( buffer, 'x', 10 );

  // //place_holder.replace( 0, 10, "x" );

  // __DEBUG_OUT__ << buffer << "Test01" << std::endl;

  // memset( buffer, 0, 150 );

  // memset( buffer, 'x', 5 );

  // __DEBUG_OUT__ << buffer << "Test02" << std::endl;
  //std::any x = exception[ "thread_id" ];

  CommonUtilities::finish();

  #ifdef __DEBUG__

    __DEBUG_OUT__ << "[end][--function--][main][u] int main(int argc, char *argv[]) //** **" << std::endl << std::endl;

  #endif

}
