#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#define ARRAY_SIZE 10

int main() {
    int pipefd[2];
    int array[ARRAY_SIZE];
    int sum = 0;
    pid_t pid;

    // Создаём неименованный канал
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Заполняем массив случайными числами от 1 до 100
    srand(time(NULL));
    printf("Родитель: массив чисел: ");
    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = rand() % 100 + 1;
        printf("%d ", array[i]);
    }
    printf("\n");

    // Создаём дочерний процесс
    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        // РОДИТЕЛЬСКИЙ ПРОЦЕСС
        
        // Закрываем ненужный конец канала для чтения
        close(pipefd[0]);
        
        // Передаём массив дочернему процессу через канал
        ssize_t bytes_written = write(pipefd[1], array, sizeof(array));
        if (bytes_written == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }
        
        printf("Родитель: массив передан (%ld байт)\n", bytes_written);
        
        // Закрываем запись в канал
        close(pipefd[1]);
        
        // Ожидаем завершения дочернего процесса
        wait(NULL);
        printf("Родитель: дочерний процесс завершён\n");
        
    } else {
        // ДОЧЕРНИЙ ПРОЦЕСС
        
        // Закрываем ненужный конец канала для записи
        close(pipefd[1]);
        
        // Получаем массив от родителя через канал
        int received_array[ARRAY_SIZE];
        ssize_t bytes_read = read(pipefd[0], received_array, sizeof(received_array));
        if (bytes_read == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }
        
        printf("Дочерний: получен массив: ");
        for (int i = 0; i < ARRAY_SIZE; i++) {
            printf("%d ", received_array[i]);
            sum += received_array[i];
        }
        printf("\n");
        
        printf("Дочерний: сумма элементов массива = %d\n", sum);
        
        // Закрываем чтение из канала
        close(pipefd[0]);
        
        exit(EXIT_SUCCESS);
    }
    
    return 0;
}