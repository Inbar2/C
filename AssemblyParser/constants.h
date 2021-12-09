#ifndef _constants_h_
#define _constants_h_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h> /*for CHAR_BIT*/

#define MAX_LINE_LENGTH 80 /*maximum length of a source line*/
#define MAX_MEM_SIZE 33554432 /*2^25 cells (cell 8 bits) - the max memory size in cells*/
#define LABEL_MAX_LENGTH 32 /*maximum length of a valid label*/
#define IC_INIT_VAL 100 /*default initial Instruction Counter value*/
#define DC_INIT_VAL 0 /*default initial Data Counter value*/
#define INST_BYTES_SIZE 4 /*size of instruction bytes*/

typedef enum num_byte_size {DB=1, DH=2, DW=4} num_byte_size;

typedef enum bool {FALSE, TRUE} bool;

typedef enum instruction_type {R=1, I, J} instruction_type;

/*source line information*/
typedef struct sourceLine_info {
		char* m_sourceLine;
		char* m_fileName;
		unsigned long m_lineNum;	
		} sourceLine_info;

/*R type instruction machine line*/	
typedef struct instR {
		unsigned int m_unusedBits : 6;
		unsigned int m_funct : 5;
		unsigned int m_rd : 5;
		unsigned int m_rt : 5;
		unsigned int m_rs : 5;	
		unsigned int m_opcode : 6;
		} instR;

/*I type instruction machine line*/	
typedef struct instI {
		long m_immed;
		unsigned int m_rt : 5;
		unsigned int m_rs :5;	
		unsigned int m_opcode : 6;
		} instI;

/*J type instruction machine line*/	
typedef struct instJ {
		long m_address;
		unsigned int m_reg : 1;
		unsigned int m_opcode : 6;
		} instJ;

typedef union instruction {
		instR m_instR;
		instI m_instI;
		instJ m_instJ;
		} instruction;

/*instruction definition*/
typedef struct codeImg {
		instruction m_instruction; /*instruction fields*/
		unsigned long m_address; /*ic*/
		instruction_type m_type; /*R /I / J*/
		char* m_labelName; /*label name used in instruction*/
		long m_binCode;
		bool m_isCoded;
		} codeImg;
	
/*directive definition*/
typedef struct dataImg {
		long m_value; /*directive operand data*/
		long m_size; /*num of operand bytes needed*/
		unsigned long m_address; /*location in the data img*/
		} dataImg;
		
/*instruction operation table line*/
typedef struct operation_info {
		char* m_name;
		instruction_type m_type;
		int m_funct;
		int m_opcode;		
		} operation_info;

/*instruction using external label data pair*/
typedef struct externInst {
		char* m_label; /*label name used*/
		long m_address; /*address of instruction*/
		} externInst;
		
#endif /* constants.h */

