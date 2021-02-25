#include "stackOperator.h"

void pushOperation(struct NodeOperation **s, char operation[OPERATOR_SIZE]) {
	struct NodeOperation *temp = (struct NodeOperation *) malloc(sizeof(struct NodeOperation));
	strcpy(temp->operation, operation);
	temp->next = *s;
	*s = temp;
}

void popOperation(struct NodeOperation **s, char operation[OPERATOR_SIZE]) {
	if (!(*s)) {
		printf("Critical error: attempted popOperation() from empty stack\n");
		exit(EXIT_FAILURE);
	}

	strcpy(operation, (*s)->operation);
	struct NodeOperation *next = (*s)->next;
	free(*s);
	*s = next;
}
