/*
	ENGR2400 Data Structures and Algorithms
	Fall 2019
	Ryley Danielson
	Programming Assignment 3 - Hash Table ADT
	The goal of this assignment is to create a Hash Table abstract data type class using a n array and linked-list as the base for the hash table implementation.
*/

//Includes libraries needed for the program
#include<iostream>
#include<iomanip>
#include <fstream>

//This program uses the standard namespace
using namespace std;

//Class used as a template to build new linked list nodes
class node
{
public:
	int data;
	int key;
	node* next;
	node* chain;
};


//Global Variables used by the program
int sizeList = 0; //Tracks the size of the list
int i = 0; //Integer fields used in class methods
int o = 0; //Integer fields used in class methods
int choice; //Varable storing user menue input
int key; //The key range used in the hash function
int keyMin; //The minimum key value
int keyMax; //The maximum key value
int buckets; //Number of hash table buckets defined by the user
int hash = 0; //Calculated Hash value for a given key
node* head; //Head pointer of the hash table linked list
ofstream myfile; //File stream for program 

/*
Creates a new node that chains from a hash table bucket
The data value is set equal int o and the key value is set to the in value k, which are detemined by user input
The pointer value of the next node is automatically set to NULL
This method returns the pointer value for the new node
*/
node* createChainNode(int o, int k)
{
	node* newNode = new node;
	newNode->key = k;
	newNode->data = o;
	newNode->next = NULL;
	return newNode;
}

/*
Creates a new bucket for the hash table
The data value is set equal to equal to int o which is detemined by user input
The pointer value of the next node and chain node is automatically set to NULL
This method returns the pointer value for the new node
*/
node* createBucketNode(int o)
{
	node* newNode = new node;
	newNode->data = o;
	newNode->next = NULL;
	newNode->chain = NULL;
	return newNode;
}


/*
Counts the number of entries in a bucket
Returns the number of key values mapped to this hash h
*/
int BucketCount(int h)
{
	
	//Checks to make sure the user input is a valid bucket
	if ((h > (buckets-1)||( h < 0)))
	{
		cout << "You have not ented a valid bucket value." << endl;
		myfile << "You have not entered a valid bucket value." << endl;
		return -1;
	}

	node* tempNode = head;
	//Finds node at index h
	while (tempNode->data != h)
	{
		tempNode = tempNode->next;
	}

	//Checks if a bucket s empty to prevent null pointer errors
	if (tempNode->chain == NULL)
	{
		cout << "There are no entries in this bucket." << endl;
		myfile << "There are no entries in this bucket." << endl;
		return -1;
	}

	tempNode = tempNode->chain;
	int num = 0;
	
	//Calculates the number of elements in the bucket if it is not empty
	while (tempNode != NULL)
	{
		tempNode = tempNode->next;
		num++;
	}

	cout << "Bucket size is: " << num << " at bucket " << h << endl;
	myfile << "Bucket size is: " << num << endl;
	return num;
}

/*
Gets the key range from the user to be using in the hash function
Checks to see if the input value is a prime number
Confirms whether or not the user wants to continue with their chosen value
Called once at the beginning of the program
*/
void GetUserParameters() 
{
	
	cout << "Enter the minimun key range value: ";
	myfile << "Enter the minimun key range value: ";

	//Min value input
	while (!(cin >> keyMin))
	{
		myfile << keyMin << endl;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Please enter an integer: ";
		myfile << "Please enter an integer: ";
	}

	cout << "Enter the maximum key range value: ";
	myfile << "Enter the maximum key range value: ";

	//Max value input
	while (!(cin >> keyMax) || (keyMax <= keyMin))
	{
		myfile << keyMax << endl;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Please enter an integer greater than " << keyMin << " : ";
		myfile << "Please enter an integer greater than " << keyMin << " : ";
	}
	 
	cout << " Your range is " << keyMin << " to " << keyMax << endl;
	myfile  << " Your range is " << keyMin << " to " << keyMax << endl;

	int approve = 0;

	//Asks for bucket size input, checks if the input is prime, and asks for user validation if it is not
	do
	{
		cout << "What should the number of buckets be for the Hash Table? (Best is prime): " << endl;
		myfile << "What should the number of buckets be for the Hash Table? (Best if prime): " << endl;
		while (!(cin >> buckets) || (buckets < 3 ))
		{
			myfile << buckets << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Please enter a postive integer greater than 2: ";
			myfile << "Please enter a postive integer greater than 2: ";
		}
	
		//Check if the input is prime
			for (int i = 2; i <= buckets / 2; ++i)
			{
				if (buckets % i == 0)
				{
					//Confirmation message 
					cout << "/nThe value you have entered is not a prime number, to continue with your current value input 1, input 0 if you would like to input a new bucket value: ";
					myfile << "/nThe value you have entered is not a prime number, to continue with your current value input 1, input 0 if you would like to input a new bucket value: ";
					cin >> approve;
					myfile << approve << endl;
					break;
				}
				else
				{
					//Confirmation message 
					cout << "\nThe bucket value " << buckets << " is prime" << endl;
					myfile << "\nThe bucket value " << buckets << " is prime" << endl;
					approve = 1;
					break;
				}
				
			}

			
	}while (approve == 0);
	
}

/*
Calculates the hash value for a given key
Returns the calculated has value
*/
int CalculateHash(int k)
{
	if (k < keyMin || k > keyMax) {

		cout << "The input key is not valid given the set range" << endl;
		myfile << "The input key is not valid given the set range" << endl;
		cout << " Your range is " << keyMin << " to " << keyMax << endl;
		myfile << " Your range is " << keyMin << " to " << keyMax << endl;

		return -1;
	}
	else
	{
		//Hash function
		int hash = k % buckets;
		return hash;
	}
}

/*
Creates a hash table bsed on the user preferences
Returns pointer to head of new table
Run one at the beginning of the program
*/
node* CreateTable()
{
	for (i = 0; i < buckets; i++)
	{
		
		//Sets the head pointer if the head node is Null at index 0
		if (i == 0 && head == NULL)
		{

			head = createBucketNode(i); //Sets head equal to a new node with data o

		}
		else
		{

			node* tempNodeI = head; //temp pointer is set equal to the head pointer
			node* tempNodeII = createBucketNode(i); //temp 2 is set equal to a new node
			int loc = 0;
			while (loc != (i - 1)) //finds node at index i-1
			{
				tempNodeI = tempNodeI->next;
				loc++;
			}

			if (i < buckets)
			{
				tempNodeI->next = tempNodeII;
			}
			
			//Performs if the element is at the end of the linked list
			if (loc == buckets)
			{
				tempNodeI->next = tempNodeII; //Current node at the location set to point at new node
			}
		}
	}

	return head;
}

/*
Returns the entry with key k and hash h
*/
int BucketGet(int h, int k)
{
	int entry = 0;
	
	node* tempNode = head;
	int loc = 0;
	while (loc != h)//Finds bucket node at index i
	{
		tempNode = tempNode->next;
		loc++;
	}

	tempNode = tempNode->chain;

	while (tempNode!= NULL && tempNode->key != k)//Finds node with key k
	{
		tempNode = tempNode->next;
	}

	if (tempNode != NULL) 
	{
		entry = tempNode->data;

		cout << "The value at this key is " << entry << endl;
		myfile << "The value at this key is " << entry << endl;
		return entry;
	}
	else
	{
		cout << "A value des not exist at this key value" << endl;
		myfile << "A value des not exist at this key value" << endl;
		return -1;
	}

	
}

/*
Adds the value v with key k in the hash bucket h
*/
void BucketPut(int h, int k, int v)
{

	node* tempNode = head;
	node* bucketNode;

	int loc = 0;
	while (loc != h)//Finds bucket node at index i
	{
		tempNode = tempNode->next;
		loc++;
	}

	bucketNode = tempNode;
	tempNode = tempNode->chain;

	//Sets the head pointer of a bucket if the head node is Null 
	if (bucketNode->chain == NULL)
	{
		bucketNode->chain = createChainNode(v,k); //Sets head equal to a new node with data o
	}
	//Sets adds a node or overrights active keys
	else
	{
		
		node* tempNodeI = bucketNode->chain; //temp pointer is set equal to the head pointer
		node* tempNodeII = createChainNode(v,k); //temp 2 is set equal to a new node

		//Overwrites the data of existing keys if the input key matches an existing key, otherwise adds a node to the list
		while (tempNodeI != NULL) 
		{
			if (tempNodeI->key == k)
			{
				tempNodeI->data = v;
				break;
			}
			else if (tempNodeI->next == NULL)
			{
				tempNodeI->next = createChainNode(v, k);
				
				break;
			}
			tempNodeI = tempNodeI->next;
			
		}

	}
}

/*
Removes an entry wth key k and hash h
Returns the removed value
*/
int BucketRemove(int h, int k)
{
	
	node* tempNode = head;
	node* bucketNode;
	
	int loc = 0;
	while (loc != h)//Finds node at index h
	{
		tempNode = tempNode->next;
		loc++;
	}

	bucketNode = tempNode;
	tempNode = bucketNode->chain;
	node* prevNode = NULL;

	int i = 0;

	//Checks if the bucket is empty
	if (tempNode == NULL) 
	{
		cout << "A Value does not exist for this key" << endl;
		myfile << "A Value does not exist for ths key" << endl;
		return - 1;
	}

	while ((tempNode->key != k) && (tempNode->next != NULL))//Finds node with key k
	{
		prevNode = tempNode;
		tempNode = tempNode->next;
		i++;
	}
	
	//Checks if the bucket does not cotain the key value and rmoves based on locatio if it does
	if ((tempNode->key != k) && (tempNode->next == NULL))
	{
		cout << "A Value does not exist for this key" << endl;
		myfile << "A Value does not exist for ths key" << endl;
		return -1;
	}
	else 
	{
		int bucketSize = BucketCount(h);
		int tempInt = 0;
		
		if (i == 0)//If the node is the first element
		{
			
			node* tempNodeI = tempNode; //temp node set equal to head
			node* tempNodeII = tempNode->next; //temp node 2 set equal to node after head

			//Performs if the pointer to the node after the head is Null
			if (tempNodeII == NULL)
			{
				bucketNode->chain = NULL; //set head node equal to null
				tempInt = tempNode->data;
			}
			else
			{
				bucketNode->chain = tempNodeII;// temp node 3 equals the node after temp 2
				tempInt = tempNode->data;
			}
			
			cout << "The element you removed is " << tempInt << endl;
			myfile << "The element you removed is " << tempInt << endl;
			return  tempInt;
		}
		//Performs if the node to be removed is at the end of the linked list
		else if (i == (bucketSize-1) )
		{

			node* tempNodeI = tempNode;
			
			tempInt = tempNodeI->data; //stores the data at the last node
			prevNode->next = NULL; //has the seond to last node pont to null, now becoming the last node

			cout << "The element you removed is " << tempInt << endl;
			myfile << "The element you removed is " << tempInt << endl;
			return  tempInt;

		}
		//Performes if removing from the middle
		else
		{

			node* tempNodeI = tempNode;
			node* tempNodeII = tempNode->next;
			node* tempNodeIII = prevNode;

			int tempInt = tempNodeI->data;// stores the data in temp node 2
			tempNodeIII->next = tempNodeII;// sets temp3 equal to the node after temp2

			cout << "The element you removed is " << tempInt <<  endl;
			myfile << "The element you removed is " << tempInt << endl;
			return  tempInt;
		}
	}
}


/*
	Runs the user interface which interacts with all ListADT methods.
	Outputs a text file of all program inputs and outputs when properly exitted
*/
int main()
{

	cout << "ENGR2400 Data Structures and Algorithms\nFall 2019\nRyley Danielson\nProgramming Assignment 3 - Hash Table ADT\nThe goal of this assignment is to create a Hash Table abstract data type class using a linked-list as the base for the list implementation. \n\n";
	myfile << "ENGR2400 Data Structures and Algorithms\nFall 2019\nRyley Danielson\nProgramming Assignment 3 - Hash Table ADT\nThe goal of this assignment is to create a Hash Table abstract data type class using a linked-list as the base for the list implementation. \n\n";
	myfile.open("OutputLog.txt");//Open file steam for program

	bool exit = false; //Begining exit value

	GetUserParameters();
	CreateTable();

	cout << "Choose an action by pressing the number for the action (1,2,3...):\n(1)Add an element to the Hash Table\n(2)Get an element from the Hash Table\n(3)Remove an element from the Hash Table\n(4)Check the number of entries in a bucket\n(5)Exit the Program\n\n";
	myfile << "Choose an action by pressing the number for the action (1,2,3...):\n(1)Add an element to the Hash Table\n(2)Get an element from the Hash Table\n(3)Remove an element from the Hash Table\n(4)Check the number of entries in a bucket\n(5)Exit the Program\n\n";

	
	//User interface loop. Exits loop when the exit value is true (Option 7)
	do
	{
		cin >> choice;
		myfile << "Choice" << choice << endl;

		switch (choice)
		{
		case 1:
			int value;
			int hash;

			cout << "What is the key value of the entry\n";
			myfile << "What is the key value of the entry\n";
			cin >> key;
			myfile << key << endl;
			cout << "What is the value of the entry at this key\n";
			myfile << "What is the value of the entry at this key\n";
			cin >> value;
			myfile << value << endl;
			
			hash = CalculateHash(key);

			if (hash == -1) {
				cout << "Select another action (1, 2, 3, ...)\n";
				myfile << "Select another action (1, 2, 3, ...)\n";
			}
			else
			{
				BucketPut(hash, key, value);
			}
			break;
		case 2:

			cout << "What is the key value of the entry you would like to get\n";
			myfile << "What is the key value of the entry you would like to get\n";
			cin >> key;
			myfile << key << endl;

			hash = CalculateHash(key);

			if (hash == -1) {
				cout << "Enter a valid field\n";
				myfile << "Enter a valid field\n";
			}
			else
			{
				BucketGet(hash, key);
			}
			break;
		case 3:

			cout << "What is the key value of the entry you would like to remove\n";
			myfile << "What is the key value of the entry you would like to remove\n";
			cin >> key;
			myfile << key << endl;

			hash = CalculateHash(key);

			if (hash == -1) {
				cout << "Enter a valid field\n";
				myfile << "Enter a valid field\n";
			}
			else
			{
				BucketRemove(hash, key);
			}

			break;
		case 4:
			cout << "Which bucket do you want to find the contents of\n";
			myfile << "Which bucket do you want to find the contents of\n";

			int b;
			cin >> b;
			myfile << b << endl;

			BucketCount(b);

			break;
		case 5:
			exit = true;

			cout << "Exiting program: " << exit << endl;
			myfile << "Exiting program: " << exit << endl;

			break;
		default:
			cout << "\nYou have input an improper selection, please choose again\n";
			myfile << "\nYou have input an improper selection, please choose again\n";

			break;
		}
		
		
		//Outputs the current Hash Table to the console
		cout << "Current Table: \n";
		myfile << "Current Table: ";
		for (i = 0; i<buckets; i++)
		{
			cout << "Bucket " << i << ": ";
			
			node* tempNode = head;
			
			while (tempNode->data != i) {
				tempNode = tempNode->next;
			}

			tempNode = tempNode->chain;
			while (tempNode != NULL)
			{
				cout << tempNode->data << ", ";
				myfile << tempNode->data << ", ";
				tempNode = tempNode->next;
			}
			cout << endl;
			myfile << endl;
		}
	} while (exit == false);
	

	myfile.close(); //Close file stream

	return 0;
}