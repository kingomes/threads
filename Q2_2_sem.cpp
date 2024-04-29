#include <iostream>
#include <thread>
#include <semaphore.h>
using namespace std;

sem_t sem1;
sem_t sem2;

void runMeFirst() {
    cout << "Run me first" << endl;
    sem_post(&sem1);
}

void runMeSecond() {
	sem_wait(&sem1);
    cout << "I run second" << endl;
    sem_post(&sem2);
}

void runMeThird() {
    sem_wait(&sem2);
    cout << "I run third" << endl;
}

int main() {
    // initialize the semaphore with the number of threads
    sem_init(&sem1, 0, 0); // 0: thread, 1: process
    sem_init(&sem2, 0, 0); // 0: thread, 1: process

    thread t1 = thread(runMeFirst);
    thread t2 = thread(runMeSecond);
    thread t3 = thread(runMeThird);
    t1.join();
    t2.join();
    t3.join();
    
    sem_destroy(&sem1); // destroy the semaphore
    sem_destroy(&sem2); // destroy the semaphore
    return 0;
}