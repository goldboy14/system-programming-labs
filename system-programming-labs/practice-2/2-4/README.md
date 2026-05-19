# Задание 2-4: Отладка с strace

## Вариант 2
Написать программу читающую stdin и пишущую в stdout. Запустить strace, найти вызовы read() и write(), подсчитать количество.

## Требования
- Использование strace для анализа системных вызовов
- Сохранение команд и результатов в analysis.txt

## Компиляция и запуск
```bash
# Компиляция
gcc -o task task.c

# Запуск strace
strace -o trace.log ./task

# Анализ результатов
grep -c "read(" trace.log
grep -c "write(" trace.log