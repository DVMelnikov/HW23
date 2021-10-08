#include <stdio.h>
#include <stdlib.h>

#define STR 3
#define ROW 3

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

    int row = 1;
    int str = 0;

    while (!feof(text)) 
         if (fgetc(text) == '\n')
             str++;

    rewind(text);

    while (fgetc(text) != '\n') 
        row++;

    printf("row = %d str = %d\n", row, str );

    int **mat_inc;
    mat_inc = (int**)calloc(row, sizeof(int*));
    for (int i = 0; i < row; i++) {
        mat_inc[i] = (int*)calloc(str, sizeof(int));
    }

    rewind(text);

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < str; j++) {
            mat_inc[i][j] = fgetc(text);
        }
    }

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < str; j++) {
            printf("%c", (char)mat_inc[i][j]);
        }
    }

    fclose(text);

    
    for (int i = 0; i < row; i++) {
        free(mat_inc[i]);
    }
    free(mat_inc);

    return 0;
}
