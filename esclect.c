#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>

sem_t wrt;
pthread_mutex_t mutex;
int readrnum = 0;
int cnt = 1;

void *writer(void *wno) {
    int id = *((int *)wno);
    sem_wait(&wrt);

    printf("Writer %d is writing...\n", id);
    cnt = cnt * 2;
    printf("Writer %d modified cnt to %d\n", id, cnt);
    sleep(1);

    sem_post(&wrt);
    return NULL;
}
void *reader(void *rno) {
    int id = *((int *)rno);

    pthread_mutex_lock(&mutex);
    readrnum++;
    if (readrnum == 1) {
        sem_wait(&wrt);  
    }
    pthread_mutex_unlock(&mutex);

    printf("Reader %d is reading... cnt = %d\n", id, cnt);
    sleep(1);

    pthread_mutex_lock(&mutex);
    readrnum--;
    if (readrnum == 0) {
        sem_post(&wrt); 
    }
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main() {
    pthread_t read[10], write[5];
    sem_init(&wrt, 0, 1);
    pthread_mutex_init(&mutex, NULL);

    int a[10];
    for (int i = 0; i < 10; i++) a[i] = i + 1;

    for (int i = 0; i < 5; i++) {
        pthread_create(&read[i], NULL, reader, &a[i]);
    }

    sleep(1);

    for (int i = 0; i < 2; i++) {
        pthread_create(&write[i], NULL, writer, &a[i]);
    }

    for (int i = 5; i < 10; i++) {
        pthread_create(&read[i], NULL, reader, &a[i]);
    }

    for (int i = 2; i < 5; i++) {
        pthread_create(&write[i], NULL, writer, &a[i]);
    }

    for (int i = 0; i < 10; i++) {
        pthread_join(read[i], NULL);
    }

    for (int i = 0; i < 5; i++) {
        pthread_join(write[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&wrt);

    return 0;
}