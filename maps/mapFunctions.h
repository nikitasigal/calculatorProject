#ifndef CALCULATORPROJECT_MAPFUNCTIONS_H
#define CALCULATORPROJECT_MAPFUNCTIONS_H

#include "../libraries.h"
#include "../stacks/stackComplex.h"
#include "hashing.h"

struct MapOperations {
	char key[KEY_SIZE];
	void (*operation)(struct NodeComplex **);
	short empty;
};

void initOperations(struct MapOperations *m);

void insertOperation(struct MapOperations *m, char *key, void (*ptr)(struct NodeComplex **));

unsigned int findOperation(struct MapOperations m[MAP_SIZE], char key[KEY_SIZE]);

#endif //CALCULATORPROJECT_MAPFUNCTIONS_H
