#pragma once
#include <random>
#include <iostream>
#include <fstream>
#include "Item.h"
#include "Date.h"
using namespace std;

Item::Item() {
	random_device rd; // obtain a random number from hardware
	mt19937 eng(rd()); // seed the generator
	//subgroup
	uniform_int_distribution<int> subgroup(0, 99);
	Subgroup = subgroup(eng);

	//group
	uniform_int_distribution<int> group(0x41, 0x5a);
	Group = static_cast<char>(group(eng));

	//name
	ifstream infile("Birds.txt");
	string tmp;
	vector<string> birds;
	while (infile.peek() != EOF) {
		getline(infile, tmp);
		birds.push_back(tmp);
	}

	uniform_int_distribution<int> name(0, (int)birds.size() - 1);
	Name = birds[name(eng)];
	infile.close();

	//timestamp
	timestamp = Date::CreateRandomDate(Date(10, 10, 2020), Date(10, 10, 2030));
}

Item::Item(char c, int i, string s, Date d) {
	Group = c;
	Subgroup = i;
	Name = s;
	timestamp = d;
}

Item::Item(const Item& itm) {
	Group = itm.Group;
	Subgroup = itm.Subgroup;
	Name = itm.Name;
	timestamp = itm.timestamp;
}

Item::~Item(){
	delete &Group;
	delete& Subgroup;
	delete& Name;
	delete& timestamp;
}