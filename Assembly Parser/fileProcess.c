/*fileProcess.c*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h> /*to check if file exist*/

#include "fileProcess.h"
#include "firstPass.h"
#include "symbolTable.h"
#include "errors.h"
#include "generalFuncs.h"
#include "instructions.h"
#include "directives.h"
#include "secondPass.h"
#include "outputFiles.h"

static bool isBlankLine(char* _str);
static bool isCommentLine(char* _str);
static void readSourceFile(FILE* _filePtr, sourceLine_info* _lineInfo);
static bool isFileEmpty(FILE* _filePtr);
static bool isFileExists(const char* _name);

void processFile(char* _fileStr, int _suffixSize)
{
	FILE* filePtr;
	int fileSize;
	sourceLine_info lineInfo;
	fileSize = strlen(_fileStr);
	
	/*check if file name has a correct file suffix*/
	if(strcmp(_fileStr+(fileSize-_suffixSize), FILE_SUFFIX))
	{
		printf("Error: File \"%s\" is not a valid assembler file and was skipped.\n", _fileStr);
		return;
	}

	filePtr = fopen(_fileStr, "r"); /*open file in read mode*/
	if(NULL==filePtr)
	{	
		perror("\nOpen File Error");
		if(!isFileExists(_fileStr))
		{
			printf("Error: File name \"%s\" doesn't exist in current folder was skipped.\n", _fileStr);
		}
		else
		{
			printf("Error: File name \"%s\" can't be opened for reading and was skipped.\n", _fileStr);
		}
		return;
	}
	
	lineInfo.m_fileName = _fileStr;
	/*file is open - read each line from the source code file*/
	readSourceFile(filePtr, &lineInfo);
	fclose(filePtr);
}

/*function reads the data from the received file pointer and process it*/
static void readSourceFile(FILE* _filePtr, sourceLine_info* _lineInfo)
{
	char line[MAX_LINE_LENGTH+2]; /*added +1 for the '\n' char in line  and the '\0' char added after copying*/
	char copyLine[MAX_LINE_LENGTH+2]; /*added +2 for the '\n' char in line and the '\0' char added after copying*/
	long lineCount = 1; /*line Counter, starts counting the lines from 1*/
	char c; /*single char in file text*/
	bool isProcSucc = TRUE; /*flag if process successfully done*/
	bool isValidLine = TRUE;
	bool isEntry = FALSE; /*flag indicate if there are enteries on file */
	bool isEmpty = FALSE;
	long icf, dcf; /*final values of ic and dc*/
	long ic = IC_INIT_VAL; /*IC - Instruction Counter*/
	long dc = DC_INIT_VAL; /*DC - Data Counter*/
	list* externalsList = NULL; /*list that hold all externals that have been used in instructions and addresses */
	list* codeImgList = createList(); /*Instruction code image*/
	list* dataImgList = createList(); /*Data image of the directives lines*/

	symbolTable* table = createSymbolTable(); /*create the symbol table to store symbols appear in the file*/
	externalsList = createList(); /*to keep addresses externals are used*/
	if(NULL == table)
	{	
		printf("\nERROR: During processing file \"%s\" the allocation of the symbol table failed", _lineInfo->m_fileName);
	}

	isEmpty = isFileEmpty(_filePtr);
	if(!isEmpty)
	{
		rewind(_filePtr); /*set file cursor to point to the beginning of the file*/		
		while(!feof(_filePtr))
		{
			if(fgets(line, MAX_LINE_LENGTH+2, _filePtr)) /*added +2 for the '\n' char in line and the '\0' char added after copying*/
			{
				isValidLine = TRUE;
				_lineInfo->m_lineNum = lineCount; /*update line info with the current line number*/
				_lineInfo->m_sourceLine = line; /*update line info to point to the current code line*/
				
				if(line[strlen(line)-1] != '\n') /*Excluding EOF line that shorter than max defined becuase it's the last line on file and doesn't have '\n'*/
				{
					/*line length is bigger than max defined*/
					/*move over extra characters in line to point to start of the next line*/
					while((c = fgetc(_filePtr)) != '\n' && c != EOF)
					{
						isValidLine &= (!isspace(c)) ? FALSE : TRUE;
					}
					if(!isValidLine)
					{
						printLineError(ERROR, _lineInfo, "contains more than %d characters.", MAX_LINE_LENGTH);
						isProcSucc = FALSE;
					}
					else
					{
						/*the command code fits in line but code has extra white space characters*/
						printf("\nNOTE: In file \"%s\", Line %ld has white spaces that extend the max %d characters length for line.", _lineInfo->m_fileName, lineCount, MAX_LINE_LENGTH);
					}
				}
				strcpy(copyLine, line);
				if(isBlankLine(copyLine)==FALSE && isCommentLine(copyLine)==FALSE && isValidLine) /*ignore blank, comment lines and command line with error*/
				{
					if(!performFirstPass(_lineInfo, table, &ic, &dc, codeImgList, dataImgList)) /*do first pass - line isn't blank nor a Comment, and command is written in the first 80 characters*/
					{
						isProcSucc = FALSE;
					}
				}
			}
			lineCount++;	
		}/*end of loop reading file source*/
	}
	 
	/*saving end values of ic and dc for building the output files later*/
	icf = ic;
	dcf = dc;
	
	/*check if first pass over the file was without errors*/
	if(isProcSucc && !isEmpty)
	{
		isProcSucc = TRUE; /*update flag for 2nd pass*/
		updateSymbolsAdresses(table, icf); /*update in symbol table each DATA symbol's address*/
		updateDataImgAddresses(dataImgList, icf); /*update each data item address on the data image*/
		rewind(_filePtr); /*set file cursor to point to the beginning of the file*/		
		lineCount = 1; /*reset line counter to first line*/
		ic = IC_INIT_VAL; /*set ic value to initial again to know instruction line address*/

		/*start second pass - go over file again*/
		while(!feof(_filePtr))
		{
			if(NULL != fgets(line, MAX_LINE_LENGTH+2, _filePtr)) /*added +2 for the '\n' char in line and the '\0' char added after copying*/
			{
				_lineInfo->m_lineNum = lineCount;
				_lineInfo->m_sourceLine = line;
				if(line[strlen(line)-1] != '\n') 
				{
					while((c = fgetc(_filePtr)) != '\n' && c != EOF); /*for lines that have only extra white spaces after max line size*/
				}
				strcpy(copyLine, line);		
				if(isBlankLine(copyLine)==FALSE && isCommentLine(copyLine)==FALSE) /*ignore blank or comment lines*/
				{
					/*if 2nd pass continues to be valid over each line the all pass will be valid, otherwise false*/
					if(!performSecondPass(_lineInfo, table, &ic, codeImgList, externalsList, &isEntry))
					{
						isProcSucc = FALSE;
					}
				}
			}
			lineCount++;
		}
		/*check total image didn't exceed the computer total memory size*/
		if(icf + dcf - IC_INIT_VAL > MAX_MEM_SIZE)
		{
			printf("\nERROR: Total image memory size (%ld) exceeded the computer total memory size(%d).", icf + dc - IC_INIT_VAL, MAX_MEM_SIZE);
		}
	}
	
	/*check if second pass over the file was without errors*/
	if(isProcSucc)
	{
		/*create output files*/
		createOutputFiles(table, externalsList, codeImgList, dataImgList, _lineInfo->m_fileName, icf, dcf, isEntry);
	}
	
	/*free external, symbol and image lists*/
	destroyList(&externalsList, elementExternalDestroy);
	destroyList(&codeImgList, elementCodeDestroy);
	destroyList(&dataImgList, elementDataDestroy);
	destroySymbolTable(&table);


} /*return to calling method on assembler.c file, and check if there is another file*/

/*check if line is blank*/
static bool isBlankLine(char* _str)
{
	return isStrOnlySpaces(_str, 0);
}

/*check if line is a comment line*/
static bool isCommentLine(char* _str)
{
	int i = skipSpaces(_str, 0);
	if(_str[i]==';')
	{
		return TRUE;
	}
	return FALSE;
}

/*check if source file is empy*/
static bool isFileEmpty(FILE* _filePtr)
{
	int end;
	fseek (_filePtr, 0, SEEK_END);
	end = ftell(_filePtr);

	if (0 == end) {
	    return TRUE;
	}
	return FALSE;
}

/*check if file exist on folder*/
static bool isFileExists(const char* _name)
{
    struct stat buffer;
    int exist = stat(_name,&buffer);
    return (exist == 0);
}

