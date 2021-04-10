/**
 *  Student Name: Raymond Li
 *  Student ID: 18028813
 */

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include "buffer.h"
#include <sys/types.h>

//Initialising count
int count = 0;

/* the buffer */
buffer_item buffer[BUFFER_SIZE];

//Declearing Semaphores
pthread_mutex_t mutex;
sem_t empty;
sem_t full;

//Task 1
    /* insert item into buffer
return 0 if successful, otherwise
return -1 indicating an error condition */
int insert_item(buffer_item item)
{
    if(count < BUFFER_SIZE)
    {
        //Adding items into the buffer
        buffer[count] = item;
        //Increase the count
        count++;
        return 0;
    }
    return -1;
}

    /* remove an object from buffer
return 0 if successful, otherwise
return -1 indicating an error condition */
int remove_item(buffer_item *item)
{
    if(count > 0)
    {
        //Removing the idem from the buffer by reducing the size of buffer
        *item = buffer[--count];
        return 0;
    }
    return -1;
}

//Task 3
//Thread to run the insert items from buffer
void *producer(void *param)
{
    buffer_item item;

    while (1)
    {
        //Pauses the execution for a random period of time from 1 to 10 seconds
        sleep(rand() % 2) +1;

        //Generates a random number to insert into the buffer
        item = (rand() % 999)+1;

        //Decreases the values of empty semaphores 
        sem_wait (&empty);

        //Lock the mutexs to protect the buffer
        pthread_mutex_lock (&mutex);

        if (insert_item(item))
        {
            fprintf(stderr, "Report error condition");
        }

        else
        {
            printf("Producer id: %lu produced item: %d\n", pthread_self(), item);

            printf("the buffer now contains %d items\n", count);
        }
        //Unlocks the mutex
        pthread_mutex_unlock (&mutex);
        
        //Increase the value of full semaphores 
        sem_post(&full);
    }
}

//Thread to run the remove items from buffer
void *consumer(void *param)
{
    buffer_item item;

    while (1)
    {
        //Pauses the execution for a random period of time from 1 to 10 seconds
        sleep(rand() % 2) + 1;

        //Decreases the values of full semaphores 
        sem_wait(&full);

        //Lock the mutex to protect the buffer
        pthread_mutex_lock(&mutex);

        if (remove_item (&item))
        {
            fprintf(stderr, "Report error condition");
        }
        else
        {
            printf("Consumer id: %lu consumed item: %d\n", pthread_self(), item);

            printf("the buffer now contrains %d items\n", count);
        }

        //Unlocks the mutex
        pthread_mutex_unlock (&mutex);

        //Increase the value of empty semaphores 
        sem_post(&empty);
    }
}

//Task 2
int main(int argc, char *argv[])
{
    int sleepTime, producerThread, consumerThread;
    
    int count;
    int i , j;

    /* 1. Get command line arguments argv[1], argv[2], argv[3]*/
    sleepTime = atoi(argv[1]);
    producerThread = atoi(argv[2]);
    consumerThread = atoi(argv[3]);

    printf("Buffer size: %d , Sleep timer: %d , Producer thread: %d , Consumer thread: %d \n", BUFFER_SIZE , sleepTime , producerThread , consumerThread);
    
    /* 2. Initialize buffer */
    pthread_mutex_init (&mutex , NULL);
    sem_init (&empty, 0 , BUFFER_SIZE);
    sem_init (&full , 0 , 0);

    /* 3. Create producer threads */
    for (i = 0; i < producerThread; i++)
    {
        pthread_t tid;
        pthread_attr_t attr;
        pthread_attr_init (&attr);
        pthread_create (&tid, &attr, producer, NULL);
    }

    /* 4. Create consumer threads */
    for (j = 0; j < consumerThread; j++)
    {
        pthread_t tid;
        pthread_attr_t attr;
        pthread_attr_init (&attr);
        pthread_create (&tid, &attr, consumer, NULL);
    }

    /* 5. Sleep */
    sleep(sleepTime);

    /* 6. Exit */
    exit(0);
}