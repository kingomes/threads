#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <stdlib.h>
using namespace std;

mutex mtx; // declare mutex
condition_variable cv; // declare condition variable
const int n = 10;
int counter = 0;
int numListeners = 1;


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
	
	for (int i = 0; i < 5; i++) {
		int timeToSleep = rand() % 3 + 1;
	    this_thread::sleep_for(chrono::seconds(timeToSleep));
	    {
			unique_lock<mutex> lck(mtx);
	    	counter++;
			cout << "Listener: request no. " << counter << endl;
			lck.unlock();
	    	requestStructure request;
		    request.request_id = counter;
		    request.ip_address = "";
		    int index = rand() % 10;
		    request.page_requested = webPages[index];
			lck.lock();
		    msg_queue.push(request);
			cout << "Listener: pushes request no. " << request.request_id << endl;
			lck.unlock();
		}
    	cv.notify_one(); // notify waiting threads
	}
	cout << "Listener: numListeners: " << numListeners << endl;
	numListeners--;
}

void do_request(int thread_id) {
	while (true) {
		unique_lock<mutex> lck(mtx);
	    if (msg_queue.empty()) {
	    	if (numListeners == 0) {
				cout << "Request: " << thread_id << " Exiting\n";
				break;
			}
	    	cv.wait(lck);
		}
	    int request_id = msg_queue.front().request_id;
		cout << "Request: retrieving request number: " << request_id << endl;
	    string page_requested = msg_queue.front().page_requested;
	    msg_queue.pop();
	    cout << "thread " << thread_id << " completed request " << request_id << " requesting webpage " << page_requested << endl;
	    lck.unlock();
		cout << "Request: numListeners: " << numListeners << endl;
	}
}

int main() {
	thread listener = thread(listen);
	listener.join();
	thread requests[n];
	for (int i = 0; i < n; i++) {
		requests[i] = thread(do_request, i);
	}
	for (int i = 0; i < n; i++) {
		requests[i].join();
	}
}