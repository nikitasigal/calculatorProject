#include "libraries.h"
#include "stacks/stackComplex.h"
#include "stacks/stackVariable.h"
#include "stacks/stackOperator.h"
#include "maps/mapComplex.h"
#include "maps/mapFunctions.h"

complex double addition(struct StackComplex *s) {
	return popComplex(&s) + popComplex(&s);
}

complex double subtraction(struct StackComplex *s) {
	return -popComplex(&s) + popComplex(&s);
}


int main() {
	struct StackComplex *sComp = NULL;
	pushComplex(&sComp, 15 + 4 * I);
	pushComplex(&sComp, 13);
	printf("%lf\n", creal(popComplex(&sComp)));
	pushComplex(&sComp, 22);


	struct StackVariable *sVar = NULL;
	pushVariable(&sVar, "B", "C-12");
	pushVariable(&sVar, "C", "23");
	pushVariable(&sVar, "Biba", "B + C - 12");
	//forwardVariable(&sVar, sVar->next);
	//struct Variable fgf = popVariable(&sVar);
	//for (int i = 0; i < fgf.elements; ++i) {
	//	printf("%s ", fgf.expression[i]);
	//}
	//printf("\n");


	struct StackOperator *sOp = NULL;
	pushOperator(&sOp, "+");
	pushOperator(&sOp, "pow");
	char temp[300];
	popOperator(&sOp, temp);
	printf("%s\n", temp);
	popOperator(&sOp, temp);
	printf("%s\n", temp);


	struct MapComplex mpComp[MAP_SIZE];
	initMapComplex(mpComp);
	insertComplex(mpComp, "abc", 345 - 4 * I);
	printf("%lf + %lfI\n", creal(getComplex(mpComp, "abc")), cimag(getComplex(mpComp, "abc")));


	struct MapFunctions mp1[MAP_SIZE];
	initMapFunctions(mp1);
	insertFunction(mp1, "+", &addition);
	insertFunction(mp1, "-", &subtraction);
	complex double res = mp1[findFunction(mp1, "+")].function(sComp);
	printf("%lf + %lfI\n", creal(res), cimag(res));



	sortVariables(&sVar, mp1);

	struct StackVariable *cur = sVar;
	cur = sVar;
	while (cur != NULL) {
		printf("%s\n", cur->var.name);
		cur = cur->next;
	}

	return 0;
}