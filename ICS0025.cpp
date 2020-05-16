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

using namespace std;

enum inputs{ready, connect, stop};
const char* c = "connect";
const char* r = "ready";
const char* s = "stop";
unsigned long nWritten = 0;
char* reply = new char[BUFSIZE];

void takeInput(queue<inputs> &q) {
	string input;
	while (input != "stop") {
		cout << "Enter input: ";
		cin >> input;
	}
}

void sendMsg(HANDLE hPipe, queue<inputs>& q) {
	while (1) {
		while (q.empty()) {
			this_thread::sleep_for(chrono::milliseconds(100)); //1/10 second
		}
		inputs i = q.front();
		switch (i) {
		case connect:
			if (!WriteFile(hPipe, c, strlen(c) + 1, &nWritten, NULL))
			{
				cout << "Unable to write into file, error " << GetLastError() << endl;
			}
			if (nWritten != strlen(c) + 1)
				cout << "Only " << nWritten << " bytes were written" << endl;
		case ready:
			if (!WriteFile(hPipe, r, strlen(c) + 1, &nWritten, NULL))
			{
				cout << "Unable to write into file, error " << GetLastError() << endl;
			}
			if (nWritten != strlen(r) + 1)
				cout << "Only " << nWritten << " bytes were written" << endl;
		case stop:
			if (!WriteFile(hPipe, s, strlen(c) + 1, &nWritten, NULL))
			{
				cout << "Unable to write into file, error " << GetLastError() << endl;
			}
			if (nWritten != strlen(s) + 1)
				cout << "Only " << nWritten << " bytes were written" << endl;
		}
		q.pop();
	}
	
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
		0,              // default attributes 
		NULL);          // no template file 

	if (hPipe == INVALID_HANDLE_VALUE)
	{
		cout << "Unable to create file, error " << GetLastError() << endl;
		return 1;
	}
	
	if (!WriteFile(hPipe, r, strlen(r) + 1, &nWritten, NULL))
	{
		cout << "Unable to write into file, error " << GetLastError() << endl;
		return 1;
	}
	if (nWritten != strlen(r) + 1)
		cout << "Only " << nWritten << " bytes were written" << endl;
	
	unsigned long nRead;
	if (SetFilePointer(hPipe, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		cout << "Unable to set the file pointer, error " << GetLastError() << endl;
		return 1;
	}
	if (!ReadFile(hPipe, reply, BUFSIZE, &nRead, NULL))
	{
		cout << "Unable to read from file, error " << GetLastError() << endl;
		return 1;
	}
	string str = reply;

	//parse group, subgroup
	char group = str[0];
	int subgroup = str[2];
	
	//parse name
	int open = str.find('<');
	int close = str.find('>');
	string name = str.substr(open + 1, close - open - 1);
	
	//parse date
	stringstream ss(str.substr(close + 2));
	int d;
	string m, y;
	ss >> d >> m >> y;
	vector<string> months = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
	int month = find(months.begin(), months.end(), m) - months.begin() + 1;
	int year = stoi(y);
	
	//make new date
	Date* date = new Date(d, month, year);

	//make new item
	Item* itm = new Item(group, subgroup, name, *date);
	
	//send ready after making new item
	if (!WriteFile(hPipe, ready, strlen(ready) + 1, &nWritten, NULL))
	{
		cout << "Unable to write into file, error " << GetLastError() << endl;
		return 1;
	}
	if (nWritten != strlen(ready) + 1)
		cout << "Only " << nWritten << " bytes were written" << endl;
	
	//send stop
	if (!WriteFile(hPipe, stop, strlen(stop) + 1, &nWritten, NULL))
	{
		cout << "Unable to write into file, error " << GetLastError() << endl;
		return 1;
	}
	if (nWritten != strlen(stop) + 1)
		cout << "Only " << nWritten << " bytes were written" << endl;
	
	queue<inputs> q;
	thread listenInput{ takeInput, q };
	thread sendServer{ sendMsg, q };
}

	listenInput.join();
	CloseHandle(hPipe);
	return 0;
}
