#include "GUI.h"
#include "../calculations.h"

#include <string.h>

GtkWidget *resultLabel;
GtkListStore *listStore;
GtkWidget *errorMessage;
GtkLabel *ErrorLabel;
GtkCssProvider *css;
GdkScreen *screen;

void init_GUI() {
    GtkBuilder *builder = gtk_builder_new_from_file("GUI/Glade/glade.glade");
    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, "MainWindow"));
    listStore = GTK_LIST_STORE(gtk_builder_get_object(builder, "liststore1"));
    resultLabel = GTK_WIDGET(gtk_builder_get_object(builder, "labelResult"));
    //GtkEntry *entry = GTK_ENTRY(gtk_builder_get_object(builder, "entry"));
    //GtkWidget *stack = GTK_WIDGET(gtk_builder_get_object(builder, "TreeBox"));
    GtkToggleButton *toggleButton = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "toggle_button"));
    //GtkTreeView *tree = GTK_TREE_VIEW(gtk_builder_get_object(builder, "Tree"));
    ErrorLabel = GTK_LABEL(gtk_builder_get_object(builder, "ErrorLabel"));
    errorMessage = GTK_WIDGET(gtk_builder_get_object(builder, "ErrorWindow"));

    GList *list = NULL;
    list = g_list_append(list, GTK_LABEL(gtk_builder_get_object(builder, "button_label_sin")));
    list = g_list_append(list, GTK_LABEL(gtk_builder_get_object(builder, "button_label_cos")));
    list = g_list_append(list, GTK_LABEL(gtk_builder_get_object(builder, "button_label_tg")));
    list = g_list_append(list, GTK_LABEL(gtk_builder_get_object(builder, "button_label_ctg")));
    list = g_list_append(list, GTK_LABEL(gtk_builder_get_object(builder, "button_label_log")));

    /*GtkTreeModel *model = gtk_tree_view_get_model(tree);
    GtkTreeIter *iter = NULL;
    gtk_tree_model_get_iter_first(model, iter);*/


    screen = gdk_screen_get_default();
    css = gtk_css_provider_new();
    gtk_css_provider_load_from_path(css, "GUI/Themes/WhiteTheme.css", NULL);
    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(css), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(css);

    /*PangoAttrList *pangoList =  pango_attr_list_new();
    PangoFontDescription *font = pango_font_description_new();
    pango_font_description_set_family(font, "Sans");
    pango_font_description_set_size(font, 18 * PANGO_SCALE);
    PangoAttribute *fontAttribute = pango_attr_font_desc_new(font);
    pango_attr_list_insert(pangoList, fontAttribute);
    gtk_entry_set_attributes(entry, pangoList);
    pango_attr_list_unref(pangoList);*/

    gtk_builder_connect_signals(builder, NULL);
    g_signal_connect(toggleButton, "clicked", G_CALLBACK(toggled_button_clicked), list);

    gtk_widget_show_all(window);
    //gtk_widget_hide(stack);

    g_object_unref(builder);
}

G_MODULE_EXPORT void on_MainWindow_destroy() {
    gtk_main_quit();
}

G_MODULE_EXPORT void labelSetValue(GtkWidget *label, char *result) {
    gtk_label_set_text(GTK_LABEL(label), result);
}

G_MODULE_EXPORT void on_button_clicked(GtkWidget *button, GtkEntry *entry) {
    struct NodeVariable *sVar = NULL;
    //test(&sVar, GTK_TREE_MODEL(listStore));
    struct MapOperations mp1[MAP_SIZE];
    initMapOperations(mp1);
    defineOperations(mp1);

    struct MapComplex mp2[MAP_SIZE];
    initMapComplex(mp2);
    defineConstants(mp2);

    GtkTreeIter iter;
    GtkTreeModel *treeModel = GTK_TREE_MODEL(listStore);
    gboolean valid = gtk_tree_model_get_iter_first(treeModel, &iter);
    while (valid) {
        gchar *str1 = malloc(sizeof(gchar) * 10);
        gchar *str2 = malloc(sizeof(gchar) * 10);

        gtk_tree_model_get(treeModel, &iter, 0, &str1, 1, &str2, -1);
        if (!str1 || !str2) {
            error_message("Incorrect variable name or value");
            return;
        }


        //printf("%s = %s", str1, str2);

        valid = gtk_tree_model_iter_next(treeModel, &iter);

        pushVariable(&sVar, str1, str2);

        g_free(str1);
        g_free(str2);
    }
    char expression[200] = { 0 };
    strcpy(expression, gtk_entry_get_text(entry));
    pushVariable(&sVar, "", expression);
    struct NodeVariable *result = sVar;

    sortVariables(&sVar, mp1, mp2);

    struct NodeVariable *cur = sVar;
    while (cur != NULL) {
        insertComplex(mp2, cur->variable.name, calculate(mp1, mp2, cur->variable));
        complex double resultVar = mp2[findComplex(mp2, cur->variable.name)].value;
        printf("%s = %lf %lfi\n", cur->variable.name, creal(resultVar), cimag(resultVar));
        cur = cur->next;
    }

    complex double resultValue = mp2[findComplex(mp2, result->variable.name)].value;
    char resultAsString[200] = { 0 };
    char realPart[50] = { 0 };
    char imagPart[50] = { 0 };
    
    /*if (fabs(creal(resultValue)) > EPS) {
        strcat(resultAsString, g_ascii_dtostr(realPart, 50, creal(resultValue)));
        if (fabs(fabs(cimag(resultValue)) - 1.0) < EPS) {
            if (cimag(resultValue) < 0) {
                strcat(resultAsString, " - i");
            } else {
                strcat(resultAsString, " + i");
            }
        } else {
            if (fabs(cimag(resultValue)) > EPS) {
                if (cimag(resultValue) < 0) {
                    strcat(resultAsString, " - ");
                } else {
                    strcat(resultAsString, " + ");
                }
                strcat(resultAsString, g_ascii_dtostr(imagPart, 50, cimag(resultValue)));
                strcat(resultAsString, "i");
            }
        }
    } else {
        if (fabs(fabs(cimag(resultValue)) - 1.0) < EPS) {
            if (cimag(resultValue) < 0) {
                strcat(resultAsString, " -i");
            } else {
                strcat(resultAsString, "i");
            }
        } else {
            strcat(resultAsString, g_ascii_dtostr(imagPart, 50, cimag(resultValue)));
            strcat(resultAsString, "i");
        }
    }*/

    if (fabs(creal(resultValue)) > EPS) {
        strcat(resultAsString, g_ascii_dtostr(realPart, 50, creal(resultValue)));
        if (fabs(fabs(cimag(resultValue)) - 1.0) < EPS) {
            if (cimag(resultValue) < 0) {
                strcat(resultAsString, " - i");
            } else {
                strcat(resultAsString, " + i");
            }
        } else {
            if (fabs(cimag(resultValue)) > EPS) {
                if (cimag(resultValue) < 0) {
                    strcat(resultAsString, " - ");
                } else {
                    strcat(resultAsString, " + ");
                }
                strcat(resultAsString, g_ascii_dtostr(imagPart, 50, fabs(cimag(resultValue))));
                strcat(resultAsString, "i");
            }
        }
    } else {
        if (fabs(cimag(resultValue)) > EPS) {
            if (fabs(fabs(cimag(resultValue)) - 1.0) < EPS) {
                if (cimag(resultValue) < 0) {
                    strcat(resultAsString, " -i");
                } else {
                    strcat(resultAsString, "i");
                }
            } else {
                strcat(resultAsString, g_ascii_dtostr(imagPart, 50, cimag(resultValue)));
                strcat(resultAsString, "i");
            }
        } else {
            strcat(resultAsString, "0");
        }
    }


    gtk_entry_set_text(entry, "");
    gtk_label_set_text(GTK_LABEL(resultLabel), resultAsString);

    /*char exp[200] = { 0 };
    strcpy(exp, gtk_entry_get_text(entry));
    double res = 1; //calculate(exp)
    char resString[100] = { 0 };
    sprintf(resString, "%.6lf", res);
    gtk_entry_set_text(entry, "");
    gtk_label_set_text(GTK_LABEL(resultLabel), resString);*/
}

G_MODULE_EXPORT void on_entry_key_press_event(GtkWidget *entry, GdkEventKey *event, gpointer user_data) {
    if (event->keyval == GDK_KEY_Return) {
        on_button_clicked(NULL, GTK_ENTRY(entry));
    }
}

G_MODULE_EXPORT void on_TreeButtonDelete_clicked(GtkWidget *button, GtkTreeSelection *row) {
    GtkTreeIter iter;
    if (gtk_tree_selection_get_selected(row, NULL, &iter)) {
        gtk_list_store_remove(listStore, &iter);
    }
}

G_MODULE_EXPORT void on_TreeButtonAdd_clicked(GtkWidget *button, GtkListStore *list) {
    GtkTreeIter iter;
    gtk_list_store_append(list, &iter);
}

G_MODULE_EXPORT void on_TreeTextFirst_edited(GtkCellRendererText *cell, gchar *path_string, gchar *new_text, gpointer user_data) {
    GtkTreeIter iter;
    gtk_tree_model_get_iter_from_string(gtk_tree_view_get_model(user_data), &iter, path_string);
    gtk_list_store_set(listStore, &iter, 0, new_text, -1);
}

G_MODULE_EXPORT void on_TreeTextSecond_edited(GtkCellRendererText *cell, gchar *path_string, gchar *new_text, gpointer user_data) {
    GtkTreeIter iter;
    gtk_tree_model_get_iter_from_string(gtk_tree_view_get_model(user_data), &iter, path_string);
    gtk_list_store_set(listStore, &iter, 1, new_text, -1);
}


G_MODULE_EXPORT void on_calculator_button_clicked_function (GtkWidget *button, GtkEntry *entry) {
    gint position = gtk_editable_get_position(GTK_EDITABLE(entry));
    GList *list = gtk_container_get_children(GTK_CONTAINER(button));
    char temp[10] =  { 0 };
    strcpy(temp, gtk_label_get_text(GTK_LABEL(list->data)));
    strcat(temp, "()");
    gtk_editable_insert_text(GTK_EDITABLE(entry), temp, sizeof(gchar) * strlen(temp), &position);
    gtk_editable_set_position(GTK_EDITABLE(entry), position - 1);
}

G_MODULE_EXPORT void on_calculator_button_clicked_sqrt (GtkWidget *button, GtkEntry *entry) {
    gint position = gtk_editable_get_position(GTK_EDITABLE(entry));
    gtk_editable_insert_text(GTK_EDITABLE(entry), "sqrt()", sizeof(gchar) * 6, &position);
    gtk_editable_set_position(GTK_EDITABLE(entry), position - 1);
}

G_MODULE_EXPORT void on_calculator_button_clicked_abs (GtkWidget *button, GtkEntry *entry) {
    gint position = gtk_editable_get_position(GTK_EDITABLE(entry));
    gtk_editable_insert_text(GTK_EDITABLE(entry), "abs()", sizeof(gchar) * 5, &position);
    gtk_editable_set_position(GTK_EDITABLE(entry), position - 1);
}

G_MODULE_EXPORT void set_focus_to_entry(GtkWidget *button, GtkEntry *entry) {
    gtk_entry_grab_focus_without_selecting(entry);
}

G_MODULE_EXPORT void on_calculator_button_clicked_move_right(GtkWidget *button, GtkEntry *entry) {
    gint position = gtk_editable_get_position(GTK_EDITABLE(entry));
    gtk_editable_set_position(GTK_EDITABLE(entry), position + 1);
}

G_MODULE_EXPORT void on_calculator_button_clicked_move_left(GtkWidget *button, GtkEntry *entry) {
    gint position = gtk_editable_get_position(GTK_EDITABLE(entry));
    gtk_editable_set_position(GTK_EDITABLE(entry), position - 1);
}

G_MODULE_EXPORT void on_calculator_button_clicked_parentheses(GtkWidget *button, GtkEntry *entry) {
    gint position = gtk_editable_get_position(GTK_EDITABLE(entry));
    gint start_pos = 0, end_pos = 0;
    if (gtk_editable_get_selection_bounds(GTK_EDITABLE(entry), &start_pos, &end_pos)) {
        end_pos++;
        gtk_editable_insert_text(GTK_EDITABLE(entry), "(", sizeof(gchar), &start_pos);
        gtk_editable_insert_text(GTK_EDITABLE(entry), ")", sizeof(gchar), &end_pos);
        gtk_editable_select_region(GTK_EDITABLE(entry), start_pos, end_pos - 1);
    } else {
        gtk_editable_insert_text(GTK_EDITABLE(entry), "()", sizeof(gchar) * 2, &position);
        gtk_editable_set_position(GTK_EDITABLE(entry), position - 1);
    }
}

G_MODULE_EXPORT void on_calculator_button_clicked_PI(GtkWidget *button, GtkEntry *entry) {
    gint position = gtk_editable_get_position(GTK_EDITABLE(entry));
    gtk_editable_insert_text(GTK_EDITABLE(entry), "PI", sizeof(gchar) * 2, &position);
    gtk_editable_set_position(GTK_EDITABLE(entry), position);
}

G_MODULE_EXPORT void open_preferences(GtkButton *button, GtkWindow *window) {
    gtk_widget_show(GTK_WIDGET(window));
    gtk_window_present(window);
}

G_MODULE_EXPORT void error_message(char * str) {
    gtk_label_set_text(ErrorLabel, str);
    gtk_widget_show(errorMessage);
    gtk_window_present(GTK_WINDOW(errorMessage));
}

G_MODULE_EXPORT void on_calculator_button_clicked_backspace(GtkWidget *button, GtkEntry *entry) {
    gint position = gtk_editable_get_position(GTK_EDITABLE(entry));
    if (gtk_editable_get_selection_bounds(GTK_EDITABLE(entry), NULL, NULL)) {
        gtk_editable_delete_selection(GTK_EDITABLE(entry));
    } else {
        gtk_editable_delete_text(GTK_EDITABLE(entry), position - 1, position);
    }
    error_message("Test error message");
}

G_MODULE_EXPORT void on_calculator_button_clicked_div(GtkWidget *button, GtkEntry *entry) {
    gint position = gtk_editable_get_position(GTK_EDITABLE(entry));
    gtk_editable_insert_text(GTK_EDITABLE(entry), "/", sizeof(gchar), &position);
    gtk_editable_set_position(GTK_EDITABLE(entry), position);
}

G_MODULE_EXPORT void on_calculator_button_clicked_mult(GtkWidget *button, GtkEntry *entry) {
    gint position = gtk_editable_get_position(GTK_EDITABLE(entry));
    gtk_editable_insert_text(GTK_EDITABLE(entry), "*", sizeof(gchar), &position);
    gtk_editable_set_position(GTK_EDITABLE(entry), position);
}

G_MODULE_EXPORT void on_calculator_button_clicked(GtkWidget *button, GtkEntry *entry) {
    GList *list = gtk_container_get_children(GTK_CONTAINER(button));
    gint position = gtk_editable_get_position(GTK_EDITABLE(entry));

    gtk_editable_insert_text(GTK_EDITABLE(entry), gtk_label_get_text(list->data), sizeof(gchar) * strlen(gtk_label_get_text(list->data)), &position);
    gtk_editable_set_position(GTK_EDITABLE(entry), position);
}

G_MODULE_EXPORT void on_variable_button_toggled(GtkToggleButton *switcher, GtkWidget *tree) {
    if (gtk_toggle_button_get_active(switcher)) {
        gtk_widget_show(tree);
    } else {
        gtk_widget_hide(tree);
    }
}

G_MODULE_EXPORT void toggled_button_clicked(GtkToggleButton *button, GList *list) {
    if (gtk_toggle_button_get_active(button)) {
        gtk_label_set_text(g_list_nth_data(list, 0), "real");
        gtk_label_set_text(g_list_nth_data(list, 1), "imag");
        gtk_label_set_text(g_list_nth_data(list, 2), "mag");
        gtk_label_set_text(g_list_nth_data(list, 3), "phase");
        gtk_label_set_text(g_list_nth_data(list, 4), "ln");
    } else {
        gtk_label_set_text(g_list_nth_data(list, 0), "sin");
        gtk_label_set_text(g_list_nth_data(list, 1), "cos");
        gtk_label_set_text(g_list_nth_data(list, 2), "tg");
        gtk_label_set_text(g_list_nth_data(list, 3), "ctg");
        gtk_label_set_text(g_list_nth_data(list, 4), "log");

    }
}

G_MODULE_EXPORT void change_the_theme(GtkComboBoxText *box) {
    if (strcmp(gtk_combo_box_text_get_active_text(box), "White theme") == 0) {
        if (css)
            gtk_style_context_remove_provider_for_screen(screen, GTK_STYLE_PROVIDER(css));

        screen = gdk_screen_get_default();
        css = gtk_css_provider_new();
        gtk_css_provider_load_from_path(css, "GUI/Themes/WhiteTheme.css", NULL);
        gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(css), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        g_object_unref(css);
    }
    if (strcmp(gtk_combo_box_text_get_active_text(box), "DOS-style theme") == 0) {
        if (css)
            gtk_style_context_remove_provider_for_screen(screen, GTK_STYLE_PROVIDER(css));

        screen = gdk_screen_get_default();
        css = gtk_css_provider_new();
        gtk_css_provider_load_from_path(css, "GUI/Themes/DOSStyleTheme.css", NULL);
        gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(css), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        g_object_unref(css);
    }
    if (strcmp(gtk_combo_box_text_get_active_text(box), "Night theme") == 0) {
        if (css)
            gtk_style_context_remove_provider_for_screen(screen, GTK_STYLE_PROVIDER(css));

        screen = gdk_screen_get_default();
        css = gtk_css_provider_new();
        gtk_css_provider_load_from_path(css, "GUI/Themes/NightTheme.css", NULL);
        gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(css), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        g_object_unref(css);
    }
}

G_MODULE_EXPORT void Preferences_destroy(GtkWidget *preferences) {
    gtk_widget_hide(preferences);
}
G_MODULE_EXPORT void ErrorWindow_destroy(GtkButton *button, GtkWidget *ErrorWindow) {
    gtk_widget_hide(ErrorWindow);
}

void test(struct NodeVariable *sVar, GtkTreeModel *listStore1) {
    struct MapOperations mp1[MAP_SIZE];
    initMapOperations(mp1);
    defineOperations(mp1);

    struct MapComplex mp2[MAP_SIZE];
    initMapComplex(mp2);
    defineConstants(mp2);

    GtkTreeIter iter;
    gboolean valid = gtk_tree_model_get_iter_first(listStore1, &iter);
    while (valid) {
        gchar *str1;
        gchar *str2;

        gtk_tree_model_get(listStore1, &iter, 0, &str1, 1, &str2, -1);

        printf("%s = %s", str1, str2);

        valid = gtk_tree_model_iter_next(listStore1, &iter);

        pushVariable(&sVar, str1, str2);

        g_free(str1);
        g_free(str2);
    }
    sortVariables(&sVar, mp1, mp2);
}
