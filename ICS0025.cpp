#pragma once
#include <iostream>
#include <fstream>
#include "Item.h"
#include "Data.h"
#include "Date.h" 
#include <windows.h> 
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <sstream>
#include <vector>
#include <thread>
#include <queue>
#include <atomic>
#include <condition_variable>
#define BUFSIZE 512
#define TIMEOUT 60*1000
using namespace std;

enum inputs{c, r, s, e};
const char* con = "connect";
const char* ready = "ready";
const char* ex = "exit";
const char* stop = "stop";
Data* ds = new Data();
bool closed = true;
bool server_error = false;

class Reader { //read from server
private:
	queue<inputs>& q;
	HANDLE& hPipe;
	HANDLE hExitEvent;
	HANDLE hHaveInput;
	mutex& mx;
	condition_variable& cv;
	unsigned long nRead = 0;
	char* reply = new char[BUFSIZE];
public:
	Reader(queue<inputs>& q, HANDLE& h1, mutex& mx, condition_variable& cv, HANDLE& h2, HANDLE& h3) : q(q), hPipe(h1), mx(mx), cv(cv), hHaveInput(h2), hExitEvent(h3) { }
	void operator() () {
		OVERLAPPED Overlapped;
		memset(&Overlapped, 0, sizeof Overlapped);
		Overlapped.hEvent = CreateEventA(NULL, FALSE, FALSE, NULL);
		HANDLE hEvents[] = { Overlapped.hEvent, hExitEvent }; //either it gets data or the user breaks off
		bool NoData = true;
		bool exit = false;
		while (1) {
			if (closed) {
				switch (WaitForSingleObject(hExitEvent, 1000)) {
				case WAIT_OBJECT_0:
					return;
				case WAIT_TIMEOUT:
					break;
				}
				continue;
			}
			if (!ReadFile(hPipe, reply, BUFSIZE, &nRead, &Overlapped))
			{
				int error = GetLastError();
				switch (error)
				{
				case ERROR_IO_PENDING:
					switch (WaitForMultipleObjects(2, hEvents, FALSE, TIMEOUT))
					{
					case WAIT_OBJECT_0:
						GetOverlappedResult(hPipe, &Overlapped, &nRead, FALSE);
						NoData = false;
						break;
					case WAIT_OBJECT_0 + 1:
						cout << "Reading broken off" << endl;
						exit = true;
						break;
					case WAIT_TIMEOUT:
						cout << "Timeout period " << TIMEOUT << "ms elapsed, nothing was received." << endl;
						exit = true;
						break; 
					default:
						cout << "Reading failed, error " << GetLastError() << endl;
						exit = true;
						server_error = true;
						break; 
					}
					break;
				default: 
					cout << "Reading failed, error " << GetLastError() << endl;
					exit = true;
					server_error = true;
					break;
				}
			}
			else {
				NoData = false;
			}
			if (exit) {
				break;
			}
			if (!NoData) {
				string str = reply;

				//parse group, subgroup
				stringstream s(reply);
				char group;
				int subgroup;
				s >> group >> subgroup;

				//parse name
				int open = str.find('<');
				int close = str.find('>');
				string name = str.substr(open + 1, close - open - 1);

				//parse date
				stringstream ss(str.substr(close + 2));
				int d;
				string m, y;
				ss >> d >> m >> y;
				vector<string> months = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
				int month = find(months.begin(), months.end(), m) - months.begin() + 1;
				int year = stoi(y);

				//make new date
				Date* date = new Date(d, month, year);

				//make new item
				ds->InsertItem(group, subgroup, name, *date);

				

				unique_lock<mutex> lock(mx);
				q.push(r);
				lock.unlock();
				cv.notify_one();
				SetEvent(hHaveInput);
			}
		}
		CloseHandle(Overlapped.hEvent); // clean
		delete reply;
	}
};

class Listener { //listen to user input
private:
	queue<inputs>& q;
	HANDLE& hPipe;
	HANDLE& hHaveInput;
	HANDLE& hExitEvent;
	mutex& mx;
	condition_variable& cv;
public:
	Listener(queue<inputs>& q, HANDLE& h1, mutex& m, condition_variable& c, HANDLE& h2, HANDLE& h3) : q(q), mx(m), cv(c), hPipe(h1), hHaveInput(h2), hExitEvent(h3) {};
	void operator() () {
		inputs i = c;
		string tmp = "";
		while (1) {
			if (server_error) {
				cout << "server error. Enter exit to exit" << endl;
			}
			cout << "Enter input: ";
			cin >> tmp;
			if (tmp == con) {
				if (!closed) {
					cout << "server has already opened" << endl;
				}
				i = c;
			}
			else if (tmp == stop) {
				if (closed) {
					cout << "server has already stopped" << endl;
				}
				i = s;
			}
			else if (tmp == ex) {
				i = e;
				SetEvent(hExitEvent);
				return;
			}
			else {
				cout << "try again" << endl;
				continue;
			}
			unique_lock<mutex> lock(mx);
			q.push(i);
			lock.unlock();
			cv.notify_one();
			SetEvent(hHaveInput);
		}
	}
};

class Sender { //send to server
private:
	queue<inputs>& q;
	HANDLE& hPipe;
	HANDLE& hHaveInput;
	HANDLE& hExitEvent;
	mutex& mx;
	condition_variable& cv;
	unsigned long nWritten = 0;
public:
	Sender(queue<inputs>& q, HANDLE& h1, mutex& m, condition_variable& c, HANDLE& h2, HANDLE& h3) : q(q), hPipe(h1), mx(m), cv(c), hHaveInput(h2), hExitEvent(h3) { };
	void operator() () {
		HANDLE hEvents[] = { hHaveInput, hExitEvent }; //either it gets data or timeout
		bool NoData = true;
		bool exit = false;
		const char* input = nullptr;
		while (1) {
			switch (WaitForMultipleObjects(2, hEvents, FALSE, TIMEOUT)) {
			case WAIT_OBJECT_0:
				NoData = false; 
				break;
			case WAIT_OBJECT_0 + 1:
				cout << endl << "Writing broken off" << endl;
				exit = true;
				break; 
			case WAIT_TIMEOUT:
				cout << "Timeout period " << TIMEOUT << "ms elapsed, nothing was received." << endl;
				exit = true;
				break; 
			default:
				cout << "Writing failed, error " << GetLastError() << endl;
				server_error = true;
				exit = true;
				break;
			}
			if (exit) {
				if (!closed) {
					CloseHandle(hPipe);
				}
				break;
			}
			if (!NoData) {
				unique_lock<mutex> lock(mx);
				inputs i = q.front();
				switch (i) {
				case r:
					input = ready;
					break;
				case c:
					if (closed) {
						hPipe = CreateFileA(
							"\\\\.\\pipe\\ICS0025",   // pipe name 
							GENERIC_READ |  // read and write access 
							GENERIC_WRITE,
							0,              // no sharing 
							NULL,           // default security attributes
							CREATE_ALWAYS,  // always open new pipe 
							FILE_FLAG_OVERLAPPED, // for asynchronous writing and reading 
							NULL);          // no template file
						if (hPipe == INVALID_HANDLE_VALUE) {
							cout << "Unable to create file, error " << GetLastError() << endl;
							return;
						}
						closed = false;
					}
					input = ready;
					break;
				case s:
					input = stop;
					closed = true;
					break;
				}
				if (!closed || input == stop) {
					if (!WriteFile(hPipe, input, strlen(input) + 1, &nWritten, NULL))
					{
						cout << "Unable to write into file, error " << GetLastError() << endl;
						server_error = true;
						return;
					}
					if (nWritten != strlen(input) + 1) {
						cout << "Only " << nWritten << " bytes were written" << endl;
						return;
					}
				}
				q.pop();
				lock.unlock();
			}
		}
	}

};

int main() {
	HANDLE hPipe;
	queue<inputs> q;
	mutex mx;
	condition_variable cv;
	HANDLE hExitEvent = CreateEventA(NULL, TRUE, FALSE, NULL);
	HANDLE hHaveInput = CreateEventA(NULL, FALSE, FALSE, NULL);
	thread listenerThread{ Listener(ref(q), ref(hPipe), mx, cv, hHaveInput, hExitEvent) };
	thread senderThread{ Sender(ref(q), ref(hPipe), mx, cv, hHaveInput, hExitEvent) };
	thread readerThread{ Reader(ref(q), ref(hPipe),  mx, cv, hHaveInput, hExitEvent) };
	listenerThread.join();
	senderThread.join();
	readerThread.join();
	CloseHandle(hExitEvent);
	CloseHandle(hHaveInput);
	ds->PrintAll();
	return 0;
	
}