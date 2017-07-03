#ifndef TIMER_H
#define TIMER_H

#include <sys/timeb.h>
#include <time.h>

/// Class to retrieve current time as seconds since midnight Jan 01, 1970 with millisecond resolution
class Timer
{
public:
  /// Retrieve time as seconds since midnight Jan 01, 1970 with millisecond resolution
  double getTime()
  {
    struct timeb tb;
    ftime(&tb);
    return (double)tb.time + ((double)tb.millitm / 1000.0f);
  }
};

#endif
