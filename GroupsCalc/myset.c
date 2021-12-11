#include "syntax.h"

/*Program receives an input from the standard input, decrypt the command line and performs it.
If receiving a 'stop' command the program is terminated.
If receiving a EOF the program will be terminated after printing relevant error. 
If the command line is more than 80 characters or blank(no characters or spaces characters) an error will arise.
As long as no stop command was received or EOF the program will continue and ask for a command line*/

/*Definition of the sets variables*/
set SETA, SETB, SETC, SETD, SETE, SETF;
	
int main()
{
	int isStop = 0, res=0, c, charCount=0;
	char line[MAX_LINE_LENGTH], copyLine[MAX_LINE_LENGTH];
	
	sets setsArr[] = {
			{"SETA", &SETA},
			{"SETB", &SETB},
			{"SETC", &SETC},
			{"SETD", &SETD},
			{"SETE", &SETE},
			{"SETF", &SETF},
			{"Not_valid_set", NULL}
	};
	
	void initiatSets();
	do
	{
		fflush(stdin);
		charCount=0;
		printf("\n------------------------------------------\nPlease enter your desired command here:\n");
   
		if(NULL!=fgets(line, MAX_LINE_LENGTH, stdin))
		{
			printf("\nThe command line been read is:\n%s", line);
			if(line[strlen(line) - 1] != '\n') /*command line is bigger than defined*/
			{
	    		while((c = getchar()) != '\n')
				{
					charCount++;
				}
				printf("\nERROR: Command line contains more than %d characters.\n", MAX_LINE_LENGTH);
				continue;
			}
			strcpy(copyLine, line);
			if(!CheckBlankString(copyLine))
			{
				res = checkLineSyntax(line, setsArr, &isStop); /*command line isn't blank*/
				
				if(!res)
				{
					/*command syntax is not valid*/
					printf("\nERROR: Line syntax is incorrect!\n");
					continue;
				}
			}
			else
			{
				printf("\nNote: Empty command line!\n");
			}		
		}
	}
	while(isStop != 1 && !feof(stdin));
	
	if(isStop)
	{
		stop();
	}
	
	/*exiting program due to EOF*/
	printf("\nERROR: Exiting the program without explicit \"stop\" command occurred.\n");
	return 0;
}


