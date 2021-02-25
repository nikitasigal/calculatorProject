#include "calculations.h"

void tNegate(struct NodeComplex **s) {
	pushComplex(s, -1 * popComplex(s));
}

void tAdd(struct NodeComplex **s) {
	pushComplex(s, popComplex(s) + popComplex(s));
}

void tSubtract(struct NodeComplex **s) {
	complex double right = popComplex(s);
	complex double left = popComplex(s);
	pushComplex(s, left - right);
}

void tMultiply(struct NodeComplex **s) {
	pushComplex(s, popComplex(s) * popComplex(s));
}

void tDivide(struct NodeComplex **s) {
	complex double right = popComplex(s);
	complex double left = popComplex(s);
	pushComplex(s, left / right);
}

void tSin(struct NodeComplex **s) {
	pushComplex(s, csin(popComplex(s)));
}

void tCos(struct NodeComplex **s) {
	pushComplex(s, ccos(popComplex(s)));
}

void tTg(struct NodeComplex **s) {
	pushComplex(s, ctan(popComplex(s)));
}

void tCtg(struct NodeComplex **s) {
	pushComplex(s, 1 / ctan(popComplex(s)));
}

void tLn(struct NodeComplex **s) {
	pushComplex(s, clog(popComplex(s)));
}

void tLg(struct NodeComplex **s) {
	pushComplex(s, clog(popComplex(s)) / clog(10));
}

void tLog(struct NodeComplex **s) {
	complex double right = popComplex(s);
	complex double left = popComplex(s);
	pushComplex(s, clog(right) / clog(left));
}

void tExp(struct NodeComplex **s) {
	pushComplex(s, cexp(popComplex(s)));
}

void tSqrt(struct NodeComplex **s) {
	pushComplex(s, csqrt(popComplex(s)));
}

void tPow(struct NodeComplex **s) {
	complex double right = popComplex(s);
	complex double left = popComplex(s);
	pushComplex(s, cpow(left, right));
}

void tAbs(struct NodeComplex **s) {
	pushComplex(s, cabs(popComplex(s)));
}

void tReal(struct NodeComplex **s) {
	pushComplex(s, creal(popComplex(s)));
}

void tImag(struct NodeComplex **s) {
	pushComplex(s, cimag(popComplex(s)));
}

void tArg(struct NodeComplex **s) {
	pushComplex(s, carg(popComplex(s)));
}

short isOperator(char *op) {
	return (short) (op[0] == '+' || op[0] == '-' || op[0] == '*' || op[0] == '/' || op[0] == '^');
}

int operatorPriority(char *op) {
	switch (op[0]) {
		case '+':
		case '-':
			return 1;
		case '*':
		case '/':
			return 2;
		case '^':
			return 10;
		case '~':
			return 100;
		default:
			return 0;
	}
}

complex double calculate(struct MapOperations opMap[MAP_SIZE], struct MapComplex varMap[MAP_SIZE], struct Variable var) {
	struct NodeComplex *valuesStack = NULL;
	struct NodeOperation *opStack = NULL;

	bool IsUnary = true;
	for (int i = 0; i < var.elements; ++i) {
		if (var.expression[i][0] == '(') {
			pushOperation(&opStack, "(");
			IsUnary = true;
		} else if (var.expression[i][0] == ')' || var.expression[i][0] == ',') {
			char temp[OPERATOR_SIZE];
			popOperation(&opStack, temp);
			while (temp[0] != '(') {
				opMap[findOperation(opMap, temp)].operation(&valuesStack);
				popOperation(&opStack, temp);
			}

			if (var.expression[i][0] == ',') {
				pushOperation(&opStack, "(");
				IsUnary = true;
			} else {
				popOperation(&opStack, temp);
				if (!isOperator(temp))
					opMap[findOperation(opMap, temp)].operation(&valuesStack);
				else
					pushOperation(&opStack, temp);
				IsUnary = false;
			}
		} else if (isdigit(var.expression[i][0])) {
			pushComplex(&valuesStack, strtod(var.expression[i], NULL));
			IsUnary = false;
		} else if (isalpha(var.expression[i][0])) {
			unsigned int id = findOperation(opMap, var.expression[i]);
			if (id == INT_MAX) {
				pushComplex(&valuesStack, varMap[findComplex(varMap, var.expression[i])].value);
				IsUnary = false;
			} else
				pushOperation(&opStack, var.expression[i]);
		} else if (isOperator(var.expression[i])) {
			while (opStack && (opStack->operation[0] != '^' && operatorPriority(var.expression[i]) <= operatorPriority(opStack->operation) ||
			                   (opStack->operation[0] == '^' && operatorPriority(var.expression[i]) < operatorPriority(opStack->operation)))) {
				char temp[OPERATOR_SIZE];
				popOperation(&opStack, temp);
				opMap[findOperation(opMap, temp)].operation(&valuesStack);
			}

			if (IsUnary && var.expression[i][0] == '-') {
				pushOperation(&opStack, "~");
			} else pushOperation(&opStack, var.expression[i]);
			IsUnary = true;
		}
	}


	while (opStack) {
		char temp[OPERATOR_SIZE];
		popOperation(&opStack, temp);
		opMap[findOperation(opMap, temp)].operation(&valuesStack);
	}

	return valuesStack->value;
}

void sortVariables(struct NodeVariable **s, struct MapOperations opMap[MAP_SIZE], struct MapComplex varMap[MAP_SIZE]) {
	struct NodeVariable *cur = (*s);
	while (cur != NULL) {
		if (!cur->variable.isSorted) {
			for (int j = 0; j < (cur->variable.elements); j++) {
				if (findOperation(opMap, cur->variable.expression[j]) == INT_MAX &&
				    findComplex(varMap, cur->variable.expression[j]) == INT_MAX &&
				    isalpha(cur->variable.expression[j][0])) {

					struct NodeVariable *temp = (*s);
					while (temp) {
						if (!strcmp(temp->variable.name, cur->variable.expression[j])) {
							forwardVariable(s, temp);
							break;
						}
						temp = temp->next;
					}

					if (temp == NULL) {
						printf("Sort() did not find the variable; go fuck yourself!\n");
						exit(EXIT_FAILURE);
					}
				}
			}
			cur->variable.isSorted = 1;
			cur = (*s);
		} else
			cur = cur->next;
	}
}

void defineOperations(struct MapOperations m[MAP_SIZE]) {
	insertOperation(m, "~", &tNegate);
	insertOperation(m, "+", &tAdd);
	insertOperation(m, "-", &tSubtract);
	insertOperation(m, "*", &tMultiply);
	insertOperation(m, "/", &tDivide);
	insertOperation(m, "sin", &tSin);
	insertOperation(m, "cos", &tCos);
	insertOperation(m, "tg", &tTg);
	insertOperation(m, "ctg", &tCtg);
	insertOperation(m, "ln", &tLn);
	insertOperation(m, "lg", &tLg);
	insertOperation(m, "log", &tLog);
	insertOperation(m, "exp", &tExp);
	insertOperation(m, "sqrt", &tSqrt);
	insertOperation(m, "pow", &tPow);
	insertOperation(m, "^", &tPow);
	insertOperation(m, "abs", &tAbs);
	insertOperation(m, "mag", &tAbs);
	insertOperation(m, "tReal", &tReal);
	insertOperation(m, "tImag", &tImag);
	insertOperation(m, "arg", &tArg);
	insertOperation(m, "phase", &tArg);
}

void defineConstants(struct MapComplex m[MAP_SIZE]) {
	insertComplex(m, "i", 0 + 1 * I);
	insertComplex(m, "e", M_E);
	insertComplex(m, "PI", M_PI);
}