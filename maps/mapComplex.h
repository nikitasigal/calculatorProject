#ifndef CALCULATORPROJECT_MAPCOMPLEX_H
#define CALCULATORPROJECT_MAPCOMPLEX_H

#include "../libraries.h"
#include "hashing.h"

struct MapComplex {
	char key[KEY_SIZE];
	long double complex value;
	short empty;
};

void initMapComplex(struct MapComplex m[MAP_SIZE]);

void insertComplex(struct MapComplex m[MAP_SIZE], char key[KEY_SIZE], long double complex value);
unsigned int findComplex(struct MapComplex *m, char key[KEY_SIZE]);

#endif //CALCULATORPROJECT_MAPCOMPLEX_H
