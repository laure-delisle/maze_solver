#ifndef ARRAY_H
#define ARRAY_H

#include <stdio.h>
#include <stdlib.h>
#include "struct.h"

//malloc the array for the maze
int malloc_maze(maze* my_maze);

//malloc the array for the maze
void free_maze(maze* my_maze);

//secure the maze with borders
void put_borders(maze* my_maze);

//random fill the maze
void alea_maze(maze* my_maze);

//load maze from file
void read_maze(FILE* in, maze* my_maze);

//test wall and display
void test_print(maze* my_maze, int x, int y, unsigned short test, char mode);

//display in ASCII art
void display(maze* my_maze, char mode);

//display the maze in terminal
void display_maze(maze* my_maze);

//display the path in terminal
void display_path(maze* my_maze);

//display the best past in terminal
void display_best_path(maze* my_maze);

//remove visited and path bits
void clean_path(maze* my_maze);

//path with hand-on-the-wall
int get_path(maze* my_maze, int hand_on_the_wall);

//compute path length depending on the hand on the wall
int get_path_length(maze* my_maze, int hand_on_the_wall);

//best past
void get_best_path(maze* my_maze);

#endif