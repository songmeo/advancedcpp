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
	Item(char, int, string, Date);
	Item(const Item&);
	~Item();
};