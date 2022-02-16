#include <iostream>
//#include <string>
//#include <cstring>
#include <utility>
#include <map>
#include <stdexcept>
#include <memory>

#include "CallStackFrame.hpp"

#define __DEBUG__     true
#define __DEBUG_OUT__ std::cout

#include <memory>

template<class T, class A = std::allocator<T>>
class VectorAllocator
{

	int sz;		// number of elements
	T* elem;	// address of first element
	int space;	// number of elements plus free-space
			// slots for new elements

	A a;
  using AllocatorTraits = std::allocator_traits<decltype(a)>; // The matching trait

public:

	VectorAllocator() : sz( 0 ), elem( 0 ), space( 0 ) {

    //

  }
	VectorAllocator( int s ) : sz( 0 ), elem( 0 ), space( 0 ) {

		//for(int i = 0; i < sz; ++i) elem[i] = 0;
    //, elem(new T[s])
    reserve( s );

	}

	VectorAllocator( const VectorAllocator& );
	VectorAllocator& operator=( const VectorAllocator& v );

	~VectorAllocator() {

    //a.destroy( elem );
    delete elem;

  }

	T& at( int n );
	const T& at( int n ) const;

	T& operator[]( int n );
	const T& operator[]( int n ) const;

	int size() const { return sz; }
	int capacity() const { return space; }

	void reserve( int alloc_size );
	void resize( int resize_size ); //, const T& val);
	void push_back( const T& d );

};

template<class T, class A >
void VectorAllocator< T, A >::reserve( int alloc_size ) {

	if ( alloc_size <= space ) return;

  T* p = AllocatorTraits::allocate( a, alloc_size );
	//T* p = a.allocate( alloc_size );

	for( int i = 0; i < sz; ++i ) {

    AllocatorTraits::construct( a, &p[ i ], std::move( elem[ i ] ) );
    //a.construct( &p[ i ], std::move( elem[ i ] ) ); //Work to C++17 in C++20 not work anymore

  };

	for( int i = space; i < alloc_size; ++i ) {

    AllocatorTraits::construct( a, &p[ i ], T() ); //Force to init the memory chunk block with the class/struct contructor
    //a.construct( &p[ i ], T() ); //Work to C++17 in C++20 not work anymore

  }

	for ( int i = 0; i < sz; ++i ) {

    AllocatorTraits::destroy( a, &elem[ i ] );
    //a.destroy( &elem[ i ] ); //Work to C++17 in C++20 not work anymore

  };

  AllocatorTraits::deallocate( a, elem, space );
	//a.deallocate( elem, space );

	elem = p;
	space = alloc_size;

}

template<class T, class A>
T& VectorAllocator<T,A>::at( int n ) {

	if ( n < 0 || sz <= n ) throw std::out_of_range( "" );

  return elem[ n ];

}

template<class T, class A>
const T& VectorAllocator<T,A>::at( int n ) const {

	if( n < 0 || sz <= n ) throw std::out_of_range( "" );

  return elem[ n ];

}

template<class T, class A>
T& VectorAllocator<T,A>::operator[]( int n ) {

	return elem[ n ];

}

template<class T, class A>
const T& VectorAllocator<T,A>::operator[]( int n ) const {

	return elem[ n ];

}

//Copy assignation
template<class T, class A>
VectorAllocator<T,A>& VectorAllocator<T,A>::operator=( const VectorAllocator<T,A>& v ) {

	if( this == &v ) return *this;

	if( v.sz <= space ) {

		for( int i = 0; i < v.sz; ++i ) {

    	elem[ i ] = v.elem[ i ];

    }

    sz = v.sz;

    return *this;

	}

  T* p = AllocatorTraits::allocate( a, v.sz );
	//T* p = a.allocate( v.sz );

  for( int i = 0; i < v.sz; ++i ) {

    AllocatorTraits::construct( a, &p[ i ], v.elem[ i ] ); //Call to copy contructor from contained class
    //a.construct( &p[ i ], v.elem[ i ] ); //Work to C++17 in C++20 not work anymore

  }

  for( int i = 0; i < sz; ++i ) {

    AllocatorTraits::destroy( a, &elem[ i ] );
    //a.destroy( &elem[ i ] ); //Work to C++17 in C++20 not work anymore

  }

  AllocatorTraits::deallocate( a, elem, space );
  //a.deallocate( elem, space );

	space = sz = v.sz;
	elem = p;

  return *this;

}

template<class T, class A>
void VectorAllocator<T,A>::resize( int resize_size ) { //, const T& val ){

	reserve( resize_size );

	// for( int i = 0; i < resize_size; ++i ) {

  //   a.construct( &elem[ i ] );

  // }

	// for( int i = 0; i < resize_size; ++i ) {

  //   a.destroy( &elem[ i ] );

  // }

	sz = resize_size;

}

template<class T, class A>
void VectorAllocator<T,A>::push_back( const T& d ){

	if( space == 0 ) {

  	reserve( 10 );

  }
	else if( sz == space ) {

  	reserve( 2 * space );

  }

	elem[ sz ] = d;

	++sz;

}

int main( int argc, char *argv[] )
{

  #ifdef __DEBUG__

    __DEBUG_OUT__ << "[begin][--function--][main][u] int main(int argc, char *argv[]) //** **" << std::endl << std::endl;

  #endif

  CommonUtilities::init( argc, argv );

  VectorAllocator<NSTD::CallStackFrame> vector_csf_1( 5 );
  //cv.reserve( 5 );
	vector_csf_1.resize( 5 );

  //NSTD::CallStackFrame call_stack_frame { "1", "2", "3" };

  //vector_csf_1[ 0 ] = std::move( call_stack_frame );

  std::cout << "First loop:" << std::endl;

	for(int i = 0; i < vector_csf_1.size(); ++i) {

    const auto& value = vector_csf_1[ i ];

  	std::cout << value << std::endl << std::endl;

	}

  std::cout << "Second loop:" << std::endl;

	//cv.reserve( 10 );
  vector_csf_1.resize( 10 );

	for(int i = 0; i < vector_csf_1.size(); ++i) {

    const auto& value = vector_csf_1[ i ];

  	std::cout << value << std::endl << std::endl;

	}

  VectorAllocator<NSTD::CallStackFrame> vector_csf_2( 5 );

  vector_csf_2.resize( 5 );

  vector_csf_2 = vector_csf_1;

  // vector<double> dv(5);
  // //dv.push_back(  );
	// dv.resize( 5 );

  // dv[ 0 ] = 4.55;
  // dv[ 1 ] = 5.55;
  // dv[ 2 ] = 6.55;
  // dv[ 3 ] = 7.55;
  // dv[ 4 ] = 8.55;

	// for(int i = 0; i < dv.size(); ++i) {

  //   double value = dv[ i ];

  // 	std::cout << value << " " << std::endl << std::endl;

	// }

  #ifdef __DEBUG__

    __DEBUG_OUT__ << "[end][--function--][main][u] int main(int argc, char *argv[]) //** **" << std::endl << std::endl;

  #endif

}
