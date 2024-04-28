#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <stdlib.h>
using namespace std;

mutex mtx; // declare mutex
condition_variable cv; // declare condition variable
bool ready = false; // shared flag

struct requestStructure {
    int request_id; 

    string ip_address;

    string page_requested;

};

queue<requestStructure> msg_queue;

string webPages[10] = {"google.com", "yahoo.com", "youtube.com", "amazon.com", "twitch.tv", "loyno.edu"
                        "totaljerkface.com", "spotify.com", "aol.com", "friv.com"};

void listen() {
    this_thread::sleep_for(chrono::seconds(3));
    msg_queue.front().ip_address = "";

    int index = rand() % 10;
    msg_queue.front().page_requested = webPages[index];
    ready = true;
    cv.notify_one(); // notify waiting threads
}

void do_request() {
    unique_lock<mutex> lck(mtx);
    if (msg_queue.empty()) cv.wait(lck);

    lck.unlock();
    cout << "thread " << 1 << " completed request " << msg_queue.front().request_id << " requesting webpage " << msg_queue.front().page_requested << endl;
}

int main() {
    do_request();
}