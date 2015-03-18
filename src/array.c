#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "struct.h"

#define W 1u << 0
#define S 1u << 1
#define E 1u << 2
#define N 1u << 3

#define KYEL  "\x1B[33m"
#define KGRN  "\x1B[32m"
#define KBLU  "\x1B[34m"
#define RESET "\033[0m"

//malloc the array for the maze
int malloc_maze(maze* my_maze)
{
	int cmptCol;
	int height = my_maze->height;
	int width = my_maze->width;

	//maze is a 1D array, size = height
	my_maze->array = malloc(sizeof(unsigned short*)*width);

	//check 1st malloc
	if (my_maze->array == NULL)
	{
		return(4);
	}

	//maze is now a 2D array, size = width*height
	for (cmptCol = 0; cmptCol < width; cmptCol++)
	{
		my_maze->array[cmptCol] = malloc(sizeof(char*)*height);
		//check 2nd malloc
		if (my_maze->array[cmptCol] == NULL)
		{
			return(4);
		}
	}

	return(0);
}

//malloc the array for the maze
void free_maze(maze* my_maze)
{
	int cmptCol;
	int width = my_maze->width;

	//maze is a 1D array, size = height
	for (cmptCol = 0; cmptCol < width; cmptCol++)
	{
		free(my_maze->array[cmptCol]);
	}

	//maze array is now totally freed
	free(my_maze->array);
}

//secure the maze with borders
void put_borders(maze* my_maze)
{
	int cmptLin = 0;
	int cmptCol = 0;
	int width = my_maze->width;
	int height = my_maze->height;

	//top and bottom
	for(cmptCol=0; cmptCol<width; cmptCol++)
	{
		my_maze->array[cmptCol][0] |= N;
		my_maze->array[cmptCol][height-1] |= S;
	}

	//left and right
	for(cmptLin=0; cmptLin<height; cmptLin++)
	{
		my_maze->array[0][cmptLin] |= W;
		my_maze->array[width-1][cmptLin] |= E;
	}
}

//random fill the maze
void alea_maze(maze* my_maze)
{
	int cmpt = 0;
	int cmptLin = 0;
	int cmptCol = 0;
	int x = 0;
	int y = 0;

	unsigned short visited = 1u << 4;

	int width = my_maze->width;
	int height = my_maze->height;

	//direction towards next box
	int mov;
	unsigned short dir;
	int dx;
	int dy;

	//next box
	int xnext;
	int ynext;

	//initialize maze with walls
	for(cmptLin=0; cmptLin<height; cmptLin++)
	{
		for(cmptCol=0; cmptCol<width; cmptCol++)
		{
			my_maze->array[cmptCol][cmptLin] = 15;
		}
	}

	//carve walls
	for(cmptLin=0; cmptLin<height; cmptLin++)
	{
		for(cmptCol=0; cmptCol<width; cmptCol++)
		{
			x = cmptCol;
			y = cmptLin;
			cmpt=0;

			mov = (rand() % 4);
			dir = 1u << mov;

			//chose direction for next box
			while (cmpt < 4)
			{
				//reset direction
				dx = 0;
				dy = 0;

				//set direction according to dir
				switch (dir)
				{
					case N:
						dy = -1;
						break;
					case S:
						dy = 1;
						break;
					case W:
						dx = -1;
						break;
					case E:
						dx = 1;
						break;
				}

				//compute next box
				xnext = x + dx;
				ynext = y + dy;

				//check borders & walls & visited
				if ((xnext >= 0) && (xnext < width) && (ynext >= 0) && (ynext<height)
					&& (my_maze->array[x][y] & dir) && !(my_maze->array[xnext][ynext] & visited))
				{
					//break the wall !
					my_maze->array[x][y] -= dir; //remove wall of box
					my_maze->array[xnext][ynext] -= 1u << (mov + 2) % 4; //remove wall of next box
					
					//ok have been visited
					my_maze->array[x][y] |= visited;

					//move on
					mov = rand() % 4;
					dir = 1u << mov;
					cmpt = 0;
					x = xnext;
					y = ynext;
				}
				//try another direction
				else
				{
					mov = (mov + 1) % 4;
					dir = 1u << mov;
					cmpt += 1;
				}
			}
			//ok have been visited
			my_maze->array[x][y] |= visited;
		}
	}

	//solve artefact
	if ((my_maze->array[1][1] & N) || (my_maze->array[1][1] & W))
	{
		//remove wall between [0][0] and [1][0]
		my_maze->array[0][0] &= ~(1u << 2);
		my_maze->array[1][0] &= ~(1u << 0);
	
		//remove wall between [0][0] and [0][1]
		my_maze->array[0][0] &= ~(1u << 1);
		my_maze->array[0][1] &= ~(1u << 3);
	} 

	//unvisit
	for(cmptLin=0; cmptLin<height; cmptLin++)
	{
		for(cmptCol=0; cmptCol<width; cmptCol++)
		{
			my_maze->array[cmptCol][cmptLin] &= 0xFEF;
		}
	}

	//borders
	put_borders(my_maze);
}

//load maze from file
void read_maze(FILE* in, maze* my_maze)
{
	int cmptLin = 0;
	int cmptCol = 0;

	for(cmptLin=0; cmptLin<my_maze->height; cmptLin++)
	{
		for(cmptCol=0; cmptCol<my_maze->width; cmptCol++)
		{
			fscanf(in, "%hu", &my_maze->array[cmptCol][cmptLin]);
		}
	}
}

//test wall and display
void test_print(maze* my_maze, int x, int y, unsigned short test, char mode)
{
	switch (test)
	{
		case S:
			if (my_maze->array[x][y] & test) printf("--");
			else printf("  ");
			break;
		case W:
			if (my_maze->array[x][y] & test) printf("|");
			else printf(" ");
			break;
		case 1u << 5:
			if ((x == my_maze->x_entry) && (y == my_maze->y_entry)) printf(KBLU "😄 "RESET);
			else if ((x == my_maze->x_exit) && (y == my_maze->y_exit)) printf(KYEL"$$"RESET);
			else if (my_maze->array[x][y] & 0x20) printf(KGRN"<>"RESET);
			else if ((my_maze->array[x][y] & 0x10) && (mode == 'p')) printf(KGRN".."RESET);
			else if ((my_maze->array[x][y] & 0x10) && (mode == 'b')) printf("  ");
			else printf("  ");
			break;
		case 0:
			if ((x == my_maze->x_entry) && (y == my_maze->y_entry)) printf(KBLU"😄 "RESET);
			else if ((x == my_maze->x_exit) && (y == my_maze->y_exit)) printf(KYEL"$$"RESET);
			else printf("  ");
			break;
	}
}

//display in ASCII art
void display(maze* my_maze, char mode)
{
	unsigned short path = 1u << 5;

	int cmptLin;
	int cmptCol;
	int width = my_maze->width;
	int height = my_maze->height;

	//top
	for(cmptCol=0; cmptCol<width; cmptCol++)
	{
		printf("+--");
	}
	printf("+\n");

	//body
	for(cmptLin=0; cmptLin<height; cmptLin++)
	{
		for(cmptCol=0; cmptCol<width; cmptCol++)
		{
			test_print(my_maze, cmptCol, cmptLin, W, 0);
			if(mode) test_print(my_maze, cmptCol, cmptLin, path, mode);
			else test_print(my_maze, cmptCol, cmptLin, 0, 0);
		}
		printf("|\n");
		for(cmptCol=0; cmptCol<width; cmptCol++)
		{
			printf("+");
			test_print(my_maze, cmptCol, cmptLin, S, 0);
		}
		printf("+\n");
	}
}

//display the maze in terminal
void display_maze(maze* my_maze)
{
	display(my_maze, 0);
}

//display the path in terminal
void display_path(maze* my_maze)
{
	display(my_maze, 'p');
}

//display the best past in terminal
void display_best_path(maze* my_maze)
{
	display(my_maze, 'b');
}

//remove visited and path bits
void clean_path(maze* my_maze)
{
	int cmptLin;
	int cmptCol;
	int width = my_maze->width;
	int height = my_maze->height;

	unsigned short visited = 1u << 4;
	unsigned short path = 1u << 5;

	for(cmptLin=0; cmptLin<height; cmptLin++)
	{
		for(cmptCol=0; cmptCol<width; cmptCol++)
		{
			//unvisit
			my_maze->array[cmptCol][cmptLin] &= ~visited;
			//unpath
			my_maze->array[cmptCol][cmptLin] &= ~path;
		}
	}
}

//path with hand-on-the-wall
int get_path(maze* my_maze, int hand_on_the_wall)
{
	int cmpt = 0;
	int cmptStop = 0;
	int x = my_maze->x_entry;
	int y = my_maze->y_entry;
	int width = my_maze->width;
	int height = my_maze->height;

	unsigned short visited = 1u << 4;
	unsigned short path = 1u << 5;

	//direction towards next box
	int mov;
	unsigned short dir;
	int dx;
	int dy;

	//next box
	int xnext;
	int ynext;

	//first move to the left
	mov = 0;
	dir = 1u << mov;

	//chose direction for next box
	while ((cmptStop<3) && (cmpt < 4) && ((x != my_maze->x_exit) || (y != my_maze->y_exit)))
	{
		//reset direction
		dx = 0;
		dy = 0;

		//set direction according to dir
		switch (dir)
		{
			case N:
				dy = -1;
				break;
			case S:
				dy = 1;
				break;
			case W:
				dx = -1;
				break;
			case E:
				dx = 1;
				break;
		}

		//compute next box
		xnext = x + dx;
		ynext = y + dy;

		// printf("[%d,%d] vers [%d,%d]\n", x,y,xnext,ynext);

		//check borders & walls
		if ((xnext < 0) || (xnext >= width) || (ynext < 0) || (ynext>=height)
			|| (my_maze->array[x][y] & dir))
		{
			//there is a wall, change direction
			if (hand_on_the_wall == 1)
			{
				//right hand
				mov = (mov + 1) % 4;
			}
			else
			{
				//left hand
				mov = (mov + 3) % 4;
			}			
			dir = 1u << mov;
			cmpt += 1;
		}
		else
		{
			//no wall, add [x;y] to path or visited
			if (my_maze->array[xnext][ynext] & path)
			{
				//been there already
				my_maze->array[x][y] |= visited;
				my_maze->array[x][y] &= ~path;
			}
			else
			{
				my_maze->array[x][y] |= path;
			}

			//move on
			if (hand_on_the_wall == 1)
			{
				//right hand
				mov = (mov + 3) % 4;
			}
			else
			{
				//left hand
				mov = (mov + 1) % 4;
			}

			//check infinite loop
			if ((x == my_maze->x_entry) && (y == my_maze->y_entry))
			{
				cmptStop++;
			}

			dir = 1u << mov;
			cmpt = 0;
			x = xnext;
			y = ynext;
		}
	}

	//no path found
	if ((cmpt == 4) || (cmptStop == 3))
	{
		display_path(my_maze);
		return(1);
	}

	//path was found
	return(0);	
}

//compute path length depending on the hand on the wall
int get_path_length(maze* my_maze)
{
	int cmptLin;
	int cmptCol;
	int length = 0;

	for(cmptLin=0; cmptLin<my_maze->height; cmptLin++)
	{
		for(cmptCol=0; cmptCol<my_maze->width; cmptCol++)
		{
			if(my_maze->array[cmptCol][cmptLin] & 0x20)
			{
				length++;
			}
		}
	}

	return(length);
}

//best past
void get_best_path(maze* my_maze)
{
	int bin = 0;
	int length_left_hand = get_path_length(my_maze);
	clean_path(my_maze);

	//right hand
	bin = get_path(my_maze, 1);
	int length_right_hand = get_path_length(my_maze);

	int best_length = 0;

	if ((length_right_hand <= length_left_hand) && !bin)
	{
		best_length = length_right_hand;
		display_best_path(my_maze);
	}
	else
	{
		clean_path(my_maze);
		bin = get_path(my_maze,2);
		best_length = length_left_hand;
		display_best_path(my_maze);
	}

	printf("The best path length is %d ! \n", best_length);
	printf("You are now officially RICH ! Congratulations 😄\n\n");
}

