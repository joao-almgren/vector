
#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

class Timer
{
	public:
		virtual void update() = 0;				// saves current time
		virtual __int64 passed() = 0;			// returns time passed since update (ms)
		virtual double resolution() const = 0;	// milliseconds per timer tick
};


class TimerLow : public Timer
{
	public:
		inline TimerLow()
		{
			start = GetTickCount();
		}

		inline void update()
		{
			start = GetTickCount();
		}

		inline __int64 passed()
		{
			return (GetTickCount() - start);
		}

		inline double resolution() const
		{
			return 1.0;
		}

	private:
		DWORD start;
};


class TimerHigh : public Timer
{
	public:
		static bool available() // check if high quality timer is available before attempting use
		{
			LARGE_INTEGER freq;
			if (QueryPerformanceFrequency(&freq))
				return true;
			else
				return false;
		}

		inline TimerHigh()
		{
			LARGE_INTEGER freq;
			if (QueryPerformanceFrequency(&freq))
			{
				res = 1000.0 / (double)freq.QuadPart;
				QueryPerformanceCounter(&start);
			}
		}

		inline void update()
		{
			QueryPerformanceCounter(&start);
		}

		inline __int64 passed()
		{
			LARGE_INTEGER current;
			QueryPerformanceCounter(&current);
			return (__int64)((current.QuadPart - start.QuadPart) * res);
		}

		inline double resolution() const
		{
			return res;
		}

		inline __int64 passedticks() // time passed in timer "ticks"
		{
			LARGE_INTEGER current;
			QueryPerformanceCounter(&current);
			return (current.QuadPart - start.QuadPart);
		}

	private:
		LARGE_INTEGER start;
		double res; 
};

Timer * getTimer()
{
	if (TimerHigh::available())
		return new TimerHigh();
	else
		return new TimerLow();
}
