#include <stdio.h>
#include <stdlib.h>

#include "Executor.h"
#include "Task.h"
#include "Timer.h"
#include "genHeapHead.h"
#include "vectorGenericHead.h" 
#include "zlog4c.h"

#define TASKS_HEAP _exe->m_tasks

 
struct Executor{
				Heap* m_tasks;
				int m_numOfTasks;
				int m_exeIsRunning;
				int m_exeIsPaused;
				};

static Executor_Result ResetPausedExecutor(Executor* _exe, Timer* _timer);
static void ResetExecutorTasksTime(Vector* _vec);

Executor* ExecutorCreate(size_t _initCapacity, size_t _blockSize)
{
	Executor* newExe = NULL;
	Vector* newVec = NULL;
	
	newExe = (Executor*) malloc(sizeof(Executor));
	if(NULL == newExe)
	{
		return NULL;
	}
	
	newVec = Vector_Create(_initCapacity, _blockSize);
	if(NULL == newVec)
	{
		return NULL;
	}
	
	newExe->m_tasks = Heap_Build(newVec, TimeComparatorFunc);
	if(NULL == newExe->m_tasks)
	{
		Vector_Destroy(&newVec, NULL);
		return NULL;
	}
	
	newExe->m_numOfTasks = Heap_Size(newExe->m_tasks);
	newExe->m_exeIsRunning = FALSE;
	newExe->m_exeIsPaused = FALSE;

	return newExe;
}


void ExecutorDestroy(Executor** _exe)
{
	Vector* tempVector = NULL; 
	Task* task = NULL;
	int i = 0;
	
	if(_exe == NULL || *_exe == NULL)
	{
		return;
	}
	tempVector = Heap_Destroy(&(*_exe)->m_tasks);
	/*Go over tasks and Ffree them as well */ /*TODO how to free tasks inside? 2 ways: in a loop over vector items and destroy each, or heap extract and for each destroy element*/
	for(i=1; i<=(*_exe)->m_numOfTasks; i++) /*because my vector starts from 1*/
	{
		Vector_Remove(tempVector, (void**)&task);
		TaskDestroy(&task);
	}
	Vector_Destroy(&tempVector, NULL);
	
	free(*_exe);
	*_exe = NULL;
}


int GetExecutorNumOfTasks(Executor* _exe)
{
	if(NULL == _exe)
	{
		return 0;
	}
	
	return _exe->m_numOfTasks;
}


Executor_Result AddTask(Executor* _exe, userTaskFunc _taskFunc, size_t _period, void* _context)
{
	Heap_ResultCode res;
	Task* newTask = NULL;
	
	if(NULL == _exe)
	{
		return EXE_UNINITIALIZED;
	}
	
	newTask = TaskCreate(_taskFunc, _period, _context);
	if(NULL == newTask)
	{
		return EXE_ADD_TASK_FAIL;
	}
	
	res = Heap_Insert(TASKS_HEAP, (void*) newTask);
	if(res != HEAP_SUCCESS)
	{
		if(res == HEAP_REALLOCATION_FAILED)
		{
			return EXE_UNINITIALIZED;
		}
		else
		{
			return EXE_OVER_FLOW;
		}
	}
	
	_exe->m_numOfTasks = Heap_Size(TASKS_HEAP);
	return EXE_SUCCESS;
}


void PrintExecutorTasks(Executor* _exe)
{
	Heap_ForEach(TASKS_HEAP, PrintTask, NULL);	
}


Executor_Result RunExecutor(Executor* _exe)
{
	Heap_ResultCode res;
	Task* userTask = NULL;
	Timer* exeTimer = NULL;
	size_t nextRunTime = 0;
	
	if(NULL == _exe)
	{
		return EXE_UNINITIALIZED;
	}
	
	if(_exe->m_numOfTasks == 0)
	{
		return EXE_UNDER_FLOW;
	}
	
	/*if executor already running don't allow to run it again*/
	if(_exe->m_exeIsRunning == TRUE)
	{
		return EXE_EXECUTOR_RUNNING;
	}

	/*run for the first time*/
	if(_exe->m_exeIsRunning == FALSE && _exe->m_exeIsPaused == FALSE)
	{
		_exe->m_exeIsRunning = TRUE;
	}	
	
	/*run after pause*/
	if(_exe->m_exeIsPaused == TRUE)
	{
		ResetPausedExecutor(_exe, exeTimer);
		_exe->m_exeIsRunning = TRUE;
	}

	exeTimer = TimerCreate(); /*create executor timer*/
	if(NULL == exeTimer)
	{
		return EXE_TIMER_ERROR;
	}	

	/*start running the executor tasks*/
	while(_exe->m_numOfTasks > 0 && _exe->m_exeIsRunning)
	{	
		/*executor need to pause and program go back to user main*/
		if(_exe->m_exeIsPaused == TRUE)
		{
			_exe->m_exeIsRunning = FALSE;
			return EXE_PAUSED;
		}
		
		userTask = (Task*) Heap_Peek(TASKS_HEAP);
		nextRunTime = GetTaskNextRun(userTask);
		
		/*check if to do sleep or run task now*/
		if(GoToSleep(exeTimer, nextRunTime) != TIMER_SUCCESS)
		{
			return EXE_TIMER_ERROR;
		}

		userTask = Heap_Extract(TASKS_HEAP);
		
		/* run task - if need to run again, reschedule task. */
		if(RunTask(userTask))
		{
			RescheduleTask(userTask, exeTimer);
			res = Heap_Insert(TASKS_HEAP, (void*) userTask);
			if(res != HEAP_SUCCESS)
			{
				return EXE_RESCHEDUAL_TASK_FAIL;
			}
		}
		else
		{
			TaskDestroy(&userTask); /*task is not rescheduled, remove it from executor*/
			_exe->m_numOfTasks--;
		}
	}
	return EXE_SUCCESS;
}


void ExecutorPause(Executor* _exe)
{
	_exe->m_exeIsPaused = TRUE;
}


/*******STATIC METHODS*******/
/* function reset the excutor after pause.
 * reset the executor timer and call function that reset tasks run time to period time. */
static Executor_Result ResetPausedExecutor(Executor* _exe, Timer* _timer)
{
	Vector* exeTasksVec = NULL;
	_exe->m_exeIsPaused = FALSE;
	
	exeTasksVec = Heap_Destroy(&(TASKS_HEAP));
	ResetExecutorTasksTime(exeTasksVec);
	DestroyTimer(&_timer);
	
	TASKS_HEAP = Heap_Build(exeTasksVec, TimeComparatorFunc);
	if(NULL == TASKS_HEAP)
	{
		return EXE_LOAD_FAIL;
	}
	
	return EXE_SUCCESS;
}

/* function reset the excutor tasks run time to period time after pause. */
static void ResetExecutorTasksTime(Vector* _vec)
{
	Task* userTask;
	int i = 0;
	size_t vecSize = 0;
	
	vecSize = Vector_Size(_vec);
	
	for(i=1; i <= vecSize; i++)
	{
		Vector_Get(_vec, i, (void**) &userTask);
		ResetTaskNextRun(userTask);	
	}	
}




