#pragma once
#include <iostream>
#include <fstream>
#include "Item.h"
#include "Data.h"
#include "Date.h"
using namespace std;

int main() {
	Data* d = new Data(2);
	//d->PrintAll();
	//cout << d->CountItems();
	//d->PrintGroup('C');
	cout << d->CountGroupItems('C');
	d->GetSubGroup('C', '1');
	return 0;
}