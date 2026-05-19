#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

int main() {
    char *str = NULL;
    char buffer[BUFFER_SIZE];
    size_t length;
    
    // Ввод строки с клавиатуры
    printf("Введите строку: ");
    if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
        fprintf(stderr, "Ошибка: не удалось прочитать строку\n");
        exit(EXIT_FAILURE);
    }
    
    // Удаляем символ новой строки
    buffer[strcspn(buffer, "\n")] = '\0';
    
    // Вычисляем длину строки
    length = strlen(buffer);
    printf("Длина введённой строки: %zu символов\n", length);
    
    // Выделяем память с помощью malloc
    str = (char*)malloc((length + 1) * sizeof(char));
    if (str == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    printf("Память выделена через malloc (%zu байт)\n", (length + 1) * sizeof(char));
    
    // Копируем строку в выделенную память
    strcpy(str, buffer);
    printf("Скопированная строка: \"%s\"\n", str);
    
    // Освобождаем память
    free(str);
    printf("Память освобождена\n");
    
    // Пример использования calloc
    char *str2 = (char*)calloc(length + 1, sizeof(char));
    if (str2 == NULL) {
        perror("calloc");
        exit(EXIT_FAILURE);
    }
    printf("Память выделена через calloc (%zu байт, обнулена)\n", (length + 1) * sizeof(char));
    
    strcpy(str2, buffer);
    printf("Строка в calloc-памяти: \"%s\"\n", str2);
    
    // Пример использования realloc (увеличиваем память)
    char *str3 = (char*)realloc(str2, (length + 10) * sizeof(char));
    if (str3 == NULL) {
        perror("realloc");
        free(str2);
        exit(EXIT_FAILURE);
    }
    str2 = str3;
    strcat(str2, " (дополнено)");
    printf("После realloc: \"%s\"\n", str2);
    
    // Освобождаем память
    free(str2);
    
    return 0;
}