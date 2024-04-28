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
    sem_wait(&sem);
    cout << "I run second" << endl;
}

int main() {
    // initialize the semaphore with the number of threads
    sem_init(&sem, 0, 0); // 0: thread, 1: process

    thread t1 = thread(runMeFirst);
    thread t2 = thread(runMeSecond);
    t1.join();
    t2.join();
    
    sem_destroy(&sem); // destroy the semaphore
    return 0;
}