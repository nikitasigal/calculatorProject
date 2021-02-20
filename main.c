#include "libraries.h"
#include "stacks/stackComplex.h"
#include "stacks/stackVariable.h"
#include "stacks/stackOperator.h"

int main() {
	struct StackComplex *sComp = NULL;
	pushComplex(&sComp, 15 + 4 * I);
	pushComplex(&sComp, 13);
	printf("%lf\n", creal(popComplex(&sComp)));
	pushComplex(&sComp, 22);
	printf("%lf\n", creal(popComplex(&sComp)));
	printf("%lf\n", creal(popComplex(&sComp)));


	struct StackVariable *sVar = NULL;
	pushVariable(&sVar, "Abc", "B + C - 12");
	printf("%s\n", popVariable(&sVar).name);


	struct StackOperator *sOp = NULL;
	pushOperator(&sOp, "+");
	pushOperator(&sOp, "pow");
	char temp[300];
	popOperator(&sOp, temp);
	printf("%s\n", temp);
	popOperator(&sOp, temp);
	printf("%s\n", temp);
	return 0;
}