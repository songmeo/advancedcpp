#include "Item.h"
#include "Date.h"
#include <random>
#include <iostream>
#include <fstream>
using namespace std;

Item::Item() {
	default_random_engine generator;

	//subgroup
	uniform_int_distribution<int> subgroup(0, 99);
	Subgroup = subgroup(generator);

	//group
	uniform_int_distribution<int> group(65, 90);
	Group = static_cast<char>(group(generator));

	//name
	ifstream infile("Birds.txt");
	string tmp;
	vector<string> birds;
	while (infile.peek() != EOF) {
		getline(infile, tmp);
		birds.push_back(tmp);
	}
	uniform_int_distribution<int> name(0, birds.size() - 1);
	Name = birds[name(generator)];
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

Item::Item(const Item&) {}

char Item::GetGroup() {
	return Group;
}

int Item::GetSubgroup() {
	return Subgroup;
}

Item::~Item(){}