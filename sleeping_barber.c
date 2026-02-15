#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define CUSTOMERS 7
#define CHAIRS 4

sem_t sem_customers;       
sem_t sem_customer_ready;  
sem_t sem_barber_done;     
sem_t sem_customer_left;  
pthread_mutex_t mutex;     

int count = 0;
int waiting_room[CHAIRS];
int head = 0, tail = 0;

void *barber_func(void *arg) {
    while (1) {
        sem_wait(&sem_customers); 

        pthread_mutex_lock(&mutex);
        int id = waiting_room[tail];
        tail = (tail + 1) % CHAIRS;
        count--;
        pthread_mutex_unlock(&mutex);

        sem_post(&sem_customer_ready);

        printf("Barber is cutting hair of Customer %d.\n", id);
        usleep(rand() % 500000); 

        printf("Barber finished cutting hair of Customer %d.\n", id);
        
        sem_post(&sem_barber_done);
        
        sem_wait(&sem_customer_left); 

        pthread_mutex_lock(&mutex);
        if (count > 0) {
            printf("Barber calls next customer.\n");
        } else {
            printf("Barber goes to sleep.\n");       
        }
        pthread_mutex_unlock(&mutex);
    }
}

void *customer_func(void *arg) {
    int id = *(int *)arg;
    free(arg);

    usleep(rand() % 2000000);
    printf("Customer %d arrives.\n", id);

    pthread_mutex_lock(&mutex);
    if (count < CHAIRS) {
        waiting_room[head] = id;
        head = (head + 1) % CHAIRS;
        count++;
        printf("Customer %d sits in waiting room (%d/%d chairs occupied).\n", 
               id, count, CHAIRS);
        
        sem_post(&sem_customers);
        pthread_mutex_unlock(&mutex);

        sem_wait(&sem_customer_ready);
        printf("Customer %d is getting a haircut.\n", id);

        sem_wait(&sem_barber_done);
        
        
        sem_post(&sem_customer_left);

    } else {
        printf("Customer %d leaves (no available chair).\n", id);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    srand(time(NULL));
    pthread_t barber, customers[CUSTOMERS];

    sem_init(&sem_customers, 0, 0);
    sem_init(&sem_customer_ready, 0, 0); 
    sem_init(&sem_barber_done, 0, 0);    
    sem_init(&sem_customer_left, 0, 0); 
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&barber, NULL, barber_func, NULL);

    for (int i = 0; i < CUSTOMERS; i++) {
        int *id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&customers[i], NULL, customer_func, id);
    }

    for (int i = 0; i < CUSTOMERS; i++) {
        pthread_join(customers[i], NULL);
    }
    
    pthread_cancel(barber);
    pthread_join(barber, NULL);

    sem_destroy(&sem_customers);
    sem_destroy(&sem_customer_ready);
    sem_destroy(&sem_barber_done);
    sem_destroy(&sem_customer_left);
    pthread_mutex_destroy(&mutex);

    return 0;
}