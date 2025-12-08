#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <locale.h>
#include <string.h>
#include <ctype.h>

// Объявление типа указателя на функцию
typedef double (*TFun)(double);

// Объявление прототипов функций
double Y(double x);
double V(double x);
double calculate_function_value(TFun f, double x);
int plot(double x0, double x1, TFun f);
int tabulate_function(TFun f, double start, double end, double step, int include_start, int include_end);
int read_params_from_file(double *start, double *end, double *step, int *include_start, int *include_end);
int save_params_to_file(double start, double end, double step, int include_start, int include_end);
int save_results_to_file(TFun f, double start, double end, double step, int include_start, int include_end);

int main()
{
    setlocale(LC_ALL, "RUS");

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

    printf("Доступные функции:\n");
    printf("\n1. Y(x) = 2*sin(πx)*sin(3πx) - 0.5\n");
    printf("2. V(x) - кусочно-заданная функция:\n");
    printf("\tесли x < 0: (1 + 2x) / (1 + x^2),\n");
    printf("\tесли 0 ≤ x < 1: sin^2(x) * √(1 + x),\n");
    printf("\tесли x ≥ 1: sin^2(x) * e^(0.2x),\n");
    printf("============================================\n\n");

    TFun current_function = NULL;
    double start = 0, end = 0, step = 0;
    int include_start = 0, include_end = 0;
    int function_choice, action_choice;
    double x_value;
    int program_running = 1; // Флаг работы программы
    int has_tabulated = 0;   // Флаг, определяющий было ли табулирование
    int has_parameters = 0;  // Флаг наличия параметров

    char input_buffer[100]; // Буфер для ввода

    int result_read_func;
    int result_save_params_func;
    int result_save_results_func;
    int result_plot;

    while (program_running)
    {
        if (current_function == NULL)
        {
            do
            {
                printf("1. Y(x)\n");
                printf("2. V(x)\n");
                printf("3. Завершение работы программы\n");
                printf("Выберите функцию (1-3): ");

                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL)
                {
                    printf("Ошибка ввода.\n");
                    continue;
                }

                if (sscanf(input_buffer, "%d", &function_choice) != 1)
                {
                    printf("Неверный ввод! Введите число от 1 до 3.\n");
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
                case 3:
                    program_running = 0;
                    continue;
                default:
                    printf("Неверный выбор! Попробуйте снова.\n");
                    continue;
                }
            } while (function_choice < 1 || function_choice > 3);

            if (!program_running)
            {
                break;
            }

            // Ввод параметров для новой функции
            printf("Выберете способ ввода параметров:\n");
            printf("\n1. Ввести с клавиатуры\n");
            printf("2. Загрузить из файла output.txt\n");
            printf("Ваш выбор (1-2): ");

            int input_method;
            if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL)
            {
                printf("Неверный ввод! Будет использован ввод с клавиатуры.\n");
                input_method = 1;
            }
            else if (sscanf(input_buffer, "%d", &input_method) != 1)
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

                printf("\nСохранить параметры в файл output.txt? (1-да, 0-нет): ");
                int save_params;
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL ||
                    sscanf(input_buffer, "%d", &save_params) != 1)
                {
                    save_params = 0;
                }

                if (save_params == 1)
                {
                    result_save_params_func = save_params_to_file(start, end, step, include_start, include_end);
                }
            }

            if (has_parameters)
            {
                has_tabulated = 0; // Сброс флага табулирования для новой функции
            }
        }

        int continue_with_function = 1; // Флаг продолжения работы с текущей функцией

        while (continue_with_function && program_running)
        {
            printf("\nТекущие параметры:\n");
            printf("Интервал: [%.10g, %.10g], Шаг: %.10g\n", start, end, step);
            printf("Начальная точка: %s, Конечная точка: %s\n",
                   include_start ? "включена" : "исключена",
                   include_end ? "включена" : "исключена");
            printf("\nДоступные действия:\n");
            printf("1. Вычислить значение функции в точке\n");
            printf("2. Табулировать функцию\n");
            printf("3. Построить график функции\n");
            printf("4. Изменить параметры\n");
            printf("5. Сменить функцию\n");
            printf("6. Сохранить параметры в файл\n");
            printf("7. Завершение работы программы\n");
            printf("Выберите действие (1-7): ");

            if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL)
            {
                printf("Ошибка ввода.\n");
                continue;
            }

            if (sscanf(input_buffer, "%d", &action_choice) != 1)
            {
                printf("Неверный ввод! Введите число от 1 до 7.\n");
                continue;
            }

            switch (action_choice)
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

                    if (output_choice == 1 || output_choice == 3)
                    {
                        int points_count = tabulate_function(current_function, start, end, step, include_start, include_end);
                        has_tabulated = 1;
                    }

                    if (output_choice == 2 || output_choice == 3)
                    {
                        result_save_results_func = save_results_to_file(current_function, start, end, step, include_start, include_end);
                        if (result_save_results_func == 0)
                        {
                            has_tabulated = 1;
                        }
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
                        int points_count = tabulate_function(current_function, start, end, step, include_start, include_end);
                    }
                }
                break;
            }

            case 3:
                result_plot = plot(start, end, current_function);
                break;

            case 4:
            {
                // Изменение параметров
                printf("\nВыберете способ ввода параметров:\n");
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
                        printf("Начало интервала: %.10g\n", start);
                        printf("Конец интервала: %.10g\n", end);
                        printf("Шаг: %.10g\n", step);
                        printf("Начальная точка: %s\n", include_start ? "включена" : "исключена");
                        printf("Конечная точка: %s\n", include_end ? "включена" : "исключена");
                        has_parameters = 1;
                        has_tabulated = 0;
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

                    has_parameters = 1;
                    has_tabulated = 0;

                    printf("Параметры успешно обновлены!\n");

                    printf("\nСохранить параметры в файл output.txt? (1-да, 0-нет): ");
                    int save_params;
                    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL ||
                        sscanf(input_buffer, "%d", &save_params) != 1)
                    {
                        save_params = 0;
                    }

                    if (save_params == 1)
                    {
                        result_save_params_func = save_params_to_file(start, end, step, include_start, include_end);
                    }
                }
                break;
            }

            case 5:
            {
                int keep_params;
                printf("Текущие параметры:\n");
                printf("Интервал: [%.10g, %.10g], Шаг: %.10g\n", start, end, step);
                printf("Хотите сохранить эти параметры для новой функции?\n");
                printf("1. Да, использовать текущие параметры\n");
                printf("2. Нет, ввести новые параметры\n");
                printf("3. Отмена, вернуться к текущей функции\n");

                int valid_input_keep_params = 0;
                do
                {
                    printf("Выберите вариант (1-3): ");
                    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL)
                    {
                        printf("Ошибка ввода. Попробуйте снова.\n");
                        continue;
                    }
                    if (sscanf(input_buffer, "%d", &keep_params) == 1 &&
                        keep_params >= 1 && keep_params <= 3)
                    {
                        valid_input_keep_params = 1;
                    }
                    else
                    {
                        printf("Неверный ввод! Введите 1, 2 или 3.\n");
                    }
                } while (!valid_input_keep_params);

                if (keep_params == 3)
                {
                    break;
                }

                current_function = NULL;
                continue_with_function = 0;
                has_tabulated = 0; // Сброс флага табулирования при смене функции
                break;
            }

            case 6:
            {
                result_save_params_func = save_params_to_file(start, end, step, include_start, include_end);
                break;
            }

            case 7:
                program_running = 0;
                continue_with_function = 0;
                break;

            default:
                printf("Неверный выбор! Попробуйте снова.\n");
                break;
            }
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

int tabulate_function(TFun f, double start, double end, double step, int include_start, int include_end)
{
    int count = 0;

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
        count++;
    }

    printf("|----------|------------|\n");
    return count;
}

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

int save_params_to_file(double start, double end, double step, int include_start, int include_end)
{
    FILE *file = fopen("output.txt", "w");
    if (!file)
    {
        printf("Ошибка открытия файла output.txt для записи\n");
        return 1;
    }

    fprintf(file, "Параметры табулирования:\n");
    fprintf(file, "Начало интервала: %.10g\n", start);
    fprintf(file, "Конец интервала: %.10g\n", end);
    fprintf(file, "Шаг: %.10g\n", step);
    fprintf(file, "Начальная точка: %s\n", include_start ? "включена" : "исключена");
    fprintf(file, "Конечная точка: %s\n", include_end ? "включена" : "исключена");

    fclose(file);
    printf("Параметры сохранены в файл output.txt\n");
    return 0;
}

int save_results_to_file(TFun f, double start, double end, double step, int include_start, int include_end)
{
    FILE *file = fopen("output.txt", "w");
    if (!file)
    {
        printf("Ошибка открытия файла output.txt для записи\n");
        return 1;
    }

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
        double y = calculate_function_value(f, x);
        fprintf(file, "| %8.2f | %10.4f |\n", x, y);
        x += step;
    }

    fprintf(file, "|----------|------------|\n");
    fclose(file);
    printf("Результаты сохранены в файл output.txt\n");
    return 0;
}