#include <iostream>
//#include <string>
//#include <cstring>
#include <utility>
#include <map>

#include "String.hpp"

#include "CommonUtilities.hpp"

#define __DEBUG__     true
#define __DEBUG_OUT__ std::cout

// template <typename T>
// T read( std::istream& in )
// {

//   T t;
//   in >> t;

//   return t;

// }

int main(int argc, char *argv[])
{

  #ifdef __DEBUG__

    __DEBUG_OUT__ << "[begin][--function--][main][u] int main(int argc, char *argv[]) //** **" << std::endl << std::endl;

  #endif

  //  int n = 1;
  //  std::allocator<std::string> alloc;
  //  auto p = alloc.allocate(n);
  //  auto q = p;
  //  alloc.construct(q, read<std::string>(std::cin));

   //std::cout << *q << std::endl;

    NSTD::String str = NSTD::String( nullptr ); //NSTD::String( 10, 0 );
    //std::string str;

    // Taking string input using getline()
    //getline( std::cin, str );

    // Displaying string
    std::cout << "The initial string is : ";
    std::cout << str << std::endl;

    // Inserting a character
    str.push_back('s');

    // Displaying string
    std::cout << "The string after push_back operation is : ";
    std::cout << str << std::endl;

    // Deleting a character
    str.pop_back();

    // Displaying string
    std::cout << "The string after pop_back operation is : ";
    std::cout << str << std::endl;

    char c = str.at( 7 );

    std::cout << c << std::endl;

    //__DEBUG_OUT__ << str << std::endl;

  #ifdef __DEBUG__

    __DEBUG_OUT__ << "[end][--function--][main][u] int main(int argc, char *argv[]) //** **" << std::endl << std::endl;

  #endif

}
