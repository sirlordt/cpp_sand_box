#include <iostream>
//#include <string>
//#include <cstring>
#include <utility>
#include <map>
#include <stdexcept>
#include <memory>

#include "CallStackFrame.hpp"

int main( int argc, char *argv[] )
{

  using AllocatorCallStackFrame = std::allocator<NSTD::CallStackFrame>;
  using AllocatorCallStackFrameTraits = std::allocator_traits<AllocatorCallStackFrame>; // The matching trait

  AllocatorCallStackFrame allocatorCallStackFrame;
  //using AllocatorCallStackFrameTraits = std::allocator_traits<decltype(allocatorCallStackFrame)>; // The matching trait

 	//NSTD::CallStackFrame* p = allocator.allocate( 3 );

  NSTD::CallStackFrame* p1 = AllocatorCallStackFrameTraits::allocate( allocatorCallStackFrame, 3 );

  int offset = 0;

	for( int i = 0; i < 3; ++i ) {

    NSTD::CallStackFrame instance { std::to_string( offset + 1 ).c_str(),
                                    std::to_string( offset + 2 ).c_str(),
                                    std::to_string( offset + 3 ).c_str() };

    AllocatorCallStackFrameTraits::construct( allocatorCallStackFrame, &p1[ i ], std::move( instance ) );       // construct the int
    //allocator.construct( &p[ i ], std::move( instance ) );

    offset += 3;

  };

	for( int i = 0; i < 3; ++i ) {

    AllocatorCallStackFrameTraits::destroy( allocatorCallStackFrame, &p1[ i ] );
    //allocator.destroy( &p[ i ] );

  }

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

  AllocatorCallStackFrameTraits::deallocate( allocatorCallStackFrame, p1, 3 );

  //***************************

  using AllocatorUInt8 = std::allocator<const char *>;
  using AllocatorUInt8Traits = std::allocator_traits<AllocatorUInt8>; // The matching trait

  AllocatorUInt8 allocatorUInt8;

  const char** p3 = AllocatorUInt8Traits::allocate( allocatorUInt8, 15 ); //reinterpret_cast<char *>(

  //const char *x = "hello world!";

  AllocatorUInt8Traits::construct( allocatorUInt8, p3, "hello world!" );

  std::cout << *p3 << std::endl;

  AllocatorUInt8Traits::destroy( allocatorUInt8, p3 );

  AllocatorUInt8Traits::construct( allocatorUInt8, p3, "hello again+!" );

  std::cout << *p3 << std::endl;

  std::cout << (*p3)[ 6 ] << std::endl;

  AllocatorUInt8Traits::destroy( allocatorUInt8, p3 );

  // AllocatorUInt8Traits::construct( allocatorUInt8, &p3[ 1 ], 'e' );

  // AllocatorUInt8Traits::construct( allocatorUInt8, &p3[ 2 ], 'l' );

  // AllocatorUInt8Traits::construct( allocatorUInt8, &p3[ 3 ], 'l' );

  // AllocatorUInt8Traits::construct( allocatorUInt8, &p3[ 4 ], 'o' );

  AllocatorUInt8Traits::deallocate( allocatorUInt8, p3, 4 ); //reinterpret_cast<uint8_t *>(

}
