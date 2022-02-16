//#ifndef VECTOR_H_INCLUDED
//#define VECTOR_H_INCLUDED
#pragma once

//https://codereview.stackexchange.com/questions/240457/stdvector-implementation-c

#include <cstddef>
#include <initializer_list>
#include <math.h>


namespace NSTD {

  #define __DEBUG_VECTOR__     true
  #define __DEBUG_VECTOR_OUT__ std::cout

  template<typename T>
  class Vector
  {
    inline static uint32_t instance_id_count = 0;

    u_int32_t instance_id = 0;
    T* values = nullptr;
    size_t  v_size = 0;
    size_t  v_capacity = 0;

  public:

    using iterator = T*;
    using const_iterator = const T*;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    Vector();
    Vector( size_t sz);
    Vector( size_t sz, const T& v );
    Vector( const std::initializer_list<T>& i_list );
    Vector( const Vector& );
    Vector( Vector&& );

    ~Vector()
    {

      __INC_INDENT

      #ifdef __DEBUG_VECTOR__

        __DEBUG_VECTOR_OUT__ << __INDENT <<  "[begin][~~destructor~~][" << instance_id << "] Vector::~Vector() //** **" << std::endl;

        __INC_INDENT_W_OFFSET( 2 )

        __DEBUG_VECTOR_OUT__ << __INDENT << std::endl;
        __DEBUG_VECTOR_OUT__ << __INDENT << "*******************" << std::endl;

        if ( values ) {

          __DEBUG_VECTOR_OUT__ << __INDENT << "Values: " << static_cast<void*>(values) << std::endl;

        }
        else {

          __DEBUG_VECTOR_OUT__ << __INDENT << "Values: nullptr" << std::endl;

        }

        __DEBUG_VECTOR_OUT__ << __INDENT << "*******************" << std::endl;
        __DEBUG_VECTOR_OUT__ << __INDENT << std::endl;

        __DEC_INDENT_W_OFFSET( 2 )

      #endif

      if ( values ) {

        delete []values;

      }

      #ifdef __DEBUG_VECTOR__

        __DEBUG_VECTOR_OUT__ << __INDENT << "[end][~~destructor~~][" << instance_id << "] Vector::~Vector() //** **" << std::endl << std::endl;

      #endif

      __DEC_INDENT

    }

    Vector<T>& operator=( const Vector<T>& );
    Vector<T>& operator=( Vector<T>&& ) noexcept;

    // element access
    const T& front() const;
    T& front(); // actually I don't see why would we need this function to be a reference, I think it should be only a const reference, any insight?
    const T& back() const;
    T& back();
    T& operator[]( size_t i );
    const T& operator[]( size_t i ) const;
    T& at( size_t i );
    const T& at( size_t i ) const;
    constexpr T* data() noexcept;
    constexpr const T* data() const noexcept;

    // iterators
    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator  cend() const;
    reverse_iterator rbegin() noexcept;
    const_reverse_iterator crbegin() const noexcept;
    reverse_iterator rend() noexcept;
    const_reverse_iterator crend() const noexcept;

    // Modifiers

    template<typename... ARGS>
    void emplace_back( ARGS&&... args ); // since C++17 the std::vector::emplace_back() function type is a reference T&, why is that? what does this change brings to the table?

    template<typename... ARGS>
    iterator emplace( const T* pos, ARGS&&... args );

    iterator insert( iterator pos, const T& v );
    iterator insert( const_iterator pos, const T& v );
    iterator insert( const_iterator pos, T&& v );
    void insert( iterator pos, size_t n, const T& v );
    iterator insert( const_iterator pos, size_t n, const T& v );
    void push_back( const T& v );
    void push_back( T&& v );
    void pop_back();
    iterator erase( const_iterator pos );
    iterator erase( iterator first, iterator last );
    void clear() noexcept;
    void resize( size_t n );
    void resize( size_t n, const T& v );

    // capacity
    int size() const noexcept;
    int capacity() const noexcept;
    constexpr bool empty() const noexcept;
    void reserve( size_t n );
    void shrink_to_fit();

    // Non-Member Functions
    template<typename H> friend bool operator==( const Vector<H>& lhs, const Vector<H>& rhs );

    // see https://stackoverflow.com/questions/3279543/what-is-the-copy-and-swap-idiom
    friend void swap( Vector& first, Vector& second )
    {

      __INC_INDENT

      #ifdef __DEBUG_VECTOR__

        __DEBUG_VECTOR_OUT__ << __INDENT << "[begin][--member--][u] friend void swap( Vector& first: " << &first << ", Vector& second: " << &second << " ) //**  **" << std::endl << std::endl;

      #endif

      using std::swap;

      swap( first.v_size, second.v_size );
      swap( first.v_capacity, second.v_capacity );
      swap( first.values, second.values );

      #ifdef __DEBUG_VECTOR__

        __DEBUG_VECTOR_OUT__ << __INDENT << "[end][--member--][u] friend void swap( Vector& first: " << &first << ", Vector& second: " << &second << " ) //**  **" << std::endl << std::endl;

      #endif

      __DEC_INDENT

    }

    inline virtual uint32_t get_instance_id() const {

      return instance_id;

    }

    inline virtual uint32_t get_instance_id_count() const {

      return instance_id_count;

    }

    inline virtual const char* get_class_name() const {

      return "Vector";

    }

  private:

    bool ctor_initialized = false;

    void reallocate();

  };

  template<typename T>
  inline Vector<T>::Vector()
  {

    __INC_INDENT

    instance_id = instance_id_count++;

    v_size = 0;
    v_capacity = 0;
    values = nullptr;

    #ifdef __DEBUG_VECTOR__

      __DEBUG_VECTOR_OUT__ << __INDENT << "[->][++constructor++][" << instance_id << "] Vector<T>::Vector() //** Default **" << std::endl << std::endl;

    #endif

    __DEC_INDENT

  }

  template<typename T>
  inline Vector<T>::Vector( size_t sz )
  {

    __INC_INDENT

    instance_id = instance_id_count++;

    ctor_initialized = true;

    v_size = sz;
    v_capacity = sz;

    values = new T[v_capacity];

    for ( int i = 0; i < sz; ++i ) {

      values[ i ] = T();

    }

    #ifdef __DEBUG_VECTOR__

      __DEBUG_VECTOR_OUT__ << __INDENT << "[->][++constructor++][" << instance_id << "] Vector<T>::Vector( size_t sz: " << sz << " ) //**  **" << std::endl << std::endl;

    #endif

    __DEC_INDENT

  }

  template<typename T>
  inline Vector<T>::Vector( size_t sz, const T& v )
  {

    __INC_INDENT

    instance_id = instance_id_count++;

    ctor_initialized = true;

    v_size = sz;
    v_capacity = sz;

    values = new T[v_capacity];

    for ( int i = 0; i < sz; ++i ) {

      values[ i ] = v;

    }

    #ifdef __DEBUG_VECTOR__

      __DEBUG_VECTOR_OUT__ << __INDENT << "[->][++constructor++][" << instance_id << "] Vector<T>::Vector( size_t sz:" << sz << ", const T& v: " << &v << " ) //**  **" << std::endl << std::endl;

    #endif

    __DEC_INDENT

  }

  template<typename T>
  inline Vector<T>::Vector( const std::initializer_list<T>& i_list )
  {

    __INC_INDENT

    instance_id = instance_id_count++;

    int sz = i_list.size();

    v_size = sz;
    v_capacity = sz;

    values = new T[v_capacity];

    for( auto iter = i_list.begin(), i = 0; iter != i_list.end(); ++i, ++iter ) {

      values[ i ] = *iter;

    }

    #ifdef __DEBUG_VECTOR__

      __DEBUG_VECTOR_OUT__ << __INDENT << "[->][++constructor++][" << instance_id << "] Vector<T>::Vector( const std::initializer_list<T>& i_list: " << &i_list << " ) //** ??? **" << std::endl << std::endl;

    #endif

    __DEC_INDENT

  }

  //Copy contructor
  template<typename T>
  inline Vector<T>::Vector( const Vector<T>& src ) :
    v_size(src.v_size),
    v_capacity(src.v_capacity) //,
    //values(new T[v_capacity]) //This generate a warning in Valgrind and the compiler (Unizializated value), and create a lot of instances
  {

    __INC_INDENT

    instance_id = instance_id_count++;

    values = new T[v_capacity];

    for ( int i = 0; i < v_size; ++i ) {

      values[ i ] = src.values[ i ];

    }

    #ifdef __DEBUG_VECTOR__

      __DEBUG_VECTOR_OUT__ << __INDENT << "[->][++constructor++][" << instance_id << "] Vector<T>::Vector( const Vector<T>& src: " << &src << " ) //** Copy const reference **" << std::endl << std::endl;

    #endif

    __DEC_INDENT

  }

  template<typename T>
  inline Vector<T>::Vector( Vector<T>&& mv )
  {

    __INC_INDENT

    instance_id = instance_id_count++;

    #ifdef __DEBUG_VECTOR__

      __DEBUG_VECTOR_OUT__ << __INDENT << "[begin][++constructor++][" << instance_id << "] Vector<T>::Vector( Vector<T>&& mv: " << &mv << " ) //** Move reference **" << std::endl << std::endl;

    #endif

    swap( *this, mv );

    #ifdef __DEBUG_VECTOR__

      __DEBUG_VECTOR_OUT__ << __INDENT << "[end][++constructor++][" << instance_id << "] Vector<T>::Vector( Vector<T>&& mv: " << &mv << " ) //** Move reference **" << std::endl << std::endl;

    #endif

    __DEC_INDENT

  }

  template<typename T>
  inline Vector<T>& Vector<T>::operator=( const Vector<T>& src )
  {

    __INC_INDENT

    #ifdef __DEBUG_VECTOR__

      __DEBUG_VECTOR_OUT__ << __INDENT << "[begin][**operator=**][" << instance_id << "] Vector<T>& Vector<T>::operator=( const Vector<T>& src: " << &src << " ) //** Copy const reference **" << std::endl << std::endl;

    #endif

    if ( values ) {

      delete [] values;

    }

    v_capacity = src.v_capacity;
    v_size = src.v_size;

    values = new T[v_capacity];

    for ( int i = 0; i < v_size; ++i ) {

      values[ i ] = src.values[ i ]; //Copy elements

    }

    //swap( *this, src );

    #ifdef __DEBUG_VECTOR__

      __DEBUG_VECTOR_OUT__ << __INDENT << "[end][**operator=**][" << instance_id << "] Vector<T>& Vector<T>::operator=( const Vector<T>& src: " << &src << " ) //** Copy const reference **" << std::endl << std::endl;

    #endif

    __DEC_INDENT

    return *this;

  }

  template<typename T>
  inline Vector<T>& Vector<T>::operator=( Vector<T>&& mv ) noexcept
  {

    __INC_INDENT

    #ifdef __DEBUG_VECTOR__

      __DEBUG_VECTOR_OUT__ << __INDENT << "[begin][**operator=**][" << instance_id << "] Vector<T>& Vector<T>::operator=( Vector<T>&& mv: " << &mv << " ) noexcept //** Move const reference **" << std::endl << std::endl;

    #endif

    swap( *this, mv );

    #ifdef __DEBUG_VECTOR__

      __DEBUG_VECTOR_OUT__ << __INDENT << "[end][**operator=**][" << instance_id << "] Vector<T>& Vector<T>::operator=( Vector<T>&& mv: " << &mv << " ) noexcept //** Move const reference **" << std::endl << std::endl;

    #endif

    __DEC_INDENT

    return *this;

  }

  template<typename T>
  inline const T& Vector<T>::back() const
  {

    return values[ v_size - 1 ];

  }

  template<typename T>
  inline T& Vector<T>::back()
  {

    return values[ v_size - 1 ];

  }

  template<typename T>
  inline const T& Vector<T>::front() const
  {

    return values[ 0 ];

  }

  template<typename T>
  inline T& Vector<T>::front()
  {

    return values[ 0 ];

  }

  template<typename T>
  inline typename Vector<T>::iterator Vector<T>::begin() noexcept
  {

    return values;

  }

  template<typename T>
  inline typename Vector<T>::const_iterator Vector<T>::begin() const noexcept
  {

    return values;

  }

  template<typename T>
  inline typename Vector<T>::iterator  Vector<T>::end() noexcept
  {

    return values + v_size;

  }

  template<typename T>
  inline typename Vector<T>::const_iterator  Vector<T>::end() const noexcept
  {

    return values + v_size;

  }

  template<typename T>
  inline typename Vector<T>::const_iterator Vector<T>::cbegin() const noexcept
  {

    return values;

  }

  template<typename T>
  inline typename Vector<T>::const_iterator  Vector<T>::cend() const
  {

    return values + v_size;

  }

  template<typename T>
  inline typename Vector<T>::reverse_iterator Vector<T>::rbegin() noexcept
  {

    return reverse_iterator( end() );

  }

  template<typename T>
  inline typename Vector<T>::reverse_iterator Vector<T>::rend() noexcept
  {
    return reverse_iterator( begin() );
  }

  template<typename T>
  inline typename Vector<T>::const_reverse_iterator Vector<T>::crbegin() const noexcept
  {

    return rbegin();

  }

  template<typename T>
  inline typename Vector<T>::const_reverse_iterator Vector<T>::crend() const noexcept
  {

    return rend();

  }

  template<typename T>
  inline T& Vector<T>::operator[]( size_t i )
  {

    return values[ i ];

  }

  template<typename T>
  inline T& Vector<T>::at( size_t i )
  {

    if ( i >= v_size ) {

      throw std::runtime_error( "out of range exception" );

    }
    else {

      return values[ i ];

    }

  }

  template<typename T>
  inline const T& Vector<T>::operator[] ( size_t i ) const
  {

    return values[ i ];

  }

  template<typename T>
  inline const T& Vector<T>::at( size_t i ) const
  {

    if ( i >= v_size ) {

      throw std::runtime_error( "out of range exception" );

    }
    else {

      return values[ i ];

    }

  }

  template<typename T>
  inline constexpr T* Vector<T>::data() noexcept
  {

    return values;

  }

  template<typename T>
  inline constexpr const T* Vector<T>::data() const noexcept
  {

    return values;

  }

  template<typename T>
  template<typename... ARGS>
  void Vector<T>::emplace_back( ARGS&&... args )
  {

    if ( v_size == v_capacity )
    {

      if ( ctor_initialized )
      {

        v_capacity *= 2;

      }
      else
      {

        if ( v_size == 0 )
        {

          v_capacity = 1;

        }
        else if( v_size < 8 )
        {

          v_capacity++;

        }
        else if (v_size >= 8)
        {

          v_capacity *= 2;

        }

      }

      reallocate();

    }

    values[ v_size++ ] = std::move( T( std::forward<ARGS>( args )... ) );

  }

  template<typename T>
  template<typename... ARGS>
  inline typename Vector<T>::iterator Vector<T>::emplace( const T* pos, ARGS&&... args )
  {
    // I found a lot of examples implementing this function but they were confusing so I came up with this, is this ok?

    const size_t dist = pos - begin();

    if( dist == v_capacity )
    {

      emplace_back( T( std::forward<T>( args )... ) );

    }
    else
    {

      if( v_size == v_capacity )
      {

        v_capacity *= 2;

        reallocate();

      }

      std::move_backward( begin() + dist, end(), end() + 1 );

      iterator iter = &values[ dist ];

      *iter = std::move( T( std::forward<ARGS>( args )... ) );

      ++v_size;

      return iter;

    }

  }

  template<typename T>
  inline typename Vector<T>::iterator Vector<T>::insert( iterator pos, const T& v )
  {

    emplace( pos, v );

  }

  template<typename T>
  inline typename Vector<T>::iterator Vector<T>::insert( const_iterator pos, const T& v )
  {

    emplace( pos, v );

  }

  template<typename T>
  inline typename Vector<T>::iterator Vector<T>::insert( const_iterator pos, T&& v )
  {

    emplace( pos, std::forward<T>( v ) );

  }

  template<typename T>
  void Vector<T>::insert( iterator pos, size_t n, const T& v )
  {

    const size_t dist = pos - begin();

    if( v_size + n > v_capacity )
    {

      v_capacity *= 2;

      reallocate();

    }

    std::move_backward(begin() + dist, end(), end() + n);

    for( int i = dist; i < dist + n; ++i )
    {

      values[ i ] = v;

    }

    v_size += n;

  }

  template<typename T>
  inline typename Vector<T>::iterator Vector<T>::insert( const_iterator pos, size_t n, const T& v )
  {

    const size_t dist = pos - begin();

    if ( v_size + n > v_capacity )
    {

      v_capacity *= 2;

      reallocate();

    }

    T* iter = &values[ dist ];

    std::move_backward( begin() + dist, end(), end() + n );

    for ( int i = dist; i < dist + n; ++i )
    {

      *iter++ = v;

    }

    v_size += n;

    return &values[ dist ];

  }

  template<typename T>
  inline void Vector<T>::push_back( const T& v )
  {

    emplace_back( v );

  }

  template<typename T>
  inline void Vector<T>::push_back( T&& v )
  {

    emplace_back( std::forward<T>( v ) );

  }

  template<typename T>
  inline void Vector<T>::pop_back()
  {

    --v_size;

    // what if I use this below, what would be happening and what would be the difference??
    /*   values[--v_size].~T(); */

  }

  template<typename T>
  inline typename Vector<T>::iterator Vector<T>::erase( const_iterator pos )
  {
    /* I cloud use other implementation of this function that is
      pretty shorter than this but I chose this one that I camne up with, is this ok? */

    /*The reason why I chose this is because when I triy erasing on empty Vector and it doesn't
      crash like the std::vector, instead it just doesn't do anything and neither does it crach
      when you pass an iterator that is out of range. Not sure if this is good or bad. Any insight? */

    const size_t dist = pos - begin();

    if ( v_size != 0 )
    {

      --v_size;

    }

    int inc;

    for( inc = 2; v_size > pow( 2, inc ); ++inc );

    if ( v_size == 0 )
    {

      v_capacity = 0;

    }
    else
    {

      v_capacity = pow( 2, inc );

    }

    if( v_capacity != 0 )
    {

      T* temp = new T[v_capacity];

      for( int i = 0, j = 0; j <= v_size; ++j )
      {

        if ( j != dist )
        {
          temp[ i++ ] = values[ j ];
        }

      }

      delete [ ] values;
      values = temp;

    }

    return &values[ dist ];

  }

  template<typename T>
  inline typename Vector<T>::iterator Vector<T>::erase( iterator first, iterator last )
  {

    const size_t n = last - first;

    std::move(last, end(), first);

    v_size -= n;

  }

  template<typename T>
  inline void  Vector<T>::clear() noexcept
  {

    v_size = 0;

  }

  template<typename T>
  inline void Vector<T>::shrink_to_fit()
  {

    v_capacity = v_size;
    reallocate();

  }

  template<typename T>
  inline void Vector<T>::reserve( size_t n )
  {

    if ( n > v_capacity )
    {

      v_capacity = n;
      reallocate();

    }

  }

  template<typename T>
  inline void Vector<T>::resize( size_t n )
  {
    if( n > v_capacity )
    {

      ctor_initialized = true;

      v_capacity = n;
      reallocate();

    }

    v_size = n;

  }

  template<typename T>
  inline void Vector<T>::resize( size_t n, const T& v )
  {

    if ( n > v_capacity )
    {

      ctor_initialized = true;

      v_capacity = n;
      reallocate();

    }

    if ( n > v_size )
    {

      for( int i = v_size; i < n; ++i ) {

        values[ i ] = v;

      }

    }

    v_size = n;

  }

  template<typename T>
  inline int  Vector<T>::size() const noexcept
  {

    return v_size;

  }

  template<typename T>
  inline int  Vector<T>::capacity() const noexcept
  {

    return v_capacity;

  }

  template<typename T>
  inline constexpr bool  Vector<T>:: empty() const noexcept
  {

    return begin() == end();

  }

  template<typename T>
  inline void Vector<T>::reallocate()
  {

    T* temp = new T[ v_capacity ];

    for ( int i = 0; i < v_size; ++i ) {

      temp[ i ] = values[ i ];

    }

    delete[ ] values;
    values = temp;

  }

  template<typename H>
  inline bool operator==(const Vector<H>& lhs, const Vector<H>& rhs)
  {
    if( lhs.v_size != rhs.v_size) {

      return false;

    }

    for ( int i = 0; i < lhs.v_size; ++i ) {

      if ( lhs.values[ i ] != rhs.values[ i ] ) {

        return false;

      }

    }

    return true;

  }

}  //namespace NSTD

//#endif // VECTOR_H_INCLUDED
