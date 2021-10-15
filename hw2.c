#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define COLUMN_FINAL_ARR 2

#define NOT_OPEN_FILE (-1)
#define WRONG_MATRIX (-2)
#define FAILED_ALLOCATE_MEMORY (-3)

void deepInGraph(int* arr, unsigned int length_str, unsigned int length_column, unsigned int cur_str, int* flag);

int main(void) {

    FILE * text = fopen("mat_inc.txt", "r");

    if (!text) 
        return NOT_OPEN_FILE;

    rewind(text);

    int str = 0;
    int column = 1;

    while (!feof(text)) // строки
         if (fgetc(text) == '\n')
             str++;

    rewind(text);

    while (fgetc(text) != '\n')  //столбцы
        column++;

    int **mat_inc;
    mat_inc = (int**)calloc(str, sizeof(int*));

    if (mat_inc == NULL) {
        printf("FAILED_ALLOCATE_MEMORY");
        return FAILED_ALLOCATE_MEMORY;
    }
    for (int i = 0; i < str; i++) {
        mat_inc[i] = (int*)calloc(column, sizeof(int));

        if (mat_inc[i] == NULL) {
            printf("FAILED_ALLOCATE_MEMORY");
            return FAILED_ALLOCATE_MEMORY;
        }
    }

    rewind(text);

    for (int i = 0; i < str; i++) { // ввод в масисив с файла
        for (int j = 0; j < column; j++) {
            if (!feof(text))
                mat_inc[i][j] = fgetc(text);
        }
    }
    fclose(text);

    int count_rib_graph = 0; // кол-во рёбер в графе
    for (int j = 0; j < column; j++) {
            if ((mat_inc[0][j] >= 'a') && (mat_inc[0][j] <= 'z')) {
                count_rib_graph++;
            }
    }

    int flag = 0;

    int **final_arr;
    int str_final_arr = (column / 2) + 1;
    final_arr = (int**)calloc(str_final_arr, sizeof(int*));

    if (final_arr == NULL) {
            printf("FAILED_ALLOCATE_MEMORY");
            return FAILED_ALLOCATE_MEMORY;
    }

    for (int i = 0; i < str_final_arr; i++) {
        final_arr[i] = (int*)calloc(COLUMN_FINAL_ARR, sizeof(int));

        if (final_arr[i] == NULL) {
            printf("FAILED_ALLOCATE_MEMORY");
            return FAILED_ALLOCATE_MEMORY;
        }
    }

    int count_str_final_arr = 0;
    int save = 0;

    for (int i = 2; i < column; i++) { // преобразование полученных данных для ЯП DOT
        if (!isdigit(mat_inc[1][i])) {
            continue;
        }
        for (int j = 1; j < str; j++) {
            if (mat_inc[j][i] == '1') {
                final_arr[count_str_final_arr][flag] = j;
                flag++;
            }
        }
        if (flag == 3) {
            printf("WRONG_MATRIX\n");
            return WRONG_MATRIX;
        }
        if (flag == 1) { // петля
            final_arr[count_str_final_arr][flag] = final_arr[count_str_final_arr][flag - 1];
        }
        flag = 0;
        count_str_final_arr++;
    }

    for (int i = 1; i < str; i++) {     // Определение отдельных не связных вершин графа
        for (int j = 2; j < column; j++) {
            if (!isdigit(mat_inc[i][j]))
                continue;
            if (mat_inc[i][j] == '1') {
                flag = 0;
                break;
            } else {
                flag++;
            }
        }
        if (flag != 0) {
            final_arr[count_str_final_arr][0] = i;
            final_arr[count_str_final_arr++][1] = -1;
        }
    }

    FILE * for_dot = fopen("out.gv", "w+");

    if (!for_dot) 
        return NOT_OPEN_FILE;

    fprintf(for_dot, "graph NEW {\n"); // Заполненние файла

    for (int i = 0; i < count_str_final_arr; i++) {

        if (final_arr[i][1] == -1) {
            fprintf(for_dot, "\t%d\n", final_arr[i][0]);
        } else {
            fprintf(for_dot, "\t%d -- %d\n", final_arr[i][0], final_arr[i][1]);
        }
    }

    fprintf(for_dot, "}");

    fclose(for_dot);

    system("dot -Tpng out.gv -o out.jpg");
    system("wslview out.jpg");

    int * two_dem_in_one_dem = (int *)calloc(count_rib_graph*(str - 1), sizeof(int)); 

    if (two_dem_in_one_dem == NULL) {
        printf("FAILED_ALLOCATE_MEMORY");
        return FAILED_ALLOCATE_MEMORY;
    }

    int counter_tdInod = 0;
    
    for (int i = 1; i < str; i++) {  // Представление двумерного массива в одномерном
        for (int j = 2; j < column; j++) {
            if ((mat_inc[i][j] == '1') || (mat_inc[i][j] == '0')) {
                two_dem_in_one_dem[counter_tdInod++] = mat_inc[i][j] - '0';
            }
        }
    }

    int* flag_visit = calloc(str, sizeof(int));

    if (flag_visit == NULL) {
        printf("FAILED_ALLOCATE_MEMORY");
        return FAILED_ALLOCATE_MEMORY;
    }
    
    flag_visit[0] = 1;
    int cur_str = 0;

    deepInGraph(two_dem_in_one_dem, str, count_rib_graph, cur_str, flag_visit); // Определение связанности графа

    int eventyally = 1;
    for (int i = 0; i < str; i++) {
        eventyally = eventyally * flag_visit[i];
    }
    if (eventyally) {
        printf("Граф связный\n");
    } else {
        printf("Граф не связный\n");
    }


    for (int i = 0; i < str; i++) {
        free(mat_inc[i]);
    }
    free(mat_inc);

    for (int i = 0; i < str_final_arr; i++) {
        free(final_arr[i]);
    }
    free(final_arr);
    
    free(two_dem_in_one_dem);

    free(flag_visit);

    return 0;
}


void deepInGraph(int* arr, unsigned int length_str, unsigned int length_column, unsigned int cur_str, int* flag) { // функция для продвижение вглубь матрицы
    unsigned int count_column;
    unsigned int count_str;
    unsigned int tmp;

    for (count_column = 0; count_column < length_column; count_column++) {
        if (arr[cur_str * length_column + count_column]) {
            for (count_str = 0; count_str < length_str; count_str++) {
                if (arr[count_str * length_column + count_column]) {
                    if (!flag[count_str]) {
                        flag[count_str] = 1;
                        tmp = cur_str;
                        cur_str = count_str;
                        deepInGraph(arr, length_str, length_column, cur_str, flag);
                        cur_str = tmp;
                    }
                }
            }
        }
    }
}
