#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

#define FILENAME "numbers.dat"

int main() {
    int fd;
    struct stat sb;
    int *data;
    int max_value;
    int count;
    int i;
    
    // Создаём файл с числами (бинарный формат)
    printf("Создаём файл %s с числами...\n", FILENAME);
    
    int numbers[] = {42, 17, 89, 33, 76, 54, 91, 28, 63, 45, 100, 3, 67, 82, 19};
    count = sizeof(numbers) / sizeof(numbers[0]);
    
    FILE *f = fopen(FILENAME, "wb");
    if (f == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    fwrite(numbers, sizeof(int), count, f);
    fclose(f);
    printf("Файл создан, записано %d чисел\n", count);
    
    // 1. Открываем файл
    fd = open(FILENAME, O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    
    // 2. Получаем размер файла
    if (fstat(fd, &sb) == -1) {
        perror("fstat");
        close(fd);
        exit(EXIT_FAILURE);
    }
    
    printf("Размер файла: %ld байт\n", sb.st_size);
    printf("Количество чисел в файле: %ld\n", sb.st_size / sizeof(int));
    
    // 3. Отображаем файл в память
    data = (int*)mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (data == MAP_FAILED) {
        perror("mmap");
        close(fd);
        exit(EXIT_FAILURE);
    }
    
    printf("Файл отображён в память по адресу %p\n", (void*)data);
    
    // 4. Ищем максимальное значение (читаем из отображённой памяти)
    max_value = data[0];
    printf("Числа из файла: ");
    for (i = 0; i < sb.st_size / sizeof(int); i++) {
        printf("%d ", data[i]);
        if (data[i] > max_value) {
            max_value = data[i];
        }
    }
    printf("\n");
    
    printf("Максимальное значение: %d\n", max_value);
    
    // 5. Освобождаем отображение
    if (munmap(data, sb.st_size) == -1) {
        perror("munmap");
        close(fd);
        exit(EXIT_FAILURE);
    }
    printf("Отображение освобождено\n");
    
    // 6. Закрываем файл
    close(fd);
    printf("Файл закрыт\n");
    
    return 0;
}