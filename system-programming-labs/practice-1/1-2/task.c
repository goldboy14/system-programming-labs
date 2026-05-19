#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    int status;
    
    printf("Родительский процесс запущен. PID: %d\n", getpid());
    printf("PPID родителя: %d\n", getppid());
    
    
    pid = fork();
    
    if (pid == -1) {
        perror("Ошибка при вызове fork()");
        exit(EXIT_FAILURE);
    }
    
    if (pid == 0) {
        
        printf("\nДочерний процесс запущен. PID: %d\n", getpid());
        printf("PPID дочернего: %d\n", getppid());
        
        
        printf("Дочерний процесс выполняет: ls -la\n");
        printf("========================================\n");
        
        if (execl("/bin/ls", "ls", "-la", NULL) == -1) {
            perror("Ошибка при вызове execl()");
            exit(EXIT_FAILURE);
        }
        
        
        exit(EXIT_FAILURE);
    } else {
        
        printf("\nРодительский процесс продолжает работу. PID: %d\n", getpid());
        printf("Дочерний PID: %d\n", pid);
        
        
        printf("Родитель ждет завершения дочернего процесса...\n");
        
        if (waitpid(pid, &status, 0) == -1) {
            perror("Ошибка при вызове waitpid()");
            exit(EXIT_FAILURE);
        }
        
        printf("========================================\n");
        
        
        if (WIFEXITED(status)) {
            printf("Дочерний процесс завершился с кодом: %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("Дочерний процесс завершен сигналом: %d\n", WTERMSIG(status));
        }
        
        printf("Команда ls завершена\n");
        printf("Родительский процесс завершает работу.\n");
    }
    
    return 0;
}