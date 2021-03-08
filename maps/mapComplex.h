#ifndef CALCULATORPROJECT_MAPCOMPLEX_H
#define CALCULATORPROJECT_MAPCOMPLEX_H

#include "../libraries.h"
#include "hashing.h"

struct MapComplex {
	char key[KEY_SIZE];
	long double complex value;
	short empty;
};

void initVariables(struct MapComplex *m);

void insertVariable(struct MapComplex *m, char *key, long double complex value);

unsigned int findVariable(struct MapComplex *m, char *key);

#endif //CALCULATORPROJECT_MAPCOMPLEX_H
