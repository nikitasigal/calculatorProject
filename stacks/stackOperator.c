#include "stackOperator.h"

void pushOperation(struct NodeOperation **s, char operation[OPERATION_SIZE]) {
	struct NodeOperation *temp = (struct NodeOperation *) malloc(sizeof(struct NodeOperation));
	strcpy(temp->operation, operation);
	temp->next = *s;
	*s = temp;
}

void popOperation(struct NodeOperation **s, char operation[OPERATION_SIZE]) {
	if (!(*s)) {
		printError("Missing operation between operands");
		strcpy(operation, "+");
		return;
	}

	strcpy(operation, (*s)->operation);
	struct NodeOperation *next = (*s)->next;
	free(*s);
	*s = next;
}
