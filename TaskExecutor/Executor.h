/** 
 *  @file Executor.h
 *  @brief header file for executor in Periodic Task Executor project.
 *
 *  @author Inbar Barhum
 * 
 */

#ifndef __EXECUTOR_H__
#define __EXECUTOR_H__

#define TRUE 1
#define FALSE 0


/*if function result is 1 then reschedule task, else remove it from executor*/
typedef int (*userTaskFunc)(void*);

typedef struct Executor Executor;

typedef enum Executor_Result {
						EXE_SUCCESS,				/*executor method worked fine*/
						EXE_UNINITIALIZED,			/*executor is not initialized*/
						EXE_ADD_TASK_FAIL,			/*fail to add task to executor*/
						EXE_PAUSED,					/*executor has been paused*/
						EXE_INVALID_INPUT,			/*input to executor is not valid*/
						EXE_RUN_TASK_FAIL,			/*fail to run the task*/
						EXE_UNDER_FLOW,				/*Executor is empty*/
						EXE_OVER_FLOW,				/*Executor can't add more tasks*/
						EXE_TIMER_ERROR,			/*Executor encountered a timer issue*/
						EXE_RESCHEDUAL_TASK_FAIL,	/*Executor fail to reschedule the task*/
						EXE_LOAD_FAIL,				/*Executor failed to rerun after pause*/
						EXE_EXECUTOR_RUNNING		/*Executor already running*/
						} Executor_Result;

/*
 * @brief: Create new executor.
 * @param[in]: _initCapacity - initial number of tasks that can be stored in the executor.
 * @param[in]: _blockSize - the executor will grow or shrink on demand by this size. 
 * @return: Executor* on success, otherwise return NULL.
 * @warning: if _blockSize is 0 the executor will be a fixed size. 
 * @warning: if both _initialCapacity and _blockSize are zero function will return NULL.
 */
Executor* ExecutorCreate(size_t _initCapacity, size_t _blockSize); 


/*
 * @description: Dynamically deallocate a previously allocated executor. 
 * @param[in]: _exe - pointer to executor to be deallocated, created from ExecutorCreate.
 * @return: void.
 */
void ExecutorDestroy(Executor** _exe);


/*
 * @description: get number of tasks in executor. 
 * @param[in]: _exe - pointer to executor, created from ExecutorCreate.
 * @return: executor's number of tasks.
 * @note: if executor is uninitialized function will return 0.
 */
int GetExecutorNumOfTasks(Executor* _exe);


/* 
 * @description: Add a task to the executor.  
 * @param[in]: _exe - pointer to executor to append task to, created from ExecutorCreate.
 * @param[in]: _taskFunc - pointer to function that the user supplied.
 * @param[in]: _period - the interval time beteen runs of the function.
 * @param[in]: _context - user provided context, will be sent as param to task function.
 * @return: Executor_Result message representing success or error message.
 * @retval: EXE_SUCCESS 
 *			EXE_UNINITIALIZED
 *			EXE_ADD_TASK_FAIL
 *			EXE_OVER_FLOW
 */
Executor_Result AddTask(Executor* _exe, userTaskFunc _taskFunc, size_t _period, void* _context);


 /* 
 * @description: Start to run the periodic execution of the tasks (run the executor).
 * @param[in]: _exe - pointer to executor to start running, created from ExecutorCreate.
 * @return: Executor_Result message representing success or error message.
 * @retval: EXE_SUCCESS 
 *			EXE_UNINITIALIZED
 *			EXE_PAUSED
 *			EXE_LOAD_FAIL
 *			EXE_TIMER_FAIL
 *			EXE_RESCHEDUAL_TASK_FAIL
 *
 * @note: after executor pause, when RunExecutor is called it will start execution cycle as if it was called for the first time (tasks will be ordered according to their period).
 */
Executor_Result RunExecutor(Executor* _exe);

/* 
 * @description: function update executor pause status to TRUE if task run order to pause.
 * @param[in]: _exe - pointer to executor, created from ExecutorCreate.
 * @return: void.
 * @note: task will complete it run and then pause the executor from running more tasks.
 */
void ExecutorPause(Executor* _exe);

/* 
 * @description: function prints the tasks next run time in the executor.
 * @param[in]: _exe - pointer to executor, created from ExecutorCreate.
 * @return: void.
 * @note: task will complete it run and then pause the executor from running more tasks.
 */
void PrintExecutorTasks(Executor* _exe);

#endif /* Executor.h */



