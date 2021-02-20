#ifndef CALCULATORPROJECT_MAPFUNCTIONS_H
#define CALCULATORPROJECT_MAPFUNCTIONS_H

#include "../libraries.h"
#include "hashing.h"
#include "../stacks/stackComplex.h"

struct MapFunctions {
	char key[KEY_SIZE];
	double complex (*function)(struct StackComplex *);
	short empty;
};

void initMapFunctions(struct MapFunctions m[MAP_SIZE]);

void insertFunction(struct MapFunctions m[MAP_SIZE], char key[KEY_SIZE], complex double (*ptr)(struct StackComplex *));

unsigned int findFunction(struct MapFunctions m[MAP_SIZE], char key[KEY_SIZE]);

#endif //CALCULATORPROJECT_MAPFUNCTIONS_H
