#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h> /*for v_args*/ 
#include <unistd.h> /*for getting the process id*/
#include <pthread.h> /*for getting the thread id*/
#include <time.h> /*for getting the data format*/


#include "zlog4c.h"

#define STRING_SIZE 64
#define STATUS_SIZE 9
#define TEXT_LINE 512

static void SetValToStruct(FILE* _file, char* _member);
static LogStatus OpenLogFile(const char* _fileName);
static char GetLevelLetter(int _levelIndex);
static LogStatus GetLevelNum(char* _levelChoice, int* _index);
static void CloseLogFile(void);

typedef struct ZLog /*struct is only known in this c file*/
{
	char m_level[STRING_SIZE];
	char m_file[STRING_SIZE];
} ZLogger_t;
					
				
static ZLogger_t* newZLog;
static FILE* logFile;
static char m_levelArr[STATUS_SIZE][STRING_SIZE] = {"LOG_TRACE", "LOG_DEBUG", "LOG_INFO","LOG_WARNING", "LOG_ERROR", "LOG_CRITICAL", "LOG_SEVERE", "LOG_FATAL", "LOG_NONE"};


LogStatus zlog_init(const char* _fileName)
{
	LogStatus res;
	
	/*create struct logger*/
	newZLog = (ZLogger_t*) malloc(sizeof(ZLogger_t));
	if(NULL == newZLog)
	{
		return ERR_LOGGER_ALLOC;
	}
	
	/*open the configuration file to get data*/
	res = OpenLogFile(_fileName);
	if(res != STATUS_OK)
	{
		return res;
	}

	/*open the log file for writing*/
	logFile = fopen(newZLog->m_file, "a"); /*Mode: append*/
	if(!logFile)
	{
		return ERR_OPENNING_FILE;
	}
	
	atexit(CloseLogFile);
	
	return STATUS_OK;
}

int IsLevelToWrite(char* _userLevelChoice)
{
	int userSever, confSever;
	LogStatus userLevel, confLevel;
	if((userLevel = GetLevelNum(_userLevelChoice, &userSever)) != STATUS_OK)
	{
		return FALSE;
	}
	
	if((confLevel = GetLevelNum(newZLog->m_level, &confSever)) != STATUS_OK)
	{
		return FALSE;
	}

	if(userSever >= confSever)
	{
		return TRUE;
	}
	return FALSE;
}


void PrintfLogToFile(char _mode, char* _levelName, char* _message, const char* _funcName, const char* _fileName, int _line, ...)
{
    time_t rawtime;
	va_list args;
	int levelNum;
    struct tm* tm;
    rawtime = time(NULL);
    tm = localtime(&rawtime);
	GetLevelNum(_levelName, &levelNum);
	
	va_start(args, _line);

	fprintf(logFile, "%d-%d-%d " ,tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday); 
	fprintf(logFile, "%d:%d:%d " ,tm->tm_hour, tm->tm_min, tm->tm_sec);		
	fprintf(logFile, " %d %lu ", getpid(), pthread_self()); 
	fprintf(logFile, "%c ", GetLevelLetter(levelNum));
	if(_mode == 'R')
	{	
		#ifndef NDEBUG
		fprintf(logFile, "%s@%s:%d ", _funcName, _fileName, _line);
		#endif /* NDEBUG */
	}
	vfprintf(logFile, _message, args);
	fprintf(logFile, "\n");

	va_end(args);
}


/*--------------------------------------------------*/
/*------------------Static Methods------------------*/
/*--------------------------------------------------*/
static void CloseLogFile(void)
{
	fclose(logFile);
	free(newZLog);
	printf("Closed log file.\n");
}


static LogStatus OpenLogFile(const char* _fileName)
{
	FILE* myConfFile;
	
	if(NULL == _fileName)
	{
		return ERR_INVALID_FILE;
	}
	
	myConfFile = fopen(_fileName, "r");
	if(!myConfFile)
	{
		return ERR_OPENNING_FILE;
	}
	
	SetValToStruct(myConfFile, newZLog->m_level);
	SetValToStruct(myConfFile, newZLog->m_file);
	
	fclose(myConfFile);
	return STATUS_OK;
}


static void SetValToStruct(FILE* _file, char* _member)
{
	char line[TEXT_LINE];
	char* pch;

	fgets(line, TEXT_LINE, _file); /*get line from file*/
  	pch = strtok (line, "= \n");
  	pch = strtok (NULL, "= \n");

	strcpy(_member, pch);
}


static char GetLevelLetter(int _levelIndex)
{	
	switch(_levelIndex)
	{
		case LOG_TRACE: 	return 'T' ; break;
		case LOG_DEBUG: 	return 'D' ; break;
		case LOG_INFO: 		return 'I' ; break;
		case LOG_WARNING: 	return 'W' ; break;
		case LOG_ERROR: 	return 'E' ; break;
		case LOG_CRITICAL: 	return 'C' ; break;
		case LOG_SEVERE: 	return 'S' ; break;
		case LOG_FATAL: 	return 'F' ; break;
	}
	return 'N';
}


/*TODO maybe change func that not each time do the while and search for the index*/
static LogStatus GetLevelNum(char* _levelChoice, int* _index)
{
	int i;
	int isFound = FALSE;
	
	if(_levelChoice == NULL)
	{
		return ERR_INVALID_PARAM;
	}

	i = 0;
	while(i < STATUS_SIZE)
	{
		if(strcmp(m_levelArr[i], _levelChoice) == 0)
		{
			isFound = TRUE;
			*_index = i;
			break;
		}
		i++;
	}
	
	if(!isFound)
	{
		return ERR_INVALID_PARAM;
	}
	return STATUS_OK;
}

