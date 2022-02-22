#pragma once

#include <string>
#include <thread>
#include <iostream>
#include <sstream>
#include <syncstream>
#include <cstring>
#include <atomic>
#include <sys/time.h>

//#include "String.hpp"

#include "CommonMacros.hpp"

#include "CommonConstants.hpp"

namespace CommonUtilities {

std::atomic<uint32_t> call_left_indent = 0;
static char call_left_indent_buffer[ 150 ] = "";

// inline const  fill_string( int count, char char_fill ) {

//   return std::string( count, char_fill ).c_str();

// }

int init( const char *thread_name, const int argc, const char *argv[] ) {

  memset( call_left_indent_buffer, 0, sizeof( call_left_indent_buffer ) );

  return 0;

}

int finish( const char *thread_name, const int argc, const char *argv[] ) {

  return 0;

}

const std::string get_thread_id() noexcept
{

  auto thread_id = std::this_thread::get_id();
  std::stringstream ss;
  ss << thread_id;
  return ss.str();

}

bool replace( std::string& str, const std::string& from, const std::string& to )
{

  size_t start_pos = str.find( from );

  if ( start_pos == std::string::npos )
  {

    return false;

  }

  str.replace( start_pos, from.length(), to );

  return true;

}

void replace_all( std::string& str, const std::string& from, const std::string& to )
{

  if ( from.empty() )
  {

    return;

  }

  size_t start_pos = 0;

  while ( ( start_pos = str.find( from, start_pos ) ) != std::string::npos )
  {

    str.replace( start_pos, from.length(), to );
    start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'

  }

}

std::string exec( const char* cmd )
{

  std::string result {};

  std::array<char, 256> buffer;

  std::unique_ptr<FILE, decltype(&pclose)> pipe( popen(cmd, "r"), pclose );

  if ( !pipe )
  {

    throw std::runtime_error( "popen() failed!" );

  }

  while ( fgets( buffer.data(), buffer.size(), pipe.get() ) != nullptr )
  {

    result += buffer.data();

  }

  return result;

}

/* Resolve symbol name and source location given the path to the executable
   and an address */
std::string addr_to_line( const char *program_name, const char *addr )
{

  char addr2line_cmd[ 512 ] = { 0 };

  /* have addr2line map the address to the relent line in the code */
  #ifdef __APPLE__
    /* apple does things differently... */
    sprintf(addr2line_cmd,"atos -o %.256s %p", program_name, addr);
  #else
    sprintf(addr2line_cmd,"/usr/bin/addr2line -f -p -e \"%.256s\" \"%s\"", program_name, addr );
  #endif

  return exec( addr2line_cmd );

}

const std::string get_local_date_time( std::string format = _DATETIME_FORMAT_01 ) noexcept
{

  /*
  time_t     now = time(0);
  struct tm  tstruct;
  char       buf[80];
  tstruct = *localtime(&now);

  strftime(buf, sizeof(buf), format.c_str(), &tstruct);

  return buf;
  */

  std::string time_stamp;
  char buffer_time_stamp[ 100 ];
  memset( buffer_time_stamp, 0, sizeof( buffer_time_stamp ) );

  struct timeval tv;
  struct tm *tm;

  gettimeofday( &tv, NULL );

  tm = localtime( &tv.tv_sec );

  strftime( buffer_time_stamp,
            sizeof( buffer_time_stamp ),
            format.c_str(),
            tm );

  // sprintf(acTimestamp, "%04d-%02d-%02dT%02d:%02d:%02d.%03d\n",
  //           tm->tm_year + 1900,
  //           tm->tm_mon + 1,
  //           tm->tm_mday,
  //           tm->tm_hour,
  //           tm->tm_min,
  //           tm->tm_sec,
  //           (int) (tv.tv_usec / 1000)
  //       );

  time_stamp = buffer_time_stamp;

  if ( format.find( "%:z" ) != std::string::npos ) {

    char buffer_time_zone_offset[20];

    memset( buffer_time_zone_offset, 0, sizeof( buffer_time_zone_offset ) );

    strftime( buffer_time_zone_offset,
              sizeof( buffer_time_zone_offset ),
              "%z",
              tm );

    buffer_time_zone_offset[ 5 ] = buffer_time_zone_offset[ 4 ];
    buffer_time_zone_offset[ 4 ] = buffer_time_zone_offset[ 3 ];
    buffer_time_zone_offset[ 3 ] = ':';

    CommonUtilities::replace( time_stamp,
                              std::string( "%:z" ),
                              buffer_time_zone_offset );

  }

  CommonUtilities::replace( time_stamp,
                            std::string( "%SSS" ),
                            std::to_string( (int) ( tv.tv_usec / 1000 ) ) );

  return time_stamp;

}

}
