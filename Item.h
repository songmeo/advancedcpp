#pragma once
#include <iostream>
#include <string>
#include "Date.h"
using namespace std;

class Item {
private:
	char Group;
	int Subgroup;
	string Name;
	Date timestamp;
public:
	Item();
	~Item();
	Item(char c, int i, string s, Date d);
	Item(const Item& itm);
	char getGroup() {
		return Group;
	};
	int getSubgroup() {
		return Subgroup;
	};
	void toStr() {
		cout << Group << " " << Subgroup << " " << Name << endl;
	}
};