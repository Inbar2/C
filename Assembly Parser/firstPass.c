/*first passing over source code*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "firstPass.h"
#include "labelAnalysis.h"
#include "generalFuncs.h"
#include "errors.h"
#include "directives.h"
#include "instructions.h"

bool isMinSpace(sourceLine_info* _lineInfo, int _pos, const char* _name)
{
	if(_lineInfo->m_sourceLine[_pos] != ' ' && _lineInfo->m_sourceLine[_pos] != '\t')
	{
		printLineError(ERROR, _lineInfo, "Missing minimum space between label \"%s\" to the next text on line.", _name);
		return FALSE;
	}
	return TRUE;	
}

void freeSymbolName(char** _symbolName)
{
	if(NULL==_symbolName)
	{
		return;
	}
	free(*_symbolName);
	*_symbolName = NULL;
}

bool performFirstPass(sourceLine_info* _lineInfo, symbolTable* _table, long* _ic, long* _dc, list* _codeImgList, list* _dataImgList)
{
	char word[MAX_LINE_LENGTH+1]; /*max word in line, +1 for '\0'*/
	char* symbolName = NULL;
	label_error labelError = NONE_ERR;
	bool isDefined = FALSE; /*is symbol defined on symbol table*/
	bool isSymbol = FALSE; /*flag indicates if word is a symbol name*/
	int pos=0; /*line start position*/
	symbol* newSymbol = NULL;
	bool res;

	extractLabelWord(_lineInfo->m_sourceLine, &pos, word); /*line isn't empty hence a text is set in the word*/
	
	/*check if label*/
	if(isValidLabel(_lineInfo, pos, word, _table, &labelError))
	{
		isSymbol = TRUE; /*valid symbol*/
		symbolName = (char*) malloc((strlen(word)+1) * sizeof(char)); /*symbol name allocation*/
		if(NULL==symbolName)
		{
			/*symbol name allocation failure*/
			printf("\nFatal Error: Memory allocation failed for symbol word. Program will be terminated.");
			exit(1);
		}
		strcpy(symbolName, word);
	}
	else
	{
		if(labelError != LABEL_COLON_ERR)
		{
			/*word is a label (has :) but has an error in name*/
			printLabelError(_lineInfo, word, labelError);
			return FALSE;
		}
		else
		{
			/*check if string contains a valid label word but missing ':'*/
			pos-=strlen(word);
			extractWord(_lineInfo->m_sourceLine, &pos, word);
			res=isValidLabelName(_lineInfo, word, _table, &labelError);
			if(res)/*no ':' and first word can be a label name*/
			{
				printLabelError(_lineInfo, word, LABEL_COLON_ERR);
				return FALSE;
			}
			else if(labelError == ALPAHBETIC_FIRST_CHAR_ERR)
			{
				if(word[0]=='.' && !(isDirectiveName(word+1)) && isValidLabelName(_lineInfo, word+1, _table, &labelError))
				{
					printLabelError(_lineInfo, word, labelError); /*word starts with '.' but not a directive*/
					return FALSE;
				}
			}
			else if (labelError != RESERVED_WORD_ERR) /*word is not a valid label and also not a directive nor instruction*/
			{	
				printLabelError(_lineInfo, word, labelError);
				return FALSE;
			}
		}
	}
	
	if(isSymbol)
	{
		if(isSymbolExist(_table, word)) /*check if label is already defined in symbol table*/
		{
			isDefined = TRUE;
		}

		/*check if only label in line*/
		if(isStrOnlySpaces(_lineInfo->m_sourceLine, pos+1)) /*+1 to skip over ':' of label*/
		{
			printLineError(ERROR, _lineInfo, "Missing text after label name \"%s\".", word);
			freeSymbolName(&symbolName);
			return FALSE;
		}
		
		/*there is text after label - check if there is a minimum 1 space between the strings*/
		if(!isMinSpace(_lineInfo, pos+1, word))
		{
			freeSymbolName(&symbolName);
			return FALSE;
		}

		/*get the next word after the label*/
		pos++;  /*+1 to skip over ':' of label*/
		extractWord(_lineInfo->m_sourceLine, &pos, word);
	}

	/*check if the word is a directive*/
	if(_lineInfo->m_sourceLine[pos-strlen(word)]=='.')
	{
		strcpy(word,(word+1));
		if(isDirectiveName(word))
		{
			/*word is necessarily a valid directive name*/			
			/*check if a storage directive*/
			if(!strcmp(word, "db")  || !strcmp(word, "dh")  || !strcmp(word, "dw")  || !strcmp(word, "asciz"))
			{
				/*if a storage directive and new symbol - insert symbol to symbol table*/
				if(isSymbol)
				{
					if(isDefined)
					{
						printLineError(ERROR, _lineInfo, "Symbol \"%s\" has already been defined in symbol table.", symbolName);
						freeSymbolName(&symbolName);
						return FALSE;
					}
					else
					{
						newSymbol=createSymbol(symbolName, *_dc, DATA);
						addSymbolToTable(_table, newSymbol);
					}
				}
				freeSymbolName(&symbolName); /*no more need of it*/
				/*process directive according to directive name*/
				if(!strcmp(word, "asciz"))
				{
					/*process string directive*/
					if(!processAscizDir(_lineInfo, word, pos, _dc, _dataImgList)) /*write at the end as 'return func' - one line*/
					{
						return FALSE;
					}
				}
				else /*process data directive*/
				{
					if(!processDataDir(_lineInfo, word, pos, _dc, _dataImgList))
					{
						return FALSE;
					}
				}
				return TRUE; /*storage directive line was legal and processed*/
			}
			else if(!strcmp(word, "entry") || !strcmp(word, "extern"))
			{
				if(isSymbol)
				{
					/*if directive .entry or .external comes after label - display warning and ignore label*/
					printLineError(WARNING, _lineInfo, "A Label named: \"%s\" appears before an \'%s\' directive and will be ignored.",
symbolName, word);
				}
				freeSymbolName(&symbolName);
				if(!strcmp(word, "entry"))
				{
					return TRUE; /*an .entry directive will be processed in the second pass*/
				}
				
				/*.external directive - check operand*/
				return processExternalDir(_lineInfo, _table, word, pos);
			}
			freeSymbolName(&symbolName);
			return TRUE; /*storage directive line was legal and processed*/
		}
		printLineError(ERROR, _lineInfo, "Unrecognized directive name: \"%s\".", word);
		freeSymbolName(&symbolName);
		return FALSE; /*directive name isn't valid*/
	}

	/*line is an instruction*/	
	/*if a new symbol - insert symbol to symbol table with 'code' attribute*/
	if(isSymbol)
	{
		if(isDefined)
		{
			printLineError(ERROR, _lineInfo, "Symbol \"%s\" has already been defined in symbol table.", symbolName);
			freeSymbolName(&symbolName);
			return FALSE;
		}
		else
		{
			newSymbol = createSymbol(symbolName, *_ic, CODE);
			addSymbolToTable(_table, newSymbol);
		}
	}
	
	/*last option is an instruction line, otherwise will return false*/
	freeSymbolName(&symbolName);
	if(!processInstruction(_lineInfo, word, pos, _ic, _codeImgList, _table))
	{
		return FALSE;
	}

	return TRUE;
}

