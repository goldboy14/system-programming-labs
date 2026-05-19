#include <stdio.h>
#include <stdlib.h>

int main() {
    char buffer[1024];
    printf("Введите текст (Ctrl+D для завершения):\n");
    
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        printf("Вы ввели: %s", buffer);
    }
    
    printf("Программа завершена\n");
    return 0;
}