#include <stdio.h>
#include <pthread.h>

#define ARRAY_SIZE 10

pthread_barrier_t barrier;

void* bubble_sort(void* arg) {
    int* array = (int*)arg;
    int i, j;
    
    for (i = 0; i < ARRAY_SIZE - 1; i++) {
        for (j = 0; j < ARRAY_SIZE - i - 1; j++) {
            if (array[j] > array[j + 1]) {
                // Intercambio de elementos
                int temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
        
        // Sincronización en la barrera después de cada iteración
        pthread_barrier_wait(&barrier);
    }
    
    return NULL;
}

int main() {
    pthread_t thread;
    int array[ARRAY_SIZE] = {5, 2, 8, 1, 9, 3, 7, 4, 6, 0};
    int i;
    
    pthread_barrier_init(&barrier, NULL, ARRAY_SIZE);
    
    // Creación del hilo para ordenar el arreglo
    pthread_create(&thread, NULL, bubble_sort, array);
    
    // Espera a que el hilo termine
    pthread_join(thread, NULL);
    
    pthread_barrier_destroy(&barrier);
    
    // Imprimir el arreglo ordenado
    printf("Arreglo ordenado: ");
    for (i = 0; i < ARRAY_SIZE; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
    
    return 0;
}
