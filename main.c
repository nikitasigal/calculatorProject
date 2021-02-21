#include "libraries.h"
#include "stacks/stackComplex.h"
#include "stacks/stackVariable.h"
#include "stacks/stackOperator.h"
#include "maps/mapComplex.h"
#include "maps/mapFunctions.h"
#include "complex math/calculations.h"

int main() {
	struct StackVariable *sVar = NULL;
	//pushVariable(&sVar, "B", "C - 12");
	pushVariable(&sVar, "css", "ln(sin(cos(ctg(5))))");
	pushVariable(&sVar, "biba", "phase(5)");
    pushVariable(&sVar, "boba", "log(12,120)");

	struct MapFunctions mp1[MAP_SIZE];
	initMapFunctions(mp1);
    insertFunction(mp1, "~", &unary);
    insertFunction(mp1, "+", &add);
    insertFunction(mp1, "-", &subtract);
    insertFunction(mp1, "*", &multiply);
    insertFunction(mp1, "/", &divide);
    insertFunction(mp1, "sin", &sinus);
    insertFunction(mp1, "cos", &cosinus);
    insertFunction(mp1, "tg", &tangens);
    insertFunction(mp1, "ctg", &cotangens);
    insertFunction(mp1, "ln", &natlog);
    insertFunction(mp1, "lg", &tenlog);
    insertFunction(mp1, "log", &justlog);
    insertFunction(mp1, "exp", &exponent);
    insertFunction(mp1, "sqrt", &root);
    insertFunction(mp1, "pow", &power);
    insertFunction(mp1, "^", &power);
    insertFunction(mp1, "abs", &module);
    insertFunction(mp1, "mag", &module);
    insertFunction(mp1, "real", &real);
    insertFunction(mp1, "imag", &imag);
    insertFunction(mp1, "arg", &argument);
    insertFunction(mp1, "phase", &argument);

	struct MapComplex mp2[MAP_SIZE];
	initMapComplex(mp2);
	insertComplex(mp2, "i", 0 + 1 * I);
	insertComplex(mp2, "e", M_E);
	insertComplex(mp2, "PI", M_PI);

	sortVariables(&sVar, mp1);

	struct StackVariable *cur = sVar;
	while (cur != NULL) {
		insertComplex(mp2, cur->var.name, calculate(mp1, mp2, cur->var));
		printf("%s = %lf %lfi\n", cur->var.name, creal(getComplex(mp2, cur->var.name)), cimag(getComplex(mp2, cur->var.name)));
		cur = cur->next;
	}

	return 0;
}