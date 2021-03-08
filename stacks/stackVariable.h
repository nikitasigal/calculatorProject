#ifndef CALCULATORPROJECT_STACKVARIABLE_H
#define CALCULATORPROJECT_STACKVARIABLE_H

#include "../libraries.h"

#define VAR_NAME_SIZE 10
#define VAR_EXPR_SIZE 300
#define VAR_ELEMENT_COUNT 300
#define VAR_ELEMENT_SIZE 200

struct Variable {
	char name[VAR_NAME_SIZE];
	int elements;
	char expression[VAR_ELEMENT_COUNT][VAR_ELEMENT_SIZE];
	bool isSorted;
};

struct NodeVariable {
	struct Variable variable;
	struct NodeVariable *prev, *next;
};

void pushVariable(struct NodeVariable **s, char name[VAR_NAME_SIZE], char expression[VAR_EXPR_SIZE]);

struct Variable popVariable(struct NodeVariable **s);

void forwardVariable(struct NodeVariable **s, struct NodeVariable *dest, struct NodeVariable *cur);

#endif //CALCULATORPROJECT_STACKVARIABLE_H
