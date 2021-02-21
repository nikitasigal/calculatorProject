#include "libraries.h"
#include "stacks/stackComplex.h"
#include "stacks/stackVariable.h"
#include "stacks/stackOperator.h"
#include "maps/mapComplex.h"
#include "maps/mapFunctions.h"
#include "complex math/calculations.h"

int main() {
	struct StackVariable *sVar = NULL;
	pushVariable(&sVar, "B", "C - 12");
	pushVariable(&sVar, "C", "23");
	pushVariable(&sVar, "Biba", "B + pow(C, B - 9)");

	struct MapFunctions mp1[MAP_SIZE];
	initMapFunctions(mp1);
	insertFunction(mp1, "+", &add);
	insertFunction(mp1, "-", &subtract);
	insertFunction(mp1, "pow", &power);
	insertFunction(mp1, "^", &power);

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