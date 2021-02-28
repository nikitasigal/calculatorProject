#ifndef CALCULATORPROJECT_GUI_H
#define CALCULATORPROJECT_GUI_H

#define EPS 1e-100

#define VALUE_LENGTH 300

#define NUMBER_LENGTH 50

#include "../calculations.h"
#include "../errorHandler.h"
#include "../stacks/stackVariable.h"
#include "../maps/mapFunctions.h"

void init_GUI();

G_MODULE_EXPORT void on_MainWindow_destroy();

G_MODULE_EXPORT void labelSetValue(GtkWidget *label, char *result);

G_MODULE_EXPORT void on_button_clicked(GtkWidget *button, GtkEntry *entry);

G_MODULE_EXPORT void on_entry_key_press_event(GtkWidget *entry, GdkEventKey *event, gpointer user_data);

G_MODULE_EXPORT void on_TreeButtonDelete_clicked(GtkWidget *button, GtkTreeSelection *row);

G_MODULE_EXPORT void on_TreeButtonAdd_clicked(GtkWidget *button, GtkListStore *list);

G_MODULE_EXPORT void
on_TreeTextFirst_edited(GtkCellRendererText *cell, gchar *path_string, gchar *new_text, gpointer user_data);

G_MODULE_EXPORT void
on_TreeTextSecond_edited(GtkCellRendererText *cell, gchar *path_string, gchar *new_text, gpointer user_data);

G_MODULE_EXPORT void on_calculator_button_clicked_function(GtkWidget *button, GtkEntry *entry);

G_MODULE_EXPORT void on_calculator_button_clicked_sqrt(GtkWidget *button, GtkEntry *entry);

G_MODULE_EXPORT void on_calculator_button_clicked_abs(GtkWidget *button, GtkEntry *entry);

G_MODULE_EXPORT void set_focus_to_entry(GtkWidget *button, GtkEntry *entry);

G_MODULE_EXPORT void on_calculator_button_clicked_move_right(GtkWidget *button, GtkEntry *entry);

G_MODULE_EXPORT void on_calculator_button_clicked_move_left(GtkWidget *button, GtkEntry *entry);

G_MODULE_EXPORT void on_calculator_button_clicked_parentheses(GtkWidget *button, GtkEntry *entry);

G_MODULE_EXPORT void on_calculator_button_clicked_PI(GtkWidget *button, GtkEntry *entry);

G_MODULE_EXPORT void open_preferences(GtkButton *button, GtkWindow *window);

G_MODULE_EXPORT void on_calculator_button_clicked_backspace(GtkWidget *button, GtkEntry *entry);

G_MODULE_EXPORT void on_calculator_button_clicked_div(GtkWidget *button, GtkEntry *entry);

G_MODULE_EXPORT void on_calculator_button_clicked_mult(GtkWidget *button, GtkEntry *entry);

G_MODULE_EXPORT void on_calculator_button_clicked(GtkWidget *button, GtkEntry *entry);

G_MODULE_EXPORT void on_variable_button_toggled(GtkToggleButton *switcher, GtkWidget *tree);

G_MODULE_EXPORT void toggled_button_clicked(GtkToggleButton *button, GList *list);

G_MODULE_EXPORT void change_the_theme(GtkComboBoxText *box);

G_MODULE_EXPORT void Preferences_destroy(GtkWidget *preferences);

G_MODULE_EXPORT void ErrorWindow_destroy(GtkButton *button, GtkWidget *ErrorWindow);

#endif //CALCULATORPROJECT_GUI_H
