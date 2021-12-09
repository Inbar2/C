/* 
 * File: ExecutorTests.c
 * Functions of Automation Tests for Periodic Task Executor project.
 * Author: Inbar Barhum 
 * Date: 28.1.2019
 */ 

#include "ExecutorTestsHead.h"
#include "zlog4c.h"

#define PER(i) i*1000000

/*
	Executor_Result {
					0-EXE_SUCCESS,
					1-EXE_UNINITIALIZED,
					2-EXE_ADD_TASK_FAIL,
					3-EXE_PAUSED,
					4-EXE_INVALID_INPUT,
					5-EXE_RUN_TASK_FAIL,
					6-EXE_UNDER_FLOW,
					7-EXE_OVER_FLOW,
					8-EXE_TIMER_FAIL,
					9-EXE_RESCHEDUAL_TASK_FAIL,
					10-EXE_LOAD_FAIL
					11-EXE_EXECUTOR_RUNNING
					}
*/

void PrintTestsResult(char _name[], AutoTestResults _testResult, Executor_Result* status)
{
	printf("\nTest name: %s \tResult: %s \tstatus: %d\n", _name, (_testResult == SUCCESS)? "Passed" : "Failed !", *status);					
}


/*---------------------------------------------------------------------------------------------------------------*/

AutoTestResults ExecutorCreate_Test1(Executor_Result* status) /* create executor */
{
 	Executor* newExe = NULL;
	size_t initCapacity = 0;
	size_t blockSize = 0;
	
	initCapacity = 5;
	blockSize = 2;
	
	newExe = ExecutorCreate(initCapacity, blockSize);
	
	if(newExe != NULL)
	{
		ExecutorDestroy(&newExe);
		*status = EXE_SUCCESS;
		return SUCCESS;
	}

	return FAIL;
}

AutoTestResults ExecutorCreate_Test2(Executor_Result* status) /* create executor with zero space to tasks */
{
 	Executor* newExe = NULL;
	size_t initCapacity = 0;
	size_t blockSize = 0;
	
	initCapacity = 0;
	blockSize = 0;
	
	newExe = ExecutorCreate(initCapacity, blockSize);
	
	if(newExe == NULL)
	{
		*status = EXE_SUCCESS;
		return SUCCESS;
	}

	return FAIL;
}

/*---------------------------------------------------------------------------------------------------------------*/
AutoTestResults ExecutorDestroy_Test1(Executor_Result* status) /* destroy executor */
{
 	Executor* newExe = NULL;
	size_t initCapacity = 0;
	size_t blockSize = 0;
	int isCreated = FALSE;
	
	initCapacity = 5;
	blockSize = 2;
	
	newExe = ExecutorCreate(initCapacity, blockSize);
	if(NULL != newExe)
	{
		isCreated = TRUE;
	}
	
	ExecutorDestroy(&newExe);
	
	if(newExe == NULL && isCreated)
	{
		*status = EXE_SUCCESS;
		return SUCCESS;
	}
	
	*status = EXE_UNINITIALIZED;
	return FAIL;
}

AutoTestResults ExecutorDestroy_Test2(Executor_Result* status) /* destroy executor twice*/
{
 	Executor* newExe = NULL;
	size_t initCapacity = 0;
	size_t blockSize = 0;
	int isCreated = FALSE;
	
	initCapacity = 5;
	blockSize = 2;
	
	newExe = ExecutorCreate(initCapacity, blockSize);
	if(NULL != newExe)
	{
		isCreated = TRUE;
	}
	
	ExecutorDestroy(&newExe);
	ExecutorDestroy(&newExe);
	
	if(newExe == NULL && isCreated)
	{
		*status = EXE_SUCCESS;
		return SUCCESS;
	}
	
	*status = EXE_UNINITIALIZED;
	return FAIL;
}

/*---------------------------------------------------------------------------------------------------------------*/
AutoTestResults AddTask_Test1(Executor_Result* status) /* add task to executor */
{
 	Executor* newExe = NULL;
	size_t initCapacity = 0;
	size_t blockSize = 0;
	size_t period = PER(5);
	 
	initCapacity = 5;
	blockSize = 2;
	
	newExe = ExecutorCreate(initCapacity, blockSize);
	*status = AddTask(newExe, funcA, period, NULL);
	
	if(newExe != NULL && *status == EXE_SUCCESS)
	{
		ExecutorDestroy(&newExe);
		return SUCCESS;
	}
	
	return FAIL;
}


AutoTestResults AddTask_Test2(Executor_Result* status) /*add task to uninitialized executor*/
{
	size_t period = PER(5);

	*status = AddTask(NULL, funcA, period, NULL);
	
	if(*status == EXE_UNINITIALIZED)
	{
		return SUCCESS;
	}
	
	return FAIL;
}


AutoTestResults AddTask_Test3(Executor_Result* status) /*add task to executor with no space for tasks*/
{
 	Executor* newExe = NULL;
	size_t initCapacity = 0;
	size_t blockSize = 0;
	size_t period = PER(5);
	 
	initCapacity = 0;
	blockSize = 0;
	
	newExe = ExecutorCreate(initCapacity, blockSize); /*vector will not be created, hence exe is NULL*/
	*status = AddTask(newExe, funcA, period, NULL);
	
	if(newExe == NULL && *status == EXE_UNINITIALIZED)
	{
		ExecutorDestroy(&newExe);
		return SUCCESS;
	}
	
	return FAIL;
}


AutoTestResults AddTask_Test4(Executor_Result* status) /*add task to executor space finished*/
{
 	Executor* newExe = NULL;
	size_t initCapacity = 0;
	size_t blockSize = 0;
	size_t period = PER(5);
	 
	initCapacity = 1;
	blockSize = 0;
	
	newExe = ExecutorCreate(initCapacity, blockSize);
	*status = AddTask(newExe, funcA, period, NULL);
	*status = AddTask(newExe, funcB, period, NULL);
	
	if(newExe != NULL && *status == EXE_OVER_FLOW)
	{
		ExecutorDestroy(&newExe);
		return SUCCESS;
	}
	
	return FAIL;
}


AutoTestResults AddTask_Test5(Executor_Result* status) /*add task to executor with no capacity but with block size*/
{
 	Executor* newExe = NULL;
	size_t initCapacity = 0;
	size_t blockSize = 0;
	size_t period = PER(5);
	 
	initCapacity = 0;
	blockSize = 2;
	
	newExe = ExecutorCreate(initCapacity, blockSize);
	*status = AddTask(newExe, funcA, period, NULL);
	
	if(newExe != NULL && *status == EXE_SUCCESS)
	{
		ExecutorDestroy(&newExe);
		return SUCCESS;
	}
	
	return FAIL;
}


AutoTestResults AddTask_Test6(Executor_Result* status) /*add multiple tasks to executor with the same period*/
{
 	Executor* newExe = NULL;
	size_t initCapacity = 0;
	size_t blockSize = 0;
	size_t period = PER(5);
	int numOfTasks = 0;
	
	initCapacity = 5;
	blockSize = 2;
	
	newExe = ExecutorCreate(initCapacity, blockSize);
	AddTask(newExe, funcA, period, NULL);
	AddTask(newExe, funcB, period, NULL);
	AddTask(newExe, funcC, period, NULL);
	numOfTasks = GetExecutorNumOfTasks(newExe);
	
	if(newExe != NULL && numOfTasks == 3)
	{
		*status = EXE_SUCCESS;
		ExecutorDestroy(&newExe);
		return SUCCESS;
	}
	
	return FAIL;
}


AutoTestResults AddTask_Test7(Executor_Result* status) /*add tasks to executor and pass capacity*/
{
 	Executor* newExe;
	size_t initCapacity;
	size_t blockSize;
	size_t period = PER(5);
	int numOfTasks = 0;
	
	initCapacity = 2;
	blockSize = 2;
	
	newExe = ExecutorCreate(initCapacity, blockSize);
	AddTask(newExe, funcA, period, NULL);
	AddTask(newExe, funcB, period, NULL);
	AddTask(newExe, funcC, period, NULL);
	numOfTasks = GetExecutorNumOfTasks(newExe);
	
	if(newExe != NULL && numOfTasks == 3)
	{
		*status = EXE_SUCCESS;
		ExecutorDestroy(&newExe);
		return SUCCESS;
	}
	
	return FAIL;
}


/*---------------------------------------------------------------------------------------------------------------*/

AutoTestResults RunExecutor_Test1(Executor_Result* status) /* run uninitialized executor*/
{
	*status = RunExecutor(NULL);
	
	if(*status == EXE_UNINITIALIZED)
	{
		return SUCCESS;
	}
	
	return FAIL;
}


AutoTestResults RunExecutor_Test2(Executor_Result* status) /* run executor with no tasks*/
{
 	Executor* newExe;
	size_t initCapacity;
	size_t blockSize;
	
	initCapacity = 5;
	blockSize = 2;
	
	newExe = ExecutorCreate(initCapacity, blockSize);
	*status = RunExecutor(newExe);
	
	if(newExe != NULL && *status == EXE_UNDER_FLOW)
	{
		ExecutorDestroy(&newExe);
		return SUCCESS;
	}
	
	return FAIL;
}


AutoTestResults RunExecutor_Test3(Executor_Result* status) /*run executor with one task*/
{
 	Executor* newExe;
	size_t initCapacity;
	size_t blockSize;
	size_t period = PER(4);
	Executor_Result addTaskRes;
	
	initCapacity = 5;
	blockSize = 2;
	
	newExe = ExecutorCreate(initCapacity, blockSize);
	addTaskRes = AddTask(newExe, funcA, period, NULL);
	*status = RunExecutor(newExe);
	
	if(newExe != NULL && addTaskRes == EXE_SUCCESS && *status == EXE_SUCCESS)
	{
		ExecutorDestroy(&newExe);
		return SUCCESS;
	}
	
	return FAIL;
}


AutoTestResults RunExecutor_Test4(Executor_Result* status) /* run executor with 3 tasks*/
{
 	Executor* newExe;
	size_t initCapacity;
	size_t blockSize;
	size_t period1 = PER(6);
	size_t period2 = PER(3);
	size_t period3 = PER(2);
	
	initCapacity = 5;
	blockSize = 2;
	
	newExe = ExecutorCreate(initCapacity, blockSize);
	AddTask(newExe, funcA, period1, NULL);
	AddTask(newExe, funcB, period2, NULL);
	AddTask(newExe, funcC, period3, NULL);
	
	PrintExecutorTasks(newExe);
	*status = RunExecutor(newExe);	
	PrintExecutorTasks(newExe);
	
	if(newExe != NULL && *status == EXE_SUCCESS)
	{
		ExecutorDestroy(&newExe);
		return SUCCESS;
	}
	
	return FAIL;
}

AutoTestResults RunExecutor_Test5(Executor_Result* status) /* run executor with 3 tasks with the same period*/
{
 	Executor* newExe;
	size_t initCapacity;
	size_t blockSize;
	size_t period = PER(2);
	
	initCapacity = 5;
	blockSize = 2;
	
	newExe = ExecutorCreate(initCapacity, blockSize);
	AddTask(newExe, funcA, period, NULL);
	AddTask(newExe, funcB, period, NULL);
	AddTask(newExe, funcC, period, NULL);
	
	*status = RunExecutor(newExe);	
	
	if(newExe != NULL && *status == EXE_SUCCESS)
	{
		ExecutorDestroy(&newExe);
		return SUCCESS;
	}
	
	return FAIL;
}

AutoTestResults RunExecutor_Test6(Executor_Result* status) /* run executor with 3 tasks with block size, different times and insert order*/
{
 	Executor* newExe;
	size_t initCapacity;
	size_t blockSize;
	size_t period1 = PER(2);
	size_t period2 = PER(4);
	size_t period3 = PER(1);
	int totalTasks = 0;
	initCapacity = 2;
	blockSize = 2;
	
	newExe = ExecutorCreate(initCapacity, blockSize);
	AddTask(newExe, funcB, period2, NULL);
	AddTask(newExe, funcA, period1, NULL);
	AddTask(newExe, funcC, period3, NULL);
	totalTasks = GetExecutorNumOfTasks(newExe);
	*status = RunExecutor(newExe);	
	
	if(newExe != NULL && *status == EXE_SUCCESS && totalTasks == 3 && GetExecutorNumOfTasks(newExe) == 0)
	{
		ExecutorDestroy(&newExe);
		return SUCCESS;
	}
	
	return FAIL;
}


AutoTestResults RunExecutor_Test7(Executor_Result* status) /* run executor with one task with one pause executor*/
{
 	Executor* newExe;
	size_t initCapacity;
	size_t blockSize;
	size_t period = PER(3);
	Executor_Result addTaskRes;
	
	initCapacity = 5;
	blockSize = 2;
	
	newExe = ExecutorCreate(initCapacity, blockSize);
	addTaskRes = AddTask(newExe, funcD, period, newExe); /*send the executor as parameter context to the function to run*/
	*status = RunExecutor(newExe); /*first task's run ends with EXE_SUCCESS because task run counter = 1*/
	
	if(newExe != NULL && addTaskRes == EXE_SUCCESS && *status == EXE_SUCCESS && GetExecutorNumOfTasks(newExe) == 0)
	{
		ExecutorDestroy(&newExe);
		return SUCCESS;
	}
	
	return FAIL;
}


AutoTestResults RunExecutor_Test8(Executor_Result* status) /* run executor with one task with one pause executor and continue*/
{
 	Executor* newExe;
	size_t initCapacity;
	size_t blockSize;
	size_t period = PER(3);
	Executor_Result addTaskRes;
	
	initCapacity = 5;
	blockSize = 2;
	
	newExe = ExecutorCreate(initCapacity, blockSize);
	addTaskRes = AddTask(newExe, funcE, period, newExe); /*send the executor as parameter context to the function to run*/
	*status = RunExecutor(newExe);
	
	if(newExe != NULL && addTaskRes == EXE_SUCCESS && *status == EXE_PAUSED && GetExecutorNumOfTasks(newExe) == 1)
	{
		ExecutorDestroy(&newExe);
		return SUCCESS;
	}
	
	return FAIL;
}

AutoTestResults RunExecutor_Test9(Executor_Result* status) /* run executor with one task with multiple pause and continue */
{
 	Executor* newExe;
	size_t initCapacity;
	size_t blockSize;
	size_t period = PER(4);
	Executor_Result addTaskRes;
	
	initCapacity = 5;
	blockSize = 2;
	
	newExe = ExecutorCreate(initCapacity, blockSize);
	addTaskRes = AddTask(newExe, funcE, period, newExe);
	*status = RunExecutor(newExe);
	if(newExe != NULL && addTaskRes == EXE_SUCCESS && *status == EXE_PAUSED)
	{
		*status = RunExecutor(newExe);
		*status = RunExecutor(newExe);
	}
		
	if(*status == EXE_UNDER_FLOW)
	{
		ExecutorDestroy(&newExe);
		return SUCCESS;
	}
	
	return FAIL;
}

AutoTestResults RunExecutor_Test10(Executor_Result* status) /* run executor with one task with one pause, add task and continue (counter = 1)*/
{
 	Executor* newExe;
	size_t initCapacity;
	size_t blockSize;
	size_t period1 = PER(6);
	size_t period2 = PER(2);
	Executor_Result addTaskRes;
	
	initCapacity = 5;
	blockSize = 2;
	
	newExe = ExecutorCreate(initCapacity, blockSize);
	addTaskRes = AddTask(newExe, funcE, period1, newExe);
	*status = RunExecutor(newExe);
	if(newExe != NULL && addTaskRes == EXE_SUCCESS && *status == EXE_PAUSED)
	{
		addTaskRes = AddTask(newExe, funcB, period2, NULL);
		*status = RunExecutor(newExe);
		*status = RunExecutor(newExe);
	}
		
	if(addTaskRes == EXE_SUCCESS && *status == EXE_SUCCESS)
	{
		ExecutorDestroy(&newExe);
		return SUCCESS;
	}
	
	return FAIL;
}

AutoTestResults RunExecutor_Test11(Executor_Result* status) /* run executor with one task with multiple pauses and continue the task (counter > 1)*/
{
 	Executor* newExe;
	size_t initCapacity;
	size_t blockSize;
	size_t period1 = PER(3);
	Executor_Result addTaskRes;
	
	initCapacity = 5;
	blockSize = 2;
	
	newExe = ExecutorCreate(initCapacity, blockSize);
	addTaskRes = AddTask(newExe, funcF, period1, newExe);
	*status = RunExecutor(newExe);
	while(newExe != NULL && addTaskRes == EXE_SUCCESS && *status == EXE_PAUSED)
	{
		*status = RunExecutor(newExe);
	}
		
	if(*status == EXE_SUCCESS)
	{
		ExecutorDestroy(&newExe);
		return SUCCESS;
	}
	
	return FAIL;
}


AutoTestResults RunExecutor_Test12(Executor_Result* status) /* run executor with multiple tasks, pause once and continue (counter = 1)*/
{
 	Executor* newExe;
	size_t initCapacity;
	size_t blockSize;
	size_t period1 = PER(6);
	size_t period2 = PER(3);
	size_t period3 = PER(6);
	
	initCapacity = 5;
	blockSize = 2;
	
	newExe = ExecutorCreate(initCapacity, blockSize);
	AddTask(newExe, funcA, period1, NULL);
	AddTask(newExe, funcB, period2, NULL);
	AddTask(newExe, funcD, period3, newExe); /*send the executor as parameter context to the function to run*/
	*status = RunExecutor(newExe);
	if(newExe != NULL && GetExecutorNumOfTasks(newExe) == 3 && *status == EXE_PAUSED)
	{
		*status = RunExecutor(newExe);
	}
	
	while(*status == EXE_PAUSED)
	{
		*status = RunExecutor(newExe);
	}
	
	if(*status == EXE_SUCCESS && GetExecutorNumOfTasks(newExe) == 0)
	{
		ExecutorDestroy(&newExe);
		return SUCCESS;
	}
	
	return FAIL;
}


AutoTestResults RunExecutor_Test13(Executor_Result* status) /* run executor with multiple tasks and pause, add task and continue (counter = 1)*/
{
 	Executor* newExe;
	size_t initCapacity;
	size_t blockSize;
	size_t period1 = PER(3);
	size_t period2 = PER(2);
	size_t period3 = PER(6);
	size_t period4 = PER(1);
	int exeSize = 0;
	initCapacity = 5;
	blockSize = 2;
	
	newExe = ExecutorCreate(initCapacity, blockSize);
	AddTask(newExe, funcA, period1, NULL);
	AddTask(newExe, funcB, period2, NULL);
	AddTask(newExe, funcD, period3, newExe);
	exeSize = GetExecutorNumOfTasks(newExe);
	*status = RunExecutor(newExe);
	if(newExe != NULL && exeSize == 3 && *status == EXE_PAUSED)
	{
		AddTask(newExe, funcC, period4, NULL);
		*status = RunExecutor(newExe);
	}

	while(*status == EXE_PAUSED)
	{
		*status = RunExecutor(newExe);
	}
	
	if(*status == EXE_SUCCESS && GetExecutorNumOfTasks(newExe) == 0)
	{
		ExecutorDestroy(&newExe);
		return SUCCESS;
	}
	
	return FAIL;
}

AutoTestResults RunExecutor_Test14(Executor_Result* status) /* run executor with multiple tasks and pause (counter = 1), add multiple tasks (also pause) and continue (counter > 1)*/
{
 	Executor* newExe;
	size_t initCapacity;
	size_t blockSize;
	size_t period1 = PER(4);
	size_t period2 = PER(3);
	size_t period3 = PER(6);
	size_t period4 = PER(2);
	size_t period5 = PER(3);
	int exeSize = 0;
	initCapacity = 5;
	blockSize = 2;
	
	newExe = ExecutorCreate(initCapacity, blockSize);
	AddTask(newExe, funcA, period1, NULL);
	AddTask(newExe, funcB, period2, NULL);
	AddTask(newExe, funcD, period3, newExe); /*run only once*/
	exeSize = GetExecutorNumOfTasks(newExe);
	*status = RunExecutor(newExe);
	if(newExe != NULL && exeSize == 3 && *status == EXE_PAUSED)
	{
		AddTask(newExe, funcC, period4, NULL);
		AddTask(newExe, funcF, period5, newExe);
		PrintExecutorTasks(newExe);	
		if(GetExecutorNumOfTasks(newExe) == 4)
		{
			*status = RunExecutor(newExe);
			while(*status == EXE_PAUSED)
			{
				*status = RunExecutor(newExe);
			}
		}
	}
	if(*status == EXE_SUCCESS && GetExecutorNumOfTasks(newExe) == 0)
	{
		ExecutorDestroy(&newExe);
		return SUCCESS;
	}
	
	return FAIL;
}


AutoTestResults RunExecutor_Test15(Executor_Result* status) /* run executor with multiple tasks and pause (counter = 1), add another executor with task and continue*/
{
 	Executor* exe_A, *exe_B;
	size_t initCapacity;
	size_t blockSize;
	Executor_Result status_B;
	
	size_t period1 = PER(4);
	size_t period2 = PER(2);
	size_t period3 = PER(6);
	size_t period4 = PER(2);
	initCapacity = 2;
	blockSize = 2;

	exe_A = ExecutorCreate(initCapacity, blockSize);
	AddTask(exe_A, funcA, period1, NULL);
	AddTask(exe_A, funcC, period2, NULL);
	AddTask(exe_A, funcE, period3, exe_A);
	PrintExecutorTasks(exe_A);	
	*status = RunExecutor(exe_A);
	if(exe_A != NULL && GetExecutorNumOfTasks(exe_A) == 3 && *status == EXE_PAUSED)
	{
		exe_B = ExecutorCreate(initCapacity, blockSize);
		AddTask(exe_B, funcB, period4, NULL);
		PrintExecutorTasks(exe_A);	
		status_B = RunExecutor(exe_B);
		*status = RunExecutor(exe_A);
		while(*status == EXE_PAUSED)
		{
			*status = RunExecutor(exe_A);
		}
	}
	
	if(exe_B != NULL && status_B == EXE_SUCCESS && *status == EXE_SUCCESS && GetExecutorNumOfTasks(exe_A) == 0 && GetExecutorNumOfTasks(exe_B) == 0)
	{
		ExecutorDestroy(&exe_A);
		ExecutorDestroy(&exe_B);
		return SUCCESS;
	}
	
	return FAIL;
}

AutoTestResults RunExecutor_Test16(Executor_Result* status) /* run executor with multiple tasks, which one of them call run executor again*/
{
 	Executor* newExe;
	size_t initCapacity;
	size_t blockSize;
	size_t period1 = PER(3);
	size_t period2 = PER(2);
	size_t period3 = PER(3);
	int exeSize = 0;
	initCapacity = 5;
	blockSize = 2;
	
	newExe = ExecutorCreate(initCapacity, blockSize);
	AddTask(newExe, funcA, period1, NULL);
	AddTask(newExe, funcB, period2, NULL);
	AddTask(newExe, funcG, period3, newExe);
	
	PrintExecutorTasks(newExe);
	exeSize = GetExecutorNumOfTasks(newExe);
	*status = RunExecutor(newExe);
	
	if(newExe != NULL && exeSize == 3 && *status == EXE_SUCCESS && GetExecutorNumOfTasks(newExe) == 0)
	{
		ExecutorDestroy(&newExe);
		return SUCCESS;
	}
	
	return FAIL;
}

/*EXAMPLE WITH LOGS*/
AutoTestResults RunExecutor_Test17(Executor_Result* status) /* run executor with Logger and multiple tasks and pause (counter = 1), and continue*/
{
 	Executor* exe_A;
	size_t initCapacity;
	size_t blockSize;
	LogStatus logStat = 0;
	Executor_Result taskAddRes;
	size_t period1 = PER(4);
	size_t period2 = PER(2);
	size_t period3 = PER(6);
	size_t period4 = PER(2);
	initCapacity = 2;
	blockSize = 2;
	
	/*call function zlog_init with file name*/
	logStat = zlog_init("zlogExeConfFile");
	printf("Logger initiation status: %d\n", logStat);
	if(logStat == 0)
	{
		printf("\n---Logger OK - Can handle logs enteries.---\n");
	}
	
	exe_A = ExecutorCreate(initCapacity, blockSize);
	AddTask(exe_A, funcA, period1, NULL);
	AddTask(exe_A, funcC, period2, NULL);
	AddTask(exe_A, funcE, period3, exe_A);
	
	printf("\nExecutor tasks times: ");
	PrintExecutorTasks(exe_A);	
	*status = RunExecutor(exe_A);
	if(exe_A != NULL && GetExecutorNumOfTasks(exe_A) == 3 && *status == EXE_PAUSED)
	{
		ZLOG("LOG_INFO", "Executor paused - message no.%d", EXE_PAUSED);
		taskAddRes = AddTask(exe_A, funcB, period4, NULL);
		if(taskAddRes != EXE_SUCCESS)
		{
			ZLOG("LOG_ERROR", "Executor add task failure - message no.%d", taskAddRes);
		}
		printf("\nExecutor tasks times: ");
		PrintExecutorTasks(exe_A);	
		*status = RunExecutor(exe_A);
		while(*status == EXE_PAUSED)
		{
			*status = RunExecutor(exe_A);
		}
	}

	if(*status == EXE_SUCCESS && GetExecutorNumOfTasks(exe_A) == 0)
	{
		ZLOG("LOG_INFO", "Executor task run success - message no.%d", EXE_SUCCESS);
		ExecutorDestroy(&exe_A);
		return SUCCESS;
	}
	
	ZLOG("LOG_SEVERE", "Executor task run failure - message no.%d", EXE_RUN_TASK_FAIL);
	return FAIL;
}
