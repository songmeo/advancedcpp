#pragma once
#include <iostream>
#include <fstream>
#include "Item.h"
#include "Data.h"
#include "Date.h" 
#include <windows.h>
#include <stdio.h>
#define PIPE_NAME "\\.\pipe\ICS0025"
using namespace std;

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
	HANDLE PipeHandle;
	DWORD BytesWritten;

    if (WaitNamedPipe((LPCWSTR)PIPE_NAME, NMPWAIT_WAIT_FOREVER) == 0)
    {
        printf("WaitNamedPipe() failed with error code %d\n", GetLastError());
        return;
    }
    else
        printf("WaitNamedPipe() is OK!\n");
    // Create the named pipe file handle
    if ((PipeHandle = CreateFile((LPCWSTR)PIPE_NAME,
        GENERIC_READ | GENERIC_WRITE, 0,
        (LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        (HANDLE)NULL)) == INVALID_HANDLE_VALUE)
    {
        printf("CreateFile() failed with error code %d\n", GetLastError());
        return;
    }
    else
        printf("CreateFile() is OK!\n");
    if (WriteFile(PipeHandle, "This is a test string", 21, &BytesWritten, NULL) == 0)
    {
        printf("WriteFile() failed with error code %d\n", GetLastError());
        printf("Closing the handle...\n");
        CloseHandle(PipeHandle);
        return;
    }
    else
        printf("WriteFile() is fine!\n");
    printf("Wrote %d bytes", BytesWritten);
    printf("Closing the handle...\n");
    CloseHandle(PipeHandle);
}

