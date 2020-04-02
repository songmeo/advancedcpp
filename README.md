
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
<br>Constructs the object with empty container.
<br>3.`~Data();`
<br>Destructs the object and releases all the memory occupied by the container and the items in it.
<br>4.`voidPrintAll();`
<br>Prints all the items stored in the container in command prompt window in easily readable format(see Appendix).
<br>Items from the same group and subgroup must be ordered by their names.
<br>5.`intCountItems();`
<br>Returnsthe total number of items in the container.
<br>6.`map<int, list<Item*> *> *GetGroup(char c);`
<br>Returns the pointer to map containing all the items from group c.
<br>7.`void PrintGroup(char c);`
<br>Prints all the items from group c in command prompt window in easily readable format(see Appendix). 
<br>Items from the same subgroup must be ordered by their names.
<br>8.`intCountGroupItems(charc);`
<br>Returns the current number of items in group c. If the group does not exist, returns 0.
<br>9.`list<Item*> *GetSubgroup(charc, int i);`
<br>Returns the pointer to listcontaining all the items from subgroup i from group c.
<br>10.`voidPrintSubgroupByNames(charc, int i);`
<br>Prints all the items from subgroup i from group cin command prompt window in easily readable format (see Appendix). 
<br>Items must be ordered by their names.
<br>11.`voidPrintSubgroupByDates(char c, int i);`
<br>Prints all the items from subgroup i from group cin command prompt window in easily readable format (see Appendix). 
<br>Items must be ordered by their timestamps.
<br>12.`intCountSubgroupItems(charc, int i);`
<br>Returns the current number of items in subgroup i from group c. If the subgroup does not exist, returns 0.
<br>13.`Item* GetItem(char c, int i, strings);`
<br>Returns the pointer to the first of items specified by group c, subgroup i and name s.
<br>14.`voidPrintItem(char c, int i, string s);`
<br>Prints the first of item specified by group c, subgroup i and name s. If the item was not found throws invalid_argument_exception.
<br>15.`Item*InsertItem(char c, int i, string s, Date d);`
<br>Creates and inserts the specified item. Returns the pointer to new item. If the specified item already exists or the input parameters are not correct, returns nullptr.
<br>16.`list<Item*> *InsertSubgroup(chars, inti, initializer_list<Item*> items);`
<br>Creates and inserts the specified subgroup (i.e. list of pointers to  items). The initializer_listcontains pointers to new items. Returns the pointer to new list. 
<br>17.`map<int, list<Item *> *> *InsertGroup(charc, initializer_list<int> subgroups, initializer_list<initializer_list<Item *>> items);`
<br>Creates and inserts the specified group (i.e. map in which the keys are Item members. 
<br>Returns the pointer to new map.
<br>18.`bool RemoveItem(charc, inti, strings);`
<br>Removes the specified item.
<br>All the not used memory must be released.
<br>19.`boolRemoveSubgroup(char c, int i);`
<br>Removes the specified subgroup(i.e. list of pointers to items).
<br>All the not used memory must be released. 
<br>20.`boolRemoveGroup(charc);`
<br>Removes the specified group. All the not used memory must be released.

# Coursework 2

## Task
Update  the  software  implemented  in  Coursework  1  with  the  possibility  to  receive  new  items through a named pipe.

## Application ICS0025PipeServer

## Requirements
1. The client application is controlled by commands typed by user: 
<br>a. connect opens the pipe file and sends the first readymessage.
<br>b. stop sends the stopmessage (see above)and closes the pipe file.After this command the client application must stay active and the user must be able to type command connectonce more.
<br>c. exit forces the client to print the contents of data structure and exit the application.The client application must obey this command at any moment.
2. The readymessages except the first onemust besent automatically.
3. The client’s software must all time be under the user’s control. It means that:
<br>a. The  keyboard  must  never  be  blocked:  the  user should  beable  to  type  commandsat  any moment.
Consequently there must at least threethreads: one for listening the keyboard, one for reading data from the server and one for sending messages to the server.Reading from and writing to server must be asynchronous.
<br>b. If the server exits or closes the connection, the client must inform the user butcontinue to run.
