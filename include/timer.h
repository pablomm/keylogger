#ifndef TIMER_H
#define TIMER_H

#include <thread>
#include <chrono>

namespace TIMER
{

class Timer
{
    std::thread Thread;
    bool Alive = false;
    long CallNumber = -1L;
    long repeat_count = -1L;
    std::chrono::milliseconds interval = std::chrono::milliseconds(0);
    std::function<void(void)> funct = nullptr;

    void SleepAndRun ();
    void ThreadFunc ();
public:
    static const long Infinite = -1L;
    Timer () {}
    Timer(const std::function<void(void)> &f,
          const unsigned long &i,
          const long repeat);
    void Start (bool Async);
    void Stop ();
    void SetFunction (const std::function<void (void)> &f);
    bool IsAlive ();
    void RepeatCount (const long r);
    long GetLeftCount ();
    long RepeatCount ();
    void SetInterval (const unsigned long &i);
    unsigned long Interval ();
    const std::function<void(void)> &Function ();
};

}

#endif

