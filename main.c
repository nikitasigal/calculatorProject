#include "libraries.h"
#include "calculations.h"
#include "GUI/GUI.h"

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    init_GUI();

    /*struct MapOperations mp1[MAP_SIZE];
    initMapOperations(mp1);
    defineOperations(mp1);

    struct MapComplex mp2[MAP_SIZE];
    initMapComplex(mp2);
    defineConstants(mp2);*/

    /*struct NodeVariable *sVar = NULL;
    pushVariable(&sVar, "a", "12+13");
    pushVariable(&sVar, "b", "a*3");

    pushVariable(&sVar, "css", "ln(sin(cos(ctg(boba))))");
    pushVariable(&sVar, "biba", "phase(css*boba + 3*i)");
    pushVariable(&sVar, "boba", "log(e, 120)");

    struct MapOperations mp1[MAP_SIZE];
    initMapOperations(mp1);
    defineOperations(mp1);

    struct MapComplex mp2[MAP_SIZE];
    initMapComplex(mp2);
    defineConstants(mp2);

    sortVariables(&sVar, mp1, mp2);

    struct NodeVariable *cur = sVar;
    while (cur != NULL) {
        insertComplex(mp2, cur->variable.name, calculate(mp1, mp2, cur->variable));
        complex double result = mp2[findComplex(mp2, cur->variable.name)].value;
        printf("%s = %lf %lfi\n", cur->variable.name, creal(result), cimag(result));
        cur = cur->next;
    }*/

    gtk_main();
    //return 0;
}