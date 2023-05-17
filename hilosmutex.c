#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int valor=0;
pthread_mutex_t mutex;

void decremento(){
    int i;
    for(i=0; i<10000; i++){
        pthread_mutex_lock(&mutex);
        valor-=1;
        pthread_mutex_unlock(&mutex);
}
}

int main(void){

    pthread_mutex_init(&mutex, NULL);

    pthread_t unHilo;
    pthread_create(&unHilo, NULL,(void*)decremento,NULL);
    pthread_t dosHilo;
    pthread_create(&dosHilo, NULL,(void*)decremento,NULL);
    pthread_join(unHilo, NULL);
    pthread_join(dosHilo, NULL);

    printf("%d\n",valor);


}