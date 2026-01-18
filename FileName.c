#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>

#define PI 3.14159265
#define H 0.0001
#define DERIVATIVE(f,x) (((f)((x)+H)-(f)((x)-H))/(2*H))

double f(double x);
double derivative(double x);
void tablica(double a, double b, int n);
double poisk_min(double a, double b, int n, double* min_x);
double poisk_max(double a, double b, int n, double* max_x);
double poisk_x_po_y(double Y, double a, double b, double shag);
int save_table_to_file(double a, double b, int n, const char* filename);
double* generate_random_values(double a, double b, int count);
int tablica_arr(double* x_arr, int size);  // изменено на int

int main() {
    setlocale(LC_ALL, "RUS");
    srand(time(NULL));

    printf("Программа анализа функции f(x)\n");
    printf("f(x) = {\n");
    printf("  tan(x)/x,                   x < -p/6\n");
    printf("  (x^2 - 9)/(x + 3),        -p/6 <= x < 2\n");
    printf("  (x^n/(n^3+1)), n=0..11,  x >= 2\n");
    printf("}\n");

    int vibor;

    do {
        printf("\n       ГЛАВНОЕ МЕНЮ     \n");
        printf("1. Значение функции в точке\n");
        printf("2. Таблица значений на интервале\n");
        printf("3. Минимум на отрезке\n");
        printf("4. Максимум на отрезке\n");
        printf("5. Найти x по заданному Y\n");
        printf("6. Производная в точке\n");
        printf("7. Сохранить таблицу в файл\n");
        printf("8. Сгенерировать массив случайных x\n");
        printf("9. Таблица значений из массива x\n");
        printf("0. Выход\n");
        printf("Выберите пункт (0-9): ");
        scanf("%d", &vibor);

        double a, b;
        int n;
        char filename[100];

        switch (vibor) {
        case 1: {
            double x;
            printf("x = ");
            scanf("%lf", &x);
            double result = f(x);
            if (result != result) printf("f(%.3f) не вычисляется\n", x);
            else printf("f(%.3f) = %.3f\n", x, result);
            break;
        }

        case 2: {
            printf("a = ");
            scanf("%lf", &a);
            printf("b = ");
            scanf("%lf", &b);
            printf("n = ");
            scanf("%d", &n);
            if (n <= 0) printf("Ошибка: n > 0\n");
            else tablica(a, b, n);
            break;
        }

        case 3: {
            double min_x;
            printf("a = ");
            scanf("%lf", &a);
            printf("b = ");
            scanf("%lf", &b);
            printf("n = ");
            scanf("%d", &n);
            if (n <= 0) printf("Ошибка: n > 0\n");
            else {
                double min_y = poisk_min(a, b, n, &min_x);
                printf("Минимум: f(%.4f) = %.3f\n", min_x, min_y);
            }
            break;
        }

        case 4: {
            double max_x;
            printf("a = ");
            scanf("%lf", &a);
            printf("b = ");
            scanf("%lf", &b);
            printf("n = ");
            scanf("%d", &n);
            if (n <= 0) printf("Ошибка: n > 0\n");
            else {
                double max_y = poisk_max(a, b, n, &max_x);
                printf("Максимум: f(%.4f) = %.3f\n", max_x, max_y);
            }
            break;
        }

        case 5: {
            double Y, shag;
            printf("Y = ");
            scanf("%lf", &Y);
            printf("a = ");
            scanf("%lf", &a);
            printf("b = ");
            scanf("%lf", &b);
            printf("shag = ");
            scanf("%lf", &shag);
            double result = poisk_x_po_y(Y, a, b, shag);
            if (result == 0) printf("Решений не найдено\n");
            break;
        }

        case 6: {
            double x;
            printf("x = ");
            scanf("%lf", &x);
            double proizv = derivative(x);
            if (proizv != proizv) printf("Не могу вычислить производную\n");
            else printf("f'(%.3f) = %.2f\n", x, proizv);
            break;
        }

        case 7: {
            printf("a = ");
            scanf("%lf", &a);
            printf("b = ");
            scanf("%lf", &b);
            printf("n = ");
            scanf("%d", &n);
            printf("Имя файла: ");
            scanf("%s", filename);
            if (n <= 0) printf("Ошибка: n > 0\n");
            else {
                if (save_table_to_file(a, b, n, filename) == 0)
                    printf("Таблица сохранена в файл: %s\n", filename);
                else printf("Ошибка сохранения файла!\n");
            }
            break;
        }

        case 8: {
            int count;
            printf("a = ");
            scanf("%lf", &a);
            printf("b = ");
            scanf("%lf", &b);
            printf("Количество случайных значений: ");
            scanf("%d", &count);
            if (count <= 0) printf("Ошибка: количество > 0\n");
            else {
                double* values = generate_random_values(a, b, count);
                if (values != NULL) {
                    printf("\nСгенерированные значения x:\n");
                    printf("Интервал: [%.2f, %.2f]\n", a, b);
                    printf("Количество: %d\n\n", count);
                    for (int i = 0; i < count; i++) {
                        double y = f(values[i]);
                        printf("x[%d] = %8.3f, f(x) = ", i, values[i]);
                        if (y != y) printf("не вычисляется\n");
                        else printf("%10.6f\n", y);
                    }
                    free(values);
                }
                else printf("Ошибка генерации значений!\n");
            }
            break;
        }

        case 9: {
            int size;
            printf("Введите размер массива x: ");
            scanf("%d", &size);
            if (size <= 0) printf("Ошибка: размер > 0\n");
            else {
                double* x_arr = (double*)malloc(size * sizeof(double));
                if (x_arr == NULL) printf("Ошибка выделения памяти\n");
                else {
                    printf("Введите %d значений x:\n", size);
                    for (int i = 0; i < size; i++) {
                        printf("x[%d] = ", i);
                        scanf("%lf", &x_arr[i]);
                    }
                    int result = tablica_arr(x_arr, size);
                    if (result == 0) printf("Таблица выведена успешно\n");
                    free(x_arr);
                }
            }
            break;
        }

        case 0:
            printf("Выход из программы\n");
            break;

        default:
            printf("Ошибка! Введите число от 0 до 9\n");
            break;
        }

    } while (vibor != 0);

    return 0;
}

double f(double x) {
    double result;

    if (x < -PI / 6) {
        if (fabs(x) < 0.000001) return 1;
        if (fabs(cos(x)) < 0.000001) return 0;
        result = tan(x) / x;
    }
    else if (x >= -PI / 6.0 && x < 2.0) {
        if (fabs(x + 3.0) < 0.000001) return -3;
        result = (x * x - 9.0) / (x + 3.0);
    }
    else {
        result = 0.0;
        for (int n = 0; n <= 11; n++) {
            double chislitel = pow(x, n);
            double znamenatel = sqrt((pow(n, 3)) + 1.0);
            result += chislitel / znamenatel;
        }
    }

    return result;
}

double derivative(double x) {
    return DERIVATIVE(f, x);
}

void tablica(double a, double b, int n) {
    printf("\n   ТАБЛИЦА ЗНАЧЕНИЙ   \n");
    printf("   x        f(x)\n");
    printf("                \n");

    double shag = (b - a) / n;
    for (int i = 0; i <= n; i++) {
        double x = a + i * shag;
        double y = f(x);
        if (y != y) printf("%8.3f   не вычисляется\n", x);
        else printf("%8.3f   %10.6f\n", x, y);
    }
}

double poisk_min(double a, double b, int n, double* min_x) {
    double shag = (b - a) / n;
    double min_y;

    for (int i = 0; i <= n; i++) {
        double x = a + i * shag;
        double y = f(x);
        if (y == y) {
            *min_x = x;
            min_y = y;
            break;
        }
    }

    for (int i = 0; i <= n; i++) {
        double x = a + i * shag;
        double y = f(x);
        if (y == y && y < min_y) {
            min_y = y;
            *min_x = x;
        }
    }

    return min_y;
}

double poisk_max(double a, double b, int n, double* max_x) {
    double shag = (b - a) / n;
    double max_y;

    for (int i = 0; i <= n; i++) {
        double x = a + i * shag;
        double y = f(x);
        if (y == y) {
            *max_x = x;
            max_y = y;
            break;
        }
    }

    for (int i = 0; i <= n; i++) {
        double x = a + i * shag;
        double y = f(x);
        if (y == y && y > max_y) {
            max_y = y;
            *max_x = x;
        }
    }

    return max_y;
}

double poisk_x_po_y(double Y, double a, double b, double shag) {
    printf("Ищем x: f(x) ≈ %.3f на [%.2f, %.2f]\n", Y, a, b);

    int naydeno = 0;
    double first_solution = 0.0;

    for (double x = a; x <= b; x += shag) {
        double fx = f(x);
        if (fx == fx) {
            if (fabs(fx - Y) < 0.000001) {
                naydeno++;
                printf("Найдено: x ≈ %.4f, f(x) = %.6f\n", x, fx);
                if (naydeno == 1) first_solution = x;
            }
        }
    }

    printf("Всего найдено решений: %d\n", naydeno);
    return first_solution;
}

int save_table_to_file(double a, double b, int n, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) return -1;

    fprintf(file, "ТАБЛИЦА ЗНАЧЕНИЙ ФУНКЦИИ\n");
    fprintf(file, "Интервал: [%.2f, %.2f], шагов: %d\n\n", a, b, n);
    fprintf(file, "    x        f(x)\n");
    fprintf(file, "-------------------\n");

    double shag = (b - a) / n;
    for (int i = 0; i <= n; i++) {
        double x = a + i * shag;
        double y = f(x);
        if (y != y) fprintf(file, "%8.3f   не вычисляется\n", x);
        else fprintf(file, "%8.3f   %10.6f\n", x, y);
    }

    fclose(file);
    return 0;
}

double* generate_random_values(double a, double b, int count) {
    if (count <= 0 || a > b) return NULL;

    double* values = malloc(count * sizeof(double));
    if (values == NULL) return NULL;

    for (int i = 0; i < count; i++) {
        values[i] = a + ((double)rand() / RAND_MAX) * (b - a);
    }
    return values;
}

int tablica_arr(double* x_arr, int size) {
    if (size <= 0) return -1;

    printf("\n   ТАБЛИЦА ЗНАЧЕНИЙ ИЗ МАССИВА\n");
    printf("   x        f(x)\n");
    printf("-------------------\n");

    for (int i = 0; i < size; i++) {
        double y = f(x_arr[i]);
        if (y != y) printf("%8.3f   не вычисляется\n", x_arr[i]);
        else printf("%8.3f   %10.6f\n", x_arr[i], y);
    }

    return 0;
}