#pragma once
#include <iostream>
#include <fstream>
#include "Item.h"
#include "Data.h"
#include "Date.h"
using namespace std;

int main() {
	Data* d = new Data(1);
	cout << d->CountItems();
	//Item* it = new Item();
	//char c = it->getGroup();
	//int i = it->getSubgroup();
	//map<char, map<int, list<Item*>*>*> ds;
	//list<Item*> l{ it };
	//map<int, list<Item*>*> m{ {i, &l} };
	//ds[c] = &m;
	//int items = 0;
	//for (auto it1 : ds) {
	//	for (auto it2 : *it1.second) {
	//		for (auto it3 : *it2.second) {
	//			items++;
	//		}
	//	}
	//}
	//cout << items;
	return 0;
}