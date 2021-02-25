#ifndef CALCULATORPROJECT_STACKOPERATOR_H
#define CALCULATORPROJECT_STACKOPERATOR_H

#include "../libraries.h"

#define OPERATOR_SIZE 10
struct NodeOperation {
	char operation[OPERATOR_SIZE];
	struct NodeOperation *next;
};

void pushOperation(struct NodeOperation **s, char operation[OPERATOR_SIZE]);

void popOperation(struct NodeOperation **s, char operation[OPERATOR_SIZE]);

#endif //CALCULATORPROJECT_STACKOPERATOR_H
