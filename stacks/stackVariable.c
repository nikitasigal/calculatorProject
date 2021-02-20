#include "stackVariable.h"

void pushVariable(struct StackVariable **s, char name[VAR_NAME_SIZE], char expression[VAR_EXPR_SIZE]) {
	struct Variable var;
	strcpy(var.name, name);
	strcpy(var.expression, expression);

	struct StackVariable *temp = (struct StackVariable *) malloc(sizeof(struct StackVariable));
	temp->var = var;
	temp->next = *s;
	temp->var.isSorted = 0;

	if (temp->next)
		temp->next->prev = temp;
	temp->prev = NULL;
	*s = temp;
}

struct Variable popVariable(struct StackVariable **s) {
	if (!(*s)) {
		printf("Critical error: attempted popVariable() from empty stack\n");
		exit(EXIT_FAILURE);
	}

	struct Variable value = (*s)->var;
	struct StackVariable *next = (*s)->next;
	if (next)
		next->prev = NULL;
	free(*s);
	*s = next;
	return value;
}

void forwardVariable(struct StackVariable **s, struct StackVariable *cur) {
	if (!(*s)) {
		printf("Critical error: attempted forwardVariable() with empty stack\n");
		exit(EXIT_FAILURE);
	}

	if (cur == (*s)) {
		printf("Potential error: attempted to forwardVariable() which was already in front of the stack\n");
		exit(EXIT_FAILURE);
	}

	struct StackVariable *prev = cur->prev;
	struct StackVariable *next = cur->next;

	prev->next = cur->next;
	if (next)
		next->prev = cur->prev;

	cur->prev = NULL;
	cur->next = *s;
	if (cur->next)
		cur->next->prev = cur;
	*s = cur;
}