
/*******************************************************************
*   CS221 Programming Assignment 4
*   Wesley Adams
*   Programming Assignment 4 and An exploration of hashing functions.
*   
*   This program is entirely my own work
*******************************************************************/

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cmath>
#include <conio.h>
#include <string.h>

#define DEBUG

#define TABLESIZE     100
#define KEYSIZE       4
#define EMPTYKEY      "----"
#define DATAFILE      "P4DATA.TXT"
#define PRIME         7

using namespace std;

//Hash data structure
struct HashStruct
{
     char key[5];
     int data;
};

//Prototypes
int getFoldingHash(char *arr);
int getMiddleSquaringHash(char *arr);
int getTruncationHash(char *arr);
int getBase26Hash(char *arr);
int ProbeDec_1(char *arr);
int ProbeDec_2(char *arr);
int ProbeDec_3(char *arr);
void InitTable(HashStruct hashT[], int TableSize);
int HashInsert(HashStruct T[], char *key, int data, int hNum, int dhNum);
void Display(HashStruct T[], int hashNum, int dHashNum, int count);


int main()
{
	//Initializations
	 int          i, hashNum, dHashNum, count;  //integer values
     ifstream     *inFile; //ifstream
     HashStruct   T[100];  // Hash table srray of 100 data structures
     char         line[64];// Array to hold lines read from file
     char         key[5];  // Array to hold 4-character keys
     int          data;    // Integer data

	 /* --- Snippet 2: The following code can be used to perform the 9 tests. Use 
                        hashNum to increment a loop for each of 3 hash functions and 
                        dHashNum to increment a nested loop for each of 3 double 
                        hash functions. --- */
				
      // For each of three hash functions
      for(hashNum = 0; hashNum < 3; hashNum++)
      {
         // For each of three double hash functions
         for(dHashNum = 0; dHashNum < 3; dHashNum++)
         {
            InitTable(T, TABLESIZE);               // Call function to initialize table to empty
            inFile = new ifstream();
            inFile->open(DATAFILE, ifstream::in);   // Open data file for this test
            if(!inFile->is_open())
            {
               cout << "Unable to open data file. Program terminating\n";
               return 0;
            }
            count = 0;     // Initialize collision counter
            for(int i = 0; i < 50; i++)
            {
                 inFile->getline(line, 64, '\n');
                 sscanf(line, "%s %d", key, &data);
                 count += HashInsert(T, key, data, hashNum, dHashNum);
                 // Note: The HashInsert() function uses the indices of hashNum and
                 // dHashNum to know which hash function and double hash function to call
                 // in this test.
            }
            inFile->close();		/* Close the text file */
            delete inFile;

			//display
			Display(T,hashNum, dHashNum, count);
         }
      }
	  //char x;
	  //cin>>x;

	return 0;
}

//Returns a hash created via folding method
int getFoldingHash(char *arr)
{

	//declare hash
	int hash = 0;
	int val1 = (int)(arr[0]);
	int val2 = (int)(arr[1]);
	int val3 = (int)(arr[2]);
	int val4 = (int)(arr[3]);

	//calculate
	hash = ((val1 * val2) + (val3*val4)) % TABLESIZE;

	return hash;

}

//Returns a hash created via middle squaring method
int getMiddleSquaringHash(char *arr)
{

	//declare hash
	int hash = 0;

	//calculate
    hash = (int)pow(((arr[1] - 'A' + 1) + (arr[2] - 'A' + 1)), 2);
	hash = hash % TABLESIZE;
	return hash;
}

//Returns a hash created via truncation method
int getTruncationHash(char *arr)
{

	//declare hash
	int hash = 0;
	int val1 = (int)(arr[0]);
	int val2 = (int)(arr[1]);

	//calculate
	hash = (val1 * val2) % TABLESIZE;

	return hash;
}

//Returns a has created via base 26 method
int getBase26Hash(char *arr)
{

	//declare and initialize hash
	int hash = 0;

	//set A
	int valA = (int)'A';

	int val1 = (int)(arr[0]);
	int val2 = (int)(arr[1]);
	int val3 = (int)(arr[2]);
	int val4 = (int)(arr[3]);

	//calculate
	hash = (((val1 - valA)+1)*pow(26,3)) + (((val2 - valA) +1)*pow(26,2)) + (((val3 - valA)+1)*pow(26,1))+ (((val4 - valA)+1)*pow(26,0));
	hash = hash % TABLESIZE;
	return hash;
}

//Increment by 1
int ProbeDec_1(char *arr)
{

	return 1;
}

//Increment by hash
int ProbeDec_2(char * arr)
{

	//calculate hash again to add to inc
	int hash = getMiddleSquaringHash(arr);
      
	return hash;
}

//Increment by other hash function
int ProbeDec_3(char *arr)
{

	//Declare hash
	int hash = 0;

	//calculate
    hash = max((arr[3] / PRIME), 1) % PRIME;

    return (hash % TABLESIZE);

}

/* --- Snippet 4: Initialize table function --- */
void InitTable(HashStruct hashT[], int TableSize)
{
    int i;
	
    for(i=0; i<TableSize; i++)
    {
        strcpy(hashT[i].key, EMPTYKEY);
        hashT[i].data = 0;
    }
}
    
/*--- Snippet 5: Hash Insert function testNum is a number from 0 through 8
				indicating which test is being run.  This can be calculated 
				from hashNum and dHashNum in snippet 2, for example:
				testNum = (hashNum * 3) + dHashNum.  --- */
int HashInsert(HashStruct T[], char *key, int data, int hNum, int dhNum)
{

    int  testNum = (hNum * 3) + dhNum;
    int  colCount = 0;
    int  hashIndex, probeDec;

    switch(testNum)
    {
            case 0 :  // Hash function 1 -- Double hash 1 (linear probing) 
                    hashIndex = getFoldingHash(key);
                    probeDec = ProbeDec_1(key); // Function just returns 1 
                    break;
            case 1 :  // Hash function 1 -- Double hash 2  
                    hashIndex = getFoldingHash(key);
                    probeDec = ProbeDec_2(key);
                    break;
            case 2 :  // Hash function 1 -- Double hash 3  
                    hashIndex = getFoldingHash(key);
                    probeDec = ProbeDec_3(key);
                    break;
            case 3 :  // Hash function 2 -- Double hash 1 (linear probing)  
                    hashIndex = getMiddleSquaringHash(key);
                    probeDec = ProbeDec_1(key); // Function just returns 1
                    break;
            case 4 :  // Hash function 2 -- Double hash 2  
                    hashIndex = getMiddleSquaringHash(key);
                    probeDec = ProbeDec_2(key);
                    break;
            case 5 :  // Hash function 2 -- Double hash 3  
                    hashIndex = getMiddleSquaringHash(key);
                    probeDec = ProbeDec_3(key);
                    break;
            case 6 :  // Hash function 3 -- Double hash 1 (linear probing)  
                    hashIndex = getTruncationHash(key);
                    probeDec = ProbeDec_1(key); // Function just returns 1
                    break;
            case 7 :  // Hash function 3 -- Double hash 2  
                    hashIndex = getTruncationHash(key);
                    probeDec = ProbeDec_2(key);
                    break;
            case 8 :  // Hash function 3 -- Double hash 3  
                    hashIndex = getTruncationHash(key);
                    probeDec = ProbeDec_3(key);
                    break;
    }
	// Find a place to insert into the table
    while(strcmp(T[hashIndex].key, EMPTYKEY) != 0)
    {
            colCount++;
            hashIndex -= probeDec;  // Decrementing was chosen you could also choose to
            if(hashIndex < 0)    //  increment and wrap back to the beginning of the table.
                hashIndex = hashIndex + TABLESIZE;
    }

	//Insert data
	strcpy(T[hashIndex].key, key);
	T[hashIndex].data = data;

	return colCount;
}

//Display function
void Display(HashStruct T[], int hashNum, int dHashNum, int count)
{
	#ifdef DEBUG
	cout<<"Reached display"<<endl;
#endif
	cout << "Testing hash function " << hashNum << " using double hash " << dHashNum << ".\n";
	cout << "Total collisions = " << count << ".\n";
	// Show the form of the array
	for(int i=0; i < 100; i++)
		if(strcmp(T[i].key, EMPTYKEY)) // strcmp gives a non-zero (true) result if Key is NOT the EMPTYKEY
			cout << "|";     // Indicate an entry at this location
		else
			cout << "-";     // Indicate no entry at this location
	cout << "\n\n";
}


