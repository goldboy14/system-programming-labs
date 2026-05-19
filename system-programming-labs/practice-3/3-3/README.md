# Задание 3.3 — Сокеты TCP

## Условие
**Вариант 2**: Клиент отправляет серверу число, сервер возвращает его квадрат.

## Требования
- Использовать `socket()`, `bind()`, `listen()`, `accept()`, `connect()`, `send()`, `recv()`, `close()`
- Реализовать клиент и сервер на C
- Обрабатывать ошибки через `perror()`
- Компиляция: `gcc -Wall -Wextra -o task task.c`

## Компиляция
```bash
gcc -Wall -Wextra -o server server.c
gcc -Wall -Wextra -o client client.c