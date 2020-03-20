#include "Data.h"
#include <algorithm>

//1
Data::Data(int n) {
	while(n--) {
		Item* itm = new Item(); //create random item
		char c = itm->getGroup();
		if (DataStructure.find(c) != DataStructure.end()) { //check if itm group exists
			int i = itm->getSubgroup();
			if (DataStructure[c]->find(i) != DataStructure[c]->end()) {  //check if itm subgroup exists
				(*(DataStructure[c]))[i]->push_back(itm); //if yes, add itm to list
			}
			else { //if no, create list
				list<Item*> l = { itm };
				map<int, list<Item*>*> m{};
				m[i] = &l;
				DataStructure[c] = &m;
			}
		}
	}
}

//2
Data::Data() {}


//4
void Data::PrintAll() {
	for (auto bird : DataStructure)
		std::cout << bird.first << std::endl;
}

Data::~Data() {}

//15
Item* Data::InsertItem(char c, int i, string s, Date d) {
	Item* it = new Item(c, i, s, d);

	if (DataStructure.find(c) == DataStructure.end()) {
		
	}
	else
		(*(DataStructure[c]))[i]->push_back(it);
}

//16
list<Item*>* Data::InsertSubgroup(char s, int i, initializer_list<Item*> items) {

}

//17
map<int, list<Item*>*>* InsertGroup(char c, initializer_list<int> subgroups, initializer_list<initializer_list<Item*>> items) {

}