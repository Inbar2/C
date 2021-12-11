/*SECOND passing over source code*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "secondPass.h"
#include "genericListDS.h"
#include "generalFuncs.h"
#include "labelAnalysis.h"
#include "instructions.h"
#include "errors.h"

static bool handleInstructionSymbol(sourceLine_info* _lineInfo, const char* _name, symbolTable* _table, long* _ic, list* _codeImgList, list* _externalsList);


bool performSecondPass(sourceLine_info* _lineInfo, symbolTable* _table, long* _ic, list* _codeImgList, list* _externalsList, bool* _isEntry)
{
	char* str = _lineInfo->m_sourceLine;
	char operand[MAX_LINE_LENGTH];
	char word[MAX_LINE_LENGTH+2]; /*max word size in line,+1 for '\n' +1 for '\0'*/
	label_error labelError;
	bool res; /*second pass process result*/
	int pos=0; /*line start index*/
	
	extractLabelWord(_lineInfo->m_sourceLine, &pos, word);

	/*if there is a symbol move to next word*/
	if(isValidLabel(_lineInfo, pos, word, _table, &labelError))
	{
		pos++; /*+1 to skip over ':' of label*/
	}
	else
	{	/*no label found*/
		pos=0;
	}
	extractWord(str, &pos, word); /*skip symbol word*/
	/*word isn't a symbol*/
	/*check if not an '.entry' directive - and skip over it too*/
	if(_lineInfo->m_sourceLine[pos-strlen(word)]=='.')
	{
		if(strcmp(word+1, "entry"))
		{
			return TRUE;
		}
		/*handle '.entry' directive*/
		/*get '.entry' operand and check if exist in symbol table*/
		extractOperandString(str, pos, operand); /*extract all text that comes after directive name*/ 
		if(strlen(operand)==0) /*no operand for '.entry' directive*/
		{
			printLineError(ERROR, _lineInfo, "A label operand for '.entry' directive doesn't exist on line.");
			return FALSE;			
		}
		if(!isSymbolExist(_table, operand)) /*check if symbol name exist in table*/
		{
			printLineError(ERROR, _lineInfo, "In '.entry' directive, the symbol \"%s\" doesn't exist in symbol table.", operand);
			return FALSE;
		}
		if(!isValidAttToAdd(_table, operand, ENTRY)) /*check if valis to add ENTRY attribute name*/
		{
			printLineError(ERROR, _lineInfo, "In '.entry' directive, the symbol \"%s\" has already been defined in symbol table as 'external'.", operand);
			return FALSE;
		}
		updateSymbolAttributes(_table, operand, ENTRY); /*add attribute ENTRY to symbol in table*/
		*_isEntry = TRUE; /*there is at least one valid entry on file*/
		return TRUE;
	}
	
	/*not a directive line - handle instruction line*/
	res = handleInstructionSymbol(_lineInfo, word, _table, _ic, _codeImgList, _externalsList);
	(*_ic)+=4; /*increase code address*/
	return res;
}
	
static bool handleInstructionSymbol(sourceLine_info* _lineInfo, const char* _name, symbolTable* _table, long* _ic, list* _codeImgList, list* _externalsList)
{
	long opcode;
	int funct;
	
	/*check what kinf of instruction it is*/
	/*handle J instruction with label*/
	opcode = getInstOpcodeByName(_name, &funct);
	
	if(opcode >= I_BRANCH_START && opcode < I_LOAD_SAVE_START) /*handle I conditional branching out instructions*/
	{
		return updateInstSymbolAddress(_lineInfo, _codeImgList, _name, _table, *_ic, I, NULL);
	}
	
	if(opcode >= J_START && opcode != STOP_OPCODE) /*handle J instructions excepts 'stop'*/
	{
		return updateInstSymbolAddress(_lineInfo, _codeImgList, _name, _table, *_ic, J, _externalsList);
	}
	return TRUE;
}
