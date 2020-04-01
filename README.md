
# Coursework 1

## Class Date:
Class Date is implemented by instructor.

## Class Item:
### Requirements:
1. To get random groups and subgroups use C++ default_random_engine_generator.<br>
2. The random names  must  be  taken  from  file Birds.txt stored in Instructor's stuff.<br>
Read the  file  into memory, create a vector of strings and use C++ default_random_engine_generator to retrieve names.
3. To create a random date use method  Date::CreateRandomDate().Tip:use intervals with width one year or even more.<br>

## Class Data:
<br>`map<char, map<int, list<Item*> *> *> DataStructure;`</br>
<br>It is a C++ map in which the Item members Group are the keys.
<br>The values are pointers to inner C++ maps in which the keys are Item members Subgroup and values are pointers to lists.
<br>The lists contain pointers to items. Class Data must contain the following public methods:</br>
<br>1.`Data(int n);`
<br>Constructs the object and fills the container with n random items.
<br>2.`Data();`
<br>Constructs the object with empty container
<br>3.`~Data();`
<br>Destructs the object and releases all the memory occupied by the container and the items in it.
<br>4.`voidPrintAll();`
<br>Prints all the items stored in the container in command prompt window in easily readable format(see Appendix).
<br>Items from the same group and subgroup must be ordered by their names.
<br>5.`intCountItems();`
<br>Returnsthe total number of items in the container.
<br>6.`map<int, list<Item*> *> *GetGroup(char c);`
<br>Returns the pointer to map containing all the items from group c. If the group does not exist, returns nullptr.
<br>7.`void PrintGroup(char c);`
<br>Prints all the items from group c in command prompt window in easily readable format(see Appendix). 
<br>Items from the same subgroup must be ordered by their names. If the group was not found, throws invalid_argument_exception.
<br>8.`intCountGroupItems(charc);`
<br>Returns the current number of items in group c. If the group does not exist, returns 0.
<br>9.`list<Item*> *GetSubgroup(charc, int i);`
<br>Returns the pointer to listcontaining all the items from subgroup i from group c.
<br>If the subgroup does not exist, returns nullptr.
<br>10.`voidPrintSubgroupByNames(charc, int i);`
<br>Prints all the items from subgroup i from group cin command prompt window in easily readable format (see Appendix). 
<br>Items must be ordered by their names. If the subgroup was not found, throws invalid_argument_exception.
<br>11.`voidPrintSubgroupByDates(char c, int i);`
<br>Prints all the items from subgroup i from group cin command prompt window in easily readable format (see Appendix). 
<br>Items must be ordered by their timestamps. If the subgroup was not found, throws invalid_argument_exception.
<br>12.`intCountSubgroupItems(charc, int i);`
<br>Returns the current number of items in subgroup i from group c. If the subgroup does not exist, returns 0.
<br>13.`Item* GetItem(char c, int i, strings);`
<br>Returns the pointer to the first of items specified by group c, subgroup i and name s. If the item was not found returns nullptr.
<br>14.`voidPrintItem(char c, int i, string s);`
<br>Prints the first of item specified by group c, subgroup i and name s. If the item was not found throws invalid_argument_exception.
<br>15.`Item*InsertItem(char c, int i, string s, Date d);`
<br>Creates and inserts the specified item. Returns the pointer to new item. If the specified item already exists or the input parameters are not correct, returns nullptr.
<br>If necessary, creates the missing group and subgroup.
<br>16.`list<Item*> *InsertSubgroup(chars, inti, initializer_list<Item*> items);`
<br>Creates and inserts the specified subgroup (i.e. list of pointers to  items). The initializer_listcontains pointers to new items. Returns the pointer to new list. 
<br>If the specified subgroup already exists or the input parameters are not correct, returns nullptr. If necessary, creates the missing group. 
<br>17.`map<int, list<Item *> *> *InsertGroup(charc, initializer_list<int> subgroups, initializer_list<initializer_list<Item *>> items);`
<br>Creates and inserts the specified group (i.e. map in which the keys are Item members. 
<br>Subgroup and values are pointers to lists containing pointers to items). The subgroups initializer_list presents the keys to be included into the new map. 
<br>The items initializer_list contains initalizer_lists presenting pointers to items to be included. The first initalizer_list from items corresponds to the first integer in subgroups.
<br>Returns the pointer to new map. If the specified group already exists or the input parameters are not correct, returns nullptr.
<br>18.`bool RemoveItem(charc, inti, strings);`
<br>Removes the specified item. If after removing the subgroup has no members(i.e. its list is empty), remove it too. If after that the group (i.e. map) is empty, remove it also. 
<br>All the not used memory must be released. Return value: false if the item was not found, otherwise true.
<br>19.`boolRemoveSubgroup(char c, int i);`
<br>Removes the specified subgroup(i.e. list of pointers to items). If after removing the corresponding group (i.e. map) has no members, remove it too. 
<br>All the not used memory must be released. Return value: false if the subgroup was not found, otherwise true.
<br>20.`boolRemoveGroup(charc);`
<br>Removes the specified group. All the not used memory must be released. Return value: false if the group was not found, otherwise true.
