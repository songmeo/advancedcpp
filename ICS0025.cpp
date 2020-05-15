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
#define BUFSIZE 512

using namespace std;

/*
void main(int argc, char** argv) {
	/*
	Data* d = new Data();
	d->InsertItem('C', 1, "S", *(new Date(1,10,2000)));
	d->InsertItem('C', 1, "D", *(new Date(2,10,2000)));
	d->InsertItem('C', 1, "A", *(new Date(4,10,2000)));
	//d->PrintAll();
	//cout << d->CountItems();
	//d->PrintGroup('C');
	//cout << d->CountGroupItems('C');
	//d->GetSubGroup('C', '1');
	//d->PrintSubgroupByDates('C', 1);
	//cout << d->CountSubgroupItems('C',1);
	//d->PrintItem('C', 0, "S");
	//d->RemoveItem('C', 1, "A");
	Item* itm = new Item('C', 2, "A", *(new Date(1, 10, 2000)));
	d->InsertSubgroup('C', 2, { itm });
*/

int main()
{
	HANDLE hPipe;
	const char* ready = "ready";
	const char* stop = "stop";
	char* reply = new char[BUFSIZE];

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
	
	unsigned long nWritten;
	if (!WriteFile(hPipe, ready, strlen(ready) + 1, &nWritten, NULL))
	{
		cout << "Unable to write into file, error " << GetLastError() << endl;
		return 1;
	}
	if (nWritten != strlen(ready) + 1)
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
	
	CloseHandle(hPipe);
	return 0;
}
