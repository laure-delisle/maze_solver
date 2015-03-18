#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // to use getopt

#include "array.h"
#include "daedalus.h"


/* global var that can be modified */
int VERBOSE = 0;

/* error function : display a message according to the error */
void error(int errorCode)
{
	printf("\n");
	switch (errorCode)
	{
		case 1:
			printf("Error ! no input file or generate specified. Try -h for help :)\n");
			break;
		case 2:
			printf("Error ! input file and generate specified at the same time. Try -h for help :)\n");
			break;
		case 3:
			printf("Error ! can not open input file. Check input file permissions :)\n");
			break;
		case 4:
			printf("Error ! not enough memory\n");
			break;
		case 5:
			printf("Error ! Enter an integer\n");
			break;
		case 6:
			printf("Error ! Treasure is not in the maze\n");
			break;
		case 7:
			printf("Error ! Treasure can't be on entry\n");
			break;
		case 8:
			printf("Error ! No path found ! The maze is crapy :(\n");
			break;
	}
}

int process_command_line(int argc, char** argv, char* progName)
{
	char *input = NULL;
	int generate = 0;
	char c = 0;
	int result = 0;

	while ((c = getopt(argc, argv, "hrgi:")) != -1)
	{
		switch (c)
		{
			case 'h':
				printf("\n%s is a maze generator and solving programm\n", progName);
				printf("\timplemented by Laure Delisle\n\n");
				printf("The synopsis is :\n\n%s [-h] [-i input_file.txt || -r || -g]\n\n", progName);
				printf("\tuse -h to display help\n");
				printf("\tuse -i to specify input file\n");
				printf("\tuse -r to generate a maze in FULL RANDOM (only if no input !)\n");
				printf("\tuse -g to generate a maze in CONTROLED RANDOM (only if no input !)\n");
				return (0);
			case 'g':
				generate = 2;
				break;
			case 'r':
				generate = 1;
				break;
			case 'i':
				input = optarg;
				break;
		}
	}

	if (!input && !generate)
	{
		error(1);
		return(1);
	}

	if (input && generate)
	{
		error(2);
		return(1);
	}

	FILE *in = NULL;
	//open the files 'input' and/or 'output'
	if (input && !generate)
	{
		in = fopen(input, "r");

		//error at opening
		if (!in)
		{
			error(3);
			return(1);
		}
	}

	//everything went smoothly, let's work !
	result = daedalus(in, generate);
	
	if (result)
	{
		error(result);
		return(1);
	}

	return(0);
}

int main(int argc, char **argv)
{
	if(process_command_line(argc, argv, argv[0]))
	{
		printf("\nTerminating...\n\n");
		return(EXIT_FAILURE);
	}

	return (EXIT_SUCCESS);
}