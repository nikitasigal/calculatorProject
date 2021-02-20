#include "stackVariable.h"

void pushVariable(struct StackVariable **s, char *name, char *expression) {
	struct Variable var;
	strcpy(var.name, name);
	strcpy(var.expression, expression);
	struct StackVariable *temp = (struct StackVariable *) malloc(sizeof(struct StackVariable));
	temp->var = var;
	temp->next = *s;
	if (temp->next)
		temp->next->prev = temp;
	temp->prev = NULL;
	*s = temp;
}

struct Variable popVariable(struct StackVariable **s) {
	struct Variable value = (*s)->var;
	struct StackVariable *next = (*s)->next;
	if (next)
		next->prev = NULL;
	free(*s);
	*s = next;
	return value;
}

void forwardVariable(struct StackVariable **s, struct StackVariable *cur) {
	struct StackVariable *prev = cur->prev;
	struct StackVariable *next = cur->next;

	prev->next = cur->next;
	if(next)
		next->prev = cur->prev;

	cur->prev = NULL;
	cur->next = *s;
	if (cur->next)
		cur->next->prev = cur;
	*s = cur;
}