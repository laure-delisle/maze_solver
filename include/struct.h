#ifndef TYPE_H
#define TYPE_H

typedef struct maze_
{
	unsigned short **array; //array filled with maze's cells
	//x is vertical
	//y is horizontal
	//my_maze->array[x][y]

	int height; //maze height = number of lines
	int width; //maze width = number of columns

	//coordinates of the maze entry
	int x_entry;
	int y_entry;

	//coordinates of the maze exit
	int x_exit;
	int y_exit;

} maze;

#endif
