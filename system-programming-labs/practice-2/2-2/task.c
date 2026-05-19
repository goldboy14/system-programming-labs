#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 10
int arr[10] = {0};
pthread_mutex_t mutex;

void* thread_func(void* arg) {
    int idx = *(int*)arg;
    pthread_mutex_lock(&mutex);
    arr[idx] = idx * 10;
    printf("Поток %d записал arr[%d] = %d\n", idx+1, idx, arr[idx]);
    pthread_mutex_unlock(&mutex);
    free(arg);
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int i;
    
    printf("Начало программы\n");
    printf("Исходный массив: ");
    for (i = 0; i < 10; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    pthread_mutex_init(&mutex, NULL);
    
    for (i = 0; i < NUM_THREADS; i++) {
        int* idx = malloc(sizeof(int));
        *idx = i;
        if (pthread_create(&threads[i], NULL, thread_func, idx) != 0) {
            perror("Ошибка создания потока");
            return 1;
        }
        printf("Создан поток %d\n", i+1);
    }
    
    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("\nИтоговый массив: ");
    for (i = 0; i < 10; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    pthread_mutex_destroy(&mutex);
    printf("Программа завершена\n");
    return 0;
}