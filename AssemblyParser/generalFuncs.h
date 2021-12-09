#ifndef _generalFuncs_h_
#define _generalFuncs_h_

#include "constants.h"

/**
 * @description: Function extracts the first word that doesn't have space-type characters from the input line.
 * @param _lineInfo A pointer to the source line.
 * @param _pos A pointer to the current position in the line, value is updated accordingly.
 * @param _word The word extracted from the line
 */
void extractWord(const char* _line, int* _pos, char* _word);

/**
 * @description: Function extracts the operand string after directive/instruction/label word until encounters a new line char or terminating null character in line.
 * @param _lineInfo A pointer to the source line details
 * @param _pos The index in the line
 * @param _word The operand word extracted from the line
 * @return TRUE if there is an operand string on line, otherwise line is empty or space-type characters and return FALSE.
 */
bool extractOperandString(const char* _line, int _pos, char* _word);

/**
 * @description: Check if all character are of space-type in string starting from the input position value until character is new-line/terminating null character.
 * @param _str The string
 * @param _pos The index in the string to start checking from
 * @return TRUE if string from position received has only space-type characters, otherwise FALSE.
 */ 
bool isStrOnlySpaces(const char* _str, int _pos);

/**
 * @description: Skips space-type characters in string starting from the input position value until character is new-line/terminition null/non-space char.
 * @param _str The string
 * @param _pos The index in the string to start checking from
 * @return position of the next non-space character in the string.
 */
int skipSpaces(const char* _str, int _pos);

/**
 * @description: Skips space-type characters from the received position and check if there is a text on the remain line.
 * @param _str The string
 * @param _pos The index in the string to start checking from
 * @return TRUE next position is not a space char, otherwise got to the end of the line and return FALSE.
 */
bool isOperandTextExist(const char* _line, int* _pos);

/**
 * @description: check if a signed number is in the received num of bytes size range.
 * @param _num The number to check
 * @param _size The size of bytes to calc the possible range of a signed number
 * @return TRUE if in range, otherwise return FALSE.
 */
bool isNumInRange(long _num, num_byte_size _size);

/**
 * @description: check if a string of number is in the correct number format.
 * @param _lineInfo A pointer to the source line details
 * @param _operand The string number to check
 * @return TRUE if a correct number format, otherwise or if string doesn't have number return FALSE.
 */
bool isValidIntNum(const char* _operand);

/**
 * @description: check if a string coversion to a long type number was possible.
 * @param _lineInfo A pointer to the source line details
 * @param _operand The string number to check
 * @return TRUE if coversion was done successfully, otherwise return FALSE.
 */
bool convertStringToLong(sourceLine_info* _lineInfo, char* _operand, long* _num);

/**
 * @description: extract string operand from received position on line and check if it is a valid number.
 * @param _lineInfo A pointer to the source line details
 * @param _pos A pointer to the current position in the line, value is updated accordingly.
 * @param _operand return param of the extracted string number operand
 * @return TRUE if the operand is a valid string number, otherwise FALSE.
 */
bool extractOperandNum(sourceLine_info* _lineInfo, int* _pos, char* _operand);

/**
 * @description: extract string operand from received position on line and check if it is a valid register.
 * @param _lineInfo A pointer to the source line details
 * @param _pos A pointer to the current position in the line, value is updated accordingly.
 * @param _operand extracted string operand in function
 * @param _reg return param of pointer to the valid register number is func succeeded
 * @return TRUE if the extracted number is valid register, otherwise FALSE.
 */
bool extractRegisterNum(sourceLine_info* _lineInfo, int* _pos, char* _operand, long* _reg);

/**
 * @description: check if line synthax from received position befor operands format is legal.
 * @param _lineInfo A pointer to the source line details
 * @param _name the command name (aka instruction or directive)
 * @param _pos A pointer to the current position in the line, value is updated accordingly.
 * @param _lineType string indicating if line is directive or instruction line
 * @return TRUE if pre-check ended legal, otherwise FALSE.
 */
bool preOperandSyntaxCheck(sourceLine_info* _lineInfo, const char* _name, int* _pos, char* _lineType);

/**
 * @description: check if line synthax from received position after operands format is legal.
 * @param _lineInfo A pointer to the source line details.
 * @param _pos A pointer to the current position in the line, value is updated accordingly.
 * @param _operand previous operand in line.
 * @param _isTotal boolean indicate if there should be more operand in line according to comman or not.
 * @return TRUE if post-check ended legal, otherwise FALSE.
 */
bool postOperandSyntaxCheck(sourceLine_info* _lineInfo, int* _pos, char* _operand, bool _isTotal);

#endif /* generalFuncs.h */
