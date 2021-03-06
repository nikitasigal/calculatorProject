#include "stackVariable.h"

void pushVariable(struct NodeVariable **s, char name[VAR_NAME_SIZE], char expression[VAR_EXPR_SIZE]) {
	struct Variable var;
	strcpy(var.name, name);

	var.elements = 0;
	var.isSorted = false;
	for (int i = 0; i < strlen(expression);) {
		char str[VAR_ELEMENT_SIZE] = {0};
		int crt = 0;
		if (isalpha(expression[i])) {
			while (isalnum(expression[i]))
				str[crt++] = expression[i++];
			strcpy(var.expression[var.elements++], str);
		} else if (isdigit(expression[i])) {
			while (isdigit(expression[i]) || expression[i] == '.')
				str[crt++] = expression[i++];
			strcpy(var.expression[var.elements++], str);
		} else {
			if (expression[i] != ' ') {
				var.expression[var.elements][1] = '\0';
				var.expression[var.elements++][0] = expression[i];
			}
			i++;
		}
	}

	struct NodeVariable *temp = (struct NodeVariable *) malloc(sizeof(struct NodeVariable));
	temp->variable = var;
	temp->next = *s;

	if (temp->next)
		temp->next->prev = temp;
	temp->prev = NULL;
	*s = temp;
}

struct Variable popVariable(struct NodeVariable **s) {
	if (!(*s)) {
		printf("Critical error: attempted popVariable() from empty stack\n");
		exit(EXIT_FAILURE);
	}

	struct Variable var = (*s)->variable;
	struct NodeVariable *next = (*s)->next;
	if (next)
		next->prev = NULL;
	free(*s);
	*s = next;
	return var;
}

void forwardVariable(struct NodeVariable **s, struct NodeVariable *dest, struct NodeVariable *cur) {
	struct NodeVariable *next = cur->next;
	struct NodeVariable *prev = cur->prev;
	if (next)
		next->prev = prev;
	if (prev)
		prev->next = next;

	if (cur == *s)
		*s = next;

	if (dest == *s) {
		*s = cur;
		dest->prev = cur;
		cur->next = dest;
		cur->prev = NULL;
		return;
	}

	prev = dest->prev;
	cur->next = dest;
	dest->prev = cur;

	cur->prev = prev;
	prev->next = cur;
}

