#include "Data.h"
#include <algorithm>

//1
Data::Data(int n) {
	while (n--) {
		Item* itm = new Item();
		char c = itm->getGroup();
		int i = itm->getSubgroup();
		if (DataStructure.find(c) == DataStructure.end()) {
			list<Item*>* l = new list<Item*>{ itm };
			map<int, list<Item*>*>* subgrp = new map<int, list<Item*>*>{ { i, l } };
			DataStructure[c] = subgrp;
		}
		else if ((*DataStructure[c]).find(i) == (*DataStructure[c]).end()) {
			list<Item*>* l = new list<Item*>{ itm };
			(*DataStructure[c])[i] = l;
		}
		else
			(*DataStructure[c])[i]->push_back(itm);
	}
}

//2
Data::Data() {}

//3
Data::~Data() {
	for (auto it1 = DataStructure.begin(); it1 != DataStructure.end(); it1++) {
		for (auto it2 = it1->second->begin(); it2 != it1->second->end(); it2++) {
			for (auto it3 = it2->second->begin(); it3 != it2->second->end(); it3++) {
				delete[] *it3;
			}
			delete[] it2->second;
		}
		delete[] it1->second;
	}
	delete[] &DataStructure;
}

//4
void Data::PrintAll() {
	int items = 0;
	for (auto const &[grp, subgrp_map] : DataStructure) {
		for(auto const &[subgrp, list] :(*subgrp_map)) {
			for (Item* itm : *list) {
				cout << itm->getGroup() << " " << itm->getSubgroup() << " " << itm->getName() << endl;
			}
		}
	}
}

//5 
int Data::CountItems() {
	int items = 0;
	for (pair<char, map<int, list<Item*>*>*> grp : DataStructure) {
		for (pair<int, list<Item*>*> subgrp : *(grp.second)) {
			for (Item* itm : *(subgrp.second)) {
				items++;
			}
		}
	}
	return items;
}

//6 
map<int, list<Item*>*>* Data::GetGroup(char c) {
	return DataStructure[c];
}

//7 map<char, map<int, list<Item*>*>*> DataStructure;
void Data::PrintGroup(char c) {
	cout << c << ":" << endl;
	for (auto it1 : *(DataStructure[c])) {
		for (auto it2 : *(it1.second)) {
			cout << it2->getSubgroup() << ": " << it2->getName() << " " << it2->getDate() << endl;
		}
	}
}
//14


/*
//15
Item* Data::InsertItem(char c, int i, string s, Date d) {
	Item* itm = new Item(c, i, s, d);
	if (DataStructure.find(c) != DataStructure.end()) { //check if itm group exists
		if (DataStructure[c]->find(i) != DataStructure[c]->end()) {  //check if itm subgroup exists
			(*(DataStructure[c]))[i]->push_back(itm); //if yes, add itm to list
		}
		else { //if no, create list
			list<Item*> l = { itm };
			(*DataStructure[c])[i] = &l;
		}
	}
	else {
		list<Item*> l = { itm };
		map<int, list<Item*>*> m;
		m[i] = &l;
		DataStructure[c] = &m;
	}
}

//16
list<Item*>* Data::InsertSubgroup(char s, int i, initializer_list<Item*> items) {

}

//17
map<int, list<Item*>*>* InsertGroup(char c, initializer_list<int> subgroups, initializer_list<initializer_list<Item*>> items) {

}
*/