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
	if (DataStructure.count(c) == 0)
		return nullptr;
	return DataStructure[c];
}

//7 
void Data::PrintGroup(char c) {
	try {
if (DataStructure.count(c) > 0) {
	throw invalid_argument("There is no such group");
}
cout << c << ":" << endl;
for (auto it1 : *(DataStructure[c])) {
	for (auto it2 : *(it1.second)) {
		cout << it2->getSubgroup() << ": " << it2->getName() << " " << it2->getDate() << endl;
	}
}
	}
	catch (const std::invalid_argument& e) {
		cout << e.what() << endl;
	}
}

//8
int Data::CountGroupItems(char c) {
	int result = 0;
	if (DataStructure.count(c) == 0) {
		return 0;
	}
	for (auto it1 : *(DataStructure[c])) {
		for (Item* it2 : *it1.second) {
			result++;
		}
	}
	return result;
}


//9 
list<Item*>* Data::GetSubGroup(char c, int i) {
	if (DataStructure.count(c) == 0 || (*DataStructure[c]).count(i) == 0) {
		return nullptr;
	}
	return (*DataStructure[c])[i];
}

//10
void Data::PrintSubgroupByNames(char c, int i) {
	try {
		list<Item*>* subgroup = this->GetSubGroup(c, i);
		if (subgroup == nullptr) {
			throw invalid_argument("There is no such subgroup");
		}
		subgroup->sort([](Item* it1, Item* it2) {
			return it1->getName()[0] < it2->getName()[0];
			});
		cout << c << ":" << endl;
		for (auto it : *subgroup) {
			cout << it->getSubgroup() << ": " << it->getName() << " " << it->getDate() << endl;
		}
	}
	catch (const std::invalid_argument& e) {
		cout << e.what() << endl;
	}
}

//11
void Data::PrintSubgroupByDates(char c, int i) {
	try {
		list<Item*>* subgroup = this->GetSubGroup(c, i);
		if (subgroup == nullptr) {
			throw invalid_argument("There is no such subgroup");
		}
		subgroup->sort([](Item* it1, Item* it2) {
			return it1->getDate() < it2->getDate();
			});
		cout << c << ":" << endl;
		for (auto it : *subgroup) {
			cout << it->getSubgroup() << ": " << it->getName() << " " << it->getDate() << endl;
		}
	}
	catch (const std::invalid_argument& e) {
		cout << e.what() << endl;
	}
}

//12  
int Data::CountSubgroupItems(char c, int i) {
	try {
		int result = 0;
		list<Item*>* subgroup = this->GetSubGroup(c, i);
		if (subgroup == nullptr) {
			throw invalid_argument("There is no such subgroup");
		}
		for (auto it : *subgroup) {
			result++;
		}
		return result;
	}
	catch (const std::invalid_argument& e) {
		cout << e.what() << endl;
	}
}

//13 
Item* Data::GetItem(char c, int i, string s) {
	if (DataStructure.count(c) == 0 || (*DataStructure[c]).count(i) == 0)
		return nullptr;
	for (Item* it : *(*DataStructure[c])[i]) {
		if (it->getName() == s)
			return it;
	}
	return nullptr;
}

//14 map<char, map<int, list<Item*>*>*> DataStructure;
void Data::PrintItem(char c, int i, string s) {
	try {
		Item* tmp = GetItem(c, i, s);
		if(tmp == nullptr)
			throw invalid_argument("There is no such group");
		cout << tmp->getSubgroup() << ": " << tmp->getName() << " " << tmp->getDate() << endl;
	}
	catch (const std::invalid_argument& e) {
		cout << e.what() << endl;
	}
}

//15
Item* Data::InsertItem(char c, int i, string s, Date d) {
	Item* itm = new Item(c, i, s, d);
	if (DataStructure.count(c) > 0) { //check if itm group exists
		if ((*DataStructure[c]).count(i) > 0) {  //check if itm subgroup exists
			(*DataStructure[c])[i]->push_back(itm); //if yes, add itm to list
		}
		else { //if no, create list
			list<Item*>* l = new list<Item*>{ itm };
			(*DataStructure[c])[i] = l;
		}
	}
	else {
		list<Item*>* l = new list<Item*>{ itm };
		map<int, list<Item*>*>* m = new map<int, list<Item*>*>{ {i,l} };
		DataStructure[c] = m;
	}
	return itm;
}
/*
//16
list<Item*>* Data::InsertSubgroup(char s, int i, initializer_list<Item*> items) {

}

//17
map<int, list<Item*>*>* InsertGroup(char c, initializer_list<int> subgroups, initializer_list<initializer_list<Item*>> items) {

}
*/