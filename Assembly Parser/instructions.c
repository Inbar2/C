/*instructions.c*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "instructions.h"
#include "errors.h"
#include "generalFuncs.h"
#include "labelAnalysis.h"
#include "genericListDS.h"

#define NONE -1
#define MAX_OP_NUM 3
#define MAX_J_OP_NUM 1
#define R_ARM_LOG 0
#define R_COPY 1
#define IMMED_BYTES 2

operation_info operations_table[] = {
									{"add",R,1,0},
									{"sub",R,2,0},
									{"and",R,3,0},
									{"or",R,4,0},
									{"nor",R,5,0},
									{"move",R,1,1},
									{"mvhi",R,2,1},
									{"mvlo",R,3,1},
									{"addi",I,0,10},
									{"subi",I,0,11},
									{"andi",I,0,12},
									{"ori",I,0,13},
									{"nori",I,0,14},
									{"bne",I,0,15},
									{"beq",I,0,16},
									{"blt",I,0,17},
									{"bgt",I,0,18},
									{"lb",I,0,19},
									{"sb",I,0,20},
									{"lw",I,0,21},
									{"sw",I,0,22},
									{"lh",I,0,23},
									{"sh",I,0,24},
									{"jmp",J,0,30},
									{"la",J,0,31},
									{"call",J,0,32},
									{"stop",J,0,STOP_OPCODE},
									{NULL,NONE,NONE,NONE}
									};

/*buffer to handle instruction operands*/
typedef struct buffer{
		long* m_items;	/*Vecay buffer*/
		size_t m_originSize; /*buffer original size*/
		size_t m_size;	/*buffer total size*/
		size_t m_nItems;/*num of actual items in buffer*/
		size_t m_blockSize; /*size to increase vector with each time*/
		} buffer;
		
static instruction_type getInstTypeByOpcode(int _opcode);
static void setR(codeImg* _imgCode, const buffer* _opBuffer, int _opcode, int _funct); /*check specific R instruction syntax*/
static void setI(codeImg* _imgCode, const buffer* _opBuffer, int _opcode); /*check specific I instruction syntax*/
static void setJ(codeImg* _imgCode, const buffer* _opBuffer, int _opcode, char* _label); /*check specific J instruction syntax*/
static void buildMachineCode(codeImg* _item, instruction_type _type);
static void buildMachineR(const instR* _item, long* _bin);
static void buildMachineI(const instI* _item, long* _bin);
static void buildMachineJ(const instJ* _item, long* _bin);
static bool checkOperandsCount(sourceLine_info* _lineInfo, int _pos, buffer* _opBuffer, const char* _name);
static bool processInstTypeR(sourceLine_info* _lineInfo, const char* _name, int _pos, long* _ic, int _opcode, buffer** _opBuffer);
static bool processInstTypeJ(sourceLine_info* _lineInfo, const char* _name, int _pos, long* _ic, int _opcode, buffer** _opBuffer, char** _label, symbolTable* _table);
static bool processInstTypeI(sourceLine_info* _lineInfo, const char* _name, int _pos, long* _ic, int _opcode, buffer** _opBuffer, char** _label, symbolTable* _table);
static codeImg* createCodeImgItem(const buffer* _opBuffer, char* _label, long _ic, int _opcode, int _funct, instruction_type _type);
static void updateExternalsList(list* _extList, char* _labelName, long _address);
static void destroyBuffer(buffer** _buffer);
static void appendBufferItem(buffer* _buffer, long _item);
static buffer* createBuffer(int _initialCapacity);
/*----------------------------------------------------------------------------------------------------------------------------------------*/

bool isInstructionName(const char* _word)
{
	int i=0;
	while(operations_table[i].m_name)
	{
		if(!strcmp(operations_table[i].m_name, _word))
		{
			return TRUE;
		}
		i++;
	}
	return FALSE;
}

int getInstOpcodeByName(const char* _word, int* _funct)
{
	int i=0;
	int op = NONE;
	*_funct = NONE;
	while(NULL != operations_table[i].m_name) /*go over the table and if instruction is found return opecode and funct*/
	{
		if(!strcmp(operations_table[i].m_name, _word))
		{
			op = operations_table[i].m_opcode;
			*_funct = operations_table[i].m_funct;
			break; 
		}
		i++;
	}
	return op;
}

char* getInstNameByOpcodeFunct(int _opcode, int _funct)
{
	int i=0;
	while(NULL != operations_table[i].m_name) /*go over the table and when opcode is found return its type*/
	{
		if(operations_table[i].m_opcode == _opcode && operations_table[i].m_funct == _funct)
		{
			break;
		}
		i++;
	}
	return operations_table[i].m_name;
}

/*destroy instruction item*/
void elementCodeDestroy(void* _item)
{
	if(NULL == _item)
	{
		return;
	}

	if(NULL != ((codeImg*)_item)->m_labelName) /*also destroy label if exist in item*/
	{
		free(((codeImg*)_item)->m_labelName);
		((codeImg*)_item)->m_labelName = NULL;
	}
	free(_item);
	_item = NULL;
}

/*destroy external pair item*/
void elementExternalDestroy(void* _item)
{
	if(NULL == _item)
	{
		return;
	}
	((externInst*)_item)->m_label = NULL;
	free(_item);
	_item = NULL;
}
				
bool processInstruction(sourceLine_info* _lineInfo, const char* _name, int _pos, long* _ic, list* _codeImgList, symbolTable* _table) /*check general instruction syntax*/
{
	int funct, opcode;
	buffer* opBuffer;
	codeImg* imgCodeItem;
	char* label = NULL; /*ptr to label string operand in I conditional branching out instruction and J instructions*/
	instruction_type type;	
	bool isValid = TRUE;	
	/*check if the word is an instruction*/
	opcode = getInstOpcodeByName(_name, &funct);
	if(NONE == opcode)
	{
		printLineError(ERROR, _lineInfo, "Word \'%s\' is not an instruction name.", _name);
		return FALSE;
	}
	
	if(strcmp(_name, "stop")) /*ll instruction except 'stop'*/
	{
		if(!preOperandSyntaxCheck(_lineInfo, _name, &_pos, "instruction")) 
		{
			return FALSE;		
		}
	}
	type = getInstTypeByOpcode(opcode);
	switch(type)
	{
		case R: {	if(!processInstTypeR(_lineInfo, _name, _pos, _ic, opcode, &opBuffer)) {
						isValid = FALSE;
						break;
					}
					imgCodeItem = createCodeImgItem(opBuffer, label, *_ic, opcode, funct, type);
					break;
				}
				
		case I: {	if(!processInstTypeI(_lineInfo, _name, _pos, _ic, opcode, &opBuffer, &label, _table)) {
						isValid = FALSE; 
						break;
					}
					imgCodeItem = createCodeImgItem(opBuffer, label, *_ic, opcode, funct, type);
					break;
				}

		case J:	{ 	if(!processInstTypeJ(_lineInfo, _name, _pos, _ic, opcode, &opBuffer, &label, _table)) {
						isValid = FALSE; 
						break;
					}
					imgCodeItem = createCodeImgItem(opBuffer, label, *_ic, opcode, funct, type);
					break;
				}
	}
	if(!isValid)
	{
		destroyBuffer(&opBuffer);
		return FALSE;
	}

	pushListTail(_codeImgList, imgCodeItem);	
	(*_ic) += INST_BYTES_SIZE;
	destroyBuffer(&opBuffer);
	return TRUE;
}

/*in 2nd pass*/
bool updateInstSymbolAddress(sourceLine_info* _lineInfo, const list* _codeImgList, const char* _name, symbolTable* _table, long _currAddress, instruction_type _type, list* _extList)
{
	codeImg* code;
	symbol* symbol;
	long labelAddress, newImmed;
	Node* temp = getListItrBegin(_codeImgList); /*first node on list*/
	Node* endList = getListItrEnd(_codeImgList);

	while(temp != endList)
	{
		code = (codeImg*)getListNodeData(temp);
		if(code->m_address == _currAddress) /*find the instruction node on list*/
		{
			if(NULL == code->m_labelName) /*exclude jmp instruction with register - already build in first pass*/
			{
				return TRUE;
			}
			
			symbol = getSymbolByName(_table, code->m_labelName); /*get symbol from the symbol table*/
			if(NULL == symbol)
			{
				printLineError(ERROR, _lineInfo, "In \'%s\' instruction, the label \"%s\" doesn't exist in symbol table.", _name, code->m_labelName);
				return FALSE;
			}

			labelAddress = symbol->m_address;
			if(_type == I)
			{
				if(isSymbolTypeExist(symbol, EXTERNAL)) 
				{
					printLineError(ERROR, _lineInfo, "In \'%s\' instruction, the label \"%s\" can't be of 'external' type.", _name, code->m_labelName);
					return FALSE;
				}
				newImmed = labelAddress - _currAddress;
				if(!isNumInRange(newImmed, IMMED_BYTES)) /*check if new address number is in the range of 16 bits*/
				{
					printLineError(ERROR, _lineInfo, "Immed operand num: %ld can't be represented in 16 bits for \'%s\' instruction.", newImmed, _name);		
					return FALSE;
				}
				code->m_instruction.m_instI.m_immed = newImmed;
				buildMachineCode(code, I);
			}
			else if(_type == J) { 
				code->m_instruction.m_instJ.m_address = labelAddress; /*update the instruction symbol address*/ 
				buildMachineCode(code, J);
				if(isSymbolTypeExist(symbol, EXTERNAL)) 
				{
					/*if symbol in J instruction is with EXTERNAL attribute - add it to list of instructions using externals*/
					updateExternalsList(_extList, symbol->m_name, _currAddress);
				}
			}
			code->m_isCoded = TRUE;
			break; /*exit loop no need to continue search instruction*/
		}
	temp = getListItrNext(temp);
	}		
	return TRUE;
}

/*get instruction type by opcode*/
static instruction_type getInstTypeByOpcode(int _opcode)
{
	int i=0;
	while(NULL != operations_table[i].m_name) /*go over the table and when opcode is found return its type*/
	{
		if(operations_table[i].m_opcode == _opcode)
		{
			break;
		}
		i++;
	}
	return operations_table[i].m_type;
}

static void setR(codeImg* _imgCode, const buffer* _opBuffer, int _opcode, int _funct)
{
	if(_opcode == R_ARM_LOG)
	{
		_imgCode->m_instruction.m_instR.m_rd = _opBuffer->m_items[2];
		_imgCode->m_instruction.m_instR.m_rt = _opBuffer->m_items[1];
	}
	else
	{/*R copy instructions - only 2 operands*/
		_imgCode->m_instruction.m_instR.m_rd = _opBuffer->m_items[1];
		_imgCode->m_instruction.m_instR.m_rt = 0;
	}
	_imgCode->m_instruction.m_instR.m_rs = _opBuffer->m_items[0];
	_imgCode->m_instruction.m_instR.m_unusedBits = 0;
	_imgCode->m_instruction.m_instR.m_funct = _funct;
	_imgCode->m_instruction.m_instR.m_opcode = _opcode;
}

static void setI(codeImg* _imgCode, const buffer* _opBuffer, int _opcode)
{
	if(_opcode >= I_BRANCH_START && _opcode <I_LOAD_SAVE_START)
	{
		/*conditional branching out instruction*/
		_imgCode->m_instruction.m_instI.m_rt = _opBuffer->m_items[1];
		_imgCode->m_instruction.m_instI.m_immed = _opBuffer->m_items[2]; /*in first pass this value is default 0*/
	}
	else
	{/*the other I instructions*/
		_imgCode->m_instruction.m_instI.m_rt = _opBuffer->m_items[2];
		_imgCode->m_instruction.m_instI.m_immed = _opBuffer->m_items[1];
	}
	_imgCode->m_instruction.m_instI.m_rs = _opBuffer->m_items[0];
	_imgCode->m_instruction.m_instI.m_opcode = _opcode;
}

static void setJ(codeImg* _imgCode, const buffer* _opBuffer, int _opcode, char* _label)
{
	if(_label == NULL && _opcode != STOP_OPCODE)
	{
		/*jmp with register*/
		_imgCode->m_instruction.m_instJ.m_address = _opBuffer->m_items[0];
		_imgCode->m_instruction.m_instJ.m_reg = 1;
	}
	else
	{	/*instruction reg and address values are 0 as defualt for instruction with label in 1st pass*/
		_imgCode->m_instruction.m_instJ.m_address = 0;
		_imgCode->m_instruction.m_instJ.m_reg = 0;		
	}
	_imgCode->m_instruction.m_instJ.m_opcode = _opcode;
}

static void buildMachineCode(codeImg* _item, instruction_type _type)
{
	switch (_type) {
		case R: { buildMachineR(&_item->m_instruction.m_instR, &_item->m_binCode); break;}
		case I: { buildMachineI(&_item->m_instruction.m_instI, &_item->m_binCode); break;} 
		case J: { buildMachineJ(&_item->m_instruction.m_instJ, &_item->m_binCode); break;}
	}
}

/*code R 32 bit instruction from the components data*/
static void buildMachineR(const instR* _item, long* _bin)
{
	unsigned long temp;
	temp = _item->m_opcode;
	temp <<= 26;
	*_bin |= temp;

	temp = _item->m_rs;
	temp <<= 21;
	*_bin |= temp;
	
	temp = _item->m_rt;
	temp <<= 16;
	*_bin |= temp;
	
	temp = _item->m_rd;
	temp <<= 11;
	*_bin |= temp;
	
	temp = _item->m_funct;
	temp <<= 6;
	*_bin |= temp;
	*_bin |= _item->m_unusedBits;
}

static void buildMachineI(const instI* _item, long* _bin)
{
	 
	unsigned long temp;
	temp = _item->m_opcode;
	temp <<= 26;
	*_bin |= temp;
	
	temp = _item->m_rs;
	temp <<= 21;
	*_bin |= temp;
	
	temp = _item->m_rt;
	temp <<= 16;
	*_bin |= temp;
	
	temp = -1; /*x0FFFFFFFF*/
	temp >>= 16; /*keep only 16 bits on*/
	temp &= _item->m_immed;
	*_bin |= temp;
}

static void buildMachineJ(const instJ* _item, long* _bin)
{	 
	unsigned long temp;
	temp = _item->m_opcode;
	temp <<= 26;
	*_bin |= temp;

	temp = _item->m_reg;
	temp <<= 25;
	*_bin |= temp;

	temp = -1; /*x0FFFFFFFF*/
	temp >>= 6; /*keep only 25 bits on*/
	temp &= _item->m_address;
	*_bin |= temp;

}

static bool checkOperandsCount(sourceLine_info* _lineInfo, int _pos, buffer* _opBuffer, const char* _name)
{
	char* str = _lineInfo->m_sourceLine; /*pointer to line*/
	if(_opBuffer->m_nItems < _opBuffer->m_originSize)
	{
		printLineError(ERROR, _lineInfo, "Missing operand - There must be %d operands in \'%s\' instruction.", _opBuffer->m_originSize, _name);
		return FALSE;
	}

	if(!isStrOnlySpaces(str, _pos))
	{
		printLineError(ERROR, _lineInfo, "Extraneous operand - There must be only %d operands in \'%s\' instruction.", _opBuffer->m_originSize, _name);
		return FALSE;
	}
	return TRUE;
}
/*check specific R instruction syntax*/
static bool processInstTypeR(sourceLine_info* _lineInfo, const char* _name, int _pos, long* _ic, int _opcode, buffer** _opBuffer)
{
	long num;
	char operand[MAX_LINE_LENGTH];
	char* str = _lineInfo->m_sourceLine; /*pointer to line*/
	
	if(_opcode == R_ARM_LOG)
	{
		/*3 registers*/
		*_opBuffer = createBuffer(MAX_OP_NUM);
	}
	else{ /*opcode is R_COPY=1 - has only 2 registers*/
		*_opBuffer = createBuffer(MAX_OP_NUM-1);
	}

	while(str[_pos] != '\n' && str[_pos] != '\0' && ((*_opBuffer)->m_nItems < (*_opBuffer)->m_originSize))
	{
		if(!extractRegisterNum(_lineInfo, &_pos, operand, &num))
		{
			return FALSE;
		}

		appendBufferItem(*_opBuffer, num);

		if(!postOperandSyntaxCheck(_lineInfo, &_pos, operand, (*_opBuffer)->m_nItems == (*_opBuffer)->m_originSize))
		{
			return FALSE;
		}
	}
	
	if(!checkOperandsCount(_lineInfo, _pos, *_opBuffer, _name) )
	{
		return FALSE;
	}
	return TRUE;
}

/*check specific J instruction syntax*/
static bool processInstTypeJ(sourceLine_info* _lineInfo, const char* _name, int _pos, long* _ic, int _opcode, buffer** _opBuffer, char** _label, symbolTable* _table)
{
	long num;
	char operand[MAX_LINE_LENGTH];
	char* str = _lineInfo->m_sourceLine; /*pointer to line*/
	*_opBuffer = createBuffer(MAX_J_OP_NUM);
	_pos = skipSpaces(str, _pos);
	if(strcmp(_name, "stop")) /*when J instruction and not 'stop'*/
	{	
		if(!strcmp(_name, "jmp") && str[_pos]=='$') /*check what kind of the operand is*/
		{/*can be a register*/
			if(!extractRegisterNum(_lineInfo, &_pos, operand, &num))
			{
				return FALSE;
			}
		}
		else
		{/*can only be a label*/
			num=0; /*defualt value until 2nd pass for entry type label*/
			*_label = malloc((strlen(str)-_pos+1)*sizeof(char));
			if(NULL==*_label)
			{
				printf("\nFatal Error: Memory allocation failed for label operand. Program will be terminated.");
				exit(1);
			}

			if(!extractOperandLabelWord(_lineInfo, &_pos, operand, _table))
			{
				free(*_label);
				*_label = NULL;
				return FALSE;
			}
			strcpy(*_label, operand);
		}

		appendBufferItem(*_opBuffer, num);
		if(!postOperandSyntaxCheck(_lineInfo, &_pos, operand, (*_opBuffer)->m_nItems == (*_opBuffer)->m_originSize))
		{
			return FALSE;
		}
	}
	else {
		*_opBuffer = createBuffer(0); /*instruction is "stop"*/
	}
	
	if(!checkOperandsCount(_lineInfo, _pos, *_opBuffer, _name))
	{
		return FALSE;;
	}

	return TRUE;
}

/*check specific I instruction syntax*/
static bool processInstTypeI(sourceLine_info* _lineInfo, const char* _name, int _pos, long* _ic, int _opcode, buffer** _opBuffer, char** _label, symbolTable* _table)
{
	long num;
	char operand[MAX_LINE_LENGTH];
	bool isSecond = FALSE, isBranch = TRUE;
	int counterOp = 1;
	char* str = _lineInfo->m_sourceLine; /*pointer to location on line*/
	
	*_opBuffer = createBuffer(MAX_OP_NUM);

	while(str[_pos] != '\n' && str[_pos] != '\0' && ((*_opBuffer)->m_nItems < (*_opBuffer)->m_originSize))
	{
		if(!isBranch && isSecond) /* not conditional branching out instruction and 2nd operand*/
		{
			if(!extractOperandNum(_lineInfo, &_pos, operand))
			{
				return FALSE;
			}
			isSecond = FALSE;
			if(!convertStringToLong(_lineInfo, operand, &num)  || (convertStringToLong(_lineInfo, operand, &num) && !isNumInRange(num, IMMED_BYTES)))
			{
				printLineError(ERROR, _lineInfo, "Immed operand num: %s can't be represented in 16 bits for \'%s\' instruction.", operand, _name);		
				return FALSE;
			}
		}
		else if(isBranch && counterOp==MAX_OP_NUM)
		{
			/*third operand of conditional branching out instruction*/
			num=0; /*defualt value until 2nd pass*/	
			*_label = malloc((strlen(str)-_pos+1)*sizeof(char));
			if(NULL==*_label)
			{
				printf("\nFatal Error: Memory allocation failed for label operand. Program will be terminated.");
				exit(1);
			}
			
			if(!extractOperandLabelWord(_lineInfo, &_pos, operand, _table))
			{
				free(*_label);
				*_label = NULL;
				return FALSE;
			}
			strcpy(*_label, operand);
		}
		else if((isBranch && counterOp<MAX_OP_NUM) || (!isBranch && counterOp==MAX_OP_NUM))
		{
			/*first operand is always register for all I instructions*/
			if(!extractRegisterNum(_lineInfo, &_pos, operand, &num))
			{
				return FALSE;
			}

			isSecond = TRUE;
			isBranch = FALSE;					
			if(_opcode >= I_BRANCH_START && _opcode < I_LOAD_SAVE_START)
			{
				isBranch = TRUE;
			}
		}
		counterOp++;
		appendBufferItem(*_opBuffer, num);
		if(!postOperandSyntaxCheck(_lineInfo, &_pos, operand, (*_opBuffer)->m_nItems == (*_opBuffer)->m_originSize))
		{
			return FALSE;
		}
		if(counterOp>MAX_OP_NUM) {break;}
	}
	
	if(!checkOperandsCount(_lineInfo, _pos, *_opBuffer, _name))
	{
		return FALSE;
	}
	
	return TRUE;
}

static codeImg* createCodeImgItem(const buffer* _opBuffer, char* _label, long _ic, int _opcode, int _funct, instruction_type _type)
{
	codeImg* codeImgItem;
	bool isCoded = TRUE;
	
	codeImgItem = (codeImg*)malloc(sizeof(codeImg));
	if(NULL == codeImgItem)
	{
		printf("\nFatal Error: Memory allocation failed for code image item. Program will be terminated.");
		exit(1);
	}
	switch (_type) {
		case R: {setR(codeImgItem, _opBuffer, _opcode, _funct); break;}
		case I: {setI(codeImgItem, _opBuffer, _opcode); break;}
		case J: {setJ(codeImgItem, _opBuffer, _opcode, _label); break;}
	}

	/*only R,I that isn't conditional branching and J jmp with register and stop can be coded now*/
	if((_opcode >= I_BRANCH_START && _opcode < I_LOAD_SAVE_START) || (_opcode>=J_START && _label != NULL))
	{
		/*instruction that may need data about label and will be processed in 2nd pass*/
		isCoded = FALSE;
	}
	codeImgItem->m_type = _type;
	codeImgItem->m_binCode = 0;
	codeImgItem->m_address = _ic;
	codeImgItem->m_labelName = _label;
	codeImgItem->m_isCoded = isCoded;
	if(isCoded)
	{
		buildMachineCode(codeImgItem, _type);
	}
	return codeImgItem;
}

static void updateExternalsList(list* _extList, char* _labelName, long _address)
{
	externInst* externItem = malloc(sizeof(externInst));
	if(NULL == externItem)
	{
		printf("\nFatal Error: Memory allocation failed for external pair item. Program will be terminated.");
		exit(1);
	}
	externItem->m_label = _labelName; /*pointer to name on symbol item in table*/
	externItem->m_address = _address;
	pushListTail(_extList, externItem);
}

static buffer* createBuffer(int _initialCapacity)
{
	buffer* bufferPtr;

	bufferPtr = (buffer*) malloc(sizeof(buffer));
	if(bufferPtr == NULL) /* allocation failed */
	{
		printf("\nFatal Error: Memory allocation failed for buffer. Program will be terminated.");
		exit(1);
	}

	bufferPtr->m_items = (long*) calloc(_initialCapacity, sizeof(long));
	if((bufferPtr->m_items) == NULL) /* buffer Vecay allocation failed */
	{	
		free(bufferPtr);
		printf("\nFatal Error: Memory allocation failed for buffer items. Program will be terminated.");
		exit(1);
	}

	bufferPtr->m_size = _initialCapacity;
	bufferPtr->m_originSize = _initialCapacity;
	bufferPtr->m_nItems = 0;
	bufferPtr->m_blockSize = 1; /*each time increase buffer size by 1*/

	return bufferPtr;
}

/*Free all allocated memory for the Buffer and buffer's ptr*/
static void destroyBuffer(buffer** _buffer)/* pointer to pointer in order to free also the buffer pointer */
{
	free((*_buffer)->m_items); /* release buffer's Vecay */
	/* because buffer doesn't contain allocated data I don't create an extra function that releases data in the b's cells */
	free(*_buffer); /* release ptr */
	*_buffer = NULL; 
}

static void appendBufferItem(buffer* _buffer, long _item)
{
	_buffer->m_items[_buffer->m_nItems] = _item;
	_buffer->m_nItems++;
}

