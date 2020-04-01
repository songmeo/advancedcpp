#pragma once
#include <iostream>
#include <fstream>
#include "Item.h"
#include "Data.h"
#include "Date.h"
using namespace std;

int main() {
	Data* d = new Data();
	d->InsertItem('C', 1, "S", *(new Date()));
	d->InsertItem('C', 1, "D", *(new Date()));
	d->InsertItem('C', 1, "A", *(new Date()));
	//d->PrintAll();
	//cout << d->CountItems();
	//d->PrintGroup('C');
	//cout << d->CountGroupItems('C');
	//d->GetSubGroup('C', '1');
	d->PrintSubgroupByNames('C', 1);
	return 0;
}