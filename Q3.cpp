#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <stdlib.h>
using namespace std;

mutex mtx; // declare mutex
condition_variable cv; // declare condition variable
const int n = 5;
const int m = 4;
int counter = 0;


struct requestStructure {
    int request_id; 

    string ip_address;

    string page_requested;

};

queue<requestStructure> msg_queue;

string webPages[10] = {"google.com", "yahoo.com", "youtube.com", "amazon.com", "twitch.tv", "loyno.edu",
                        "totaljerkface.com", "spotify.com", "aol.com", "friv.com"};

void listen() {
	srand(time(NULL));
	for (int i = 0; i < 6; i++) {
		int timeToSleep = rand() % 3 + 1;
	    this_thread::sleep_for(chrono::seconds(timeToSleep));
	    {
	    	counter++;
	    	requestStructure request;
		    request.request_id = counter;
		    request.ip_address = "";
		    int index = rand() % 10;
		    request.page_requested = webPages[index];
		    msg_queue.push(request);
		}
	    
    	cv.notify_one(); // notify waiting threads
	}
}
void do_request() {
	int thread_id = 0;
	while (true) {
		unique_lock<mutex> lck(mtx);
	    if (msg_queue.empty()) cv.wait(lck);
	    int request_id = msg_queue.front().request_id;
	    string page_requested = msg_queue.front().page_requested;
	    msg_queue.pop();
	    cout << "thread " << std::this_thread::get_id() << " completed request " << request_id << " requesting webpage " << page_requested << endl;
	    thread_id++;
	    lck.unlock();
	    if (thread_id == 5) break;
	}
}

int main() {
	for (int i = 0; i < n; i++) {
		thread t1(listen);
		thread t2(do_request);
		t1.join();
		t2.join();
	}
}