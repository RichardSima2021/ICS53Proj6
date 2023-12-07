#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

int *buffer;
int delayPr;
int buffLen;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *blewh(void *vargp) {
    // printf("Hello from producer\n");
    int i = * ((int*) vargp);
    printf("I am producer %d\n", i);
    int produceCount = 0;
}

void *nom(void *vargp) {
    // printf("Hello from consumer\n");
    int i = * ((int*) vargp);
    printf("I am consumer %d\n", i);
    int consumeCount = 0;

}


int main(int argc, char **argv) {
    int numProd = atoi(argv[1]);
    int numCons = atoi(argv[2]);
    int prodNum = atoi(argv[3]);
    buffLen = atoi(argv[4]);
    delayPr = atoi(argv[5]);
    int consumeLimit = numProd * prodNum / numCons;

    buffer = (int *)malloc(buffLen * sizeof(int));

    pthread_t producers[numProd];
    pthread_t consumers[numCons];

    for (int i = 0; i < numProd; i++) {
        printf("Create producer_%d\n", i);
        int *a = malloc(sizeof(int));
        *a = i;
        pthread_create(&producers[i], NULL, blewh, a);
    }

    for (int i = 0; i < numCons; i++) {
        printf("Create consumer_%d\n", i);
        int *a = malloc(sizeof(int));
        *a = i;
        pthread_create(&consumers[i], NULL, nom, a);
    }

    // printf("debug\n");

    for (int i = 0; i < numProd; i++) {
        pthread_join(producers[i], NULL);
    }

    for (int i = 0; i < numCons; i++) {
        pthread_join(consumers[i], NULL);
    }

    free(buffer);   

    return 0;
}