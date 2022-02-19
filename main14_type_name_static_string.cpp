#include <iostream>
//#include <string>
//#include <cstring>
#include <utility>
#include <cstddef>
#include <stdexcept>
#include <cstring>
#include <ostream>

#ifndef _MSC_VER
#  if __cplusplus < 201103
#    define CONSTEXPR11_TN
#    define CONSTEXPR14_TN
#    define NOEXCEPT_TN
#  elif __cplusplus < 201402
#    define CONSTEXPR11_TN constexpr
#    define CONSTEXPR14_TN
#    define NOEXCEPT_TN noexcept
#  else
#    define CONSTEXPR11_TN constexpr
#    define CONSTEXPR14_TN constexpr
#    define NOEXCEPT_TN noexcept
#  endif
#else  // _MSC_VER
#  if _MSC_VER < 1900
#    define CONSTEXPR11_TN
#    define CONSTEXPR14_TN
#    define NOEXCEPT_TN
#  elif _MSC_VER < 2000
#    define CONSTEXPR11_TN constexpr
#    define CONSTEXPR14_TN
#    define NOEXCEPT_TN noexcept
#  else
#    define CONSTEXPR11_TN constexpr
#    define CONSTEXPR14_TN constexpr
#    define NOEXCEPT_TN noexcept
#  endif
#endif  // _MSC_VER

class StaticString
{

  const char* const p_;
  const std::size_t sz_;

public:

  typedef const char* const_iterator;

  template <std::size_t N>
  CONSTEXPR11_TN StaticString( const char (&a)[ N ] ) NOEXCEPT_TN :
        p_( a ),
        sz_( N-1 )
        {}

  CONSTEXPR11_TN StaticString( const char* p, std::size_t N ) NOEXCEPT_TN :
        p_( p ),
        sz_( N )
        {}

  CONSTEXPR11_TN const char* data() const NOEXCEPT_TN { return p_; }
  CONSTEXPR11_TN std::size_t size() const NOEXCEPT_TN { return sz_; }

  CONSTEXPR11_TN const_iterator begin() const NOEXCEPT_TN { return p_; }
  CONSTEXPR11_TN const_iterator end()   const NOEXCEPT_TN { return p_ + sz_; }

  CONSTEXPR11_TN char operator[]( std::size_t n ) const
  {

    return n < sz_ ? p_[n] : throw std::out_of_range( "static_string" );

  }

};

inline std::ostream& operator<<( std::ostream& os, const StaticString& s )
{

  return os.write( s.data(), s.size() );

}

template <class T>
CONSTEXPR14_TN StaticString type_name()
{

  #ifdef __clang__
    const StaticString p = __PRETTY_FUNCTION__;
    return StaticString( p.data() + 31, p.size() - 31 - 1 );
  #elif defined( __GNUC__ )
    StaticString p = __PRETTY_FUNCTION__;
    #if __cplusplus < 201402
      return StaticString( p.data() + 36, p.size() - 36 - 1 );
    #else
      return StaticString( p.data() + 46, p.size() - 46 - 1 );
    #endif
  #elif defined( _MSC_VER )
    StaticString p = __FUNCSIG__;
    return StaticString( p.data() + 38, p.size() - 38 - 7 );
  #endif

}

int& foo_lref() {

  static int x = 0;

  return x;

}

int&& foo_rref() {

  return 0;

}

int foo_value() {

  return 0;

}

int main( int argc, char *argv[] )
{

  int i = 0;
  const int ci = 0;
  int max = 10;

  StaticString temp = "XXXXXXXXXX";

  for ( int j = 0; j < max; j++ ) {

    StaticString temp2 = StaticString( temp.data(), ( temp.size() - max ) + j ); // + StaticString( "X" );

    // for ( int u = 0; u < temp.size(); u++ ) {

    //   temp.data() + 1 = 'X';

    // }

    std::cout << "Test01: " << temp2 << " " << StaticString( "World! " ) << std::to_string( j ) << std::endl;

  }

  std::cout << "decltype(i) is " << type_name<decltype(i)>() << '\n';
  std::cout << "decltype((i)) is " << type_name<decltype((i))>() << '\n';
  std::cout << "decltype(ci) is " << type_name<decltype(ci)>() << '\n';
  std::cout << "decltype((ci)) is " << type_name<decltype((ci))>() << '\n';
  std::cout << "decltype(static_cast<int&>(i)) is " << type_name<decltype(static_cast<int&>(i))>() << '\n';
  std::cout << "decltype(static_cast<int&&>(i)) is " << type_name<decltype(static_cast<int&&>(i))>() << '\n';
  std::cout << "decltype(static_cast<int>(i)) is " << type_name<decltype(static_cast<int>(i))>() << '\n';
  std::cout << "decltype(foo_lref()) is " << type_name<decltype(foo_lref())>() << '\n';
  std::cout << "decltype(foo_rref()) is " << type_name<decltype(foo_rref())>() << '\n';
  std::cout << "decltype(foo_value()) is " << type_name<decltype(foo_value())>() << '\n';

}
