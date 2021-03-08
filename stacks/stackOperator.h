#ifndef CALCULATORPROJECT_STACKOPERATOR_H
#define CALCULATORPROJECT_STACKOPERATOR_H

#include "../libraries.h"
#include "../errorHandler.h"

#define OPERATION_SIZE 10
struct NodeOperation {
	char operation[OPERATION_SIZE];
	struct NodeOperation *next;
};

void pushOperation(struct NodeOperation **s, char operation[OPERATION_SIZE]);

void popOperation(struct NodeOperation **s, char operation[OPERATION_SIZE]);

#endif //CALCULATORPROJECT_STACKOPERATOR_H
