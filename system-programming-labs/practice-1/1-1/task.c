#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define TEXT_FILE "text.txt"
#define COUNT_FILE "count.txt"
#define BUFFER_SIZE 1024

int main() {
    int fd_text, fd_count;
    ssize_t bytes_read, bytes_written;
    char buffer[BUFFER_SIZE];
    int count_a = 0;
    char result_str[50];
    int length;
    
    
    const char* sample_text = 
        "System programming is a low-level programming discipline that focuses on "
        "interacting directly with computer hardware and operating system services. "
        "It involves using system calls, managing processes, threads, memory, and "
        "file systems. This field requires understanding of hardware constraints, "
        "performance optimization, and resource management. Programmers often work "
        "with C and assembly languages to write operating systems, device drivers, "
        "embedded systems, and performance-critical applications. The key aspects "
        "include process control, memory management, file I/O, and inter-process "
        "communication. Learning system programming provides deep insight into how "
        "software actually interacts with hardware. Example words with 'a': "
        "algorithm, application, architecture, assembly, asynchronous.\n";
    
    
    fd_text = open(TEXT_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_text == -1) {
        perror("Ошибка при создании text.txt");
        exit(EXIT_FAILURE);
    }
    
    
    bytes_written = write(fd_text, sample_text, strlen(sample_text));
    if (bytes_written == -1) {
        perror("Ошибка при записи в text.txt");
        close(fd_text);
        exit(EXIT_FAILURE);
    }
    
    close(fd_text);
    printf("Файл %s создан (%ld байт)\n", TEXT_FILE, bytes_written);
    
    
    fd_text = open(TEXT_FILE, O_RDONLY);
    if (fd_text == -1) {
        perror("Ошибка при открытии text.txt для чтения");
        exit(EXIT_FAILURE);
    }
    
    
    while ((bytes_read = read(fd_text, buffer, BUFFER_SIZE)) > 0) {
        for (int i = 0; i < bytes_read; i++) {
            if (buffer[i] == 'a' || buffer[i] == 'A') {
                count_a++;
            }
        }
    }
    
    if (bytes_read == -1) {
        perror("Ошибка при чтении text.txt");
        close(fd_text);
        exit(EXIT_FAILURE);
    }
    
    close(fd_text);
    
    
    fd_count = open(COUNT_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_count == -1) {
        perror("Ошибка при создании count.txt");
        exit(EXIT_FAILURE);
    }
    
    
    length = snprintf(result_str, sizeof(result_str), 
                      "Count of 'a' and 'A': %d\n", count_a);
    
    bytes_written = write(fd_count, result_str, length);
    if (bytes_written == -1) {
        perror("Ошибка при записи в count.txt");
        close(fd_count);
        exit(EXIT_FAILURE);
    }
    
    close(fd_count);
    
    printf("Результат сохранен в %s\n", COUNT_FILE);
    printf("Найдено букв 'a' и 'A': %d\n", count_a);
    
    return 0;
}