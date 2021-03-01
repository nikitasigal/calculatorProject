#include "GUI.h"
#include <ctype.h>

GtkWidget *resultLabel;
GtkListStore *listStore;
GtkWidget *errorWindow;
GtkLabel *errorLabel;
GtkLabel *errorLabelWithName;
GtkComboBox *themeChooser;
GtkComboBox *calculatorChooser;
GtkCssProvider *css;
GtkTreeSelection *selection;
GtkTreeViewColumn *firstColumn;
GtkTreeViewColumn *secondColumn;
GdkScreen *screen;

enum State {
    BEGINNING,
    START,
    NUMBER,
    VARIABLE,
    CLOSE_BRACKET,
    OPERATOR
};

/*
 * Предпроверка выражения на корректность. Обрабатывает основные простые ошибки в выражении, не удаляет инпут
 * пользователя и выводит сообщение о том, что пошло не так. Алгоритм таков: идём по выражению, поддерживая текущее
 * состояние, в котором мы находимся - начало выражения (BEGINNING), начало подвыражения (START), число (NUMBER),
 * переменная (VARIABLE), закрывающаяся скобка (CLOSE_BRACKET), оператор (OPERATOR). При смене состояния проводится
 * проверка на совместимость состояний, например, если мы из NUMBER переходим в NUMBER, то явно пропущен оператор,
 * алгоритм выкидывает ERROR и сообщение об ошибке.
 *
 * Основные проверки:
 * 1) Наличие оператора между числами/переменными/скобками
 * 2) Наличие двух операндов у оператора
 * 3) Проверка функции на наличие следом идущей открывающейся скобки
 * 4) Корректность имени переменной
 * 5) Корректность числа: отсутствие двойной точки
 * 6) Отсутствие неожиданно-встреченных символов по типу точки или запятой
 * 7) Ещё что-то по мелочи, что мне лень писать
 */
bool isIncorrect(char *str, struct MapOperations *mp1) {
    enum State state = BEGINNING;

    for (int i = 0; i < strlen(str);) {
        // Пропускаем пробелы
        if (isblank(str[i])) {
            ++i;
            continue;
        }

        if (i < strlen(str) && isdigit(str[i])) {  // Если цифра
            bool wasDot = false;
            while (i < strlen(str) && (isalnum(str[i]) || str[i] == '.')) {
                if (str[i] == '.' && wasDot)
                    printError("Wrong number format: double-dot in number.");
                if (str[i] == '.')
                    wasDot = true;
                if (isalpha(str[i]))
                    printError("Wrong variable's name: variable can not start with digit.");
                ++i;
            }
            if (state == NUMBER)
                printError("Missed operator between numbers.");
            if (state == VARIABLE)
                printError("Missed operator between variable and number.");
            if (state == CLOSE_BRACKET)
                printError("Missed operator after close bracket.");

            state = NUMBER;
        } else if (i < strlen(str) && isalpha(str[i])) { // Переменная или функция или константа
            char curWord[WORD_LENGTH] = {0};
            int curSym = 0;
            while (i < strlen(str) && !isOperator(&(str[i])) && !isblank(str[i]) && str[i] != '(' && str[i] != ')' &&
                   str[i] != ',') {
                if (!isalnum(str[i])) {
                    char tempStr[WORD_LENGTH] = "Wrong variable name: incorrect symbol ";
                    char tempSymbol[2] = {0};
                    tempSymbol[0] = str[i];
                    strcat(tempStr, tempSymbol);
                    printError(tempStr);
                }
                curWord[curSym++] = str[i++];
            }
            if (!strcmp(curWord, "i")) {
                if (state == NUMBER)
                    printError("Missed operator between numbers.");
                if (state == VARIABLE)
                    printError("Missed operator between variable and number.");
                if (state == CLOSE_BRACKET)
                    printError("Missed operator after close bracket.");
                state = NUMBER;
                continue;
            }
            // Если функция
            if (findOperation(mp1, curWord) != INT_MAX) {
                while (isblank(str[i])) {
                    ++i;
                }
                if (str[i] != '(')
                    printError("Wrong function usage: missed open bracket after function.");
                else {
                    if (state == NUMBER) {
                        printError("Missed operator between number and function.");
                        return ERROR;
                    }
                    if (state == VARIABLE) {
                        printError("Missed operator between variable and function.");
                        return ERROR;
                    }
                    if (state == CLOSE_BRACKET) {
                        printError("Missed operator after close bracket.");
                        return ERROR;
                    }
                    state = START;
                }
                ++i;
            } else {    // Переменная
                if (state == NUMBER) {
                    printError("Missed operator between number and variable.");
                    return ERROR;
                }
                if (state == VARIABLE) {
                    printError("Missed operator between variables.");
                    return ERROR;
                }
                if (state == CLOSE_BRACKET) {
                    printError("Missed operator after close bracket.");
                    return ERROR;
                }
                state = VARIABLE;
            }
        } else if (i < strlen(str) && isOperator(&(str[i]))) {  // Оператор
            if (state == BEGINNING && (str[i] != '-')) {
                printError("Not unary operation at the beginning of expression.");
                return ERROR;
            }
            if (state == START && (str[i] != '-')) {
                printError("Not unary operation after '(' or ','.");
                return ERROR;
            }
            if (state == OPERATOR) {
                printError("Missed number/variable between operators.");
                return ERROR;
            }
            state = OPERATOR;
            ++i;
        } else if (str[i] == ')') {
            if (state == OPERATOR) {
                printError("Missed number/variable before close bracket.");
                return ERROR;
            }
            if (state == BEGINNING) {
                printError("Incorrect bracket sequence.");
                return ERROR;
            }
            state = CLOSE_BRACKET;
            ++i;
        } else if (str[i] == '(') {
            if (state == NUMBER) {
                printError("Missed operator between number and open bracket.");
                return ERROR;
            }
            if (state == VARIABLE) {
                printError("Missed operator between variable and open bracket.");
                return ERROR;
            }
            if (state == CLOSE_BRACKET) {
                printError("Missed operator between brackets.");
                return ERROR;
            }
            state = START;
            ++i;
        } else if (str[i] == ',') {
            state = START;
            ++i;
        } else if (str[i] == '.') {
            printError("Unexpected dot.");
            return ERROR;
        } else {
            char tempStr[WORD_LENGTH] = "Unknown symbol: ";
            char tempSymbol[2] = {0};
            tempSymbol[0] = str[i];
            strcat(tempStr, tempSymbol);
            printError(tempStr);
            return ERROR;
        }
    }
    if (state == OPERATOR) {
        printError("Unfinished expression: missed number after operation.");
        return ERROR;
    }
    if (state == START) {
        printError("Unfinished expression: unexpected '(' or ',' at the end of expression.");
        return ERROR;
    }

    return ERROR;
}

/*
 * Установка темы в соответствие с config.ini
 */
void setTheme(int mode) {
    screen = gdk_screen_get_default();
    css = gtk_css_provider_new();

    switch (mode) {
        case 0:
            gtk_css_provider_load_from_path(css, "GUI/Themes/WhiteTheme.css", NULL);
            break;
        case 1:
            gtk_css_provider_load_from_path(css, "GUI/Themes/NightTheme.css", NULL);
            break;
        case 2:
            gtk_css_provider_load_from_path(css, "GUI/Themes/DOSStyleTheme.css", NULL);
            break;
        default:
            printf("WARNING: unknown theme. Rollback to white theme.\n");
            gtk_css_provider_load_from_path(css, "GUI/Themes/WhiteTheme.css", NULL);
            mode = DEFAULT;
    }

    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(css), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(css);

    gtk_combo_box_set_active(themeChooser, mode);
}

/*
 * Установка режима калькулятора в соответствие с config.ini
 */
void setCalculatorMode(GtkBuilder *builder, int mode) {
    GtkStack *calculatorStack = GTK_STACK(gtk_builder_get_object(builder, "CalculatorStack"));
    GtkWidget *engineering = GTK_WIDGET(gtk_builder_get_object(builder, "Grid"));
    GtkWidget *basic = GTK_WIDGET(gtk_builder_get_object(builder, "GridBasic"));
    switch (mode) {
        case 0:
            gtk_stack_set_visible_child(calculatorStack, basic);
            break;
        case 1:
            gtk_stack_set_visible_child(calculatorStack, engineering);
            break;
        default:
            printf("WARNING: unknown calculator mode. Rollback to basic.");
            gtk_stack_set_visible_child(calculatorStack, basic);
            mode = DEFAULT;
    }

    gtk_combo_box_set_active(calculatorChooser, mode);
}

/*
 * Обработка config.ini. Читает файл и устанавливает соответствующие настройки
 */
void setUserSettings(GtkBuilder *builder) {
    FILE *config = fopen("GUI/Settings/config.ini", "r+");

    char argument[INI_ARGUMENT_LENGTH] = {0};
    fgets(argument, INI_ARGUMENT_LENGTH, config);

    char optionName[INI_ARGUMENT_LENGTH] = {0};
    char optionValue[INI_ARGUMENT_LENGTH] = {0};
    long index = DEFAULT;

    // Активная тема
    fscanf(config, "%s = %s", optionName, optionValue);
    if (!strcmp(optionName, "ActiveTheme")) {
        index = strtol(optionValue, NULL, 10);
        setTheme(index);
    } else {
        printf("WARNING: incorrect .ini format. Rollback to default settings\n");
        setTheme(DEFAULT);
        setCalculatorMode(builder, DEFAULT);
        return;
    }

    // Режим калькулятора
    fscanf(config, "%s = %s", optionName, optionValue);
    if (!strcmp(optionName, "CalculatorDefaultMode")) {
        index = strtol(optionValue, NULL, 10);
        setCalculatorMode(builder, index);
    } else {
        printf("WARNING: incorrect .ini format. Rollback to default settings\n");
        setTheme(DEFAULT);
        setCalculatorMode(builder, DEFAULT);
        return;
    }

    fclose(config);
}

/*
 * Функция инициализирует переменные, требуемые в процессе работы, и устанавливает настройки по умолчанию
 */
void init_GUI() {
    // Инициализация переменных
    GtkBuilder *builder = gtk_builder_new_from_file("GUI/Glade/glade.glade");
    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, "MainWindow"));
    GtkToggleButton *toggleButton = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "toggle_button"));
    firstColumn = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "TreeColumnFirst"));
    secondColumn = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "TreeColumnSecond"));
    selection = GTK_TREE_SELECTION(gtk_builder_get_object(builder, "TreeSelection"));
    listStore = GTK_LIST_STORE(gtk_builder_get_object(builder, "liststore1"));
    resultLabel = GTK_WIDGET(gtk_builder_get_object(builder, "labelResult"));
    errorLabel = GTK_LABEL(gtk_builder_get_object(builder, "ErrorLabel"));
    errorLabelWithName = GTK_LABEL(gtk_builder_get_object(builder, "ErrorLabelWithName"));
    errorWindow = GTK_WIDGET(gtk_builder_get_object(builder, "ErrorWindow"));
    themeChooser = GTK_COMBO_BOX(gtk_builder_get_object(builder, "ThemeChooser"));
    calculatorChooser = GTK_COMBO_BOX(gtk_builder_get_object(builder, "CalculatorChooser"));

    // Ручная настройка кнопки 2nd. Костыль, но это из-за ограничения xml файла с интерфейсом
    GList *list = NULL;
    list = g_list_append(list, GTK_LABEL(gtk_builder_get_object(builder, "button_label_sin")));
    list = g_list_append(list, GTK_LABEL(gtk_builder_get_object(builder, "button_label_cos")));
    list = g_list_append(list, GTK_LABEL(gtk_builder_get_object(builder, "button_label_tg")));
    list = g_list_append(list, GTK_LABEL(gtk_builder_get_object(builder, "button_label_ctg")));
    list = g_list_append(list, GTK_LABEL(gtk_builder_get_object(builder, "button_label_log")));

    // Установка темы по умолчанию
    setUserSettings(builder);

    // Подключение всех сингналов (функций к виджетам) и ручное подключение
    gtk_builder_connect_signals(builder, NULL);
    g_signal_connect(toggleButton, "clicked", G_CALLBACK(toggled_button_clicked), list);

    // Добавление пустой строки в дерево переменных
    on_TreeButtonAdd_clicked(NULL, listStore);

    // Выкладываем все виджеты окна, которые нужно показать, на экран
    gtk_widget_show_all(window);

    // Удаляем ссылку на xml файл, потому что он больше не нужон
    g_object_unref(builder);
}

/*
 * Сохранение настроек в config.ini
 */
void save() {
    FILE *config = fopen("GUI/Settings/config.ini", "w");

    fprintf(config, "%s\n", "[PreferencesWindow]");
    int index;

    // Активная тема
    index = gtk_combo_box_get_active(themeChooser);
    fprintf(config, "%s%d\n", "ActiveTheme = ", index);

    // Режим калькулятора
    index = gtk_combo_box_get_active(calculatorChooser);
    fprintf(config, "%s%d", "CalculatorDefaultMode = ", index);
}

/*
 * Остановка программы при закрытии главного окна
 */
G_MODULE_EXPORT void on_MainWindow_destroy() {
    save();
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
    // Очистка статических переменных
    ERROR = false;
    gtk_label_set_text(errorLabelWithName, "");
    gtk_label_set_text(errorLabel, "");

    // Инициализация мапов функций и констант
    struct NodeVariable *sVar = NULL;
    struct MapOperations mp1[MAP_SIZE];
    initOperations(mp1);
    defineOperations(mp1);

    struct MapComplex mp2[MAP_SIZE];
    initVariables(mp2);
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
            printError("Incorrect variable name or value");
            return;
        }

        // Переходим к следующей строке. valid - переменная типа bool, приобретает значение 0, когда дерево кончается
        valid = gtk_tree_model_iter_next(treeModel, &iter);

        // Предпроверка корректности выражения
        if (isIncorrect(varValue, mp1)) {
            char errorMsg[WORD_LENGTH] = {0};
            strcat(errorMsg, "Mistake in value of variable '");
            strcat(errorMsg, varName);
            strcat(errorMsg, "'.");
            gtk_label_set_text(errorLabelWithName, errorMsg);
            gtk_label_set_text(GTK_LABEL(resultLabel), "nan");
            return;
        }

        // Пушаем переменную в стек
        pushVariable(&sVar, varName, varValue);

        // Очищаем динамические строки
        g_free(varName);
        g_free(varValue);
    }

    // Теперь берём строку-выражение
    char expression[VALUE_LENGTH] = {0};
    strcpy(expression, gtk_entry_get_text(entry));
    if (isIncorrect(expression, mp1)) {
        char errorMsg[WORD_LENGTH] = {0};
        strcat(errorMsg, "Mistake in expression.");
        gtk_label_set_text(errorLabelWithName, errorMsg);
        gtk_label_set_text(GTK_LABEL(resultLabel), "nan");
        return;
    }
    pushVariable(&sVar, "", expression);

    // Сортируем
    sortVariables(&sVar, mp1, mp2);

    // Считаем
    while (sVar) {
        struct Variable var = popVariable(&sVar);
        insertVariable(mp2, var.name, calculate(mp1, mp2, var));
    }

    // Берём посчитанное значение выражения
    complex long double resultValue = mp2[findVariable(mp2, "")].value;
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
                    strcat(resultAsString, " -");
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
            g_ascii_dtostr(resultAsString, VALUE_LENGTH, resultValue);
        }
    }

    // Посчитали. Очищаем поле ввода и выводим ответ на экран
    if (!ERROR)
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
    if (strlen(new_text) == 0 || !isalpha(new_text[0]) || (strlen(new_text) > 1 && new_text[1] < 0)) {
        return;
    }
    for (int i = 0; i < strlen(new_text); ++i) {
        if (!isalnum(new_text[i])) {
            return;
        }
    }

    GtkTreeIter iter;
    GtkTreeModel *model = gtk_tree_view_get_model(user_data);
    gtk_tree_model_get_iter_from_string(model, &iter, path_string);
    gtk_list_store_set(listStore, &iter, 0, new_text, -1);

    GList *listOfSelectedRows = gtk_tree_selection_get_selected_rows(selection, &model);
    int *index = gtk_tree_path_get_indices(listOfSelectedRows->data);
    int treeSize = gtk_tree_model_iter_n_children(GTK_TREE_MODEL(listStore), NULL);

    GtkTreePath *path = gtk_tree_model_get_path(model, &iter);
    if (*index + 1 == treeSize && strlen(new_text) != 0) {
        GtkTreeView *tree = gtk_tree_selection_get_tree_view(selection);
        gtk_list_store_append(listStore, &iter);
        gtk_tree_view_set_cursor_on_cell(tree, path, secondColumn, NULL, 0);
    }
}

/*
 * Если изменено значение переменной
 */
G_MODULE_EXPORT void
on_TreeTextSecond_edited(GtkCellRendererText *cell, gchar *path_string, gchar *new_text, gpointer user_data) {
    if (strlen(new_text) == 0 || (strlen(new_text) > 1 && new_text[1] < 0)) {
        return;
    }
    for (int i = 0; i < strlen(new_text); ++i) {
        if (new_text[i] < 0) {
            return;
        }
    }
    GtkTreeIter iter;
    GtkTreeModel *model = gtk_tree_view_get_model(user_data);
    gtk_tree_model_get_iter_from_string(model, &iter, path_string);
    gtk_list_store_set(listStore, &iter, 1, new_text, -1);

    GtkTreeView *tree = gtk_tree_selection_get_tree_view(selection);
    gboolean valid = gtk_tree_model_iter_next(model, &iter);
    if (valid) {
        GtkTreePath *path = gtk_tree_model_get_path(model, &iter);
        gtk_tree_view_set_cursor(tree, path, firstColumn, 0);
    }
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
