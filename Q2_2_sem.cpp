#include <iostream>
#include <thread>
#include <semaphore.h>
using namespace std;

sem_t sem;

void runMeFirst() {
    cout << "Run me first" << endl;
    sem_post(&sem);
}

void runMeSecond() {
    cout << "I run second" << endl;
    sem_post(&sem);
}

void runMeThird() {
    sem_wait(&sem);
    cout << "I run third" << endl;
}

int main() {
    // initialize the semaphore with the number of threads
    sem_init(&sem, 0, -1); // 0: thread, 1: process

    thread t1 = thread(runMeFirst);
    thread t2 = thread(runMeSecond);
    thread t3 = thread(runMeThird);
    t1.join();
    t2.join();
    t3.join();
    
    sem_destroy(&sem); // destroy the semaphore
    return 0;
}