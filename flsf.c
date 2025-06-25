#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <unistd.h>

#define NUM_PHIL 5
pthread_mutex_t mutex;
sem_t forks[NUM_PHIL];

void think(int phil) {
    printf("Philosopher %d is thinking.\n", phil);
    sleep(1);
}
void eat(int phil) {
    printf("Philosopher %d is eating.\n", phil);
    sleep(1);
}

void take_forks(int phil){
    pthread_mutex_lock(&mutex);
    sem_wait(&forks[phil]);
    sem_wait(&forks[(phil + 1) % NUM_PHIL]);
    pthread_mutex_unlock(&mutex);
}

void put_forks(int phil){
    sem_post(&forks[phil]);
    sem_post(&forks[(phil + 1) % NUM_PHIL]);
}

void* philosopher(void* arg) {
    int id = *(int*)arg;
    while (1) {
        think(id);
        take_forks(id);
        eat(id);
        put_forks(id);
    }
}

int main(){
    pthread_t phil[NUM_PHIL];
    int ids[NUM_PHIL];
    pthread_mutex_init(&mutex, NULL);


    for (int i = 0; i < NUM_PHIL; i++) {
        sem_init(&forks[i], 0, 1);
    }
    for (int i = 0; i < NUM_PHIL; i++) {
        ids[i] = i;
        pthread_create(&phil[i], NULL, philosopher, &ids[i]);
    }
    for (int i = 0; i < NUM_PHIL; i++) {
        pthread_join(phil[i], NULL);
    }

    pthreqad_mutex_destroy(&mutex);
    return 0;
}

