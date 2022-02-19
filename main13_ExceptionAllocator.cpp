#include <iostream>
//#include <string>
//#include <cstring>
#include <utility>
#include <map>
#include <stdexcept>
#include <memory>

#include "CallStackFrame.hpp"
#include "Exception.hpp"

int main( int argc, char *argv[] )
{

 // default allocator for strings
        std::allocator<std::string> alloc;
        // matching traits
        using traits_t = std::allocator_traits<decltype(alloc)>;

        // Rebinding the allocator using the trait for strings gets the same type
        traits_t::rebind_alloc<std::string> alloc_ = alloc;

        std::string* p = traits_t::allocate(alloc, 2); // space for 2 strings

        traits_t::construct(alloc, p, "foo");
        traits_t::construct(alloc, p + 1, "bar");

        const auto x = p[0];

        std::cout << x << ' ' << p[1] << '\n';

        traits_t::destroy(alloc, p + 1);
        traits_t::destroy(alloc, p);
        traits_t::deallocate(alloc, p, 2);

  using AllocatorException = std::allocator<NSTD::Exception>;
  using AllocatorExceptionTraits = std::allocator_traits<AllocatorException>; // The matching trait

  AllocatorException allocatorException;

  //using AllocatorCallStackFrameTraits = std::allocator_traits<decltype(allocatorCallStackFrame)>; // The matching trait

 	//NSTD::CallStackFrame* p = allocator.allocate( 3 );

  constexpr uint8_t exceptions = 1;

  NSTD::Exception* p1 = AllocatorExceptionTraits::allocate( allocatorException, exceptions );

  //Valgrind complaints about if you not inizialite the buffer calling to constructors, "conditional jump or move depends on uninitialised value(s)"
	for( int i = 0; i < exceptions; ++i ) {

    // AllocatorExceptionTraits::construct( allocatorException, p1 + i, NSTD::Exception() );       // construct the class CallStackFrame
    AllocatorExceptionTraits::construct( allocatorException, &p1[ i ], NSTD::Exception() );       // construct the class CallStackFrame

  }

  int offset = 0;

	for( int i = 0; i < exceptions; ++i ) {

    NSTD::Vector<NSTD::CallStackFrame> call_stack
    {

      { //First element in the vector

        ( "function_" + std::to_string( offset + 1 ) ).c_str(),
        ( "file_" + std::to_string( offset + 2 ) ).c_str(),
        ( "line_" + std::to_string( offset + 3 ) ).c_str()

      }

    };

    // std::cout << "Here" << std::endl;
    // NSTD::CallStackFrame call_stack_frame { ( "function_" + std::to_string( offset + 1 ) ).c_str(),
    //                                         ( "file_" + std::to_string( offset + 2 ) ).c_str(),
    //                                         ( "line_" + std::to_string( offset + 3 ) ).c_str() };

    NSTD::Exception exception
    {

      ( "thread_id_" + std::to_string( offset + 4 ) ).c_str(),
      ( "thread_name_" + std::to_string( offset + 5 ) ).c_str(),
      offset + 1,
      ( "code_" + std::to_string( offset + 7 ) ).c_str(),
      ( "message_" + std::to_string( offset + 8 ) ).c_str(),
      ( "at_" + std::to_string( offset + 9 ) ).c_str(),
      &call_stack

    };

    //AllocatorExceptionTraits::construct( allocatorException, p1 + i, std::move( instance ) );       // construct the int
    AllocatorExceptionTraits::construct( allocatorException, &p1[ i ], std::move( exception ) );       // construct the int
    // //allocator.construct( &p[ i ], std::move( instance ) );

    offset += 9;

  };

  //std::cout << "Destroy begin" << std::endl;

	for( int i = 0; i < exceptions; ++i ) {

    //const NSTD::Exception& p = p1[ i ];

    //std::cout << p << std::endl;

    //AllocatorExceptionTraits::destroy( allocatorException, p1 + i );
    AllocatorExceptionTraits::destroy( allocatorException, &p1[ i ] );
    //allocator.destroy( &p[ i ] );

  }

  //std::cout << "Destroy end" << std::endl;

  // NSTD::CallStackFrame* p2 = AllocatorTraits::allocate( allocator, 3 );

	// for( int i = 0; i < 3; ++i ) {

  //   NSTD::CallStackFrame instance { std::to_string( offset + 1 ).c_str(),
  //                                   std::to_string( offset + 2 ).c_str(),
  //                                   std::to_string( offset + 3 ).c_str() };

  //   AllocatorTraits::construct( allocator, &p2[ i ], std::move( instance ) );       // construct the int
  //   //allocator.construct( &p[ i ], std::move( instance ) );

  //   offset += 3;

  // };

	// for( int i = 0; i < 3; ++i ) {

  //   AllocatorTraits::destroy( allocator, &p2[ i ] );
  //   //allocator.destroy( &p[ i ] );

  // }

  // AllocatorTraits::deallocate( allocator, p2, 3 );

  AllocatorExceptionTraits::deallocate( allocatorException, p1, exceptions );

  //***************************

  // using AllocatorConstChar = std::allocator<const char *>;
  // using AllocatorConstCharTraits = std::allocator_traits<AllocatorConstChar>; // The matching trait

  // AllocatorConstChar allocatorConstChar;

  // const char** p3 = AllocatorConstCharTraits::allocate( allocatorConstChar, 18 ); //reinterpret_cast<char *>(

  // const char** p4 = AllocatorConstCharTraits::allocate( allocatorConstChar, 50 ); //reinterpret_cast<char *>(

  // //const char *x = "hello world!";

  // AllocatorConstCharTraits::construct( allocatorConstChar, p3, "hello world p3!" );

  // std::cout << "*p3: " << *p3 << std::endl;
  // std::cout << "(*p3)[ 6 ]: " << (*p3)[ 6 ] << std::endl; //print w

  // // AllocatorConstCharTraits::destroy( allocatorConstChar, p3 );

  // // AllocatorConstCharTraits::construct( allocatorConstChar, p3, "hello again+!" );

  // // std::cout << *p3 << std::endl;

  // // std::cout << (*p3)[ 6 ] << std::endl;

  // // AllocatorConstCharTraits::destroy( allocatorConstChar, p3 );

  // // // AllocatorUInt8Traits::construct( allocatorUInt8, &p3[ 1 ], 'e' );

  // // // AllocatorUInt8Traits::construct( allocatorUInt8, &p3[ 2 ], 'l' );

  // // // AllocatorUInt8Traits::construct( allocatorUInt8, &p3[ 3 ], 'l' );

  // // // AllocatorUInt8Traits::construct( allocatorUInt8, &p3[ 4 ], 'o' );

  // AllocatorConstCharTraits::construct( allocatorConstChar, p4, "hello world p4!-1\0" );

  // AllocatorConstCharTraits::construct( allocatorConstChar, (p4 + 17), "hello world p4-2!\0" );

  // std::cout << "*p4-1: " << *p4 << std::endl;
  // std::cout << "(*p4)[ 1 ]: " << (*p4)[ 1 ] << std::endl; //print e
  // std::cout << "(*p4)[ 11 ] + (*p4)[ 12 ] + (*p4)[ 13 ] : " << (*p4)[ 12 ] << (*p4)[ 13 ] << (*p4)[ 14 ] << std::endl; //print p4!

  // std::cout << "*p4-2: " << *(p4 + 17) << std::endl;

  // std::cout << "(*p4-1+21)[ 21 ] : " << (*p4)[ 34 ] << std::endl; //print p4!

  // AllocatorConstCharTraits::destroy( allocatorConstChar, p3 );

  // AllocatorConstCharTraits::destroy( allocatorConstChar, p4 );

  // AllocatorConstCharTraits::destroy( allocatorConstChar, p4 + 16 );

  // AllocatorConstCharTraits::deallocate( allocatorConstChar, p4, 20 ); //reinterpret_cast<uint8_t *>(

  // AllocatorConstCharTraits::deallocate( allocatorConstChar, p3, 18 ); //reinterpret_cast<uint8_t *>(

}
