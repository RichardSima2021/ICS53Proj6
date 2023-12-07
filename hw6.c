#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

// pointer to buffers
int *buffer;
// delay producer true?
int delayPr;
// length of buffer
int buffLen;
// num of items produced into the buffer
int bufferProdCount;
// num of items consumed from buffer
int bufferConsumeCount;
// num of items currently in the buffer
int count;
// how many items are produced by each producer
int prodNum;
// consumer limit
int consumeLimit;


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t notFull = PTHREAD_COND_INITIALIZER;
pthread_cond_t notEmpty = PTHREAD_COND_INITIALIZER;

void *blewh(void *vargp) {
    int producerID = * ((int*) vargp);
    for (int i = 0; i < prodNum; i++) {
        // the item to be produced by producer_x on the ith iteration
        int itemNum = producerID * prodNum + i;
        pthread_mutex_lock(&mutex);
        while (count == buffLen) {
            // block and wait
            pthread_cond_wait(&notFull, &mutex);
        }
        bufferProdCount++;
        count ++;
        int prodIndex = bufferProdCount % buffLen;
        buffer[prodIndex] = itemNum;
        printf("producer_%d produced item %d\n", producerID, itemNum);
        if (delayPr) {
            usleep(500000);
        }
        pthread_cond_signal(&notEmpty);
        pthread_mutex_unlock(&mutex);
    }

    // exit(0);
}

void *nom(void *vargp) {
    int consumerID = * ((int*) vargp);
    for (int i = 0; i < consumeLimit; i++) {
        pthread_mutex_lock(&mutex);
        while (count == 0) {
            // block and wait
            pthread_cond_wait(&notEmpty, &mutex);
        }
        bufferConsumeCount++;
        count--;
        int consumeIndex = bufferConsumeCount % buffLen;
        int consumedItem = buffer[consumeIndex];
        printf("consumer_%d consumed item %d\n", consumerID, consumedItem);
        if (!delayPr) {
            usleep(500000);
        }
        
        pthread_cond_signal(&notFull);
        pthread_mutex_unlock(&mutex);
    }

    // exit(0);
}


int main(int argc, char **argv) {
    int numProd = atoi(argv[1]);
    int numCons = atoi(argv[2]);
    prodNum = atoi(argv[3]);
    buffLen = atoi(argv[4]);
    delayPr = atoi(argv[5]);
    consumeLimit = numProd * prodNum / numCons;
    bufferProdCount = 0;
    bufferConsumeCount = 0;
    count = 0;
    buffer = (int *)malloc(buffLen * sizeof(int));

    pthread_t producers[numProd];
    pthread_t consumers[numCons];

    for (int i = 0; i < numProd; i++) {
        // printf("Create producer_%d\n", i);
        int *a = malloc(sizeof(int));
        *a = i;
        pthread_create(&producers[i], NULL, blewh, a);
    }

    for (int i = 0; i < numCons; i++) {
        // printf("Create consumer_%d\n", i);
        int *a = malloc(sizeof(int));
        *a = i;
        pthread_create(&consumers[i], NULL, nom, a);
    }

    for (int i = 0; i < numProd; i++) {
        pthread_join(producers[i], NULL);
    }

    for (int i = 0; i < numCons; i++) {
        pthread_join(consumers[i], NULL);
    }

    free(buffer);   

    return 0;
}