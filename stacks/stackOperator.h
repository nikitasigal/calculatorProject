#ifndef CALCULATORPROJECT_STACKOPERATOR_H
#define CALCULATORPROJECT_STACKOPERATOR_H

#include "../libraries.h"

#define OPERATOR_SIZE 10
struct StackOperator {
	char op[OPERATOR_SIZE];
	struct StackOperator *next;
};

void pushOperation(struct StackOperator **s, char *op);

void popOperation(struct StackOperator **s, char *op);

#endif //CALCULATORPROJECT_STACKOPERATOR_H
