#ifndef _labelAnalysis_h_
#define _labelAnalysis_h_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "constants.h"
#include "symbolTable.h"

/*enum of label possible errors*/
typedef enum label_error {NONE_ERR=-1, NO_LABEL_NAME_ERR=0, ALL_ALPHANUM_CHARS_ERR, LABEL_ALREADY_DEFINED_ERR, RESERVED_WORD_ERR, ALPAHBETIC_FIRST_CHAR_ERR, WORD_LENGTH_ERR, LABEL_COLON_ERR} label_error;

/**
 * @description: Function prints to stdout a label error message.
 * @param _lineInfo A pointer to the source line details where the error took place.
 * @param _word The label name string to display on error if needed.
 * @param _labelError The error number value to print the message by.
 */
void printLabelError(sourceLine_info* _lineInfo, const char* _word, label_error _labelError);

/**
 * @description: Function extracts the label string on line until it encounters ':' or new line or terminating null character.
 * @param _line The string line to extract the string from.
 * @param _pos A pointer to the current position in the line, value is updated accordingly.
 * @param _str The string extracted from the line.
 */
void extractLabelWord(const char* _line, int* _pos, char* _str);

/**
 * @description: Function check if received string is in valid start line label format.
 * @param _lineInfo A pointer to the source line details.
 * @param _pos A pointer to the current position in the line, value is updated accordingly.
 * @param _str The string to test if can be a valid label.
 * @param _table The symbol table to to check if label alreafy defined.
 * @param _labelError A return param if an error is found during check.
 * @return TRUE if label is a valid start line label format, otherwise FALSE.
 */
bool isValidLabel(sourceLine_info* _lineInfo, int _pos, const char* _str, symbolTable* _table, label_error* _labelError);

/**
 * @description: Function check if received string is a valid label name (wether in start line or as an operand).
 * Label should be consisted of 32 characters at most, starts with alphabetic character and has only alphanumeric characters, is not a reserved word.
 * @param _lineInfo A pointer to the source line details.
 * @param _pos A pointer to the current position in the line, value is updated accordingly.
 * @param _str The string to test if can be a valid label name.
 * @param _table The symbol table to to check if label alreafy defined.
 * @param _labelError A return param if an error is found during check.
 * @return TRUE if label name is valid, otherwise FALSE.
 */
bool isValidLabelName(sourceLine_info* _lineInfo, const char* _str, symbolTable* _table, label_error* _labelError);

/**
 * @description: Function extracts an operand label from the received position string on line.
 * @param _lineInfo A pointer to the source line details.
 * @param _pos A pointer to the current position in the line to start extract the string from, value is updated accordingly.
 * @param _word The word extracted.
 * @param _table The symbol table to to check if label alreafy defined.
 * @return TRUE if extracted operand label has valid name, otherwise FALSE.
 */
bool extractOperandLabelWord(sourceLine_info* _lineInfo, int* _pos, char* _word, symbolTable* _table);

#endif /*labelAnalysis.h*/
