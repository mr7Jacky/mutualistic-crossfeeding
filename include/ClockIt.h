#ifndef _ClockIt_
#define _ClockIt_
#include <sys/time.h>
//
// Class ClockIt can be used on a Linux/Unix system to time portions of code. It
// depends upon calls to the gettimeofday(..) function in sys/time.h to read the system
// clock.
//
// Usage snippet:
//
/*
 int main()
 {

      ClockIt clock1;              // declare ClockIt instance

      clock1.start();              // start the instance (reads the clock)

  // === Code to be timed === //  // carry out computation

      clock1.stop();               // stop the clock (reads the clock again)

      cout << "Time elapsed in Milli-seconds" << clock1.getMilliSecElapsedTime() << endl;

 }
 */
class ClockIt
{
public:

    ClockIt()
    {
    elapsedTime = 0;
    }

    ClockIt(const ClockIt& clock)
    {
    elapsedTime = clock.elapsedTime;
    t1          = clock.t1;
    t2          = clock.t2;
    }

    //
    // Reads the clock 
    //

    inline void start() {gettimeofday(&t1, 0);}

    //
    // Reads the clock again, and computes the 
    // the elapsed time. 
    //
    // Returns the time in milliseconds since start() called
    //

    inline double stop()
    {
    gettimeofday(&t2, 0);
    //
    // Compute elapsed time in milliseconds
    //
    elapsedTime  = (t2.tv_sec  - t1.tv_sec)  * 1000.0;    // seconds to milliseconds
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;    // microseconds to milliseconds
    return elapsedTime;
    }

    double getMilliSecElapsedTime()
    {return elapsedTime;}

    double getMicroSecElapsedTime()
    {return elapsedTime*1000.0;}

    double getSecElapsedTime()
    {return elapsedTime/1000.0;}

    timeval t1, t2;
    double elapsedTime;
};
#endif
