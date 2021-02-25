#include "stackComplex.h"

void pushComplex(struct NodeComplex **s, complex double value) {
	struct NodeComplex *temp = (struct NodeComplex *) malloc(sizeof(struct NodeComplex));
	temp->value = value;
	temp->next = *s;
	*s = temp;
}

complex double popComplex(struct NodeComplex **s) {
	if (!(*s)) {
		printf("Critical error: attempted popComplex() from empty stack\n");
		exit(EXIT_FAILURE);
	}

	complex double value = (*s)->value;
	struct NodeComplex *next = (*s)->next;
	free(*s);
	*s = next;
	return value;
}