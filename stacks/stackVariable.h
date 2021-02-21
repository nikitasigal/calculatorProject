#ifndef CALCULATORPROJECT_STACKVARIABLE_H
#define CALCULATORPROJECT_STACKVARIABLE_H

#include "../libraries.h"

#define VAR_NAME_SIZE 10
#define VAR_EXPR_SIZE 300
#define VAR_ELEMENT_COUNT 300
#define VAR_ELEMENT_SIZE 20
struct Variable {
	char name[VAR_NAME_SIZE];
	int elements;
	char expression[VAR_ELEMENT_COUNT][VAR_ELEMENT_SIZE];
	short isSorted;
};

struct StackVariable {
	struct Variable var;
	struct StackVariable *prev, *next;
};

void pushVariable(struct StackVariable **s, char name[VAR_NAME_SIZE], char expression[VAR_EXPR_SIZE]);

struct Variable popVariable(struct StackVariable **s);

void forwardVariable(struct StackVariable **s, struct StackVariable *cur);

#endif //CALCULATORPROJECT_STACKVARIABLE_H
