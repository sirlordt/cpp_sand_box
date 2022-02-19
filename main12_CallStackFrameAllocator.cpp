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

  // NSTD::CallStackFrame instance { "FUNCTION",
  //                                 "FILE",
  //                                 "LINE" };

  //using AllocatorCallStackFrameTraits = std::allocator_traits<decltype(allocatorCallStackFrame)>; // The matching trait

 	//NSTD::CallStackFrame* p = allocator.allocate( 3 );

  constexpr uint8_t call_stack_fames = 4;

  NSTD::CallStackFrame* p1 = AllocatorCallStackFrameTraits::allocate( allocatorCallStackFrame, call_stack_fames );

  //Valgrind complaints about if you not inizialite the buffer calling to constructors, "conditional jump or move depends on uninitialised value(s)"
	for( int i = 0; i < call_stack_fames; ++i ) {

    AllocatorCallStackFrameTraits::construct( allocatorCallStackFrame, &p1[ i ], NSTD::CallStackFrame() );       // construct the class CallStackFrame

  }

  int offset = 0;

	for( int i = 0; i < call_stack_fames; ++i ) {

    NSTD::CallStackFrame instance { ( "0-" + std::to_string( offset + 1 ) ).c_str(),
                                    ( "1-1-" + std::to_string( offset + 2 ) ).c_str(),
                                    ( "2-2-2-" + std::to_string( offset + 3 ) ).c_str() };

    std::cout << instance << std::endl;

    AllocatorCallStackFrameTraits::construct( allocatorCallStackFrame, &p1[ i ], std::move( instance ) );       // construct the int
    //allocator.construct( &p[ i ], std::move( instance ) );

    offset += 3;

  };

	for( int i = 0; i < call_stack_fames; ++i ) {

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

  AllocatorCallStackFrameTraits::deallocate( allocatorCallStackFrame, p1, call_stack_fames );

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
