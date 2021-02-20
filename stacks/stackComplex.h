#ifndef CALCULATORPROJECT_STACKCOMPLEX_H
#define CALCULATORPROJECT_STACKCOMPLEX_H

#include "../libraries.h"

struct StackComplex {
	complex double value;
	struct StackComplex *next;
};

void pushComplex(struct StackComplex **s, complex double value);

complex double popComplex(struct StackComplex **s);

#endif //CALCULATORPROJECT_STACKCOMPLEX_H