#ifndef CALCULATORPROJECT_CALCULATIONS_H
#define CALCULATORPROJECT_CALCULATIONS_H

#include "libraries.h"
#include "stacks/stackComplex.h"
#include "stacks/stackVariable.h"
#include "stacks/stackOperator.h"
#include "maps/mapComplex.h"
#include "maps/mapFunctions.h"
#include "errorHandler.h"

void tNegate(struct NodeComplex **s);

void tAdd(struct NodeComplex **s);

void tSubtract(struct NodeComplex **s);

void tMultiply(struct NodeComplex **s);

void tDivide(struct NodeComplex **s);

void tSin(struct NodeComplex **s);

void tCos(struct NodeComplex **s);

void tTg(struct NodeComplex **s);

void tCtg(struct NodeComplex **s);

void tLn(struct NodeComplex **s);

void tLg(struct NodeComplex **s);

void tLog(struct NodeComplex **s);

void tExp(struct NodeComplex **s);

void tSqrt(struct NodeComplex **s);

void tPow(struct NodeComplex **s);

void tAbs(struct NodeComplex **s);

void tReal(struct NodeComplex **s);

void tImag(struct NodeComplex **s);

void tArg(struct NodeComplex **s);

short isOperator(const char op[OPERATION_SIZE]);

int operatorPriority(const char op[OPERATION_SIZE]);

complex long double calculate(struct MapOperations opMap[MAP_SIZE], struct MapComplex varMap[MAP_SIZE], struct Variable var);

void sortVariables(struct NodeVariable **s, struct MapOperations opMap[MAP_SIZE], struct MapComplex varMap[MAP_SIZE]);

void defineOperations(struct MapOperations m[MAP_SIZE]);

void defineConstants(struct MapComplex m[MAP_SIZE]);

#endif //CALCULATORPROJECT_CALCULATIONS_H
