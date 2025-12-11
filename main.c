#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <locale.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

// Объявление типа указателя на функцию
typedef double (*TFun)(double);

// Объявление прототипов функций
double Y(double x);
double V(double x);
double calculate_function_value(TFun f, double x);
int plot(double x0, double x1, TFun f);
void tabulate_function(TFun f, double start, double end, double step, int include_start, int include_end);
int read_params_from_file(double *start, double *end, double *step, int *include_start, int *include_end);

// Функции для работы с массивом
double* full_array(double *arr, int size);
int read_array_from_file(double **arr, int *size);
int write_array_to_file(double *arr, int size);
void put_array(double *arr, int size);

int main()
{
    setlocale(LC_ALL, "RUS");
    srand(time(NULL)); // Инициализация генератора случайных чисел

    printf("********************************************\n");
    printf("*                                          *\n");
    printf("*           Курсовой проект                *\n");
    printf("*                                          *\n");
    printf("* Разработка программы для табулирования   *\n");
    printf("*               функции                    *\n");
    printf("*                                          *\n");
    printf("*   Выполнил: Сафонов П.Р.                 *\n");
    printf("*   Руководитель: Курипта О.В.             *\n");
    printf("*   Группа: бИПТ-251                       *\n");
    printf("*                                          *\n");
    printf("********************************************\n\n");

    int main_menu_choice;
    int program_running = 1; // Флаг работы программы
    char input_buffer[100];  // Буфер для ввода

    while (program_running)
    {
        printf("\nГлавное меню:\n");
        printf("1. Работа с математическими функциями\n");
        printf("2. Работа с массивом\n");
        printf("3. Завершение работы программы\n");
        printf("Выберите действие (1-3): ");

        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL)
        {
            printf("Ошибка ввода.\n");
            continue;
        }

        if (sscanf(input_buffer, "%d", &main_menu_choice) != 1)
        {
            printf("Неверный ввод! Введите число от 1 до 3.\n");
            continue;
        }

        switch (main_menu_choice)
        {
        case 1:
        {
            // Работа с математическими функциями
            TFun current_function = NULL;
            double start = 0, end = 0, step = 0;
            int include_start = 0, include_end = 0;
            int function_choice, main_action_choice;
            double x_value;
            int has_tabulated = 0;  // Флаг, определяющий было ли табулирование
            int has_parameters = 0; // Флаг наличия параметров
            int result_plot;
            int result_read_func;

            printf("\nДоступные функции:\n");
            printf("\n1. Y(x) = 2*sin(πx)*sin(3πx) - 0.5\n");
            printf("2. V(x) - кусочно-заданная функция:\n");
            printf("\tесли x < 0: (1 + 2x) / (1 + x^2),\n");
            printf("\tесли 0 ≤ x < 1: sin^2(x) * √(1 + x),\n");
            printf("\tесли x ≥ 1: sin^2(x) * e^(0.2x),\n");
            printf("============================================\n\n");

            printf("Выберите функцию (1-2): ");

            if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL ||
                sscanf(input_buffer, "%d", &function_choice) != 1)
            {
                printf("Неверный ввод!\n");
                continue;
            }

            switch (function_choice)
            {
            case 1:
                current_function = Y;
                printf("\nВыбрана функция: Y(x) = 2*sin(πx)*sin(3πx) - 0.5\n");
                break;

            case 2:
                current_function = V;
                printf("\nВыбрана функция: V(x) - кусочная функция\n");
                break;

            default:
                printf("Неверный выбор! Попробуйте снова.\n");
                continue;
            }

            int continue_with_function = 1; // Флаг продолжения работы с текущей функцией

            while (continue_with_function && program_running)
            {
                printf("\nДоступные действия с функцией:\n");
                printf("1. Вычислить значение функции в точке\n");
                printf("2. Ввести параметры для табулирования/построения графика\n");
                printf("3. Сменить функцию\n");
                printf("4. Вернуться в главное меню\n");
                printf("Выберите действие (1-4): ");

                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL)
                {
                    printf("Ошибка ввода.\n");
                    continue;
                }

                if (sscanf(input_buffer, "%d", &main_action_choice) != 1)
                {
                    printf("Неверный ввод! Введите число от 1 до 4.\n");
                    continue;
                }

                switch (main_action_choice)
                {
                case 1:
                {
                    int valid_input_x = 0;
                    do
                    {
                        printf("\nВведите значение x: ");
                        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL)
                        {
                            printf("Ошибка ввода. Попробуйте снова.\n");
                            continue;
                        }
                        if (sscanf(input_buffer, "%lf", &x_value) == 1)
                        {
                            valid_input_x = 1;
                        }
                        else
                        {
                            printf("Неверный ввод! Введите число.\n");
                        }
                    } while (!valid_input_x);

                    double result = calculate_function_value(current_function, x_value);
                    printf("\n----------------------------------------\n");
                    printf("Результат: %.6f\n", result);
                    printf("----------------------------------------\n");
                    break;
                }

                case 2:
                {
                    printf("\nВыберите способ ввода параметров:\n");
                    printf("1. Ввести с клавиатуры\n");
                    printf("2. Загрузить из файла output.txt\n");
                    printf("Ваш выбор (1-2): ");

                    int input_method;
                    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL ||
                        sscanf(input_buffer, "%d", &input_method) != 1)
                    {
                        printf("Неверный ввод! Будет использован ввод с клавиатуры.\n");
                        input_method = 1;
                    }

                    if (input_method == 2)
                    {
                        result_read_func = read_params_from_file(&start, &end, &step, &include_start, &include_end);
                        if (result_read_func == 0)
                        {
                            printf("\nПараметры успешно загружены из файла output.txt\n");
                            has_parameters = 1;
                        }
                        else
                        {
                            printf("\nНе удалось загрузить параметры из файла.\n");
                            printf("Будет использован ввод с клавиатуры.\n");
                            input_method = 1;
                        }
                    }

                    if (input_method == 1)
                    {
                        printf("\nВведите параметры для работы с функцией:\n");

                        int valid_input_start = 0;
                        do
                        {
                            printf("Начало интервала: ");
                            if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL)
                            {
                                printf("Ошибка ввода. Попробуйте снова.\n");
                                continue;
                            }
                            if (sscanf(input_buffer, "%lf", &start) == 1)
                            {
                                valid_input_start = 1;
                            }
                            else
                            {
                                printf("Неверный ввод! Введите число.\n");
                            }
                        } while (!valid_input_start);

                        int valid_input_end = 0;
                        do
                        {
                            printf("Конец интервала: ");
                            if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL)
                            {
                                printf("Ошибка ввода. Попробуйте снова.\n");
                                continue;
                            }
                            if (sscanf(input_buffer, "%lf", &end) == 1)
                            {
                                valid_input_end = 1;
                            }
                            else
                            {
                                printf("Неверный ввод! Введите число.\n");
                            }
                        } while (!valid_input_end);

                        do
                        {
                            printf("Шаг табулирования: ");
                            if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL)
                            {
                                printf("Ошибка ввода. Попробуйте снова.\n");
                                continue;
                            }
                            if (sscanf(input_buffer, "%lf", &step) != 1 || step <= 0)
                            {
                                printf("Шаг должен быть положительным числом! Попробуйте снова.\n");
                            }
                        } while (step <= 0);

                        int valid_input_inc_start = 0;
                        do
                        {
                            printf("Включить начальную точку? (0-нет, 1-да): ");
                            if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL)
                            {
                                printf("Ошибка ввода. Попробуйте снова.\n");
                                continue;
                            }
                            if (sscanf(input_buffer, "%d", &include_start) == 1 &&
                                (include_start == 0 || include_start == 1))
                            {
                                valid_input_inc_start = 1;
                            }
                            else
                            {
                                printf("Неверный ввод! Введите 0 или 1.\n");
                            }
                        } while (!valid_input_inc_start);

                        int valid_input_inc_end = 0;
                        do
                        {
                            printf("Включить конечную точку? (0-нет, 1-да): ");
                            if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL)
                            {
                                printf("Ошибка ввода. Попробуйте снова.\n");
                                continue;
                            }
                            if (sscanf(input_buffer, "%d", &include_end) == 1 &&
                                (include_end == 0 || include_end == 1))
                            {
                                valid_input_inc_end = 1;
                            }
                            else
                            {
                                printf("Неверный ввод! Введите 0 или 1.\n");
                            }
                        } while (!valid_input_inc_end);

                        has_parameters = 1;
                    }

                    has_tabulated = 0;

                    if (has_parameters)
                    {
                        printf("\nПараметры успешно установлены!\n");
                    }

                    int continue_with_params = 1;
                    while (continue_with_params && program_running)
                    {
                        printf("\nТекущие параметры:\n");
                        printf("Интервал: [%.10g, %.10g], Шаг: %.10g\n", start, end, step);
                        printf("Начальная точка: %s, Конечная точка: %s\n",
                               include_start ? "включена" : "исключена",
                               include_end ? "включена" : "исключена");
                        printf("\nДоступные действия:\n");
                        printf("1. Табулировать функцию\n");
                        printf("2. Построить график функции\n");
                        printf("3. Изменить параметры\n");
                        printf("4. Вернуться к выбору действия с функцией\n");
                        printf("Выберите действие (1-4): ");

                        int param_action_choice;
                        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL ||
                            sscanf(input_buffer, "%d", &param_action_choice) != 1)
                        {
                            printf("Неверный ввод!\n");
                            continue;
                        }

                        switch (param_action_choice)
                        {
                        case 1:
                        {
                            if (!has_tabulated)
                            {
                                printf("\nВывести результат табулирования:\n");
                                printf("1. На экран\n");
                                printf("2. В файл output.txt\n");
                                printf("3. На экран и в файл\n");
                                printf("Ваш выбор (1-3): ");

                                int output_choice;
                                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL ||
                                    sscanf(input_buffer, "%d", &output_choice) != 1 ||
                                    output_choice < 1 || output_choice > 3)
                                {
                                    printf("Неверный ввод! Будут выведены результаты на экран.\n");
                                    output_choice = 1;
                                }

                                switch (output_choice)
                                {
                                case 1:
                                    tabulate_function(current_function, start, end, step, include_start, include_end);
                                    has_tabulated = 1;
                                    break;

                                case 2:
                                {
                                    // Сохранение результатов в файл с параметрами
                                    FILE *file = fopen("output.txt", "w");
                                    if (!file)
                                    {
                                        printf("Ошибка открытия файла output.txt для записи\n");
                                    }
                                    else
                                    {
                                        fprintf(file, "========================================\n");
                                        if (current_function == Y)
                                        {
                                            fprintf(file, "Функция: Y(x) = 2*sin(πx)*sin(3πx) - 0.5\n");
                                        }
                                        else
                                        {
                                            fprintf(file, "Функция: V(x) - кусочная функция\n");
                                        }
                                        fprintf(file, "========================================\n\n");

                                        fprintf(file, "Параметры табулирования:\n");
                                        fprintf(file, "Начало интервала: %.10g\n", start);
                                        fprintf(file, "Конец интервала: %.10g\n", end);
                                        fprintf(file, "Шаг: %.10g\n", step);
                                        fprintf(file, "Начальная точка: %s\n", include_start ? "включена" : "исключена");
                                        fprintf(file, "Конечная точка: %s\n", include_end ? "включена" : "исключена");
                                        fprintf(file, "\n");

                                        fprintf(file, "Результаты табулирования:\n");
                                        fprintf(file, "|----------|------------|\n");
                                        fprintf(file, "|    x     |     y      |\n");
                                        fprintf(file, "|----------|------------|\n");

                                        double x = include_start ? start : start + step;
                                        double limit = include_end ? end : end - step;

                                        while (x <= limit)
                                        {
                                            double y = calculate_function_value(current_function, x);
                                            fprintf(file, "| %8.2f | %10.4f |\n", x, y);
                                            x += step;
                                        }

                                        fprintf(file, "|----------|------------|\n");
                                        fclose(file);
                                        printf("Результаты сохранены в файл output.txt\n");
                                        has_tabulated = 1;
                                    }
                                    break;
                                }

                                case 3:
                                    tabulate_function(current_function, start, end, step, include_start, include_end);
                                    has_tabulated = 1;

                                    {
                                        FILE *file = fopen("output.txt", "w");
                                        if (!file)
                                        {
                                            printf("Ошибка открытия файла output.txt для записи\n");
                                        }
                                        else
                                        {
                                            fprintf(file, "========================================\n");
                                            if (current_function == Y)
                                            {
                                                fprintf(file, "Функция: Y(x) = 2*sin(πx)*sin(3πx) - 0.5\n");
                                            }
                                            else
                                            {
                                                fprintf(file, "Функция: V(x) - кусочная функция\n");
                                            }
                                            fprintf(file, "========================================\n\n");

                                            fprintf(file, "Параметры табулирования:\n");
                                            fprintf(file, "Начало интервала: %.10g\n", start);
                                            fprintf(file, "Конец интервала: %.10g\n", end);
                                            fprintf(file, "Шаг: %.10g\n", step);
                                            fprintf(file, "Начальная точка: %s\n", include_start ? "включена" : "исключена");
                                            fprintf(file, "Конечная точка: %s\n", include_end ? "включена" : "исключена");
                                            fprintf(file, "\n");

                                            fprintf(file, "Результаты табулирования:\n");
                                            fprintf(file, "|----------|------------|\n");
                                            fprintf(file, "|    x     |     y      |\n");
                                            fprintf(file, "|----------|------------|\n");

                                            double x = include_start ? start : start + step;
                                            double limit = include_end ? end : end - step;

                                            while (x <= limit)
                                            {
                                                double y = calculate_function_value(current_function, x);
                                                fprintf(file, "| %8.2f | %10.4f |\n", x, y);
                                                x += step;
                                            }

                                            fprintf(file, "|----------|------------|\n");
                                            fclose(file);
                                            printf("Результаты сохранены в файл output.txt\n");
                                        }
                                    }
                                    break;
                                }
                            }
                            else
                            {
                                printf("\nТабулирование уже было выполнено для этих параметров.\n");
                                printf("Хотите выполнить табулирование снова? (1-да, 0-нет): ");

                                int tabulate_again;
                                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL ||
                                    sscanf(input_buffer, "%d", &tabulate_again) != 1)
                                {
                                    tabulate_again = 0;
                                }

                                if (tabulate_again == 1)
                                {
                                    tabulate_function(current_function, start, end, step, include_start, include_end);
                                }
                            }
                            break;
                        }

                        case 2:
                            result_plot = plot(start, end, current_function);
                            break;

                        case 3:
                        {
                            // Изменить параметры
                            printf("\nТекущие параметры:\n");
                            printf("Интервал: [%.10g, %.10g], Шаг: %.10g\n", start, end, step);
                            printf("Начальная точка: %s, Конечная точка: %s\n",
                                   include_start ? "включена" : "исключена",
                                   include_end ? "включена" : "исключена");

                            printf("\nВыберите способ изменения параметров:\n");
                            printf("1. Ввести новые параметры с клавиатуры\n");
                            printf("2. Загрузить из файла output.txt\n");
                            printf("3. Отмена (вернуться назад)\n");
                            printf("Ваш выбор (1-3): ");

                            int change_method;
                            if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL ||
                                sscanf(input_buffer, "%d", &change_method) != 1)
                            {
                                printf("Неверный ввод!\n");
                                break;
                            }

                            switch (change_method)
                            {
                            case 1:
                            {
                                printf("\nВведите новые параметры:\n");

                                int valid_input_start = 0;
                                do
                                {
                                    printf("Начало интервала: ");
                                    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL)
                                    {
                                        printf("Ошибка ввода. Попробуйте снова.\n");
                                        continue;
                                    }
                                    if (sscanf(input_buffer, "%lf", &start) == 1)
                                    {
                                        valid_input_start = 1;
                                    }
                                    else
                                    {
                                        printf("Неверный ввод! Введите число.\n");
                                    }
                                } while (!valid_input_start);

                                int valid_input_end = 0;
                                do
                                {
                                    printf("Конец интервала: ");
                                    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL)
                                    {
                                        printf("Ошибка ввода. Попробуйте снова.\n");
                                        continue;
                                    }
                                    if (sscanf(input_buffer, "%lf", &end) == 1)
                                    {
                                        valid_input_end = 1;
                                    }
                                    else
                                    {
                                        printf("Неверный ввод! Введите число.\n");
                                    }
                                } while (!valid_input_end);

                                do
                                {
                                    printf("Шаг табулирования: ");
                                    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL)
                                    {
                                        printf("Ошибка ввода. Попробуйте снова.\n");
                                        continue;
                                    }
                                    if (sscanf(input_buffer, "%lf", &step) != 1 || step <= 0)
                                    {
                                        printf("Шаг должен быть положительным числом! Попробуйте снова.\n");
                                    }
                                } while (step <= 0);

                                int valid_input_inc_start = 0;
                                do
                                {
                                    printf("Включить начальную точку? (0-нет, 1-да): ");
                                    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL)
                                    {
                                        printf("Ошибка ввода. Попробуйте снова.\n");
                                        continue;
                                    }
                                    if (sscanf(input_buffer, "%d", &include_start) == 1 &&
                                        (include_start == 0 || include_start == 1))
                                    {
                                        valid_input_inc_start = 1;
                                    }
                                    else
                                    {
                                        printf("Неверный ввод! Введите 0 или 1.\n");
                                    }
                                } while (!valid_input_inc_start);

                                int valid_input_inc_end = 0;
                                do
                                {
                                    printf("Включить конечную точку? (0-нет, 1-да): ");
                                    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL)
                                    {
                                        printf("Ошибка ввода. Попробуйте снова.\n");
                                        continue;
                                    }
                                    if (sscanf(input_buffer, "%d", &include_end) == 1 &&
                                        (include_end == 0 || include_end == 1))
                                    {
                                        valid_input_inc_end = 1;
                                    }
                                    else
                                    {
                                        printf("Неверный ввод! Введите 0 или 1.\n");
                                    }
                                } while (!valid_input_inc_end);

                                has_tabulated = 0;
                                printf("\nПараметры успешно изменены!\n");
                                break;
                            }

                            case 2:
                            {
                                result_read_func = read_params_from_file(&start, &end, &step, &include_start, &include_end);
                                if (result_read_func == 0)
                                {
                                    printf("\nПараметры успешно загружены из файла output.txt\n");
                                    has_tabulated = 0;
                                }
                                else
                                {
                                    printf("\nНе удалось загрузить параметры из файла.\n");
                                }
                                break;
                            }

                            case 3:
                                // Отмена изменения параметров
                                break;

                            default:
                                printf("Неверный выбор!\n");
                                break;
                            }
                            break;
                        }

                        case 4:
                            // Возврат к выбору действия с функцией
                            continue_with_params = 0;
                            continue_with_function = 1;
                            break;

                        default:
                            printf("Неверный выбор! Попробуйте снова.\n");
                            break;
                        }
                    }
                    break;
                }

                case 3:
                {
                    // Смена функции
                    printf("\nТекущая функция: ");
                    if (current_function == Y)
                    {
                        printf("Y(x) = 2*sin(πx)*sin(3πx) - 0.5\n");
                    }
                    else
                    {
                        printf("V(x) - кусочная функция\n");
                    }

                    if (has_parameters)
                    {
                        printf("Текущие параметры:\n");
                        printf("Интервал: [%.10g, %.10g], Шаг: %.10g\n", start, end, step);
                        printf("Начальная точка: %s, Конечная точка: %s\n",
                               include_start ? "включена" : "исключена",
                               include_end ? "включена" : "исключена");

                        printf("\nХотите использовать текущие параметры для новой функции?\n");
                        printf("1. Да, использовать текущие параметры\n");
                        printf("2. Нет, ввести новые параметры для новой функции\n");
                        printf("3. Отмена (оставить текущую функцию)\n");
                        printf("Ваш выбор (1-3): ");

                        int keep_params;
                        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL ||
                            sscanf(input_buffer, "%d", &keep_params) != 1)
                        {
                            printf("Неверный ввод! Отмена смены функции.\n");
                            break;
                        }

                        switch (keep_params)
                        {
                        case 1:
                            // Сохранение текущих параметров
                            break;

                        case 2:
                            // Ввод новых параметров
                            has_parameters = 0;
                            has_tabulated = 0;
                            break;

                        case 3:
                            // Отмена смены функции
                            continue;

                        default:
                            printf("Неверный выбор! Отмена смены функции.\n");
                            continue;
                        }
                    }

                    // Выбор новой функции
                    printf("\nВыберите новую функцию:\n");
                    printf("1. Y(x) = 2*sin(πx)*sin(3πx) - 0.5\n");
                    printf("2. V(x) - кусочная функция\n");
                    printf("3. Отмена (оставить текущую функцию)\n");
                    printf("Ваш выбор (1-3): ");

                    int new_function_choice;
                    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL ||
                        sscanf(input_buffer, "%d", &new_function_choice) != 1)
                    {
                        printf("Неверный ввод! Отмена смены функции.\n");
                        break;
                    }

                    switch (new_function_choice)
                    {
                    case 1:
                        current_function = Y;
                        printf("\nВыбрана функция: Y(x) = 2*sin(πx)*sin(3πx) - 0.5\n");
                        has_tabulated = 0;
                        break;

                    case 2:
                        current_function = V;
                        printf("\nВыбрана функция: V(x) - кусочная функция\n");
                        has_tabulated = 0;
                        break;

                    case 3:
                        printf("Смена функции отменена.\n");
                        break;

                    default:
                        printf("Неверный выбор! Отмена смены функции.\n");
                        break;
                    }
                    break;
                }

                case 4:
                    // Возврат в главное меню
                    continue_with_function = 0;
                    break;

                default:
                    printf("Неверный выбор! Попробуйте снова.\n");
                    break;
                }
            }
            break;
        }

        case 2:
        {
            // Работа с массивом
            double *arr = NULL; // Указатель на массив
            int size = 0; // Размер массива
            int arr_choice;

            printf("\nВыберите операции над массивом:\n");
            printf("1. Сгенерировать массив данных\n");
            printf("2. Считать данные из файла\n");
            printf("3. Вернуться в главное меню\n");
            printf("Ваш выбор: ");

            if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL ||
                sscanf(input_buffer, "%d", &arr_choice) != 1)
            {
                printf("Неверный ввод!\n");
                break;
            }

            switch (arr_choice)
            {
            case 1:
            {
                printf("Введите размер массива: ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL ||
                    sscanf(input_buffer, "%d", &size) != 1 || size <= 0)
                {
                    printf("Неверный размер массива! Размер должен быть положительным числом\n");
                    break;
                }

                // Выделение памяти
                arr = (double *)malloc(size * sizeof(double));
                if (arr == NULL)
                {
                    printf("Ошибка выделения памяти для массива!\n");
                    break;
                }

                arr = full_array(arr, size);
                put_array(arr, size);

                printf("\nСохранить массив в файл? (1-да, 0-нет): ");
                int save_choice;
                if (fgets(input_buffer, sizeof(input_buffer), stdin) != NULL &&
                    sscanf(input_buffer, "%d", &save_choice) == 1 && save_choice == 1)
                {
                    int result = write_array_to_file(arr, size);
                    if (result == 0)
                    {
                        printf("Массив успешно сохранен в файл array_data.txt\n");
                    }
                    else
                    {
                        printf("Ошибка сохранения массива в файл!\n");
                    }
                }

                free(arr);
                arr = NULL; // Сброс указателя на массив
                size = 0;   // Сброс размера массива
                break;
            }

            case 2:
            {
                int result = read_array_from_file(&arr, &size);

                switch (result)
                {
                case 0:
                    printf("Массив успешно загружен из файла array_data.txt\n");
                    put_array(arr, size);
                    break;

                case -2:
                    printf("Файл array_data.txt не найден!\n");
                    break;

                case -3:
                case -5:
                    printf("Ошибка чтения данных из файла!\n");
                    break;

                case -4:
                    printf("Неверный размер массива в файле!\n");
                    break;

                default:
                    printf("Неизвестная ошибка при чтении файла!\n");
                    break;
                }

                // Освобождение памяти, если массив был загружен
                if (arr != NULL)
                {
                    free(arr);
                    arr = NULL; // Сброс указателя
                    size = 0;   // Сброс размера массива
                }
                break;
            }

            case 3:
                // Возвращение в главное меню
                break;

            default:
                printf("Неверный выбор!\n");
                break;
            }
            break;
        }

        case 3:
            program_running = 0;
            break;

        default:
            printf("Неверный выбор! Попробуйте снова.\n");
            break;
        }
    }

    return 0;
}

//--- Математические функции ---

// Y(x) = 2*sin(πx)*sin(3πx) - 0.5
double Y(double x)
{
    return 2 * sin(M_PI * x) * sin(3 * M_PI * x) - 0.5;
}

// V(x) - кусочная функция
double V(double x)
{
    if (x < 0)
    {
        // Проверка деления на ноль
        if (1 + pow(x, 2) == 0)
        {
            return 0;
        }
        return (1 + 2 * x) / (1 + pow(x, 2));
    }
    else if (x < 1)
    {
        // Проверка корня из отрицательного числа
        if (1 + x < 0)
        {
            return 0;
        }
        return pow(sin(x), 2) * sqrt(1 + x);
    }
    else
    {
        return pow(sin(x), 2) * exp(0.2 * x);
    }
}

double calculate_function_value(TFun f, double x)
{
    return f(x);
}

int plot(double x0, double x1, TFun f)
{
    const int SCREENW = 80, SCREENH = 20;
    char screen[80][20];
    double ymin = 1e10, ymax = -1e10;
    double hx, hy;
    int i, j;
    int xz, yz;

    if (x0 >= x1)
    {
        printf("Неверный интервал: x0 должен быть меньше x1\n");
        return -1;
    }

    hx = (x1 - x0) / (SCREENW - 1);

    // Нахождение min и max значения функции
    for (i = 0; i < SCREENW; ++i)
    {
        double x = x0 + i * hx;
        double y_val = calculate_function_value(f, x);
        if (y_val < ymin)
            ymin = y_val;
        if (y_val > ymax)
            ymax = y_val;
    }

    // Если функция постоянная
    if (ymax - ymin < 1e-10)
    {
        ymin -= 1;
        ymax += 1;
    }

    double yrange = ymax - ymin;
    ymin -= yrange * 0.1;
    ymax += yrange * 0.1;

    hy = (ymax - ymin) / (SCREENH - 1);

    // Нахождение положение осей
    yz = (int)((ymax - 0) / hy + 0.5);
    if (yz < 0)
        yz = 0;
    if (yz >= SCREENH)
        yz = SCREENH - 1;

    xz = (int)((0 - x0) / hx + 0.5);
    if (xz < 0)
        xz = 0;
    if (xz >= SCREENW)
        xz = SCREENW - 1;

    // Инициализация экрана
    for (j = 0; j < SCREENH; ++j)
    {
        for (i = 0; i < SCREENW; ++i)
        {
            screen[i][j] = ' ';
        }
    }

    // Рисуем оси
    for (i = 0; i < SCREENW; ++i)
    {
        screen[i][yz] = '-';
    }
    for (j = 0; j < SCREENH; ++j)
    {
        screen[xz][j] = '|';
    }
    screen[xz][yz] = '+';

    // Рисуем график
    for (i = 0; i < SCREENW; ++i)
    {
        double x = x0 + i * hx;
        double y_val = calculate_function_value(f, x);

        // Рассчитываем позицию Y на экране
        j = (int)((ymax - y_val) / hy + 0.5);

        // Проверяем, чтобы точка не выходила за границы экрана
        if (j >= 0 && j < SCREENH)
        {
            // Если на этой позиции уже есть символ оси, рисуем поверх
            if (screen[i][j] == ' ')
            {
                screen[i][j] = '*';
            }
            else if (screen[i][j] == '-' || screen[i][j] == '|')
            {
                screen[i][j] = '*'; // Точка на оси
            }
        }
    }

    printf("\nГрафик функции:\n");
    for (j = 0; j < SCREENH; ++j)
    {
        for (i = 0; i < SCREENW; ++i)
        {
            putchar(screen[i][j]);
        }
        putchar('\n');
    }

    return 0;
}

void tabulate_function(TFun f, double start, double end, double step, int include_start, int include_end)
{
    printf("\n");
    printf("|----------|------------|\n");
    printf("|    x     |     y      |\n");
    printf("|----------|------------|\n");

    double x = include_start ? start : start + step;
    double limit = include_end ? end : end - step;

    while (x <= limit)
    {
        double y = calculate_function_value(f, x);
        printf("| %8.2f | %10.4f |\n", x, y);
        x += step;
    }

    printf("|----------|------------|\n");
}

// Функция для чтения параметров из файла
int read_params_from_file(double *start, double *end, double *step, int *include_start, int *include_end)
{
    FILE *file = fopen("output.txt", "r");
    if (!file)
    {
        printf("Ошибка открытия файла output.txt для чтения\n");
        return 1;
    }

    char line[256];
    int params_found = 0;

    while (fgets(line, sizeof(line), file))
    {
        // Пропуск пустых строк
        if (strlen(line) == 0)
            continue;

        if (strstr(line, "Параметры табулирования:") != NULL)
            continue;

        if (strstr(line, "Результаты табулирования:") != NULL)
            break;

        if (strstr(line, "Начало интервала:") != NULL)
        {
            if (sscanf(line, "Начало интервала: %lf", start) == 1)
                params_found++;
        }
        else if (strstr(line, "Конец интервала:") != NULL)
        {
            if (sscanf(line, "Конец интервала: %lf", end) == 1)
                params_found++;
        }
        else if (strstr(line, "Шаг:") != NULL)
        {
            if (sscanf(line, "Шаг: %lf", step) == 1)
                params_found++;
        }
        else if (strstr(line, "Начальная точка:") != NULL)
        {
            char value[20];
            if (sscanf(line, "Начальная точка: %19s", value) == 1)
            {
                if (strcmp(value, "включена") == 0)
                    *include_start = 1;
                else if (strcmp(value, "исключена") == 0)
                    *include_start = 0;
                else
                    continue; // Некорректное значение
                params_found++;
            }
        }
        else if (strstr(line, "Конечная точка:") != NULL)
        {
            char value[20];
            if (sscanf(line, "Конечная точка: %19s", value) == 1)
            {
                if (strcmp(value, "включена") == 0)
                    *include_end = 1;
                else if (strcmp(value, "исключена") == 0)
                    *include_end = 0;
                else
                    continue; // Некорректное значение
                params_found++;
            }
        }
    }

    fclose(file);

    // Проверяем, что все 5 параметров были найдены
    if (params_found == 5)
    {
        return 0; // Успешно
    }
    else
    {
        return 1; // Ошибка
    }
}

//--- Функции для работы с массивом ---

// Генерация массива
double* full_array(double *arr, int size)
{
    if (arr == NULL || size <= 0)
    {
        return arr;
    }

    for (int i = 0; i < size; i++)
    {
        double num = (double)rand() / RAND_MAX * 50.0;
        arr[i] = num;
    }
    return arr;
}

int read_array_from_file(double **arr, int *size)
{
    if (arr == NULL || size == NULL)
    {
        return -1; // Неверные параметры
    }

    FILE *file = fopen("array_data.txt", "r");
    if (file == NULL)
    {
        return -2; // Файл не найден
    }

    // Чтение размера массива
    int file_size;
    if (fscanf(file, "%d", &file_size) != 1)
    {
        fclose(file);
        return -3; // Ошибка чтения размера массива
    }

    if (file_size <= 0)
    {
        fclose(file);
        return -4; // Неверный размер
    }

    double *temp = (double *)realloc(*arr, file_size * sizeof(double));
    if (temp == NULL)
    {
        fclose(file);
        return -5; // Ошибка выделения памяти
    }

    *arr = temp; // Обновление указателя

    for (int i = 0; i < file_size; i++)
    {
        if (fscanf(file, "%lf", &(*arr)[i]) != 1)
        {
            fclose(file);
            return -6; // Ошибка чтения элемента
        }
    }

    *size = file_size; // Обновление размера
    fclose(file);
    return 0; // Успешно
}

// Запись массива в файл
int write_array_to_file(double *arr, int size)
{
    if (arr == NULL || size <= 0)
    {
        return -1; // Неверные параметры
    }

    FILE *file = fopen("array_data.txt", "w");
    if (file == NULL)
    {
        return -2; // Ошибка открытия файла
    }

    // Записываем размер массива
    fprintf(file, "%d\n", size);

    // Записываем элементы массива
    for (int i = 0; i < size; i++)
    {
        fprintf(file, "%.3f\n", arr[i]);
    }

    fclose(file);
    return 0; // Успешно
}

// Отображение массива
void put_array(double *arr, int size)
{
    if (arr == NULL || size <= 0)
    {
        printf("Массив пуст или не существует!\n");
        return;
    }

    printf("\nРезультаты операции над массивом:\n");
    printf("    \n");
    printf("    | i  |  arr[i]  |\n");
    printf("    |----|----------|\n");
    for (int i = 0; i < size; i++)
    {
        printf("    | %2d | %8.3f |\n", i, arr[i]);
    }
    printf("\n");
}