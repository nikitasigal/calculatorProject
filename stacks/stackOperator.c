#include "stackOperator.h"

void pushOperation(struct StackOperator **s, char *op) {
	struct StackOperator *temp = (struct StackOperator *) malloc(sizeof(struct StackOperator));
	strcpy(temp->op, op);
	temp->next = *s;
	*s = temp;
}

void popOperation(struct StackOperator **s, char *op) {
	if (!(*s)) {
		printf("Critical error: attempted popOperation() from empty stack\n");
		exit(EXIT_FAILURE);
	}

	strcpy(op, (*s)->op);
	struct StackOperator *next = (*s)->next;
	free(*s);
	*s = next;
}
