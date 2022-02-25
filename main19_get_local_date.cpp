#define __STDC_WANT_LIB_EXT1__ 1
#define _XOPEN_SOURCE // for putenv
#include <time.h>
#include <stdio.h>
#include <stdlib.h>   // for putenv

#include <chrono>
#include <iostream>
#include <cstring>

#include <chrono>
#include <ctime>
#include <errno.h>

//Taken from glibc/time/offtime.c
//https://code.woboq.org/userspace/glibc/time/offtime.c.html#__offtime

#define  SECS_PER_HOUR         ( 60 * 60 )
#define  SECS_PER_DAY          ( SECS_PER_HOUR * 24 )
//#define __time64_t             time_t
#define DIV( a, b )            ( ( a ) / ( b ) - ( ( a ) % ( b ) < 0 ) )
#define LEAPS_THRU_END_OF( y ) ( DIV( y, 4 ) - DIV( y, 100 ) + DIV( y, 400 ) )

const unsigned short int __mon_yday[ 2 ][ 13 ] =
  {
    /* Normal years.  */
    { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 },
    /* Leap years.  */
    { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 }
  };

int off_time( time_t t, long int offset, struct tm *tp )
{

  time_t days, rem, y;
  const unsigned short int *ip;
  days = t / SECS_PER_DAY;
  rem = t % SECS_PER_DAY;
  rem += offset;
  while (rem < 0)
    {
      rem += SECS_PER_DAY;
      --days;
    }
  while (rem >= SECS_PER_DAY)
    {
      rem -= SECS_PER_DAY;
      ++days;
    }
  tp->tm_hour = rem / SECS_PER_HOUR;
  rem %= SECS_PER_HOUR;
  tp->tm_min = rem / 60;
  tp->tm_sec = rem % 60;
  /* January 1, 1970 was a Thursday.  */
  tp->tm_wday = (4 + days) % 7;
  if (tp->tm_wday < 0)
    tp->tm_wday += 7;
  y = 1970;
  while (days < 0 || days >= (__isleap (y) ? 366 : 365))
    {
      /* Guess a corrected year, assuming 365 days per year.  */
      time_t yg = y + days / 365 - (days % 365 < 0);
      /* Adjust DAYS and Y to match the guessed year.  */
      days -= ((yg - y) * 365
               + LEAPS_THRU_END_OF (yg - 1)
               - LEAPS_THRU_END_OF (y - 1));
      y = yg;
    }
  tp->tm_year = y - 1900;
  if (tp->tm_year != y - 1900)
    {
      /* The year cannot be represented due to overflow.  */
      //__set_errno (EOVERFLOW);
      return 0;
    }
  tp->tm_yday = days;
  ip = __mon_yday[__isleap(y)];
  for (y = 11; days < (long int) ip[y]; --y)
    continue;
  days -= ip[y];
  tp->tm_mon = y;
  tp->tm_mday = days + 1;
  return 1;
}

struct TimeStamp
{
    int year;    // 0-based
    int month;   // [1-12]
    int day;     // [1-31]
    int hour;    // [0-23]
    int minute;  // [0-59]
    int sec;     // [0-59]
    int ms;      // [0-999]
};

// Avoid the 9 alloc and 9 free from localtime gmtime and another C/C++ functions
// Done
TimeStamp get_local_date()
{

  TimeStamp result;

  auto now = std::chrono::system_clock::now();
  auto seconds = std::chrono::time_point_cast<std::chrono::seconds>(now);
  auto fraction = now - seconds;
  auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(fraction);

  //time_t tnow = std::chrono::system_clock::to_time_t(now);
  //tm* ptm = localtime(&tnow);
  tm date_time;
  time_t seconds_from_1970 = time( NULL );

  //GMT offset los angeles = GMT-8 = 8 * 3600
  off_time( seconds_from_1970, -( 8 * 3600 ), &date_time );

  result.year   = date_time.tm_year + 1900;
  result.month  = date_time.tm_mon + 1;
  result.day    = date_time.tm_mday;
  result.hour   = date_time.tm_hour;
  result.minute = date_time.tm_min;
  result.sec    = date_time.tm_sec;
  result.ms     = static_cast<int>(milliseconds.count());

  return result;

}

int main(void)
{

    TimeStamp x = get_local_date();

    std::cout << "Year: " << x.year << std::endl;
    std::cout << "Month: " << x.month << std::endl;
    std::cout << "Day: " << x.day << std::endl;

    std::cout << "Hour: " << x.hour << std::endl;
    std::cout << "Minutes: " << x.minute << std::endl;
    std::cout << "Seconds: " << x.sec << std::endl;
    std::cout << "Millis: " << x.ms << std::endl;

    // tm date_time;
    // time_t result = time( NULL );

    // const int x = __offtime( result, 0, &date_time );

    // std::cout << "Sucess: " << x << std::endl;
    // std::cout << "Year: " << date_time.tm_year + 1900 << std::endl;
    // std::cout << "Month: " << date_time.tm_mon + 1 << std::endl;
    // std::cout << "Day: " << date_time.tm_mday << std::endl;

    // std::cout << "Hours: " << date_time.tm_hour << std::endl;
    // std::cout << "Minutes: " << date_time.tm_min << std::endl;
    // std::cout << "Seconds: " << date_time.tm_sec << std::endl;

    // auto now = std::chrono::system_clock::now();
    // auto seconds = std::chrono::time_point_cast<std::chrono::seconds>(now);
    // auto fraction = now - seconds;
    // auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(fraction);



//     std::string key {};

//     while ( key != "x" ) {

//       int64_t millis_from_1970 = 1645818390673; //std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

//       double years = double( millis_from_1970 ) / 31556952000; //( 365 * 24 * 60 * 60 * 1000);

//       std::cout << "Millis from 1970: " << millis_from_1970 << std::endl;
//       std::cout << "Years: " << years << std::endl;

//       double month = double( millis_from_1970 ) / 2629746000;
//       std::cout << "Months: " << month << std::endl;

//       getline( std::cin, key );

//     }

//     //time_t t = time(NULL);

//     //gmtime(&t);
//     //printf("UTC:       %s", asctime(gmtime(&t)));
//     //printf("local:     %s", asctime(localtime(&t)));
//     // POSIX-specific
//     //putenv("TZ=Asia/Singapore");
//     //printf("Singapore: %s", asctime(localtime(&t)));

// #ifdef __STDC_LIB_EXT1__
//     struct tm buf;
//     char str[26];
//     asctime_s(str,sizeof str,gmtime_s(&t, &buf));
//     printf("UTC:       %s", str);
//     asctime_s(str,sizeof str,localtime_s(&t, &buf));
//     printf("local:     %s", str);
// #endif
}
