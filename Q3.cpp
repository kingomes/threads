#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <stdlib.h>
using namespace std;

mutex mtx; // declare mutex
condition_variable cv; // declare condition variable
const int n = 10; // number of do_requests
const int m = 1; // number of listens
int counter = 0;
int numListeners = m;



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
	requestStructure request;
	
	for (int i = 0; i < 6; i++) {
		int timeToSleep = rand() % 1 + 1;
	    this_thread::sleep_for(chrono::seconds(timeToSleep));	
	    counter++;
		request.request_id = counter;
		request.ip_address = "";
		int index = rand() % 10;
		request.page_requested = webPages[index];
		cout << "Listen: processing request no. " << counter << " accessing webpagew: " << request.page_requested << endl;
		{
		    unique_lock<mutex> lck(mtx);
		    msg_queue.push(request);
			lck.unlock();
			cv.notify_one(); // notify waiting threads
			//this_thread::sleep_for(chrono::seconds(2));
			
		}
		
	}
	numListeners--;
	cout << "Listener: numListeners: " << numListeners << endl;
	cv.notify_all ();
}
void do_request(int thread_id) {
	requestStructure req;
	while (true) {
		unique_lock<mutex> lck(mtx);
	    while (msg_queue.empty()) {
	    	if (numListeners == 0 && msg_queue.empty()) {
				cout << "Request: thread " << thread_id << " Exiting\n";
				return;
			}
	    	cv.wait(lck);	
		}
	    req = msg_queue.front();
	    msg_queue.pop();
	    
	    int request_id = req.request_id;
	    string page_requested = req.page_requested;
	    cout << "thread " << thread_id << " completed request " << request_id << " requesting webpage " << page_requested << endl;
	    	
	    lck.unlock();
	    //this_thread::sleep_for(chrono::seconds(2));
		
		
	}
}

int main() {
	thread listener = thread(listen);
	thread requests[n];
	for (int i = 0; i < n; i++) {
		requests[i] = thread(do_request, i);
	}
	listener.join();
	for (int i = 0; i < n; i++) {
		requests[i].join();
	}
}