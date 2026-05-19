#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#define FIFO_NAME "/tmp/my_fifo"
#define ARRAY_SIZE 10

int main() {
    int fd;
    int array[ARRAY_SIZE];
    int received_array[ARRAY_SIZE];
    pid_t pid;
    int sum = 0;
    double average;
    
    // Создаём FIFO (именованный канал)
    if (mkfifo(FIFO_NAME, 0666) == -1) {
        if (errno != EEXIST) {
            perror("mkfifo");
            exit(EXIT_FAILURE);
        }
    }
    
    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    
    if (pid > 0) {
        // РОДИТЕЛЬ (ОТПРАВИТЕЛЬ)
        
        // Заполняем массив случайными числами
        srand(time(NULL));
        printf("Родитель (отправитель): массив из %d чисел: ", ARRAY_SIZE);
        for (int i = 0; i < ARRAY_SIZE; i++) {
            array[i] = rand() % 100 + 1;
            printf("%d ", array[i]);
        }
        printf("\n");
        
        // Открываем FIFO для записи
        fd = open(FIFO_NAME, O_WRONLY);
        if (fd == -1) {
            perror("open (родитель)");
            exit(EXIT_FAILURE);
        }
        
        // Передаём массив через FIFO
        ssize_t bytes_written = write(fd, array, sizeof(array));
        if (bytes_written == -1) {
            perror("write");
            close(fd);
            exit(EXIT_FAILURE);
        }
        
        printf("Родитель: отправлено %ld байт\n", bytes_written);
        
        // Закрываем FIFO
        close(fd);
        
        // Ждём завершения дочернего процесса
        wait(NULL);
        
        // Удаляем FIFO из файловой системы
        unlink(FIFO_NAME);
        printf("Родитель: FIFO удалён\n");
        
    } else {
        // ДОЧЕРНИЙ (ПОЛУЧАТЕЛЬ)
        
        // Небольшая задержка, чтобы родитель успел создать FIFO
        sleep(1);
        
        // Открываем FIFO для чтения
        fd = open(FIFO_NAME, O_RDONLY);
        if (fd == -1) {
            perror("open (дочерний)");
            exit(EXIT_FAILURE);
        }
        
        // Читаем массив из FIFO
        ssize_t bytes_read = read(fd, received_array, sizeof(received_array));
        if (bytes_read == -1) {
            perror("read");
            close(fd);
            exit(EXIT_FAILURE);
        }
        
        printf("Дочерний (получатель): получено %ld байт\n", bytes_read);
        printf("Дочерний: получен массив: ");
        for (int i = 0; i < ARRAY_SIZE; i++) {
            printf("%d ", received_array[i]);
            sum += received_array[i];
        }
        printf("\n");
        
        // Вычисляем среднее арифметическое
        average = (double)sum / ARRAY_SIZE;
        printf("Дочерний: сумма элементов = %d\n", sum);
        printf("Дочерний: среднее арифметическое = %.2f\n", average);
        
        // Закрываем FIFO
        close(fd);
        
        exit(EXIT_SUCCESS);
    }
    
    return 0;
}