#ifndef CALCULATORPROJECT_STACKVARIABLE_H
#define CALCULATORPROJECT_STACKVARIABLE_H

#include "../libraries.h"

#define VAR_NAME_SIZE 10
#define VAR_EXPR_SIZE 300
struct Variable {
	char name[VAR_NAME_SIZE];
	char expression[VAR_EXPR_SIZE];
};

struct StackVariable {
	struct Variable var;
	struct StackVariable *prev, *next;
};

void pushVariable(struct StackVariable **s, char *name, char *expression);

struct Variable popVariable(struct StackVariable **s);

void forwardVariable(struct StackVariable **s, struct StackVariable *cur);

#endif //CALCULATORPROJECT_STACKVARIABLE_H
