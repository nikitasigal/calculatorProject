#ifndef CALCULATORPROJECT_STACKCOMPLEX_H
#define CALCULATORPROJECT_STACKCOMPLEX_H

#include "../libraries.h"

struct NodeComplex {
	complex long double value;
	struct NodeComplex *next;
};

void pushComplex(struct NodeComplex **s, complex long double value);

complex long double popComplex(struct NodeComplex **s);

#endif //CALCULATORPROJECT_STACKCOMPLEX_H