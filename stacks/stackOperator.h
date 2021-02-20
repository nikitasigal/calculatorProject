#ifndef CALCULATORPROJECT_STACKOPERATOR_H
#define CALCULATORPROJECT_STACKOPERATOR_H

#include "../libraries.h"

#define OPERATOR_SIZE 10
struct StackOperator {
	char op[OPERATOR_SIZE];
	struct StackOperator *next;
};

void pushOperator(struct StackOperator **s, char *op);

void popOperator(struct StackOperator **s, char *op);

#endif //CALCULATORPROJECT_STACKOPERATOR_H
