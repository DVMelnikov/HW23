#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define COLUMN_FINAL_ARR 2


/*Проверки: 
    Удалось ли открыть файл
    Правильные ли символы введены в матрицу инцидентности 
*/

// Определние количества строк и столбцов в файле
// Создания массива
// Выделение памяти
// Чтение с файла
// Перевод в массив, который нужно отправить в прогу для построения графа
// Запись конечного массива в файл

/* Коды ошибок:
    -1 - не удалось открыть файл
 */

int main(void) {

    FILE * text = fopen("mat_inc.txt", "r");

    if (!text) 
        return -1;

    rewind(text);

    int str = 0;
    int column = 1;

    while (!feof(text)) // строки
         if (fgetc(text) == '\n')
             str++;

    rewind(text);

    while (fgetc(text) != '\n')  //столбцы
        column++;
/*
    printf("str = %d column = %d\n", str, column ); */

    int **mat_inc;
    mat_inc = (int**)calloc(str, sizeof(int*));
    for (int i = 0; i < str; i++) {
        mat_inc[i] = (int*)calloc(column, sizeof(int));
    }

    rewind(text);

    for (int i = 0; i < str; i++) { // ввод в масисив
        for (int j = 0; j < column; j++) {
            if (!feof(text))
                mat_inc[i][j] = fgetc(text);
        }
    }
    fclose(text);
    
    int flag = 0;

    int **final_arr;
    int str_final_arr = (column / 2) + 1;
    final_arr = (int**)calloc(str_final_arr, sizeof(int*));
    for (int i = 0; i < str_final_arr; i++) {
        final_arr[i] = (int*)calloc(COLUMN_FINAL_ARR, sizeof(int));
    }
/*
    for (int i = 1; i < str; i++) {
        for (int j = 2; j < column; j++) {
            printf("str = %d column = %d: %c\n", i, j, mat_inc[i][j]);
            // printf("%c", (char)mat_inc[i][j]);
        }
    } */
    int count_str_final_arr = 0;

    for (int i = 2; i < column; i++) {
        if (!isdigit(mat_inc[1][i])) {
            continue;
        }
        for (int j = 1; j < str; j++) {
            if (mat_inc[j][i] == '1') {
                final_arr[count_str_final_arr][flag] = j;
                flag++;
            }
        }
        if (flag == 1) { // петля
            final_arr[count_str_final_arr][flag] = final_arr[count_str_final_arr][flag - 1];
        }
        flag = 0;
        count_str_final_arr++;
    }

    for (int i = 0; i < count_str_final_arr; i++ ) {
        for (int j = 0; j < COLUMN_FINAL_ARR; j++) {
            printf("%d ", final_arr[i][j]);
        }
        printf("\n"); 
    }
    
    FILE * for_dot = fopen("out.gv", "w+");

    fprintf(for_dot, "graph NEW {\n");

    for (int i = 0; i < count_str_final_arr; i++) {
            fprintf(for_dot, "\t%d -- %d\n", final_arr[i][0], final_arr[i][1]);
    }

    fprintf(for_dot, "}");

    fclose(for_dot);

    system("dot -Tpng out.gv -o out.jpg");
    system("wslview out.jpg");

    for (int i = 0; i < str; i++) {
        free(mat_inc[i]);
    }
    free(mat_inc);

    for (int i = 0; i < str_final_arr; i++) {
        free(final_arr[i]);
    }
    free(final_arr);

    return 0;
}
