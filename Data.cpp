#include "Data.h"
#include <algorithm>

//2
Data::Data() {}

Data::Data(int n) {
	
}

//4
void Data::PrintAll() {
	for (auto bird : DataStructure) {
		std::cout << bird.first << std::endl;
	}
}

//15
Item* Data::InsertItem(char c, int i, string s, Date d) {
	Item* it = new Item(c, i, s, d);

	if (DataStructure.find(it->GetSubgroup) == DataStructure.end()) {
	}
}

//16
list<Item*>* Data::InsertSubgroup(char s, int i, initializer_list<Item*> items) {

}

//17
map<int, list<Item*>*>* InsertGroup(char c, initializer_list<int> subgroups, initializer_list<initializer_list<Item*>> items) {

}