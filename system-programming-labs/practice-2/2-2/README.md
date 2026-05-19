# Задание 2-2: Синхронизация с мьютексами

## Вариант 2
Глобальный массив `arr[10]={0}`. Создать 10 потоков. Каждый поток `i` записывает `arr[i]=i*10`. Использовать мьютекс для защиты.

## Требования
- Использование `pthread_mutex_t`
- Функции: `pthread_mutex_init()`, `pthread_mutex_lock()`, `pthread_mutex_unlock()`, `pthread_mutex_destroy()`

## Компиляция и запуск
```bash
gcc -pthread -Wall -Wextra -o task task.c
./task