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
#define BUFSIZE 512
#define TIMEOUT 60*1000

using namespace std;

enum inputs{c, s, e};
const char* con = "connect";
const char* ready = "ready";
const char* ex = "exit";
const char* stop = "stop";
unsigned long nWritten = 0;
unsigned long nRead = 0;
char* reply = new char[BUFSIZE];

void takeInput(queue<inputs> &q, HANDLE hExitEvent) {
	inputs i = c;
	string tmp = "";
	while (tmp != ex) {
		cout << "Enter input: ";
		cin >> tmp;
		if (tmp == con) {
			i = c;
		}
		else if (tmp == stop) {
			i = s;
		}
		else if (tmp == ex) {
			i = e;
			SetEvent(hExitEvent);
			return;
		}
		q.push(i);
	}
}

void sendMsg(HANDLE hPipe, queue<inputs> &q, HANDLE hExitEvent) {
	const char* input = nullptr;
	while (1) {
		if (q.empty()) {
			this_thread::sleep_for(chrono::milliseconds(100)); //wait for input
			continue;
		}
		inputs i = q.front();
		switch (i) {
		case c:
			input = ready;
			break;
		case s:
			input = stop;
			break;
		}
		if (!WriteFile(hPipe, input, strlen(input) + 1, &nWritten, NULL))
		{
			cout << "Unable to write into file, error " << GetLastError() << endl;
			return;
		}
		if (nWritten != strlen(input) + 1) {
			cout << "Only " << nWritten << " bytes were written" << endl;
			return;
		}
		q.pop();
	}
}

void takeReply(HANDLE hPipe, HANDLE hExitEvent) {
	OVERLAPPED Overlapped;
	memset(&Overlapped, 0, sizeof Overlapped);
	Overlapped.hEvent = CreateEventA(NULL, FALSE, FALSE, NULL);
	HANDLE hEvents[] = { Overlapped.hEvent, hExitEvent }; //either it gets data or the user breaks off
	bool NoData = true;
	bool exit = false;
	while (1) {
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
					NoData = false; // Got some data, waiting ended
					break;
				case WAIT_OBJECT_0 + 1:
					cout << "Reading broken off" << endl;
					exit = true;
					break; // user has broken the ending off
				case WAIT_TIMEOUT:
					cout << "Timeout period " << TIMEOUT << "ms elapsed, nothing was received. Press a key to exit" << endl;
					exit = true;
					break; // timeout
				default:
					cout << "Reading failed, error " << GetLastError() << ". Press a key to exit" << endl;
					exit = true;
					break; // some system errors
				}
				break;
			default: // some system errors
				cout << "Reading failed, error " << GetLastError() << ". Press a key to exit" << endl;
				exit = true;
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
			Item* itm = new Item(group, subgroup, name, *date);
		}
	}
	CloseHandle(Overlapped.hEvent); // clean
	delete reply;
}

int main()
{
	HANDLE hPipe;
	hPipe = CreateFile(
		L"\\\\.\\pipe\\ICS0025",   // pipe name 
		GENERIC_READ |  // read and write access 
		GENERIC_WRITE,
		0,              // no sharing 
		NULL,           // default security attributes
		OPEN_EXISTING,  // opens existing pipe 
		FILE_FLAG_OVERLAPPED, // for asynchronous writing and reading 
		NULL);          // no template file 

	if (hPipe == INVALID_HANDLE_VALUE)
	{
		cout << "Unable to create file, error " << GetLastError() << endl;
		return 1;
	}
	queue<inputs> q;
	HANDLE hExitEvent = CreateEventA(NULL, TRUE, FALSE, NULL);
	thread listenInput{ takeInput, ref(q), hExitEvent };
	thread sendServer{ sendMsg, hPipe, ref(q), hExitEvent};
	thread takeOutput(takeReply, hPipe, hExitEvent);
	listenInput.join();
	sendServer.join();
	takeOutput.join();

	CloseHandle(hPipe);
	return 0;
}
