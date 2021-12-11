#include "syntax.h"

int checkStrOnlySpaces(const char* _str)
{
	while(*_str != '\0')
	{
		if(isspace(*_str)==0)
		{
			return 0;
		}
		_str++;
	}
	return 1;
}

int countSpacesCluster(const char* _str)
{
	int count=0;
	while(*_str != '\0')
	{
		if(!isspace(*_str))
		{
			return count;
		}
		_str++;
		count++;
	}
	return count;
}

/*if first non-space char is a comma return the amount of characters passed, otherwise the string finished or all characters are spaces - return -1*/
int countSpacesToComma(const char* _str)
{
	int count=0;
	count = countSpacesCluster(_str);
	if(*(_str+count) == ',')
	{
		return count;
	}
	return -1; /*no comma*/
}	

int CheckBlankString(char* _str)
{
	if(NULL==strtok(_str, " \n\r\t\v\b\f"))
	{
		return 1;
	}
	return 0;
}

int checkCmdName(char* _cmdName)
{
	if(strcmp(_cmdName, "read_set")==0 || strcmp(_cmdName, "print_set")==0 || strcmp(_cmdName, "union_set")==0 || strcmp(_cmdName, "intersect_set")==0 || 	strcmp(_cmdName, "sub_set")==0 || strcmp(_cmdName, "symdiff_set")==0 || strcmp(_cmdName, "stop")==0)
	{
		return 1;
	}
	return 0;
}

int isNumExist(int* _arr, int _size, int _numParams)
{
	int i=0;
	while(i<_size)
	{
		if(_arr[i] == _numParams)
		{
			return 1;
		}
		i++;
	}
	return 0;
}

set* getSetByName(char* _setName, const sets* _setsArr)
{
	int i=0;
	while(NULL != _setsArr[i].m_set)
	{
		if(!strcmp(_setsArr[i].m_name, _setName))
		{
			return _setsArr[i].m_set; /*return relevant set*/
		}
		i++;
	}
	return NULL;
}
	
int checkParamsSyntax(char* _str, int* _paramArr, int* _paramCount)
{
	char* numStr, *endParam;
	char strCopy[MAX_LINE_LENGTH];
	int i, num, isSpaceInNum=0;
	size_t numSize;
	int negFlag=0, digFlag=0, spacesSize=0; /*if a negative mark(minus) appears or a digit*/
	
	*_paramCount=0;
	
	/*check if word is an integer*/
	strcpy(strCopy, _str);
	numStr = strtok(strCopy, ",");

	while(NULL!=numStr)
	{		
		isSpaceInNum=0;
		spacesSize = countSpacesToComma(_str);
		if(spacesSize >= 0)
		{
			printf("\nInvalid set member – Multiple consecutive commas\n"); /*missing an integer between commas*/
			return 0;
		}
		numSize = strlen(numStr);

		/*check operand string*/
		for(i=0; i<numSize; i++)
		{
			if(isdigit(numStr[i]))
			{
				digFlag = 1; /*char is a digit*/
				spacesSize=0;
				continue;
			}
			if(isalpha(numStr[i]) || ispunct(numStr[i])) /*char isn't a digit*/
			{
				if(numStr[i] == '-' && !negFlag && !digFlag) /*the negative mark didn't appear before and appears before a digit*/
				{
					negFlag = 1; /*the negative mark appeard once and not after a digit*/
					continue;
				}
			
				printf("\nInvalid set member – not an integer\n");
				return 0;
			}
			
			if(isspace(numStr[i])) /*char is a space*/
			{
				if(digFlag)
				{
					if(!checkStrOnlySpaces(numStr+i))
					{
						isSpaceInNum = 1;
						strncpy(numStr, numStr, i);
						break;
					}
				}
			}
		}

	/* numStr ia an integer number or termination number*/
	num = atoi(numStr);

	if(num==0 && strcmp(numStr,"0"))
	{
		printf("\nInvalid set member – not an integer\n");
		return 0;
	}
	if(num>127 || (num<0 && num != atoi(TERM_OP)))
	{
		printf("\nInvalid set member – value out of range\n");
		return 0;
	}

	/*if num is the termination number of params*/
	if(num == atoi(TERM_OP))
	{
		endParam = strtok(NULL, ",");
		if(endParam != NULL || !checkStrOnlySpaces(strstr(numStr,TERM_OP)+strlen(TERM_OP)))
		{
			printf("\nInvalid set member – Extraneous text after termination integer\n");
			return 0;
		}
		else
		{
			return 1; /*command line ended correctly*/
		}
	}
		
	if(NULL==strstr(_str, TERM_OP))
	{
		printf("\nList of set members is not terminated correctly\n");
		return 0;
	}
	
	if(isSpaceInNum)
	{
		printf("\nInvalid set member – Missing comma between integers\n"); /*the space appears between an integer and another parameter*/
		return 0;
	}
	/*If integer number is valid and didn't appear before, store the number in the array and increase it's items count*/		
	if(isNumExist(_paramArr, *_paramCount, num)==0)
	{
		_paramArr[*_paramCount] = num;
		*_paramCount = *_paramCount + 1;
	}

	_str = _str+numSize+1; /*+1 for the comma after it*/
	strcpy(strCopy, _str);
	numStr = strtok(strCopy, ",");

	/*reset var for next word check*/
	spacesSize = 0;
	digFlag = 0;
	negFlag = 0;
	}
	
	return 1;
}


int checkLineSyntax(char* _line, const sets* _setsArr, int* _isStop)
{
	int cmdSize, spaceCount, setsCount=0, paramSize=0, paramRes;
	int paramArr[MAX_BITS] = {-1}; /*array to store the numbers*/
	char* cmdStr, *newCmdName, *setName;
	char cmdName[MAX_LINE_LENGTH], lineCopy[MAX_LINE_LENGTH]; /*buffer is the size of line because line can be one word (all letters attached)*/
	set* mySet, *funcSets[MAX_SET_NUM];
	
	/*if there are blank spaces in the beginning of the line*/
	spaceCount = countSpacesCluster(_line);
	if(spaceCount)
	{
		/*there were spaces - remove them*/
		_line = _line+spaceCount;
	}
	
	strcpy(lineCopy, _line);
	cmdStr = strtok(lineCopy, " \n\r\t\v\b\f"); /*extract the first word from the line*/
	if(NULL == cmdStr)
	{
		return 0;
	}

	/*check if command word doesn't start with a symbol or digit*/
	if(ispunct(*cmdStr) || isdigit(*cmdStr))
	{
		printf("\nUndefined command name\n");
		return 0;
	}

	if(!strncmp(cmdStr, "stop", strlen("stop"))) /*check if cmd is stop*/
	{
		/*check if there isn't any operands except spaces after a stop command*/
		if(!strcmp(cmdStr, "stop") && checkStrOnlySpaces(_line+strlen(cmdStr)))
		{
			*_isStop=1;
			return 1;
		}
		printf("\nIllegal operand (afrer stop command name)\n");
		return 0;
	}
	
	/*check if the first word is a command name*/
	if(!checkCmdName(cmdStr))
	{
		strcpy(cmdName, cmdStr);
		newCmdName = strtok(cmdName, ","); /*ignore comma at the end of command name*/

		if(strcmp(cmdStr, newCmdName) != 0) /*if not equals, there is a comma at the end of command word*/
		{
			if(checkCmdName(newCmdName)) /*if after removing the comma the command name is legal*/
			{
				printf("\nIllegal comma (as part of the command name word)\n");
				return 0;
			}
		}
		printf("\nUndefined command name\n");
		return 0;
	}
	strcpy(cmdName, cmdStr);

	/*check if there is a comma after command name and before the set name*/
	cmdSize = strlen(cmdStr);
	_line = _line+cmdSize;

	/*check for set name after command name*/
	while(setsCount < MAX_SET_NUM)
	{
		spaceCount = countSpacesCluster(_line); /*start position of next word*/
		_line = _line+spaceCount; /*points to the rest of line*/
		if(*_line==',') /*set hasn't been read yet, only command name*/
		{
			if(!setsCount)
			{
				printf("\nIllegal comma (afrer command name word)\n");
				return 0;
			}
			else
			{
				printf("\nInvalid set member – Multiple consecutive commas\n"); /*missing an integer between commas*/
				return 0;
			}
		}
	
		strcpy(lineCopy, _line); /*points to the rest of line*/
		setName = strtok(lineCopy, ", \n\r\t\v\b\f");
		if(*_line=='\0')
		{
			printf("\nMissing parameter (set name)\n");
			return 0;
		}
					
		mySet = getSetByName(setName, _setsArr); /*check if the word represents a set, if so - the set is returned to var*/

		if(NULL == mySet)
		{
			printf("\nUndefined set name\n"); /*not a set name*/
			return 0;
		}
		funcSets[setsCount] = mySet;
		setsCount++;
		_line = _line+strlen(setName); /*points to the rest of line*/
		strcpy(lineCopy, _line); /*copy rest of line after the set name*/	

		/*check for a comma after set name, except if command is print_set*/	
		if(checkStrOnlySpaces(lineCopy))
		{
			/*if command is print_set*/
			if(!strcmp(cmdName,"print_set"))
			{
				print_set(mySet);
				return 1;
			}
		}
		else if(setsCount == MAX_SET_NUM || !strcmp(cmdName,"print_set"))
		{
				printf("\nExtraneous text after end of command\n");
				return 0;	
		}

		if((spaceCount=countSpacesToComma(lineCopy))==-1 && setsCount < MAX_SET_NUM)
		{
				printf("\nMissing comma\n");
				return 0;
		}
		
		_line = _line+spaceCount+1; /*copy rest of line after the set name, spaces and comma*/	
	
		if(!strcmp(cmdName,"read_set"))
		{
			paramRes = checkParamsSyntax(_line, paramArr, &paramSize);
			if(paramRes)
			{
				read_set(mySet, paramArr, paramSize);
				return 1;
			}
			else
			{
				return 0;
			}
		}
	}
	
	/*command line is valid*/
	if(!strcmp(cmdName,"union_set"))
	{
		union_set(funcSets[0], funcSets[1], funcSets[2]);
	}
	
	if(!strcmp(cmdName,"intersect_set"))
	{
		intersect_set(funcSets[0], funcSets[1], funcSets[2]);
	}
	
	if(!strcmp(cmdName,"sub_set"))
	{
		sub_set(funcSets[0], funcSets[1], funcSets[2]);
	}
	
	if(!strcmp(cmdName,"symdiff_set"))
	{
		symdiff_set(funcSets[0], funcSets[1], funcSets[2]);
	}
		
	return 1;
}
