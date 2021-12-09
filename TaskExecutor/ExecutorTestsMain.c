/*Tests executor*/
#include <stdio.h>
#include <stdlib.h>

#include "ExecutorTestsHead.h"
#include "zlog4c.h"

/*print func - run counter: 3*/
int funcA(void* _context)
{
	static int numOfRuns = 3;
	
	printf("Hi test A\n");
	numOfRuns--;
	if(numOfRuns > 0)
	{
		return TRUE;
	}
	
	numOfRuns = 3;
	return FALSE;
}

/*print func - run counter: 4*/
int funcB(void* _context)
{
	static int numOfRuns = 4;

	printf("Hi test B\n");
	numOfRuns--;
	if(numOfRuns > 0)
	{
		return TRUE;
	}	

	numOfRuns = 4;
	return FALSE;
}

/*print func - run counter: 5*/
int funcC(void* _context)
{
	static int numOfRuns = 5;

	printf("Hi test C\n");
	numOfRuns--;
	if(numOfRuns > 0)
	{
		return TRUE;
	}

	numOfRuns = 5;
	return FALSE;
}

/*print func and pause - run counter: 1*/
int funcD(void* _context)
{
	static int numOfRuns = 1;
	
	printf("Hi test D paused executor\n");	
	ExecutorPause((Executor*) _context);
	numOfRuns--;
	if(numOfRuns > 0)
	{
		return TRUE;
	}

	numOfRuns = 1;	
	return FALSE;
}

/*print func and pause - run counter: 2*/
int funcE(void* _context)
{
	static int numOfRuns = 2;
	
	printf("Hi test E paused executor\n");	
	ExecutorPause((Executor*) _context);
	numOfRuns--;
	if(numOfRuns > 0)
	{
		return TRUE;
	}

	numOfRuns = 2;	
	return FALSE;
}

/*print func and pause - run counter: 3*/
int funcF(void* _context)
{
	static int numOfRuns = 3;
	
	printf("Hi test F paused executor\n");	
	ExecutorPause((Executor*) _context);
	numOfRuns--;
	if(numOfRuns > 0)
	{
		return TRUE;
	}

	numOfRuns = 3;	
	return FALSE;
}

/*print and run executor func - run counter: 2*/
int funcG(void* _context)
{
	static int numOfRuns = 2;
	Executor_Result exeStatus;
	
	printf("Hi test G - ask to executor to run again\n");
	exeStatus = RunExecutor(_context);
	ZLOG("LOG_WARNING", "Function tried to run executor again - message no.%d", exeStatus);
	numOfRuns--;
	if(numOfRuns > 0)
	{
		return TRUE;
	}

	numOfRuns = 2;
	return FALSE;
}

int main()
{
Executor_Result status;

/*
PrintTestsResult("ExecutorCreate_Test1", ExecutorCreate_Test1(&status), &status);
PrintTestsResult("ExecutorCreate_Test2", ExecutorCreate_Test2(&status), &status);

PrintTestsResult("ExecutorDestroy_Test1", ExecutorDestroy_Test1(&status), &status);
PrintTestsResult("ExecutorDestroy_Test2", ExecutorDestroy_Test2(&status), &status);

PrintTestsResult("AddTask_Test1", AddTask_Test1(&status), &status);
PrintTestsResult("AddTask_Test2", AddTask_Test2(&status), &status);
PrintTestsResult("AddTask_Test3", AddTask_Test3(&status), &status);
PrintTestsResult("AddTask_Test4", AddTask_Test4(&status), &status);
PrintTestsResult("AddTask_Test5", AddTask_Test5(&status), &status);
PrintTestsResult("AddTask_Test6", AddTask_Test6(&status), &status);
PrintTestsResult("AddTask_Test7", AddTask_Test7(&status), &status);

PrintTestsResult("RunExecutor_Test1", RunExecutor_Test1(&status), &status);
PrintTestsResult("RunExecutor_Test2", RunExecutor_Test2(&status), &status);
PrintTestsResult("RunExecutor_Test3", RunExecutor_Test3(&status), &status);
PrintTestsResult("RunExecutor_Test4", RunExecutor_Test4(&status), &status);
PrintTestsResult("RunExecutor_Test5", RunExecutor_Test5(&status), &status);
PrintTestsResult("RunExecutor_Test6", RunExecutor_Test6(&status), &status);
PrintTestsResult("RunExecutor_Test7", RunExecutor_Test7(&status), &status);
PrintTestsResult("RunExecutor_Test8", RunExecutor_Test8(&status), &status);
PrintTestsResult("RunExecutor_Test9", RunExecutor_Test9(&status), &status);
PrintTestsResult("RunExecutor_Test10", RunExecutor_Test10(&status), &status);
PrintTestsResult("RunExecutor_Test11", RunExecutor_Test11(&status), &status);
PrintTestsResult("RunExecutor_Test12", RunExecutor_Test12(&status), &status);
PrintTestsResult("RunExecutor_Test13", RunExecutor_Test13(&status), &status);
PrintTestsResult("RunExecutor_Test14", RunExecutor_Test14(&status), &status);
PrintTestsResult("RunExecutor_Test15", RunExecutor_Test15(&status), &status);
*/
PrintTestsResult("RunExecutor_Test16", RunExecutor_Test16(&status), &status);
PrintTestsResult("RunExecutor_Test17", RunExecutor_Test17(&status), &status);


	return 0;
}
