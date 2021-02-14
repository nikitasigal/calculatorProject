#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define SIZE 100
#define LENGTH 100
#define VARIABLES_SIZE 100
char *PTR = NULL;


typedef struct {
	char variableName[LENGTH];
	char variableValue[LENGTH];
} VARIABLE;
VARIABLE vars[VARIABLES_SIZE];
int varsSize = 0;
char expression[LENGTH];

typedef struct Link {
	char *value;
	struct Link *next;
} Link;

double calculate(char *);

bool isEmpty(Link **first) {
	return *first == NULL;
}

void insertFirst(char val[SIZE], Link **first) {
	Link *current = malloc(sizeof(Link));
	current->value = val;
	current->next = *first;
	*first = current;
}

char *deleteFirst(Link **first) {
	Link *tmp = *first;
	*first = (*first)->next;
	char *t = tmp->value;
	free(tmp);

	return t;
}

void display(Link **first) {
	Link *current = *first;
	while (current) {
		printf("%s "
		       "", current->value);
		current = current->next;
	}

	//printf("\n");
	free(current);
}

void recreate(char str[SIZE], int *size) {
	for (int i = 0; i < *size; ++i)
		str[i] = '\0';
	*size = 0;
}

bool isRightAssociated(char ch) {
	return ch == '^';
}

int parseString(char str[SIZE], char dest[SIZE][LENGTH]) {
	char temp[SIZE];
	int c = 0;
	int ptr = 0;

	for (int i = 0; i < strlen(str) / sizeof(char); ++i) {
		char cur = str[i];
		if (isdigit(cur) || cur == '.' || (i == 0 && cur == '-') || (cur == '-' && str[i - 1] == '(')) {
			temp[ptr++] = cur;
		} else if (isalpha(cur)) {
			char variable[LENGTH] = {0};
			int tempVarSize = 0;
			while (isalpha(str[i])) {
				variable[tempVarSize++] = str[i++];
			}
			bool isVariable = false;
			for (int j = 0; j < varsSize; ++j) {
				if (strcmp(variable, vars[j].variableName) == 0) {
					char tempString[LENGTH] = {0};
					snprintf(tempString, LENGTH, "%lf", calculate(vars[j].variableValue));
					strcpy(dest[c++], tempString);
					isVariable = true;
					--i;
					break;
				}
			}
			if (!isVariable) {
				if (strcmp(variable, "pow") == 0) {
					char firstArgument[LENGTH] = {0};
					int firstSize = 0;
					char secondArgument[LENGTH] = {0};
					int secondSize = 0;
					++i;
					while (str[i] != ',') {
						firstArgument[firstSize++] = str[i++];
					}
					++i;
					while (str[i] != ')') {
						secondArgument[secondSize++] = str[i++];
					}

					char tempString[LENGTH] = {0};
					double a = calculate(firstArgument);
					double b = calculate(secondArgument);
					double res = pow(a, b);
					sprintf(tempString, "%0.15lf", res);
					strcpy(dest[c++], tempString);
				}
				if (strcmp(variable, "cos") == 0) {
					char argument[LENGTH] = {0};
					int argSize = 0;
					char tempString[LENGTH] = {0};
					while (str[i] != ')') {
						argument[argSize++] = str[i++];
					}
					sprintf(tempString, "%0.15lf", cos(calculate(argument)));
					strcpy(dest[c++], tempString);
				}
			}
		} else if (cur != ' ') {
			if (strlen(temp)) {
				strcpy(dest[c++], temp);
				recreate(temp, &ptr);
			}
			dest[c][0] = cur;
			c++;
		}
		/*if (cur != ' ') {
			if (strlen(temp)) {
				strcpy(dest[c++], temp);
				recreate(temp, &ptr);
			}
			dest[c][0] = cur;
			c++;
		}*/
	}

	if (strlen(temp))
		strcpy(dest[c++], temp);

	return c;
}

int priority(char sign) {
	switch (sign) {
		case '-':
		case '+':
			return 1;
		case '*':
		case '/':
			return 2;
		case '^':
			return 3;
		default:
			return 0;
	}
}

void listReverseCopy(Link **dest, Link **source) {
	while (*source) {
		char *tmp = deleteFirst(source);
		insertFirst(tmp, dest);
	}
}

void infixToPostfix(char *str, Link **ans, Link **ops) {
	char words[SIZE][LENGTH];
	int size = parseString(str, words);

	for (int i = 0; i < size; ++i) {
		if (isdigit(words[i][0]) || (strlen(words[i]) != 1 && words[i][0] == '-')) {
			insertFirst(words[i], &(*ans));
		} else {
			char cur = words[i][0];
			if (cur == '(') {
				char tmp[1] = {cur};
				insertFirst(tmp, &(*ops));
			} else {
				if (cur == ')') {
					while (!isEmpty(&(*ops))) {
						char *opTop = deleteFirst(&(*ops));
						if (opTop[0] != '(')
							insertFirst(opTop, &(*ans));
						else
							break; //1+2-3*4+5/6
					}
				} else {
					while (!isEmpty(&(*ops))) {
						char *opTop = deleteFirst(&(*ops));
						if (!isRightAssociated(words[i][0]) && priority(opTop[0]) < priority(words[i][0])
						    || isRightAssociated(words[i][0]) && priority(opTop[0]) <= priority(words[i][0])) {
							insertFirst(opTop, &(*ops));
							break;
						} else {
							insertFirst(opTop, &(*ans));
						}
					}
					insertFirst(words[i], &(*ops));
				}
			}
		}
	}

	while (!isEmpty(&(*ops))) {
		char *opTop = deleteFirst(&(*ops));
		insertFirst(opTop, &(*ans));
	}
}

double calc(Link **first) {
	double stack[SIZE];
	int p = -1;

	while (!isEmpty(first)) {
		char *current = deleteFirst(first);
		if (isdigit(current[0]) || (strlen(current) != 1 && current[0] == '-')) {
			stack[++p] = strtod(current, &PTR);
		} else {
			char sign = current[0];
			switch (sign) {
				case '+':
					stack[p - 1] += stack[p];
					--p;
					break;
				case '-':
					if (p == 0) {
						stack[p] = -stack[p];
					} else {
						stack[p - 1] -= stack[p];
						--p;
					}
					break;
				case '*':
					stack[p - 1] *= stack[p];
					--p;
					break;
				case '/':
					stack[p - 1] /= stack[p];
					--p;
					break;
				case '^':
					stack[p - 1] = pow(stack[p - 1], stack[p]);
					--p;
					break;
				default:
					printf("Invalid symbol");
			}
		}
	}
	return stack[0];
}

double calculate(char *expr) {
	Link *ans = NULL;
	Link *ops = NULL;
	Link *postFix = NULL;

	infixToPostfix(expr, &ans, &ops);
	listReverseCopy(&postFix, &ans);
	//display(&postFix);
	//puts("\n");

	return calc(&postFix);
}

void parser(FILE *file) {
	fgets(expression, LENGTH, file);
	expression[strlen(expression) - 1] = '\0';
	char temp[LENGTH];
	while (!feof(file) && fgets(temp, LENGTH, file)) {
		temp[strlen(temp) - 1] = '\0';
		sscanf(temp, "%s = %s", vars[varsSize].variableName, vars[varsSize].variableValue);
		varsSize++;
	}
}

#include <complex.h>

int main() {
	double n = 2.5;
	double complex z1 = 1 + I;
	double complex z2 = 3.5 - 4 * I;
	double complex conjugate = conj(z1);
	printf("%lf %lf", creal(z1 * z2), cimag(z1 * z2));

	/*FILE* input = fopen("input.txt", "r");
	parser(input);
	*//*char str[SIZE];
    gets(str);*//*

    printf("Variables:\n");
    for (int i = 0; i < varsSize; ++i) {
        printf("%s = %s\n", vars[i].variableName, vars[i].variableValue);
    }
    //test();
    printf("\nExpression: %s = %.15lf", expression, calculate(expression));
    printf("\n\n%lf", cos(3.14/3));
*/

	/*printf("Expression:\n%s\n", str);

	infixToPostfix(str);
	listReverseCopy(&postFix, &ans);

	printf("Reverse Polish Notation:\n");
	display(&postFix);

	int solve = calc(&postFix);
	printf("\nResult:\n%d", solve);*/
}