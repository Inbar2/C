/*
File: zlog4c.h
Description: Header file for LOGGER.
*/


#ifndef __ZLOG4C_H__
#define __ZLOG4C_H__

#define TRUE 1
#define FALSE 0


#define NDEBUG
/*
A log entry written to the log file will look differently depending on the build type.
Debug builds:
If NDEBUG is not defined when building the code using the log facility, then an entry will look like: 

 +- year
 |   +- month              +- process id
 |   |  +- day             |      +- thread id      
 |   |  |                  |      |                 
2016-4-29  22:43:20.244  40059  1299 I query dequeue@reader.c:9 Queued tasks: 4 
            |                        |  |     |      |        | +-- message w/ params
            +- time                  |  |     |      |        +--- line number
                                     |  |     |      +- source file name
                                     |  |     +- function name   
                                     |  +- tag - Module name
                                     +- log level (first letter)
                                      
if NDEBUG is defined print log without file name, function name and line number, else print log in Release mode.*/
#ifndef NDEBUG
#define ZLOG(level, message, ...) (IsLevelToWrite(level) ? PrintfLogToFile('R', level, message, __func__, __FILE__, __LINE__, __VA_ARGS__) : (void)0)

#else
#define ZLOG(level, message, ...) (IsLevelToWrite(level) ? PrintfLogToFile('D', level, message, __func__, __FILE__, __LINE__, __VA_ARGS__) : (void)0)
#endif




enum LogStatus{
				STATUS_OK,
				ERR_LOGGER_ALLOC,
				ERR_INVALID_FILE,
				ERR_OPENNING_FILE,
				ERR_WRITING_FILE,
				ERR_CLOSE_FILE,
				ERR_INVALID_PARAM
				};
typedef enum LogStatus LogStatus;

/*Log Levels - The Log will support various levels of logging.*/
/*LOG_TRACE has the lowest priority, thus will allow all log entries to write to the log file.*/
typedef enum LogLevel{
				LOG_TRACE = 0,		/*Trace message usually very detailed*/
				LOG_DEBUG,			/*A message useful for debugging*/
				LOG_INFO,			/*Informative message*/
				LOG_WARNING,		/*Warning*/
				LOG_ERROR,			/*An error occurred*/
				LOG_CRITICAL,		/*Critical error*/
				LOG_SEVERE,			/*Server error requiring immediate intervention*/
				LOG_FATAL,			/*Fatal error signaling an emergency*/
				LOG_NONE			/*Used only in configuration file*/
				} LogLevel;				


/*
The Log facility will be configured by a simple configuration file.
The configuration file will have general setting, for example:

	Level = default_log_level
	File = log_base_directory/default_log_file_name
*/


/*
 * @Description: function open configuration log file, load configuration and open the log file.
 * @Param: _FILEnAME - The configuration file name to read from.
 * @Return: FILE_OK - logStatus saying all initiation process went fine.
 * 			ERR_LOGGER_ALLOC,
			ERR_INVALID_FILE,
			ERR_OPENNING_FILE
 * @Notes: At exit function closes log file.
 */
LogStatus zlog_init(const char* _fileName);


/*
 * @Description: function gets user entry log level name and check if the severity of an entry log is lower than the current log level.
 				If it is it will return TRUE, otherwise it will be written to the log file.
 * @Param: _userLevelChoice - The configuration file name to read from.
 * @Return: If log entry severity is lower than the current log level it will return FALSE (log will be ignored).
 			Otherwise return TRUE (log will be written to the log file).
 * @Warnning: If log level doesn't exist, will return FALSE (log will be ignored).
 */
int IsLevelToWrite(char* _userLevelChoice);


void PrintfLogToFile(char _mode, char* _levelName, char* _message, const char* _funcName, const char* _fileName, int _line, ...);




#endif /* zlog4c.h */
