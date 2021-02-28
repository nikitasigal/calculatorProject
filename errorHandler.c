#include "errorHandler.h"

G_MODULE_EXPORT void printError(char *str) {
	if(ERROR)
		return;
    gtk_label_set_text(errorLabel, str);
    gtk_widget_show(errorWindow);
    gtk_window_present(GTK_WINDOW(errorWindow));
    ERROR = true;
}