#ifndef _instructions_h_
#define _instructions_h_

#include "constants.h"
#include "labelAnalysis.h"
#include "genericListDS.h"

#define I_ARM_LOG_START 10 /*the start opcode of the I arithmetic and logic instructions group*/
#define I_BRANCH_START 15 /*the start opcode of the I conditional branching out instructions group*/
#define I_LOAD_SAVE_START 19 /*the start opcode of the I loading instructions group*/
#define J_START 30 /*the start opcode of the J instructions group*/
#define STOP_OPCODE 63 /*stop opcode value*/

/**
 * @description: Function to destroy data element of the code image list's node
 * @param _item The element to destroy.
*/
void elementCodeDestroy(void* _item);

/**
 * @description: Function to destroy data element of the externals list's node
 * @param _item The element to destroy.
*/
void elementExternalDestroy(void* _item);

/**
 * @description: Function checks if received string word is a valid instruction name.
 * @param _word The string word to verify.
 * @return TRUE if name is valid, otherwise FALSE.
 */
bool isInstructionName(const char* _word);

/**
 * @description: Function gets instruction opcode by according to given name.
 * @param _word The string word to verify.
 * @param _funct A return param of instruction funct if there is, otherwise return NONE.
 * @return opcode num if found, otherwise return NONE (not found).
 */
int getInstOpcodeByName(const char* _word, int* _funct);

/**
 * @description: Function process an instruction line syntax.
 * @param _lineInfo A pointer to the line details.
 * @param _name The name of the instruction.
 * @param _pos The position on line to start the processing from.
 * @param _ic A pointer to the instruction address counter to update.
 * @param _codeImgList The code image list.
 * @param _table The symbol table.
 * @return TRUE if syntax is valid and process succeeded, otherwise return FALSE.
 */
bool processInstruction(sourceLine_info* _lineInfo, const char* _name, int _pos, long* _ic, list* _codeImgList, symbolTable* _table);

/**
 * @description: Function that updates in the 2nd pass the address of each label operand  according to instruction and symbol address in the symbol table.
 * @param _lineInfo A pointer to the line details.
 * @param _codeImgList the code image list.
 * @param _name The name of the instruction.
 * @param _table the symbol table.
 * @param _currAddress The current address of the instruction in the code image.
 * @param _type The instruction type.
 * @param _externalsList The externals list that saves each external label used as instruction operand.
 * @return TRUE if update was successful, otherwise return FALSE.
 */
bool updateInstSymbolAddress(sourceLine_info* _lineInfo, const list* _codeImgList, const char* _name, symbolTable* _table, long _currAdress, instruction_type _type, list* _externalsList);

#endif /* instructions.h */
