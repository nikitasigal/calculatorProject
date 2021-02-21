#ifndef CALCULATORPROJECT_CALCULATIONS_H
#define CALCULATORPROJECT_CALCULATIONS_H

#include "../libraries.h"
#include "../stacks/stackComplex.h"
#include "../stacks/stackVariable.h"
#include "../stacks/stackOperator.h"
#include "../maps/mapComplex.h"
#include "../maps/mapFunctions.h"

void add(struct StackComplex **s);

void subtract(struct StackComplex **s);

void power(struct StackComplex **s);

void unary(struct StackComplex **s);

short isOperator(char op[OPERATOR_SIZE]);

int operatorPriority(char c[OPERATOR_SIZE]);

complex double calculate(struct MapFunctions functions[MAP_SIZE], struct MapComplex variables[MAP_SIZE], struct Variable var);

#endif //CALCULATORPROJECT_CALCULATIONS_H
