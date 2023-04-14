#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define PHILOS 5
#define LEFT (phil + 5) % 5
#define RIGHT (phil + 1) % 5
sem_t room;         // semaphore to check if the room is empty
sem_t chopstick[5]; // binary semaphore to check if the chopsticks is available

void *philosopher(void *);
void eat(int);

void eat(int phil) {
  printf("\nPhilosopher %d is eating", phil);
}

int main() {
  int i, a[5];
  pthread_t philo[PHILOS]; // threads representing the philosophers

  sem_init(&room, 0, 4); // initializations of semaphore  varring from 0 to 4.

  for (i = 0; i < 5; i++)
    sem_init(&chopstick[i], 0, 1); // initializations of semaphore  referring to chopsticks.

  for (i = 0; i < 5; i++ ){
    a[i] = i;
    pthread_create(&philo[i], NULL, philosopher, (void *)&a[i]); // creation of philosophers threads.
  }
  for (i = 0; i < 5; i++)
    pthread_join(philo[i], NULL); // join the threads.
  printf("\n"); 
}

void *philosopher(void *num) {
  int phil = *(int *)num;


  sem_wait(&room); // semaphore function to checks if resources are available.
  printf("\nPhilosopher %d has entered room", phil);

  printf("\nPhilosopher %d: tries to get %s chopstick %d\n", phil, "right", RIGHT);
  sem_wait(&chopstick[RIGHT]); // semaphore function to checks if chopstick is available.
  printf("\nPhilosopher %d: got %s chopstick %d\n", phil, "right", RIGHT);

  printf("\nPhilosopher %d: tries to get %s chopstick %d\n", phil, "right", LEFT);
  sem_wait(&chopstick[LEFT]);
  printf("\nPhilosopher %d: got %s chopstick %d\n", phil, "left", LEFT);

  eat(phil); // philosopher is eating
  sleep(2); 
  printf("\nPhilosopher %d has finished eating", phil);

  sem_post(&chopstick[RIGHT]); // gives confirmation if semaphore is released successfully
  printf("\nPhilosopher %d: leaves %s chopstick %d\n", phil, "right", RIGHT);

  sem_post(&chopstick[LEFT]);
  printf("\nPhilosopher %d: leaves %s chopstick %d\n", phil, "left", LEFT);
  sem_post(&room); // philosopher has left the room
  printf("\nPhilosopher %d: left the room", phil); 
}
