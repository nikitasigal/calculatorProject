#ifndef CALCULATORPROJECT_ERRORHANDLER_H
#define CALCULATORPROJECT_ERRORHANDLER_H

#include "gtk/gtk.h"
#include <stdbool.h>

extern GtkLabel *errorLabel;
extern GtkWidget *errorWindow;
extern bool ERROR;

G_MODULE_EXPORT void printError(char *str);

#endif //CALCULATORPROJECT_ERRORHANDLER_H
