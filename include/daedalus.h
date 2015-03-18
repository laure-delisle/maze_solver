#ifndef DAEDALUS_H
#define DAEDALUS_H

#include <stdio.h>
#include <stdlib.h>
#include "struct.h"

//control the generation of the maze
int gen(FILE* in, maze* my_maze);

//call the gen/read/solve functions
int daedalus(FILE* in, int generate);

#endif