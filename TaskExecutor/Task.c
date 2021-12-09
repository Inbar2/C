#include <stdio.h>
#include <stdlib.h>

#include "Task.h"

struct Task{
			size_t m_taskNextRun; /*in miliseconds*/
			size_t m_taskPeriod;
			int (*m_userTaskFunc)(void*);
			void* m_context;
			};



Task* TaskCreate(int (*userTaskFunc)(void*), size_t _period, void* _context)
{
	Task* newTask = NULL;
	
	newTask = (Task*) malloc(sizeof(Task));
	if(NULL == newTask)
	{
		return NULL;
	}
	
	newTask->m_taskNextRun =  _period;
	newTask->m_taskPeriod = _period;
	newTask->m_userTaskFunc = userTaskFunc;
	newTask->m_context = _context;
	
	return newTask;
}


void TaskDestroy(Task** _task)
{
	if(_task == NULL || *_task == NULL)
	{
		return;
	}
	
	free(*_task);
	*_task = NULL;
}


int RunTask(Task* _task)
{
	if(NULL == _task)
	{
		return FALSE;
	}
	
	/*run the task's function with its context*/
	return _task->m_userTaskFunc(_task->m_context);
}


void RescheduleTask(Task* _task, Timer* _timer)
{
	size_t diffRes = 0;
	
	CalcTimerRun(_timer, &diffRes);
	_task->m_taskNextRun = diffRes + _task->m_taskPeriod;
	/*TODO print for tests*/
	printf("Next run in time: %ld\n", _task->m_taskNextRun);
}


void ResetTaskNextRun(Task* _task)
{
	_task->m_taskNextRun = _task->m_taskPeriod;
}


size_t GetTaskNextRun(Task* _task)
{
	return _task->m_taskNextRun;
}


/*an action function using as pointer to function that foreach function will get and executes the action on each element*/
int PrintTask(const void* _elem, void* _context)
{
	if(_elem == NULL)
	{
		return 0;
	}
	printf("%d, ", *((int*)_elem));
	return 1;
}
