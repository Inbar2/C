#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vectorGeneric.h"


int main()
{
size_t arrIndex, blockSize, arrSize;
int progNum, actionResult, setNewData;
int* newItemData;
void* myDeletedItem, *myGetItem;

Vector* myVector = NULL;



void ptrFuncDes(void* _item)
{
	if(_item != NULL)
	{
		free(_item);
	}
	return;
}


/*TODO create char array to char array with the messages to the output, and use it like this in function printf("&s", str); */ 

while(progNum!=10)
{
	printf("\n\nChoose number of program from menu: " 
	"\n1-Create vector" 
	"\n2-Destroy vector(free memory)"
	"\n3-Add item"
	"\n4-Delete item"
	"\n5-Get item data"
	"\n6-Set item data"
	"\n7-Get vector's num of items"
	"\n8-Get vector's capacity"
	"\n9-Print items array"
	"\n10-Exit\n"
	"--------------------------------------------------------------\n"); 
	
	scanf("%d",&progNum);
	switch(progNum)
	{
		case 1: {
			printf("\nInsert array size: ");
			scanf("%lu", &arrSize);
			printf("\nInsert block size increasment: ");
			scanf("%lu", &blockSize);
			myVector = Vector_Create(arrSize, blockSize);
			
			if(myVector == NULL)
			{
				printf("\nERROR: NULL. Vector cannot be created due to illegal inputs or not enogth free memory space.");
			}
			else
			{
				printf("\nVector was created!");
			}
			};break;
		case 2: {
			if(myVector == NULL)
			{
				printf("\nVector hasn't been assigned yet");
			}
			else
			{			
				Vector_Destroy(&myVector, ptrFuncDes);
				printf("\nVector was destroyed and memory is free.");
			}
			};break;
		case 3: {
			printf("\nInsert integer type number to vector: ");
			newItemData = (int*) malloc (sizeof(int)); /* don't forget to free it in destroy vector and remove */
			scanf("%d", newItemData);
			actionResult = Vector_Append(myVector, newItemData);
			if(actionResult == VECTOR_SUCCESS)
			{
				printf("\nItem was added successfully!");
			}
			else if (actionResult == VECTOR_OUTPUT_PARAMETER_ERROR)
			{
				printf("\nERROR: can't add item to array. Reason: VECTOR_OUTPUT_PARAMETER_ERROR.");
			}					
			else if (actionResult == VECTOR_UNITIALIZED_ERROR)
			{
				printf("\nERROR: can't add item to array. Reason: VECTOR_UNITIALIZED_ERROR.");
			}	
			else if (actionResult == VECTOR_ALLOCATION_ERROR)
			{
				printf("\nERROR: can't add item to array. Reason: VECTOR_ALLOCATION_ERROR.");
			}
			else if (actionResult == VECTOR_OVER_FLOW_ERROR)
			{
				printf("\nERROR: can't add item to array. Reason: VECTOR_OVER_FLOW_ERROR.");
			}
			else
			{
				printf("\nERROR: can't add item to array. Reason: %d.", actionResult);
			}
			}; break;
			
		case 4: {

			actionResult = Vector_Remove(myVector, &myDeletedItem);

			switch(actionResult)
			{
				case VECTOR_SUCCESS: {
										free(myDeletedItem);
										printf("\nItem %p was Removed successfully!", myDeletedItem);break;
									}
				case VECTOR_OUTPUT_PARAMETER_ERROR: printf("\nERROR: can't delete item from array. Reason: VECTOR_OUTPUT_PARAMETER_ERROR."); break;
				case VECTOR_UNDER_FLOW_ERROR: printf("\nERROR: can't delete item from array. Reason: VECTOR_UNDER_FLOW_ERROR."); break;
				case VECTOR_UNITIALIZED_ERROR: printf("\nERROR: can't delete item from array. Reason: VECTOR_UNITIALIZED_ERROR."); break;
				case VECTOR_ALLOCATION_ERROR: printf("\nERROR: can't delete item from array. Reason: VECTOR_ALLOCATION_ERROR."); break;
			};
			}; break;
		case 5: {
			printf("\nInsert index cell in vector to get its data: ");
			scanf("%lu",&arrIndex);
			actionResult = Vector_Get(myVector, arrIndex, &myGetItem);
			if(actionResult==VECTOR_SUCCESS)
			{
				printf("\nThe value of item in position %lu is: %p", arrIndex, myGetItem);
			}			
			else if (actionResult == VECTOR_UNITIALIZED_ERROR)
			{
				printf("\nERROR: can't get item from array. Reason: VECTOR_UNITIALIZED_ERROR.");
			}	
			else
			{
				printf("\nERROR: can't get item from array. Reason: VECTOR_INDEX_OUT_OF_BOUNDS_ERROR.");
			}
			}; break;
		case 6: {
			printf("\nInsert index cell in vector to change its data: ");
			scanf("%lu",&arrIndex);
			printf("\nInsert the integer data you wish to change the item with: ");
			scanf("%d", &setNewData);
			actionResult = Vector_Set(myVector, arrIndex, &setNewData);
			if(actionResult==VECTOR_SUCCESS)
			{
				printf("\nThe value of item in position %lu was changed to: %d\n", arrIndex, *newItemData);
			}			
			else if (actionResult == VECTOR_UNITIALIZED_ERROR)
			{
				printf("\nERROR: can't change item data in array. Reason: VECTOR_UNITIALIZED_ERROR.");
			}	
			else
			{
				printf("\nERROR: can't change item data in array. Reason: ERR_INDEX_NOT_FOUND.");
			}	
			}; break;
		case 7: {
				printf("\nvector's num of items is: %d", (int)Vector_Size(myVector));
			};break;
		case 8: {
				printf("\nvector's capacity is: %d", (int)Vector_Capacity(myVector));
			};break;
		case 9: {
				VectorPrint(myVector);
			};break;
		case 10: {
				Vector_Destroy(&myVector, ptrFuncDes);
				printf("\nYou decided to exit program. Good Bye!\n");
			} break;
		default : 
			{
			printf("\nUse a number between 1-10. There is no program No. %d on the menu.\n", progNum);
			};break;
			}
}

return 0;
}
