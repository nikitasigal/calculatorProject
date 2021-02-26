#include "GUI.h"
#include "../calculations.h"
#include "fontconfig/fontconfig.h"

#include <string.h>

GtkWidget *resultLabel;
GtkListStore *listStore;
GtkWidget *errorMessage;
GtkLabel *ErrorLabel;
GtkCssProvider *css;
GdkScreen *screen;

/*
 * Функция инициализирует переменные, требуемые в процессе работы, и устанавливает настройки по умолчанию
 */
void init_GUI() {
    // Инициализация переменных
    GtkBuilder *builder = gtk_builder_new_from_file("GUI/Glade/glade.glade");
    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, "MainWindow"));
    GtkToggleButton *toggleButton = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "toggle_button"));
    listStore = GTK_LIST_STORE(gtk_builder_get_object(builder, "liststore1"));
    resultLabel = GTK_WIDGET(gtk_builder_get_object(builder, "labelResult"));
    ErrorLabel = GTK_LABEL(gtk_builder_get_object(builder, "ErrorLabel"));
    errorMessage = GTK_WIDGET(gtk_builder_get_object(builder, "ErrorWindow"));

    // Ручная настройка кнопки 2nd. Костыль, но это из-за ограничения xml файла с интерфейсом
    GList *list = NULL;
    list = g_list_append(list, GTK_LABEL(gtk_builder_get_object(builder, "button_label_sin")));
    list = g_list_append(list, GTK_LABEL(gtk_builder_get_object(builder, "button_label_cos")));
    list = g_list_append(list, GTK_LABEL(gtk_builder_get_object(builder, "button_label_tg")));
    list = g_list_append(list, GTK_LABEL(gtk_builder_get_object(builder, "button_label_ctg")));
    list = g_list_append(list, GTK_LABEL(gtk_builder_get_object(builder, "button_label_log")));

    // Установка темы по умолчанию
    screen = gdk_screen_get_default();
    css = gtk_css_provider_new();
    gtk_css_provider_load_from_path(css, "GUI/Themes/WhiteTheme.css", NULL);
    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(css), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(css);

    // Подключение всех сингналов (функций к виджетам) и ручное подключение
    gtk_builder_connect_signals(builder, NULL);
    g_signal_connect(toggleButton, "clicked", G_CALLBACK(toggled_button_clicked), list);

    // Выкладываем все виджеты окна, которые нужно показать, на экран
    gtk_widget_show_all(window);

    // Удаляем ссылку на xml файл, потому что он больше не нужон
    g_object_unref(builder);
}

/*
 * Остановка программы при закрытии главного окна
 */
G_MODULE_EXPORT void on_MainWindow_destroy() {
    gtk_main_quit();
}

/*
 * Установка текста в поле текста
 */
G_MODULE_EXPORT void labelSetValue(GtkWidget *label, char *result) {
    gtk_label_set_text(GTK_LABEL(label), result);
}

/*
 * Активирование функции calculate. Вызывается при нажатии enter в поле ввода или при нажатии клавиши '='
 */
G_MODULE_EXPORT void on_button_clicked(GtkWidget *button, GtkEntry *entry) {
    // Инициализация мапов функций и констант
    struct NodeVariable *sVar = NULL;
    struct MapOperations mp1[MAP_SIZE];
    initMapOperations(mp1);
    defineOperations(mp1);

    struct MapComplex mp2[MAP_SIZE];
    initMapComplex(mp2);
    defineConstants(mp2);

    // Чтение переменных из дерева. Следующие три строчки инициализирует структуру дерева и берёт первый элемент
    GtkTreeIter iter;
    GtkTreeModel *treeModel = GTK_TREE_MODEL(listStore);
    gboolean valid = gtk_tree_model_get_iter_first(treeModel, &iter);
    // Пока есть строчки в дереве, выполняем цикл
    while (valid) {
        gchar *varName = malloc(sizeof(gchar) * VALUE_LENGTH);
        gchar *varValue = malloc(sizeof(gchar) * VALUE_LENGTH);

        // Берём строчку из дерева
        gtk_tree_model_get(treeModel, &iter, 0, &varName, 1, &varValue, -1);
        // Если строка полностью пустая (без имени и значения), то скипаем её
        if (!varName && !varValue) {
            valid = gtk_tree_model_iter_next(treeModel, &iter);
            g_free(varName);
            g_free(varValue);
            continue;
        }
        // Если нет имени или значения, то это неправильная переменная, выдаём ошибку
        if (!varName || !varValue) {
            error_message("Incorrect variable name or value");
            return;
        }

        // Переходим к следующей строке. valid - переменная типа bool, приобретает значение 0, когда дерево кончается
        valid = gtk_tree_model_iter_next(treeModel, &iter);

        // Пушаем переменную в стек
        pushVariable(&sVar, varName, varValue);

        // Очищаем динамические строки
        g_free(varName);
        g_free(varValue);
    }

    // Теперь берём строку-выражение
    char expression[VALUE_LENGTH] = {0};
    strcpy(expression, gtk_entry_get_text(entry));
    pushVariable(&sVar, "", expression);
    // Указатель на выражение
    struct NodeVariable *result = sVar;

    // Сортируем
    sortVariables(&sVar, mp1, mp2);

    // Считаем
    struct NodeVariable *cur = sVar;
    while (cur != NULL) {
        insertComplex(mp2, cur->variable.name, calculate(mp1, mp2, cur->variable));
        complex long double resultVar = mp2[findComplex(mp2, cur->variable.name)].value;
        printf("%s = %lf %lfi\n", cur->variable.name, creal(resultVar), cimag(resultVar));
        cur = cur->next;
    }

    // Берём посчитанное значение выражения
    complex long double resultValue = mp2[findComplex(mp2, result->variable.name)].value;
    char resultAsString[NUMBER_LENGTH * 2] = {0};
    char realPart[NUMBER_LENGTH] = {0};
    char imagPart[NUMBER_LENGTH] = {0};

    // Обработка красоты вывода. Чтобы не выводилось 0 + -0i или 1 - -1i и подобное
    if (fabs(creal(resultValue)) > EPS) {
        strcat(resultAsString, g_ascii_dtostr(realPart, NUMBER_LENGTH, creal(resultValue)));
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
                strcat(resultAsString, g_ascii_dtostr(imagPart, NUMBER_LENGTH, fabs(cimag(resultValue))));
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
                strcat(resultAsString, g_ascii_dtostr(imagPart, NUMBER_LENGTH, cimag(resultValue)));
                strcat(resultAsString, "i");
            }
        } else {
            strcat(resultAsString, "0");
        }
    }

    // Посчитали. Очищаем поле ввода и выводим ответ на экран
    gtk_entry_set_text(entry, "");
    gtk_label_set_text(GTK_LABEL(resultLabel), resultAsString);
}

/*
 * Сигнал при нажатии на enter
 */
G_MODULE_EXPORT void on_entry_key_press_event(GtkWidget *entry, GdkEventKey *event, gpointer user_data) {
    if (event->keyval == GDK_KEY_Return) {
        on_button_clicked(NULL, GTK_ENTRY(entry));
    }
}

/*
 * Обработка удаления строки из дерева переменных
 */
G_MODULE_EXPORT void on_TreeButtonDelete_clicked(GtkWidget *button, GtkTreeSelection *row) {
    GtkTreeIter iter;
    if (gtk_tree_selection_get_selected(row, NULL, &iter)) {
        gtk_list_store_remove(listStore, &iter);
    }
}

/*
 * Добавление строки в дерево переменных
 */
G_MODULE_EXPORT void on_TreeButtonAdd_clicked(GtkWidget *button, GtkListStore *list) {
    GtkTreeIter iter;
    gtk_list_store_append(list, &iter);
}

/*
 * Если имя переменной было изменено, то изменяем
 */
G_MODULE_EXPORT void
on_TreeTextFirst_edited(GtkCellRendererText *cell, gchar *path_string, gchar *new_text, gpointer user_data) {
    GtkTreeIter iter;
    gtk_tree_model_get_iter_from_string(gtk_tree_view_get_model(user_data), &iter, path_string);
    gtk_list_store_set(listStore, &iter, 0, new_text, -1);
}

/*
 * Если изменено значение переменной
 */
G_MODULE_EXPORT void
on_TreeTextSecond_edited(GtkCellRendererText *cell, gchar *path_string, gchar *new_text, gpointer user_data) {
    GtkTreeIter iter;
    gtk_tree_model_get_iter_from_string(gtk_tree_view_get_model(user_data), &iter, path_string);
    gtk_list_store_set(listStore, &iter, 1, new_text, -1);
}

/*
 * Обработка нажатия клавиш некоторых функций
 */
G_MODULE_EXPORT void on_calculator_button_clicked_function(GtkWidget *button, GtkEntry *entry) {
    gint position = gtk_editable_get_position(GTK_EDITABLE(entry));
    GList *list = gtk_container_get_children(GTK_CONTAINER(button));
    char temp[10] = {0};
    strcpy(temp, gtk_label_get_text(GTK_LABEL(list->data)));
    strcat(temp, "()");
    gtk_editable_insert_text(GTK_EDITABLE(entry), temp, sizeof(gchar) * strlen(temp), &position);
    gtk_editable_set_position(GTK_EDITABLE(entry), position - 1);
}

/*
 * Обработка нажатия sqrt (другая логика)
 */
G_MODULE_EXPORT void on_calculator_button_clicked_sqrt(GtkWidget *button, GtkEntry *entry) {
    gint position = gtk_editable_get_position(GTK_EDITABLE(entry));
    gtk_editable_insert_text(GTK_EDITABLE(entry), "sqrt()", sizeof(gchar) * 6, &position);
    gtk_editable_set_position(GTK_EDITABLE(entry), position - 1);
}

/*
 * Обработка abs
 */
G_MODULE_EXPORT void on_calculator_button_clicked_abs(GtkWidget *button, GtkEntry *entry) {
    gint position = gtk_editable_get_position(GTK_EDITABLE(entry));
    gtk_editable_insert_text(GTK_EDITABLE(entry), "abs()", sizeof(gchar) * 5, &position);
    gtk_editable_set_position(GTK_EDITABLE(entry), position - 1);
}

/*
 * При нажатии клавиш калькулятора, устанавливаем фокус (курсор) на окно ввода выражения
 */
G_MODULE_EXPORT void set_focus_to_entry(GtkWidget *button, GtkEntry *entry) {
    gtk_entry_grab_focus_without_selecting(entry);
}

/*
 * Обработка смещения вправо
 */
G_MODULE_EXPORT void on_calculator_button_clicked_move_right(GtkWidget *button, GtkEntry *entry) {
    gint position = gtk_editable_get_position(GTK_EDITABLE(entry));
    gtk_editable_set_position(GTK_EDITABLE(entry), position + 1);
}

/*
 * Обработка смещения влево
 */
G_MODULE_EXPORT void on_calculator_button_clicked_move_left(GtkWidget *button, GtkEntry *entry) {
    gint position = gtk_editable_get_position(GTK_EDITABLE(entry));
    gtk_editable_set_position(GTK_EDITABLE(entry), position - 1);
}

/*
 * Обработка ()
 */
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

/*
 * Обработка PI
 */
G_MODULE_EXPORT void on_calculator_button_clicked_PI(GtkWidget *button, GtkEntry *entry) {
    gint position = gtk_editable_get_position(GTK_EDITABLE(entry));
    gtk_editable_insert_text(GTK_EDITABLE(entry), "PI", sizeof(gchar) * 2, &position);
    gtk_editable_set_position(GTK_EDITABLE(entry), position);
}

/*
 * Обработка открытия окна настроек
 */
G_MODULE_EXPORT void open_preferences(GtkButton *button, GtkWindow *window) {
    gtk_widget_show(GTK_WIDGET(window));
    gtk_window_present(window);
}

/*
 * Вывод сообщения ошибки
 */
G_MODULE_EXPORT void error_message(char *str) {
    gtk_label_set_text(ErrorLabel, str);
    gtk_widget_show(errorMessage);
    gtk_window_present(GTK_WINDOW(errorMessage));
}

/*
 * Обработка удаления символа
 */
G_MODULE_EXPORT void on_calculator_button_clicked_backspace(GtkWidget *button, GtkEntry *entry) {
    gint position = gtk_editable_get_position(GTK_EDITABLE(entry));
    if (gtk_editable_get_selection_bounds(GTK_EDITABLE(entry), NULL, NULL)) {
        gtk_editable_delete_selection(GTK_EDITABLE(entry));
    } else {
        gtk_editable_delete_text(GTK_EDITABLE(entry), position - 1, position);
    }
    error_message("Test error message");
}

/*
 * Обработка /
 */
G_MODULE_EXPORT void on_calculator_button_clicked_div(GtkWidget *button, GtkEntry *entry) {
    gint position = gtk_editable_get_position(GTK_EDITABLE(entry));
    gtk_editable_insert_text(GTK_EDITABLE(entry), "/", sizeof(gchar), &position);
    gtk_editable_set_position(GTK_EDITABLE(entry), position);
}

/*
 * Обработка *
 */
G_MODULE_EXPORT void on_calculator_button_clicked_mult(GtkWidget *button, GtkEntry *entry) {
    gint position = gtk_editable_get_position(GTK_EDITABLE(entry));
    gtk_editable_insert_text(GTK_EDITABLE(entry), "*", sizeof(gchar), &position);
    gtk_editable_set_position(GTK_EDITABLE(entry), position);
}

/*
 * Обработка остальных клавиш калькулятора
 */
G_MODULE_EXPORT void on_calculator_button_clicked(GtkWidget *button, GtkEntry *entry) {
    GList *list = gtk_container_get_children(GTK_CONTAINER(button));
    gint position = gtk_editable_get_position(GTK_EDITABLE(entry));

    gtk_editable_insert_text(GTK_EDITABLE(entry), gtk_label_get_text(list->data),
                             sizeof(gchar) * strlen(gtk_label_get_text(list->data)), &position);
    gtk_editable_set_position(GTK_EDITABLE(entry), position);
}

/*
 * Открытие/закрытие дерева переменных
 */
G_MODULE_EXPORT void on_variable_button_toggled(GtkToggleButton *switcher, GtkWidget *tree) {
    if (gtk_toggle_button_get_active(switcher)) {
        gtk_widget_show(tree);
    } else {
        gtk_widget_hide(tree);
    }
}

/*
 * Обработка 2nd
 */
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

/*
 * Обработка смены темы. Код для каждой темы выглядит одинаково, но почему-то адекватно у меня не получилось оформить,
 * какие-то беды с ссылками происходят
 */
G_MODULE_EXPORT void change_the_theme(GtkComboBoxText *box) {
    if (strcmp(gtk_combo_box_text_get_active_text(box), "White theme") == 0) {
        if (css)
            gtk_style_context_remove_provider_for_screen(screen, GTK_STYLE_PROVIDER(css));

        screen = gdk_screen_get_default();
        css = gtk_css_provider_new();
        gtk_css_provider_load_from_path(css, "GUI/Themes/WhiteTheme.css", NULL);
        gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(css),
                                                  GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        g_object_unref(css);
    }
    if (strcmp(gtk_combo_box_text_get_active_text(box), "DOS-style theme") == 0) {
        if (css)
            gtk_style_context_remove_provider_for_screen(screen, GTK_STYLE_PROVIDER(css));

        screen = gdk_screen_get_default();
        css = gtk_css_provider_new();
        gtk_css_provider_load_from_path(css, "GUI/Themes/DOSStyleTheme.css", NULL);
        gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(css),
                                                  GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        g_object_unref(css);
    }
    if (strcmp(gtk_combo_box_text_get_active_text(box), "Night theme") == 0) {
        if (css)
            gtk_style_context_remove_provider_for_screen(screen, GTK_STYLE_PROVIDER(css));

        screen = gdk_screen_get_default();
        css = gtk_css_provider_new();
        gtk_css_provider_load_from_path(css, "GUI/Themes/NightTheme.css", NULL);
        gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(css),
                                                  GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        g_object_unref(css);
    }
}

/*
 * Закрытие окна настоек
 */
G_MODULE_EXPORT void Preferences_destroy(GtkWidget *preferences) {
    gtk_widget_hide(preferences);
}

/*
 * Закрытие окна ошибки
 */
G_MODULE_EXPORT void ErrorWindow_destroy(GtkButton *button, GtkWidget *ErrorWindow) {
    gtk_widget_hide(ErrorWindow);
}
