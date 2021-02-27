#include "GUI/GUI.h"

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    init_GUI();
    gtk_main();

    return 0;
}