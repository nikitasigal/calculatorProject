#include "calculations.h"

void tNegate(struct NodeComplex **s) {
    pushComplex(s, -1 * popComplex(s));
}

void tAdd(struct NodeComplex **s) {
    pushComplex(s, popComplex(s) + popComplex(s));
}

void tSubtract(struct NodeComplex **s) {
    complex long double right = popComplex(s);
    complex long double left = popComplex(s);
    pushComplex(s, left - right);
}

void tMultiply(struct NodeComplex **s) {
    pushComplex(s, popComplex(s) * popComplex(s));
}

void tDivide(struct NodeComplex **s) {
    complex long double right = popComplex(s);
    complex long double left = popComplex(s);
    pushComplex(s, left / right);
}

void tSin(struct NodeComplex **s) {
    complex long double number = popComplex(s);
    if ((int) ((number) / (M_PI)) - (number) / (M_PI) == 0) {
        pushComplex(s, 0);
    } else {
        pushComplex(s, csin(number));
    }
}

void tCos(struct NodeComplex **s) {
    complex long double number = popComplex(s);
    if ((int) ((number + M_PI_2) / (M_PI)) - (number + M_PI_2) / (M_PI) == 0) {
        pushComplex(s, 0);
    } else {
        pushComplex(s, ccos(number));
    }
}

void tTg(struct NodeComplex **s) {
    complex long double number = popComplex(s);
    if ((int) ((number + M_PI_2) / (M_PI)) - (number + M_PI_2) / (M_PI) == 0) {
        pushComplex(s, INFINITY);
    } else if ((int) ((number) / (M_PI)) - (number) / (M_PI) == 0) {
        pushComplex(s, 0);
    } else {
        pushComplex(s, ctan(number));
    }
}

void tCtg(struct NodeComplex **s) {
    complex double number = popComplex(s);
    if ((int) ((number) / (M_PI)) - (number) / (M_PI) == 0) {
        pushComplex(s, INFINITY);
    } else if ((int) ((number + M_PI_2) / (M_PI)) - (number + M_PI_2) / (M_PI) == 0) {
        pushComplex(s, 0);
    } else {
        pushComplex(s, 1 / ctan(number));
    }
}

void tLn(struct NodeComplex **s) {
    pushComplex(s, clog(popComplex(s)));
}

void tLg(struct NodeComplex **s) {
    pushComplex(s, clog(popComplex(s)) / clog(10));
}

void tLog(struct NodeComplex **s) {
    complex long double right = popComplex(s);
    complex long double left = popComplex(s);
    pushComplex(s, clog(right) / clog(left));
}

void tExp(struct NodeComplex **s) {
    pushComplex(s, cexp(popComplex(s)));
}

void tSqrt(struct NodeComplex **s) {
    pushComplex(s, csqrt(popComplex(s)));
}

void tPow(struct NodeComplex **s) {
    complex long double right = popComplex(s);
    complex long double left = popComplex(s);
    if (cimag(right) == 0 && cimag(left) == 0) {
        long double realResult = powl(creal(left), creal(right));
        complex long double complexResult = realResult;
        pushComplex(s, complexResult);
    } else {
        pushComplex(s, cpow(left, right));
    }
}

void tRoot(struct NodeComplex **s) {
    complex long double right = popComplex(s);
    complex long double left = popComplex(s);
    pushComplex(s, cpow(right, 1 / left));
}

void tAbs(struct NodeComplex **s) {
    pushComplex(s, cabsl(popComplex(s)));
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

short isOperator(const char *op) {
    return (short) (op[0] == '+' || op[0] == '-' || op[0] == '*' || op[0] == '/' || op[0] == '^');
}

int operatorPriority(const char *op) {
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
            return 5;
        default:
            return 0;
    }
}

complex long double
calculate(struct MapOperations opMap[MAP_SIZE], struct MapComplex varMap[MAP_SIZE], struct Variable var) {
    struct NodeComplex *valuesStack = NULL;
    struct NodeOperation *opStack = NULL;

    bool IsUnary = true;
    for (int i = 0; i < var.elements; ++i) {
        if (var.expression[i][0] == '(') {
            pushOperation(&opStack, "(");
            IsUnary = true;
        } else if (var.expression[i][0] == ')' || var.expression[i][0] == ',') {
            char temp[OPERATION_SIZE] = {0};
            while (opStack && opStack->operation[0] != '(') {
                popOperation(&opStack, temp);
                opMap[findOperation(opMap, temp)].operation(&valuesStack);
            }

            if (!opStack) {
                printError("Incorrect bracket sequence");
                continue;
            }

            if (var.expression[i][0] == ',') {
                IsUnary = true;
            } else {
                popOperation(&opStack, temp);
                if (opStack && !isOperator(opStack->operation) && opStack->operation[0] != '(') {
                    popOperation(&opStack, temp);
                    opMap[findOperation(opMap, temp)].operation(&valuesStack);
                }
                IsUnary = false;
            }
        } else if (isdigit(var.expression[i][0])) {
            pushComplex(&valuesStack, g_strtod(var.expression[i], NULL));
            IsUnary = false;
        } else if (isalpha(var.expression[i][0])) {
            unsigned int id = findOperation(opMap, var.expression[i]);
            if (id == INT_MAX) {
                id = findVariable(varMap, var.expression[i]);
                if (id != INT_MAX) {
                    pushComplex(&valuesStack, varMap[id].value);
                    IsUnary = false;
                } else {
                    char msg[ERROR_SIZE] = {0};
                    sprintf(msg, "'%s' is not defined", var.expression[i]);
                    printError(msg);
                    pushComplex(&valuesStack, NAN);
                }
            } else
                pushOperation(&opStack, var.expression[i]);
        } else if (isOperator(var.expression[i])) {
            while (opStack && (opStack->operation[0] != '^' &&
                               operatorPriority(var.expression[i]) <= operatorPriority(opStack->operation) ||
                               (opStack->operation[0] == '^' &&
                                operatorPriority(var.expression[i]) < operatorPriority(opStack->operation)))) {
                char temp[OPERATION_SIZE];
                popOperation(&opStack, temp);
                opMap[findOperation(opMap, temp)].operation(&valuesStack);
            }

            if (IsUnary && var.expression[i][0] == '-') {
                pushOperation(&opStack, "~");
            } else pushOperation(&opStack, var.expression[i]);
            IsUnary = false;
        } else {
            char msg[ERROR_SIZE] = {0};
            sprintf(msg, "'%s' is not defined", var.expression[i]);
            printError(msg);
        }
    }


    while (opStack) {
        char temp[OPERATION_SIZE];
        popOperation(&opStack, temp);
        unsigned id = findOperation(opMap, temp);
        if (id != INT_MAX)
            opMap[id].operation(&valuesStack);
        else
            printError("Incorrect bracket sequence");
    }

    if (!ERROR && valuesStack)
        if (valuesStack->next) {
            printError("Missing operation between operands");
            return NAN;
        } else
            return valuesStack->value;
    else
        return NAN;
}

void sortVariables(struct NodeVariable **s, struct MapOperations opMap[MAP_SIZE], struct MapComplex varMap[MAP_SIZE]) {
    struct NodeVariable *cur = (*s);
    while (cur != NULL) {
        if (!cur->variable.isSorted) {
            for (int j = 0; j < (cur->variable.elements); j++) {
                if (findOperation(opMap, cur->variable.expression[j]) == INT_MAX &&
                    findVariable(varMap, cur->variable.expression[j]) == INT_MAX &&
                    isalpha(cur->variable.expression[j][0])) {

                    struct NodeVariable *temp = (*s);
                    while (temp) {
                        if (!strcmp(temp->variable.name, cur->variable.expression[j])) {
                        	if(!temp->variable.isSorted)
                                forwardVariable(s, cur, temp);
                            break;
                        }
                        temp = temp->next;
                    }

                    if (temp == NULL) {
                        char msg[ERROR_SIZE] = {0};
                        sprintf(msg, "'%s' is not defined", cur->variable.expression[j]);
                        printError(msg);
                    }
                }
            }
            cur->variable.isSorted = true;
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
    insertOperation(m, "root", &tRoot);
    insertOperation(m, "^", &tPow);
    insertOperation(m, "abs", &tAbs);
    insertOperation(m, "mag", &tAbs);
    insertOperation(m, "real", &tReal);
    insertOperation(m, "imag", &tImag);
    insertOperation(m, "arg", &tArg);
    insertOperation(m, "phase", &tArg);
}

void defineConstants(struct MapComplex m[MAP_SIZE]) {
    insertVariable(m, "i", 0 + 1 * I);
    insertVariable(m, "e", M_E);
    insertVariable(m, "PI", M_PI);
}