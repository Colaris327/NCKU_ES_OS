//Producer consumer problem with multithreading using semaphore
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>

int buffer[10];
int count = 0;
int x = 0;
sem_t sem_empty;
sem_t sem_full;
struct timespec sem_timelimit; //this is for sem_timedwait() function

void *Producer(void *args)
{
    while (x < 200)
    {
        usleep(10);
        sem_wait(&sem_empty);
        buffer[count] = x;
        count++;
        printf("put %d. counter = %d\n", x, count);
        x++;
        sem_post(&sem_full); //signal
    }
}

void *Consumer(void *args)
{
    while (x <= 200)
    {
        usleep(10);
        clock_gettime(CLOCK_REALTIME, &sem_timelimit);
        sem_timelimit.tv_sec += 1; //if semaphore wait exceed 1 second, we stop the program
        if (sem_timedwait(&sem_full, &sem_timelimit) == -1)
            break;
        int y;
        y = buffer[count - 1];
        count--;
        printf("Got %d. counter = %d\n", y, count);
        sem_post(&sem_empty);
    }
}

int main()
{
    pthread_t Pt; //Producer thread
    pthread_t Ct; //Consumer thread
    sem_init(&sem_empty, 0, 10); //empty = buffer's entries
    sem_init(&sem_full, 0, 0);

    if (pthread_create(&Pt, NULL, &Producer, NULL) != 0)
        perror("Failed to create Producer thread\n");
    if (pthread_create(&Ct, NULL, &Consumer, NULL) != 0)
        perror("Failed to create Consumer thread\n");

    if (pthread_join(Pt, NULL) != 0)
        perror("Failed to join Producer thread\n");
    if (pthread_join(Ct, NULL) != 0)
        perror("Failed to join Consumer thread\n");

    sem_destroy(&sem_empty);
    sem_destroy(&sem_full);
    return 0;
}