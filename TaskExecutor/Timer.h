/** 
 *  @file Timer.h
 *  @brief header file for Timer in Periodic Task Executor project.
 *
 *  @author Inbar Barhum
 * 
 */

#ifndef __TIMER_H__
#define __TIMER_H__

#include <sys/time.h> /*for using gettimeofday, timersub func*/
#include <time.h> /*for all linux time functions*/
#include <unistd.h> /*for using usleep func*/
#include <stdlib.h> /*for using malloc*/

#define _BSD_SOURCE

#define TRUE 1
#define FALSE 0

typedef enum Timer_Result {
						TIMER_SUCCESS,
						TIMER_UNINITIALIZED,
						TIMER_SLEEP_FAIL
						} Timer_Result;


typedef struct Timer Timer;

/*
 * @brief: Create new timer.
 * @return: Timer* on success, otherwise return NULL;
 */
Timer* TimerCreate();


/*
 * @description: Dynamically deallocate a previously allocated timer. 
 * @param[in]: _timer - pointer to timer to be deallocated, created from TimerCreate.
 * @return void.
 */
void DestroyTimer(Timer** _timer);


/* 
 * @description: put to sleep the task in microsecond intervals.
 * @param[in]: _timer - pointer to timer, created from TimerCreate.
 * @param[in]: _duration - the period to put the task to sleep in microseconds.
 * @return: TIMER_SUCCESS if sleep succeeded.
 			TIMER_SLEEP_FAIL if sleep failed.
 */
Timer_Result GoToSleep(Timer* _timer, size_t _duration);


/* 
 * @description: get the initial timer start time.
 * @return: timer's initial time in microseconds.
 */
size_t GetTimerStart(Timer* _timer);


/* 
 * @description: get the time now.
 * @return: the time now in microseconds.
 */
size_t GetCurrentTime();


/* 
 * @description: function calculates the delta between task time and init time .
 * @param[in]: _timer - pointer to timer, created from TimerCreate (containing the timer's initial time in microseconds).
 * @param[out]: _diffRes - output pointer to delta time (it the relative time from starting the timer).
 * @return: delta time in microseconds.
 */
Timer_Result CalcTimerRun(Timer* _timer, size_t* _diffRes);


/* 
 * @description: function compare between two void* elements.
 * @param[in]: _a - void pointer.
 * @param[in]: _b - void pointer.  
 * @return: TURE - if _a smaller from _b, otherwise return FALSE.
 */
int TimeComparatorFunc(const void* _a, const void* _b);


#endif /* Timer.h */


