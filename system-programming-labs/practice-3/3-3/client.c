#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    int number, result;
    ssize_t bytes_sent, bytes_received;
    
    // 1. Создание сокета
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    printf("Клиент: сокет создан (fd=%d)\n", sock_fd);
    
    // 2. Настройка адреса сервера
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }
    
    // 3. Подключение к серверу
    if (connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }
    printf("Клиент: подключен к серверу 127.0.0.1:%d\n", PORT);
    
    // 4. Ввод числа от пользователя
    printf("Введите целое число: ");
    if (scanf("%d", &number) != 1) {
        fprintf(stderr, "Ошибка: введите корректное число\n");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }
    
    // 5. Отправка числа серверу
    snprintf(buffer, BUFFER_SIZE, "%d", number);
    bytes_sent = send(sock_fd, buffer, strlen(buffer), 0);
    if (bytes_sent == -1) {
        perror("send");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }
    printf("Клиент: отправлено число %d (%ld байт)\n", number, bytes_sent);
    
    // 6. Получение результата от сервера
    bytes_received = recv(sock_fd, buffer, BUFFER_SIZE - 1, 0);
    if (bytes_received == -1) {
        perror("recv");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }
    
    buffer[bytes_received] = '\0';
    result = atoi(buffer);
    printf("Клиент: получен результат: %d\n", result);
    printf("Клиент: квадрат числа %d равен %d\n", number, result);
    
    // 7. Закрытие сокета
    close(sock_fd);
    printf("Клиент: соединение закрыто\n");
    
    return 0;
}