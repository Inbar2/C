/** 
 *  @file Task.h
 *  @brief header file for task in Periodic Task Executor project.
 *
 *  @author Inbar Barhum
 * 
 */


#ifndef __TASK_H__
#define __TASK_H__

#include "Timer.h"
#include "zlog4c.h"

typedef struct Task Task;

typedef enum Task_Result {
						TASK_SUCCESS,
						TASK_UNINITIALIZED,
						TASK_FAIL						
						} Task_Result;

/*
 * @brief: Create new task.
 * @param[in]: _taskFunc - pointer to function that the user supplied.
 * @param[in]: _period - the interval time beteen runs of the function.
 * @param[in]: _context - user provided context, will be sent as param to task function.
 * @return: Task* on success, otherwise return NULL;
 */
Task* TaskCreate(int (*userTaskFunc)(void*), size_t _period, void* _context);


/*
 * @description: Dynamically deallocate a previously allocated task.
 * @param[in]: _task - pointer to task to be deallocated, created from TaskCreate.
 * @return void.
 */
void TaskDestroy(Task** _task);


/* 
 * @description: Run task in the executor.  
 * @param[in]: _task - pointer to task, created from TaskCreate.
 * @return: TRUE - if task need to be scheduled for another period, FALSE otherwise.
 */
int RunTask(Task* _task);


/* 
 * @description: Update m_taskNextRun with time.
 * @param[in]: _timeToAdd - number of microseconds to add to task period in order to know next run.
 * @param[in]: _timer - timer provided in order to get current time from to cala the next run.
 * @return: void.
 */
void RescheduleTask(Task* _task, Timer* _timer);


/* 
 * @description: Set task's next run time to initial period time.  
 * @param[in]: _task - pointer to task, created from TaskCreate.
 * @return: void.
 */
void ResetTaskNextRun(Task* _task);


/* 
 * @description: Get task's next run time.  
 * @param[in]: _task - pointer to task, created from TaskCreate.
 * @return: task's next run time in microseconds.
 */
size_t GetTaskNextRun(Task* _task);


/* 
 * @description: print task's next run time.  
 * @param[in]: _elem - element to print.
 * @param[in]: _context - provided context.
 * @return: task's next run time in microseconds.
 */
int PrintTask(const void* _elem, void* _context);

#endif /* Task.h */


