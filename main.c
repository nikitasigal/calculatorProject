#include "libraries.h"
#include "stacks/stackComplex.h"
#include "stacks/stackVariable.h"
#include "stacks/stackOperator.h"
#include "maps/mapComplex.h"
#include "maps/mapFunctions.h"

complex double addition(struct StackComplex *s) {
	return popComplex(&s) + popComplex(&s);
}

int main() {
	struct StackComplex *sComp = NULL;
	pushComplex(&sComp, 15 + 4 * I);
	pushComplex(&sComp, 13);
	printf("%lf\n", creal(popComplex(&sComp)));
	pushComplex(&sComp, 22);


	struct StackVariable *sVar = NULL;
	pushVariable(&sVar, "Abc", "B + C - 12");
	pushVariable(&sVar, "loss", "log(cos(sin(tg(I+10))), pow(exp(3)*ln(2.718281828^2), ctg(sqrt(real(10 - 3 * mag(I + 5 * I - 10))))))");
	forwardVariable(&sVar, sVar->next);
	struct Variable fgf = popVariable(&sVar);
	for (int i = 0; i < fgf.elements; ++i) {
		printf("%s ", fgf.expression[i]);
	}
	printf("\n");


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
	complex double res = mp1[findFunction(mp1, "+")].function(sComp);
	printf("%lf + %lfI", creal(res), cimag(res));


	return 0;
}