#include "stackComplex.h"

void pushComplex(struct StackComplex **s, complex double value) {
	struct StackComplex *temp = (struct StackComplex *) malloc(sizeof(struct StackComplex));
	temp->value = value;
	temp->next = *s;
	*s = temp;
}

complex double popComplex(struct StackComplex **s) {
	if (!(*s)) {
		printf("Critical error: attempted popComplex() from empty stack\n");
		exit(EXIT_FAILURE);
	}

	complex double value = (*s)->value;
	struct StackComplex *next = (*s)->next;
	free(*s);
	*s = next;
	return value;
}