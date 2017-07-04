
#include "timer.h"

namespace TIMER
{

void Timer::SleepAndRun ()
{
    std::this_thread::sleep_for(interval);
    if (Alive)
        Function ()();
}

void Timer::ThreadFunc ()
{
    if (CallNumber == Infinite)
        while (Alive)
            SleepAndRun ();
    else

        while (repeat_count--)
            SleepAndRun ();
}

Timer::Timer(const std::function<void(void)> &f,
             const unsigned long &i,
             const long repeat = Timer::Infinite) : CallNumber (repeat),
    interval (std::chrono::milliseconds(i)),funct (f) {}

void Timer::Start (bool Async = true)
{

    if (IsAlive ())
        return;

    Alive = true;

    repeat_count = CallNumber;

    if (Async)
        Thread = std::thread (ThreadFunc, this);
    else
        this->ThreadFunc ();
}

void Timer::Stop ()
{

    Alive = false;


    Thread.join ();
}
void Timer::SetFunction (const std::function<void (void)> &f)
{
    funct = f;
}
bool Timer::IsAlive ()
{
    return Alive;
}
void Timer::RepeatCount (const long r)
{

    if (Alive)
        return;
    CallNumber = r;
}

long Timer::GetLeftCount ()
{
    return repeat_count;
}

long Timer::RepeatCount ()
{
    return CallNumber;
}
void Timer::SetInterval (const unsigned long &i)
{

    if (Alive)
        return;;

    interval = std::chrono::milliseconds(i);
}

unsigned long Timer::Interval ()
{
    return interval.count();
}

const std::function<void(void)> & Timer::Function ()
{

    return funct;
}

}
