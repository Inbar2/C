/*create output files*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "outputFiles.h"
#include "instructions.h"

#define EXT_EXTEN ".ext"
#define ENT_EXTEN ".ent"
#define OB_EXTEN ".ob"

static void printExternalsFormat(void* _filePtr, void* _element);
static void printEntriesFormat(void* _filePtr, void* _element);
static void printCodeFormat(void* _filePtr, void* _element);
static void printDataToFile(FILE* _filePtr, list* _dataImgList, long _icf, long _dcf);
static void createOBFile(list* _codeImgList, list* _dataImgList, const char* _fileName, long _icf, long _dcf);
static void createFile(list* _list,  const char* _fileName, const char* _fileExtension, listActionFunc _action);
static char* createFullFileName(const char* _name, const char* _exten);

void createOutputFiles(symbolTable* _table, list* _extList, list* _codeImgList, list* _dataImgList, const char* _fileName, long _icf, long _dcf, bool _isEntry)
{
	if(!isEmptyList(_extList))
	{
		createFile(_extList, _fileName, EXT_EXTEN, printExternalsFormat);
	}
	if(_isEntry)
	{
		createFile((list*)_table, _fileName, ENT_EXTEN, printEntriesFormat);
	}
	createOBFile(_codeImgList, _dataImgList, _fileName, _icf, _dcf);
}

/*function create the full file name including the file extension*/
static char* createFullFileName(const char* _name, const char* _exten)
{
	char* fileName;
	int i=0, size = strlen(_name)-strlen(".as"); /*calc size of name without ".as" suffix from file name*/

	fileName = (char*) malloc(size+strlen(_exten)+1);
	if(NULL==fileName)
	{
		printf("\nFatal Error: Memory allocation failed for file full name. Program will be terminated.");
		exit(1);
	}
	for(i=0; i<size; i++)
	{
		fileName[i] = _name[i];
	}
	fileName[i] = '\0';
	strcat(fileName+size, _exten); /*add extension*/
	return fileName;
}

/*function to print the requested syntax in the externals file*/
static void printExternalsFormat(void* _filePtr, void* _element)
{
	fprintf((FILE*)_filePtr, "%s	%04ld\n", ((externInst*)_element)->m_label, ((externInst*)_element)->m_address);
}

/*function to print the requested syntax in the entries file*/
static void printEntriesFormat(void* _filePtr, void* _element)
{
	if(isSymbolTypeExist(_element, ENTRY))
	{
		fprintf((FILE*)_filePtr, "%s	%04ld\n", ((externInst*)_element)->m_label, ((externInst*)_element)->m_address);
	}
}

/*function to print the requested syntax in the OB file - INSTRUCTIONS PART*/
static void printCodeFormat(void* _filePtr, void* _element)
{
	unsigned char toPrint=0;
	int i=0;
	long temp, mask=-1;/*x0FFFFFFFF mask to control bit on-off*/
	temp = ((codeImg*)_element)->m_binCode;
	toPrint = temp;

	fprintf((FILE*)_filePtr, "%04ld ", ((codeImg*)_element)->m_address);
	while(i<INST_BYTES_SIZE)
	{
		toPrint = (unsigned char) temp; 
		fprintf((FILE*)_filePtr, "%02X ", toPrint);
		temp = temp>>CHAR_BIT;
		mask >>= CHAR_BIT;
		temp &= mask; /*each time takes 8 bits (=char) and prints in hexa*/
		i++;
	}
	fprintf((FILE*)_filePtr, "\n");
}

/*function to print the requested syntax in the OB file - DIRECTIVES PART*/
static void printDataToFile(FILE* _filePtr, list* _dataImgList, long _icf, long _dcf)
{
	long temp, mask, size, totalSize=0;
	unsigned long toPrint;
	int i=0;
	dataImg* item;
	unsigned long address = _icf; /*start address of the data image*/

	Node* next = getListItrBegin(_dataImgList); /*first node on list*/
	Node* endList = getListItrEnd(_dataImgList);
	
	fprintf(_filePtr, "%04ld ", address);
	while(next != endList) /*go over each item in list*/
	{
		mask=-1;
		i=0;
		item = (dataImg*)getListNodeData(next);
		temp = item->m_value;
		size = item->m_size; /*size in bits*/
		mask >>= (sizeof(long)-size); /*mask to get requested value size*/
		temp &= mask; /*takes only the value bits*/
		while(i<(size/CHAR_BIT))
		{
			if(totalSize==CHAR_BIT*4) /*total 32 bits on line*/
			{
				fprintf(_filePtr, "\n"); /*start new line*/
				address+=totalSize/CHAR_BIT;
				fprintf(_filePtr, "%04ld ", address); /*increase address for new line*/
				totalSize=0;
			}
			toPrint = (unsigned char) temp; /*each time takes 8 bits (=char) and prints in hexa*/
			fprintf(_filePtr, "%02lX ", toPrint);
			totalSize+=CHAR_BIT;
			temp >>= CHAR_BIT;
			i++;
		}
		next = getListItrNext(next);
	}
}

/*create externals or entries file according to extension*/
static void createFile(list* _list,  const char* _fileName, const char* _fileExtension, listActionFunc _action)
{
	FILE* filePtr;
	char* outputFileName = createFullFileName(_fileName, _fileExtension);
	filePtr = fopen(outputFileName, "w");
	if(NULL==filePtr)
	{
		perror("\nWriting to File Error");
		printf("Error: File name \"%s\" can't be created or written to.\n", outputFileName);
		return;
	}
	else {
		doForEach(_list, _action, filePtr);
	}
	fclose(filePtr);
	free(outputFileName);
	outputFileName = NULL;
}

/*create OB file*/
static void createOBFile(list* _codeImgList, list* _dataImgList, const char* _fileName, long _icf, long _dcf)
{
	FILE* filePtr;
	char* outputFileName = createFullFileName(_fileName, OB_EXTEN);
	filePtr = fopen(outputFileName, "w");
	if(NULL==filePtr)
	{
		perror("\nWriting to File Error");
		printf("Error: File name \"%s\" can't be created or written to.\n", outputFileName);
		return;
	}
	else {
		fprintf(filePtr,"\t %ld	%ld\n", _icf-IC_INIT_VAL, _dcf);
		if(!(isEmptyList(_codeImgList)) || !(isEmptyList(_dataImgList)))
		{
			doForEach(_codeImgList, printCodeFormat, filePtr);
			printDataToFile(filePtr, _dataImgList, _icf, _dcf);
		}
	}
	fclose(filePtr);
	free(outputFileName);
	outputFileName = NULL;
}

