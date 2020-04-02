#pragma once
#include <iostream>
#include <fstream>
#include "Item.h"
#include "Data.h"
#include "Date.h"
using namespace std;

int main() {
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
	return 0;
}