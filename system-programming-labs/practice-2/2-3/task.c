#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

volatile sig_atomic_t counter = 0;
volatile sig_atomic_t running = 1;

void handle_sigusr1(int sig) {
    char msg[100];
    int len = snprintf(msg, sizeof(msg), "[SIGUSR1] Текущий счетчик: %d\n", counter);
    write(STDOUT_FILENO, msg, len);
}

void handle_sigterm(int sig) {
    write(STDOUT_FILENO, "[SIGTERM] Завершение программы...\n", 34);
    running = 0;
}

int main() {
    char info[100];
    int len;
    
    len = snprintf(info, sizeof(info), 
                   "Программа запущена. PID: %d\n"
                   "Используйте команды:\n"
                   "  kill -SIGUSR1 %d  - показать счетчик\n"
                   "  kill -SIGTERM %d  - завершить программу\n"
                   "Счетчик от 1 до 100 с задержкой 1 секунда:\n",
                   getpid(), getpid(), getpid());
    write(STDOUT_FILENO, info, len);
    
    signal(SIGUSR1, handle_sigusr1);
    signal(SIGTERM, handle_sigterm);
    
    for (counter = 1; counter <= 100 && running; counter++) {
        printf("Счетчик: %d\n", counter);
        sleep(1);
    }
    
    printf("Программа завершена. Итоговый счетчик: %d\n", counter-1);
    return 0;
}