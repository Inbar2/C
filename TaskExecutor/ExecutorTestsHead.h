/* 
 * File: ExecutorTestsHead.h
 * Signatures for Automation Tests for Periodic Task Executor project.
 *
 * Author: Inbar Barhum 
 * Date: 29.1.2019
 */ 

#ifndef __EXECUTORTESTSHEAD_H__
#define __EXECUTORTESTSHEAD_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "AutoTestsEnum.h"
#include "Executor.h"


#define TRUE 1
#define FALSE 0



void PrintTestsResult(char _name[], AutoTestResults _testResult, Executor_Result* _status);

/*user functions to send to executor*/
int funcA(void* _context); /*print func - run counter: 3*/
int funcB(void* _context); /*print func - run counter: 4*/
int funcC(void* _context); /*print func - run counter: 5*/
int funcD(void* _context); /*print and pause func - run counter: 1*/
int funcE(void* _context); /*print and pause func - run counter: 2*/
int funcF(void* _context); /*print and pause func - run counter: 3*/
int funcG(void* _context); /*print and run executor func - run counter: 2*/

/*---------------------------------------------------------------------------------------------------------------*/
AutoTestResults ExecutorCreate_Test1(Executor_Result* status); /* create executor */
AutoTestResults ExecutorCreate_Test2(Executor_Result* status); /* create executor with zero space to tasks */

/*---------------------------------------------------------------------------------------------------------------*/
AutoTestResults ExecutorDestroy_Test1(Executor_Result* status); /* destroy executor */
AutoTestResults ExecutorDestroy_Test2(Executor_Result* status); /* destroy executor twice*/

/*---------------------------------------------------------------------------------------------------------------*/
AutoTestResults AddTask_Test1(Executor_Result* status); /*add task to executor*/
AutoTestResults AddTask_Test2(Executor_Result* status); /*add task to uninitialized executor*/
AutoTestResults AddTask_Test3(Executor_Result* status); /*add task to executor with no space for tasks*/
AutoTestResults AddTask_Test4(Executor_Result* status); /*add task to executor space finished*/
AutoTestResults AddTask_Test5(Executor_Result* status); /*add task to executor with no capacity but with block size*/
AutoTestResults AddTask_Test6(Executor_Result* status); /*add multiple tasks to executor with the same period*/
AutoTestResults AddTask_Test7(Executor_Result* status); /*add tasks to executor and pass capacity*/

/*---------------------------------------------------------------------------------------------------------------*/
AutoTestResults RunExecutor_Test1(Executor_Result* status); /* run uninitialized executor*/
AutoTestResults RunExecutor_Test2(Executor_Result* status); /* run executor with no tasks*/
AutoTestResults RunExecutor_Test3(Executor_Result* status); /* run executor with one task*/
AutoTestResults RunExecutor_Test4(Executor_Result* status); /* run executor with 3 tasks*/
AutoTestResults RunExecutor_Test5(Executor_Result* status); /* run executor with 3 tasks with the same period*/
AutoTestResults RunExecutor_Test6(Executor_Result* status); /* run executor with 3 tasks with block size, different times and insert order*/
AutoTestResults RunExecutor_Test7(Executor_Result* status); /* run executor with one task with one pause executor*/
AutoTestResults RunExecutor_Test8(Executor_Result* status); /* run executor with one task with one pause executor and continue*/
AutoTestResults RunExecutor_Test9(Executor_Result* status); /* run executor with one task with multiple pause and continue */
AutoTestResults RunExecutor_Test10(Executor_Result* status); /* run executor with one task with one pause, add task and continue*/
AutoTestResults RunExecutor_Test11(Executor_Result* status); /* run executor with one task with multiple pauses and continue the task (counter > 1)*/
AutoTestResults RunExecutor_Test12(Executor_Result* status); /* run executor with multiple tasks, pause once and continue (counter = 1)*/
AutoTestResults RunExecutor_Test13(Executor_Result* status); /* run executor with multiple tasks and pause, add task and continue (counter = 1)*/
AutoTestResults RunExecutor_Test14(Executor_Result* status); /* run executor with multiple tasks and pause (counter = 1), add multiple tasks (also pause) and continue (counter > 1)*/
AutoTestResults RunExecutor_Test15(Executor_Result* status); /* run executor with multiple tasks and pause (counter = 1), add another executor with one task and continue*/
AutoTestResults RunExecutor_Test16(Executor_Result* status); /* run executor with multiple tasks, which one of them call run executor again*/
AutoTestResults RunExecutor_Test17(Executor_Result* status); /* run executor with Logger and multiple tasks and pause (counter = 1), and continue*/

#endif /* "ExecutorTestsHead.h" include */


