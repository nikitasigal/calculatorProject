#include "calculations.h"
#include <stdbool.h>

void add(struct StackComplex **s) {
	pushComplex(s, popComplex(s) + popComplex(s));
}

void subtract(struct StackComplex **s) {
	pushComplex(s, -1 * popComplex(s) + popComplex(s));
}

void unary(struct StackComplex **s) {
    pushComplex(s, -1 * popComplex(s));
}

short isOperator(char *op) {
	return (short) (op[0] == '+' || op[0] == '-' || op[0] == '*' || op[0] == '/' || op[0] == '^');
}

int operatorPriority(char *c) {
	switch (c[0]) {
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

complex double calculate(struct MapFunctions *functions, struct MapComplex *variables, struct Variable var) {
	struct StackComplex *values = NULL;
	struct StackOperator *operations = NULL;
	bool IsUnary = true;
	for (int i = 0; i < var.elements; ++i) {
		if (var.expression[i][0] == '(') {
            pushOperation(&operations, "(");
            IsUnary = true;
        }
		else if (var.expression[i][0] == ')' || var.expression[i][0] == ',') {
			char temp[OPERATOR_SIZE];
			popOperation(&operations, temp);
			while (temp[0] != '(') {
				functions[findOperation(functions, temp)].function(&values);
				popOperation(&operations, temp);
			}

			if (var.expression[i][0] == ',') {
                pushOperation(&operations, "(");
                IsUnary = true;
            }
			else {
				popOperation(&operations, temp);
				if (!isOperator(temp))
					functions[findOperation(functions, temp)].function(&values);
				else
					pushOperation(&operations, temp);
                IsUnary = false;
			}
		} else if (isdigit(var.expression[i][0])) {
			pushComplex(&values, strtod(var.expression[i], NULL));
			IsUnary = false;
		} else if (isalpha(var.expression[i][0])) {
			unsigned int id = findOperation(functions, var.expression[i]);
			if (id == INT_MAX){
				pushComplex(&values, getComplex(variables, var.expression[i]));
			    IsUnary = false;
			}
			else
				pushOperation(&operations, var.expression[i]);
		} else if (isOperator(var.expression[i])) {
			while (operations && (operations->op[0] != '^' && operatorPriority(var.expression[i]) <= operatorPriority(operations->op) ||
			                      (operations->op[0] == '^' && operatorPriority(var.expression[i]) < operatorPriority(operations->op)))) {
				char temp[OPERATOR_SIZE];
				popOperation(&operations, temp);
				functions[findOperation(functions, temp)].function(&values);
			}
			if (IsUnary && var.expression[i][0] == '-'){
                pushOperation(&operations, "~");
            }
			else pushOperation(&operations, var.expression[i]);
			IsUnary = true;
		}
	}


	while (operations) {
		char temp[OPERATOR_SIZE];
		popOperation(&operations, temp);
		functions[findOperation(functions, temp)].function(&values);
	}

	return values->value;
}

void power(struct StackComplex **s) {
	complex double right = popComplex(s);
	complex double left = popComplex(s);
	pushComplex(s, cpow(left, right));
}
