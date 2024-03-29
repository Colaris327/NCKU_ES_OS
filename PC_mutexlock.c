//Producer consumer problem with multithreading using mutex lock
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

int buffer[10];
int count = 0;
pthread_mutex_t mutexbuffer;
int x = 0;
int x_prevent = 10; //prevent that we don't finish getting items but program stop

void *Producer(void *args)
{
    while (x < 200)
    {
        usleep(10);
        pthread_mutex_lock(&mutexbuffer);
        if (count < 10)
        {
            buffer[count] = x;
            count++;
            printf("put %d. counter = %d\n", x, count);
            x++;
        }
        pthread_mutex_unlock(&mutexbuffer);
    }
}

void *Consumer(void *args)
{
    while (x <= 200)
    {
        usleep(10);
        pthread_mutex_lock(&mutexbuffer);
        int y;
        if (count > 0)
        {
            y = buffer[count - 1];
            count--;
            printf("Got %d. counter = %d\n", y, count);
        }
        pthread_mutex_unlock(&mutexbuffer);
        if (x == 200) //prevent that we finish our job but program don't stop
        {
            x_prevent--;
            if (x_prevent == 0)
                break;
        }
    }
}

int main()
{
    pthread_t Pt; //Producer thread
    pthread_t Ct; //Consumer thread
    pthread_mutex_init(&mutexbuffer, NULL);

    if (pthread_create(&Pt, NULL, &Producer, NULL) != 0)
        perror("Failed to create Producer thread\n");
    if (pthread_create(&Ct, NULL, &Consumer, NULL) != 0)
        perror("Failed to create Consumer thread\n");

    if (pthread_join(Pt, NULL) != 0)
        perror("Failed to join Producer thread\n");
    if (pthread_join(Ct, NULL) != 0)
        perror("Failed to join Consumer thread\n");

    pthread_mutex_destroy(&mutexbuffer);
    return 0;
}