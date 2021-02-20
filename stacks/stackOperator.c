#include "stackOperator.h"

void pushOperator(struct StackOperator **s, char *op) {
	struct StackOperator *temp = (struct StackOperator *) malloc(sizeof(struct StackOperator));
	strcpy(temp->op, op);
	temp->next = *s;
	*s = temp;
}

void popOperator(struct StackOperator **s, char *op) {
	strcpy(op, (*s)->op);
	struct StackOperator *next = (*s)->next;
	free(*s);
	*s = next;
}