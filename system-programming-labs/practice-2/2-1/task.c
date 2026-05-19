#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <inttypes.h>

// Структура для передачи аргументов потоку
typedef struct {
    int start;
    int end;
    int is_sum; // 1 - сумма, 0 - произведение
} ThreadArgs;

// Функция потока для вычисления суммы или произведения
void* compute(void* arg) {
    ThreadArgs* args = (ThreadArgs*)arg;
    long long* result = malloc(sizeof(long long));
    *result = (args->is_sum) ? 0 : 1; // Инициализируем
    
    if (args->is_sum) {
        // Вычисляем сумму
        for (int i = args->start; i <= args->end; i++) {
            *result += i;
        }
        printf("Поток для суммы: вычислил сумму от %d до %d\n", 
               args->start, args->end);
    } else {
        // Вычисляем произведение
        for (int i = args->start; i <= args->end; i++) {
            *result *= i;
        }
        printf("Поток для произведения: вычислил произведение от %d до %d\n", 
               args->start, args->end);
    }
    
    // Возвращаем результат через pthread_exit
    pthread_exit(result);
}

int main() {
    pthread_t thread1, thread2;
    ThreadArgs args1, args2;
    long long* sum_result;
    long long* prod_result;
    int ret;
    
    printf("Главный поток запущен\n");
    
    // Настраиваем аргументы для первого потока (сумма 1-1000)
    args1.start = 1;
    args1.end = 1000;
    args1.is_sum = 1;
    
    // Настраиваем аргументы для второго потока (произведение 1-10)
    args2.start = 1;
    args2.end = 10;
    args2.is_sum = 0;
    
    // Создаем первый поток
    printf("Создаю поток для вычисления суммы 1-1000...\n");
    ret = pthread_create(&thread1, NULL, compute, &args1);
    if (ret != 0) {
        fprintf(stderr, "Ошибка создания потока 1: %d\n", ret);
        return 1;
    }
    
    // Создаем второй поток
    printf("Создаю поток для вычисления произведения 1-10...\n");
    ret = pthread_create(&thread2, NULL, compute, &args2);
    if (ret != 0) {
        fprintf(stderr, "Ошибка создания потока 2: %d\n", ret);
        return 1;
    }
    
    printf("Оба потока созданы, главный поток ждет их завершения...\n");
    
    // Ждем завершения первого потока и получаем результат
    ret = pthread_join(thread1, (void**)&sum_result);
    if (ret != 0) {
        fprintf(stderr, "Ошибка pthread_join для потока 1: %d\n", ret);
        return 1;
    }
    printf("Поток 1 завершился\n");
    
    // Ждем завершения второго потока и получаем результат
    ret = pthread_join(thread2, (void**)&prod_result);
    if (ret != 0) {
        fprintf(stderr, "Ошибка pthread_join для потока 2: %d\n", ret);
        free(sum_result);
        return 1;
    }
    printf("Поток 2 завершился\n");
    
    // Выводим результаты
    printf("\n========== РЕЗУЛЬТАТЫ ==========\n");
    printf("Сумма чисел от 1 до 1000: %lld\n", *sum_result);
    printf("Произведение чисел от 1 до 10: %lld\n", *prod_result);
    printf("================================\n");
    
    // Проверка вычислений (опционально)
    long long check_sum = 0;
    for (int i = 1; i <= 1000; i++) check_sum += i;
    
    long long check_prod = 1;
    for (int i = 1; i <= 10; i++) check_prod *= i;
    
    if (*sum_result == check_sum && *prod_result == check_prod) {
        printf("Проверка: вычисления верны!\n");
    } else {
        printf("Проверка: обнаружена ошибка в вычислениях!\n");
    }
    
    // Освобождаем память
    free(sum_result);
    free(prod_result);
    
    printf("Главный поток завершает работу\n");
    
    return 0;
}