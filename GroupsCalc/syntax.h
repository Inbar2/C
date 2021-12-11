#include "set.h"

/*method check if characters type from given ptr position to end of string consists only spaces.
Return 1 if it is, otherwise return 0*/
int checkStrOnlySpaces(const char* _str);

/*method counts the space type characters from given ptr position until it encounter a non-space char type or end of string. Returns the amount of spaces characters passed, If none, return 0.*/
int countSpacesCluster(const char* _str);

/*method counts the space type characters from given ptr position until it encounter a comma char or end of string. Return the amount of spaces characters passed, if didn't encounter a comma, returns -1.*/
int countSpacesToComma(const char* _str);	

/*method check if command line string has space type characters in the beginning of the string, if it does returns 1, otherwise return 0*/
int CheckBlankString(char* _str);

/*check if given cmd name string is one of the command words possible, if it does returns 1, otherwise return 0*/
int checkCmdName(char* _cmdName);

/*check if the number already exist in the parameters array, if it does returns 1, otherwise return 0*/
int isNumExist(int* _arr, int _size, int _numParams);

/*check if given set name string exists in the sets array, if it does returns the actual set variable with the same name, otherwise return NULL*/
set* getSetByName(char* _setName, const sets* _setsArr);

/*check the parameters string for the read_set command. If syntax is not valid prints the first error encounterd and return 0, otherwise insert the parameters (numbers) to parameters array and return 1*/
int checkParamsSyntax(char* _str, int* _paramArr, int* _paramCount);

/*Check the command line syntax. If syntax is not valid prints the first error encounterd and return 0, otherwise return 1. If command is 'stop' update the isStop value with 1*/
int checkLineSyntax(char* _line, const sets* _setsArr, int* _isStop);

