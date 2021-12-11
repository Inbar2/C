/*assembler.c*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fileProcess.h"
#include "constants.h"

			   
int main(int argc, char* argv[])
{
	int i;
	int suffixSize;
	
	/*check if any arguments ware typed in command line besides the run program file command*/
	if(argc<=1)
	{
		printf("\nError: Missing file name\\s to process. Exit Program.");
		exit(1);
	}
	
	suffixSize = strlen(FILE_SUFFIX);
	
	/*there is an argument besides the run program file*/
	/*read arguments of command line one after the other*/
	for(i=1; i<argc; i++)
	{
		processFile(argv[i], suffixSize);
		printf("\n");
	}

	return 0;
}
