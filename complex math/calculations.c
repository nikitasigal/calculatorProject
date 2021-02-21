#include "calculations.h"
#include <stdbool.h>

void unary(struct StackComplex **s) {
    pushComplex(s, -1 * popComplex(s));
}

void add(struct StackComplex **s) {
    pushComplex(s, popComplex(s) + popComplex(s));
}

void subtract(struct StackComplex **s) {
    complex double right = popComplex(s);
    complex double left = popComplex(s);
    pushComplex(s, left - right);
}

void multiply(struct StackComplex **s) {
    pushComplex(s, popComplex(s) * popComplex(s));
}

void divide(struct StackComplex **s) {
    complex double right = popComplex(s);
    complex double left = popComplex(s);
    pushComplex(s, left / right);
}

void sinus(struct StackComplex **s) {
    pushComplex(s, csin(popComplex(s)));
}

void cosinus(struct StackComplex **s) {
    pushComplex(s, ccos(popComplex(s)));
}

void tangens(struct StackComplex **s) {
    pushComplex(s, ctan(popComplex(s)));
}

void cotangens(struct StackComplex **s) {
    pushComplex(s, 1 / ctan(popComplex(s)));
}

void natlog(struct StackComplex **s) {
    pushComplex(s, clog(popComplex(s)));
}

void tenlog(struct StackComplex **s) {
    pushComplex(s, clog(popComplex(s)) / clog(10));
}

void justlog(struct StackComplex **s) {
    complex double right = popComplex(s);
    complex double left = popComplex(s);
    pushComplex(s, clog(right) / clog(left));
}

void exponent(struct StackComplex **s) {
    pushComplex(s, cexp(popComplex(s)));
}

void root(struct StackComplex **s) {
    pushComplex(s, csqrt(popComplex(s)));
}

void power(struct StackComplex **s) {
    complex double right = popComplex(s);
    complex double left = popComplex(s);
    pushComplex(s, cpow(left, right));
}

void module(struct StackComplex **s) {
    pushComplex(s, cabs(popComplex(s)));
}

void real(struct StackComplex **s) {
    pushComplex(s, creal(popComplex(s)));
}

void imag(struct StackComplex **s) {
    pushComplex(s, cimag(popComplex(s)));
}

void argument(struct StackComplex **s) {
    pushComplex(s, carg(popComplex(s)));
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
