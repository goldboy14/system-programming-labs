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
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];
    int number, square;
    ssize_t bytes_received, bytes_sent;
    
    // 1. Создание сокета
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    printf("Сервер: сокет создан (fd=%d)\n", server_fd);
    
    // Настройка опции повторного использования адреса
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        perror("setsockopt");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    
    // 2. Привязка к адресу и порту
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("Сервер: привязан к порту %d\n", PORT);
    
    // 3. Прослушивание входящих соединений
    if (listen(server_fd, 5) == -1) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("Сервер: ожидание подключений...\n");
    
    // 4. Принятие соединения от клиента
    client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
    if (client_fd == -1) {
        perror("accept");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    
    printf("Сервер: клиент подключен (fd=%d)\n", client_fd);
    printf("Сервер: IP клиента: %s, порт: %d\n", 
           inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    
    // 5. Получение числа от клиента
    bytes_received = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
    if (bytes_received == -1) {
        perror("recv");
        close(client_fd);
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    
    buffer[bytes_received] = '\0';
    number = atoi(buffer);
    printf("Сервер: получено число %d\n", number);
    
    // 6. Вычисление квадрата
    square = number * number;
    printf("Сервер: квадрат числа = %d\n", square);
    
    // 7. Отправка результата клиенту
    snprintf(buffer, BUFFER_SIZE, "%d", square);
    bytes_sent = send(client_fd, buffer, strlen(buffer), 0);
    if (bytes_sent == -1) {
        perror("send");
        close(client_fd);
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    
    printf("Сервер: отправлен результат (%ld байт)\n", bytes_sent);
    
    // 8. Закрытие соединений
    close(client_fd);
    close(server_fd);
    printf("Сервер: соединения закрыты\n");
    
    return 0;
}