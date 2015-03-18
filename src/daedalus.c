#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "struct.h"
#include "array.h"

//control the generation of the maze
int gen(FILE* in, maze* my_maze, int full_alea)
{
	int status = 0;

	//seed random number generator
	srand(time(0));

	if (!full_alea)
	{
		printf("Please specify the caracteristics of your maze :\n\n");
		
		//size of the maze
		printf("width: ");
		if (scanf("%d", &my_maze->width) != 1) return(5);
		printf("height: ");
		if(scanf("%d", &my_maze->height) != 1) return(5);
		
		//entry
		printf("Entry is [0;0]\n");
		my_maze->x_entry = 0;
		my_maze->y_entry = 0;
		
		//exit
		printf("Treasure abscissa: ");
		if (scanf("%d", &my_maze->x_exit) != 1) return(5);
		printf("Treasure ordinate: ");
		if(scanf("%d", &my_maze->y_exit) != 1) return(5);
		if(	my_maze->x_exit <0 || my_maze->x_exit>= my_maze->width ||
			my_maze->y_exit <0 || my_maze->y_exit>= my_maze->height) return(6);
		if((my_maze->x_exit == my_maze->x_entry) && (my_maze->y_exit == my_maze->y_entry)) return(7);
	}
	else if (full_alea)
	{
		my_maze->width = (rand() % 40) + 1;
		my_maze->height = (rand() % 15) + 1;

		my_maze->x_entry = (rand() % my_maze->width);
		my_maze->y_entry = (rand() % my_maze->height);

		do
		{
			my_maze->x_exit = (rand() % my_maze->width);
			my_maze->y_exit = (rand() % my_maze->height);
		}
		while ((my_maze->x_exit == my_maze->x_entry) && (my_maze->y_exit == my_maze->y_entry));
	}

	//write caracteristics in file
	fprintf(in, "%d ", my_maze->width);
	fprintf(in, "%d ", my_maze->height);
	fprintf(in, "%d ", my_maze->x_entry);
	fprintf(in, "%d ", my_maze->y_entry);
	fprintf(in, "%d ", my_maze->x_exit);
	fprintf(in, "%d\n", my_maze->y_exit);

	//malloc
	status = malloc_maze(my_maze);
	if(status)
	{
		return(status);
	}

	//generate
	alea_maze(my_maze);

	//write maze in file
	int cmptCol = 0;
	int cmptLin = 0;

	for(cmptLin=0; cmptLin<my_maze->height; cmptLin++)
	{
		for(cmptCol=0; cmptCol<my_maze->width; cmptCol++)
		{
			fprintf(in, "%hu ", my_maze->array[cmptCol][cmptLin]);
		}
		fprintf(in, "\n");
	}
	
	return(0);
}

//call the gen/read/solve functions
int daedalus(FILE* in, int generate)
{
	//declaration
	maze* my_maze = NULL;
	my_maze = malloc(sizeof(maze)*1);
	int status = 0;

	int answer = 0;

	//check malloc
	if (my_maze == NULL)
	{
		return(4);
	}

	//input file specified : read from input file
	if (in)
	{
		//size of the maze
		fscanf(in, "%d", &my_maze->width);
		fscanf(in, "%d", &my_maze->height);
		
		//entry
		fscanf(in, "%d", &my_maze->x_entry);
		fscanf(in, "%d", &my_maze->y_entry);
		
		//exit
		fscanf(in, "%d", &my_maze->x_exit);
		fscanf(in, "%d", &my_maze->y_exit);

		//malloc
		status = malloc_maze(my_maze);
		if(status) return(status);

		//load maze from file
		read_maze(in, my_maze);
		put_borders(my_maze);
	}


	//generate==1 -> generate a maze and save in a file
	else if (generate)
	{
		//write in the file
		in = fopen("generated_maze", "w");

		//create maze into file
		if (generate == 1) status = gen(in, my_maze, 1); //full_alea
		else if (generate == 2) status = gen(in, my_maze, 0); //controled gen
		if(status) return(status);

		//read the file
		fclose(in);
		in = fopen("generated_maze", "r");
	}

	//display maze in terminal
	printf("Here is your maze :\n");
	display_path(my_maze);

	//find path
	printf("\nDo you want to find a path from the entry to the treasure ? (yes = 1/no = 0) : ");
	scanf("%d", &answer);
	if(answer == 1)
	{
		printf("\n-> You follow the popular technique :\n");
		printf("-> You put your left hand on the wall on your left and walk forward\n\n");
		clean_path(my_maze);
		if (get_path(my_maze,2))
		{
			free_maze(my_maze);
			return(8);
		}
		display_path(my_maze);
	}

	answer = 0;

	printf("\nA legend says that an ancient secret path conducts directly to the treasure.\n");
	printf("Do you want to try & find it ? (yes = 1/no = 0) : ");
	scanf("%d", &answer);
	if (answer == 1)
	{
		printf("\n");
		get_best_path(my_maze);
	}
	else
	{
		printf("Ok ! Good bye :)\n");
	}

	free_maze(my_maze);

	return(0);
}