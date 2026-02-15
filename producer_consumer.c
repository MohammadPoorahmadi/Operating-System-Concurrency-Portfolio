#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 10
int produce_even() { return (rand() % 5000) * 2; }

int buffer[BUFFER_SIZE];
int count = 0;  // Number of items in the buffer

// synchronization:
// no explicit locks or semaphores are used.
// threads only check buffer state (count < BUFFER_SIZE / count > 0) before acting.
// this approach is not threadsafe and can lead to race conditions.
// the goal is to observe nondeterministic behavior.

void* producer(void* arg) {
    // producer logic:
    // runs forever to keep producing (while(1)).
    //if buffer is not full write an item and increment count.
    //otherwise  print that the buffer is full.
    while (1) {
        //check if the buffer is not full
        if (count < BUFFER_SIZE){
            int item = produce_even();
            buffer[count] = item; //place item at the end of buffer
            count++; //increase count
            printf("[Producer] Produced: %d | Buffer count: %d\n" , item , count);
        } else {
            //buffer is full producer waits
            printf("[Producer] Buffer Full , waiting... \n");
        }
        //TODO
        sleep(rand() % 2); // Sleep for a random period
    }
    return NULL;
}

void* consumer(void* arg) {
    // consumer logic:
    //runs forever to keep consuming (while(1)).
    //if buffer is not empty take the last item and decrement count.
    //otherwise print that the buffer is empty.
    while (1) {
        //check if the buffer is empty
        if (count > 0){
            int item = buffer[count - 1]; //take item from the end
            count--; //decrease count
            printf("[Consumer] Consumed: %d | Buffer count: %d\n" , item , count);
        } else {
            //buffer is empty consumer waits
            printf("[Consumer] Buffer Empty , waiting... \n");
        }
        //TODO
        sleep(rand() % 2); // Sleep for a random period
    }
    return NULL;
}

int main() {
    pthread_t producers, consumers;
    int producer_ids, consumer_ids;

    // Create producer threads
        producer_ids =  1;
        pthread_create(&producers, NULL, producer, &producer_ids);
   
    // Create consumer threads
        consumer_ids =  1;
        pthread_create(&consumers, NULL, consumer, &consumer_ids);
   
    // Join threads (in this case, the program will run indefinitely)
        pthread_join(producers, NULL);
        pthread_join(consumers, NULL);
    
    return 0;
}
