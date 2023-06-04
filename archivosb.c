#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 4
#define BUFFER_SIZE 256

pthread_barrier_t barrier;
char buffer[BUFFER_SIZE];
int buffer_index = 0;

void* read_file(void* arg) {
    FILE* file = fopen("input.txt", "r");
    if (file == NULL) {
        printf("No se pudo abrir el archivo de entrada.\n");
        return NULL;
    }
    
    while (1) {
        char ch;
        
        // Lectura de un carácter del archivo
        if (fscanf(file, "%c", &ch) != 1) {
            break;  // Fin del archivo
        }
        
        // Almacenamiento del carácter en el búfer compartido
        buffer[buffer_index] = ch;
        buffer_index++;
        
        // Verificación del límite del búfer
        if (buffer_index >= BUFFER_SIZE) {
            // Sincronización en la barrera antes de escribir el búfer en el archivo
            pthread_barrier_wait(&barrier);
            buffer_index = 0;
        }
    }
    
    // Sincronización final en la barrera antes de terminar
    pthread_barrier_wait(&barrier);
    
    fclose(file);
    
    return NULL;
}

void* write_file(void* arg) {
    FILE* file = fopen("output.txt", "w");
    if (file == NULL) {
        printf("No se pudo abrir el archivo de salida.\n");
        return NULL;
    }
    
    while (1) {
        // Sincronización en la barrera antes de leer el búfer compartido
        pthread_barrier_wait(&barrier);
        
        // Verificación del fin del archivo
        if (buffer_index == 0) {
            break;
        }
        
        // Escritura del contenido del búfer en el archivo
        fwrite(buffer, sizeof(char), buffer_index, file);
        buffer_index = 0;
    }
    
    fclose(file);
    
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int i;
    
    pthread_barrier_init(&barrier, NULL, NUM_THREADS);
    
    for (i = 0; i < NUM_THREADS - 1; i++) {
        pthread_create(&threads[i], NULL, read_file, NULL);
    }
    
    pthread_create(&threads[NUM_THREADS - 1], NULL, write_file, NULL);
    
    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    pthread_barrier_destroy(&barrier);
    
    printf("Proceso completado.\n");
    
    return 0;
}
