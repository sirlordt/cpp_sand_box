#include <iostream>
//#include <string>
//#include <cstring>
#include <utility>
#include <map>

//#include "String.hpp"

//#include "CommonUtilities.hpp"

//#include "Exception.hpp"

#define __DEBUG__     true
#define __DEBUG_OUT__ std::cout

//Generic
template <typename X>
X min( X a, X b ) {

  return a <= b ? a: b;

}

//Specialization float
template <>
float min( float a, float b ) {

  return a <= b ? a: b;

}

int main( int argc, char *argv[] )
{

  #ifdef __DEBUG__

    __DEBUG_OUT__ << "[begin][--function--][main][u] int main(int argc, char *argv[]) //** **" << std::endl << std::endl;

  #endif

  __DEBUG_OUT__ << min( 10, 20 ) << std::endl << std::endl;

  __DEBUG_OUT__ << min<float>( 5.28, 13.90 ) << std::endl << std::endl;

  #ifdef __DEBUG__

    __DEBUG_OUT__ << "[end][--function--][main][u] int main(int argc, char *argv[]) //** **" << std::endl << std::endl;

  #endif

}
