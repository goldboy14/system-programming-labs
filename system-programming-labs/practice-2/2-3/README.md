# Задание 2-3: Обработка сигналов

## Вариант 2
Программа считает от 1 до 100 с задержкой 1 секунда. При SIGUSR1 выводит текущий счётчик. При SIGTERM завершается.

## Требования
- Использование `signal()` или `sigaction()`
- Обработчик async-signal-safe (используйте `write()` вместо `printf()`)
- Флаги: `volatile sig_atomic_t`

## Компиляция и запуск
```bash
gcc -Wall -Wextra -o task task.c
./task