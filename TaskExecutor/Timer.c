#define _XOPEN_SOURCE 500
#include "Timer.h"
#include "zlog4c.h"
#include <stdio.h>

#define MICRO 1000000
/*Using struct timeval in order to get time now in microseconds
{
     time_t      tv_sec;
     suseconds_t tv_usec;
};*/



struct Timer{
			 size_t m_initTime; /*in MICRO*/
			};


Timer* TimerCreate()
{
	Timer* newTimer;
	struct timeval newTimeval;
	
	newTimer = (Timer*) malloc(sizeof(Timer));
	if(NULL == newTimer)
	{
		return NULL;
	}
	
	gettimeofday(&newTimeval, NULL);

	newTimer->m_initTime = (newTimeval.tv_usec) + (newTimeval.tv_sec*MICRO); /*convert seconds to microseconds*/
	
	return newTimer;
}


void DestroyTimer(Timer** _timer)
{
	if(NULL == _timer || NULL == *_timer)
	{
		return;
	}
	
	free(*_timer);
	*_timer = NULL;
}


size_t GetCurrentTime()
{
	struct timeval newTimeval;

	gettimeofday(&newTimeval, NULL);
	
	return (newTimeval.tv_usec) + (newTimeval.tv_sec*MICRO); /*convert seconds to microseconds*/
}


size_t GetTimerStart(Timer* _timer)
{
	return _timer->m_initTime; /*in miliseconds*/
}
	
		
Timer_Result GoToSleep(Timer* _timer, size_t _duration)
{
	size_t relativeTime;
	int result = 0;
	
	relativeTime = GetCurrentTime() - GetTimerStart(_timer);
	printf("\nRelative time is: %ld\n", relativeTime);
	printf("Schedule run time is: %ld\n", _duration);
	if(relativeTime < _duration)
	{
		printf(">> Start sleeping for: %ld microseconds\n", _duration - relativeTime);
		ZLOG("LOG_INFO", "Relative time: %ld, Schedule run time: %ld, Sleep time: %ld", relativeTime, _duration, _duration - relativeTime);

		result = usleep(_duration - relativeTime);
		printf("I slept.\n");
		
		if(result != 0) /*usleep returns 0 on success, -1 on error*/
		{
			ZLOG("LOG_ERROR", "Executor timer failure - message no.%d", TIMER_SLEEP_FAIL);				
			return TIMER_SLEEP_FAIL;
		}
	}
	
	return TIMER_SUCCESS;
}

Timer_Result CalcTimerRun(Timer* _timer, size_t* _diffRes)
{
	struct timeval timevalNow;
	struct timeval timevalInit;
	struct timeval timevalDiff;
	
	if(NULL == _timer)
	{
		return TIMER_UNINITIALIZED;
	}
	
	timevalNow.tv_usec = GetCurrentTime();
	timevalInit.tv_usec = _timer->m_initTime;
	
	
	/* subtracts the time value in b from the time value in a, and places the result in the timeval pointed to by res. */
	timersub(&timevalNow, &timevalInit, &timevalDiff);
	*_diffRes = (timevalDiff.tv_usec);
	
	return TIMER_SUCCESS;
}


int TimeComparatorFunc(const void* _a, const void* _b)
{
	
	if(NULL == _a || NULL == _b)
	{
		return 0;
	}
	
	return ((*((size_t*)_a) < *((size_t*)_b)) ? TRUE: FALSE);
}
