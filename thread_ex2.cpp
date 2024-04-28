#include <iostream>
#include <thread>
using namespace std;

void thread1(int & c) {
    for (int i=0; i<100; i++) c++; 
}
void thread2(int & c) { 
    for (int i=0; i<100; i++) c++; 
}

int main() {
    thread t1, t2;
    int counter = 0;
    t1 = thread(thread1, std::ref(counter));
    t2 = thread(thread2, std::ref(counter));
    t1.join(); 
    t2.join();
    cout << "counter = " << counter << endl;
    return 0;
}