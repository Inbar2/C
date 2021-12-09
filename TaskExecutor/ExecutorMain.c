/*/work/advenceC/TaskExecutor$ gcc -g -Wall -ansi -pedantic -I../includeGen -I../projects/LoggerPro ../genericVector/vectorGenericFunc.c ../genericHeap/genHeapFunc.c ExecutorTestsMain.c ExecutorTestsFunctions.c ExecutorTestsHead.h Executor.c ../projects/LoggerPro/zlog4c.c
*/



#include "Executor.h"
#include "zlog4c.h"

/* https://www.draw.io/#G1Bci-Q53G9uXbe_XUibmPM4cwHQEzac-5 */

int main()
{


CreateExecutor();

DestroyExecutor(exe);

AddTask(exe, taskFunc, period, context);

StartExecutor(_exe);

IsExecutorRunning();


return 0;
}

