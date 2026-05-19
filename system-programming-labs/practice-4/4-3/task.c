#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#define PIDFILE "/tmp/pidfile.txt"
#define LOGFILE "/tmp/daemon.log"

volatile sig_atomic_t running = 1;

// Обработчик сигналов завершения
void signal_handler(int sig) {
    if (sig == SIGTERM || sig == SIGINT) {
        running = 0;
    }
}

// Запись сообщения в лог-файл
void write_log(const char *message) {
    FILE *log = fopen(LOGFILE, "a");
    if (log != NULL) {
        time_t now;
        struct tm *tm_info;
        char time_buf[20];
        
        time(&now);
        tm_info = localtime(&now);
        strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", tm_info);
        
        fprintf(log, "[%s] PID=%d %s\n", time_buf, getpid(), message);
        fclose(log);
    }
}

int main() {
    pid_t pid;
    FILE *pidfile;
    
    // 1. Создаём дочерний процесс
    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    
    if (pid > 0) {
        // Родительский процесс завершается
        printf("Демон запущен, PID=%d\n", pid);
        printf("Файл с PID: %s\n", PIDFILE);
        printf("Лог-файл: %s\n", LOGFILE);
        printf("Остановить демона: kill -TERM %d\n", pid);
        exit(EXIT_SUCCESS);
    }
    
    // 2. Создаём новую сессию (отсоединяемся от терминала)
    if (setsid() == -1) {
        perror("setsid");
        exit(EXIT_FAILURE);
    }
    
    // 3. Изменяем рабочий каталог на корневой
    if (chdir("/") == -1) {
        perror("chdir");
        exit(EXIT_FAILURE);
    }
    
    // 4. Устанавливаем маску создания файлов
    umask(0);
    
    // 5. Закрываем стандартные файловые дескрипторы
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    
    // 6. Создаём файл с PID
    pidfile = fopen(PIDFILE, "w");
    if (pidfile == NULL) {
        perror("fopen pidfile");
        exit(EXIT_FAILURE);
    }
    fprintf(pidfile, "%d\n", getpid());
    fclose(pidfile);
    
    // 7. Настраиваем обработчики сигналов
    signal(SIGTERM, signal_handler);
    signal(SIGINT, signal_handler);
    signal(SIGHUP, SIG_IGN);  // игнорируем SIGHUP
    
    // 8. Записываем в лог о запуске
    write_log("Демон запущен");
    
    // 9. Основной цикл демона
    while (running) {
        write_log("Демон работает...");
        sleep(5);
    }
    
    // 10. Завершение демона
    write_log("Демон завершается");
    
    // 11. Удаляем PID-файл
    if (unlink(PIDFILE) == -1) {
        perror("unlink pidfile");
    }
    
    write_log("PID-файл удалён");
    
    return 0;
}