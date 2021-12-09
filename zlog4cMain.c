/*
File: zlog4cMain.c
Description: Main file for LOGGER.
Created by: Inbar Barhum
Last Update: 22.1.2019
*/

/* gcc -g -Wall -ansi -pedantic zlog4cMain.c zlog4c.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "zlog4c.h"

int main()
{
	LogStatus status;

	/*call function zlog_init with file name*/
	status = zlog_init("zlogConfFile");
	printf("Logger initiation status: %d\n", status);
	if(status == 0)
	{
		printf("\nLogger OK - Can handle logs enteries.\n");
		ZLOG("LOG_TRACE", "My name is inbar %d ", 9);
		ZLOG("LOG_ERROR", "Hello my message is %d %c", 4, 'u');
		ZLOG("LOG_SEVERE", "Severe error happened %s%c", "error 12345", '!');
	}
	
	return 0;
}






