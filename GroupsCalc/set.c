#include "set.h"

void initiatSets(sets* _setsArr)
{
	int i=0,j;

	while(_setsArr[i].m_set != NULL)
	{
		for(j=0; j<ARR_SIZE;j++) /*set group*/
		{
			_setsArr[i].m_set->m_group[j] = 0; /*empty group*/
		}
		
		i++;
	}
}

void resetSet(set* _set)
{
	int i;
	for(i=0; i < ARR_SIZE; i++)
	{
		_set->m_group[i] = 0; /*reset set*/
	}
}

void read_set(set* _set, int* const _params, int _numParams)
{
	int i, byteIndex, pos=0;
	unsigned char mask = 1; /*00000001*/

	resetSet(_set);

	for(i=0; i<_numParams; i++)
	{
		mask = 1;
		byteIndex = _params[i] / CHAR_BIT; /* in which byte the bit is */
		pos = _params[i] % CHAR_BIT; /* in which position the bit is in the byte */		
		mask <<= pos;
		_set->m_group[byteIndex] = _set->m_group[byteIndex] | mask;
	}
}

void print_set(set* _set)
{
	int i, pos=0, isEmpty=1, counter=0, isStarted=0;
	unsigned char mask;

	printf("\nPrint set result is:\n");
	for(i=0; i < ARR_SIZE; i++) /*go over all 128 bits in 16 unsigned chars*/
	{
		mask = 1; /*00000001*/
		while(mask!=0)
		{
			if(counter==16)
			{
				printf("\n");
				counter=0;
				isStarted=0;
			}
			
			if((_set->m_group[i] & mask) == mask)
			{
				if(isStarted)
				{
					printf(",");
				}
				printf("%d", pos);
				isStarted=1;
				isEmpty=0;
				counter++;
			}		
			mask <<= 1;
			pos++;
			
		}
	}
	if(isEmpty)
	{
		printf("The set is empty\n");
	}
}

void union_set(set* _setA, set* _setB, set* _setC)
{
	int i;
	for(i=0; i < ARR_SIZE; i++)
	{
		_setC->m_group[i] = _setA->m_group[i] | _setB->m_group[i];
	}
}

void intersect_set(set* _setA, set* _setB, set* _setC)
{
	int i;
	for(i=0; i < ARR_SIZE; i++)
	{
		_setC->m_group[i] = _setA->m_group[i] & _setB->m_group[i];
	}
}

void sub_set(set* _setA, set* _setB, set* _setC)
{
	int i;
	for(i=0; i < ARR_SIZE; i++)
	{
		_setC->m_group[i] = _setA->m_group[i] & (~_setB->m_group[i]);
	}
}

void symdiff_set(set* _setA, set* _setB, set* _setC)
{
	int i;
	for(i=0; i < ARR_SIZE; i++)
	{
		_setC->m_group[i] = _setA->m_group[i] ^ _setB->m_group[i];
	}
}

void stop()
{
	printf("\nstop - exit program\n");
	exit(0);
}

