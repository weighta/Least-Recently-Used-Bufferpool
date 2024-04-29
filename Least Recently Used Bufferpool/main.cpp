/*
* Alexander P. Weight
* Professor Jason Christian
* CSIS215: Algorithms and Data Structures
* April 28, 2024
* 
*/
/* 
 * File:   main.cpp
 * Author: Professor Terri Sipantzi
 *
 * Created on August 25, 2012, 8:49 AM
 */

#include "constants.h"
#include "LRUBufferPool.h"
#include "macros.h"
//#include <string> //I do not need string for my liking

//using namespace std; //For good practice, I will not be using this

int main() {
	sayl("Alexander Weight -- CSIS 215 Programming Assignment 3 -- Least Recently Used Bufferpool\n");


    //initialize buffer pool
	char filename[0xFF]; //by default windows allows a maximum of 255 characters for filename
	memset(filename, 0x0, 0xFF); // this will save resources instead of using string
	memcpy(filename, &"mydatafile.txt", 15); //just copy a character array to the filename

    LRUBufferPool* bp = new LRUBufferPool(filename, POOL_SIZE, BLOCKSIZE); //Open the buffer pool
    
    //get data from the buffer
    char* data = new char[10];
    bp->getBytes(data, 10, 5030);
    printChars(data, 10, 5030 >> 12); //Changed '/' to '>>' because dividing by 4096 is equivalent to binary shift 12 to the right
    bp->printBufferBlockOrder();
	/*Output should be something like the following:
		My data for block 1 is: "ment all o"
		My buffer block order from most recently used to LRU is:
			1, 0, 2, 3, 4,
	*/
    
	//re-initialize the char array and get the next block of data
    initializeCharArray(10, data);
    bp->getBytes(data, 10, 16500);
    printChars(data, 10, 16500 >> 12);
    bp->printBufferBlockOrder();
	/*Output should be something like the following:
		My data for block 4 is: "e for the "
		My buffer block order from most recently used to LRU is:
			4, 1, 0, 2, 3,
	*/
	

	//re-initialize the char array and get the next block of data
    initializeCharArray(10, data);
    bp->getBytes(data, 10, 24640);
    printChars(data, 10, 24640 >> 12);
    bp->printBufferBlockOrder();
	/*Output should be something like the following:
		My data for block 6 is: "ent a Buff"
		My buffer block order from most recently used to LRU is:
			6, 4, 1, 0, 2,
	*/
	
	//re-initialize the char array and get the next block of data
    initializeCharArray(10, data);
    bp->getBytes(data, 10, 28700);
    printChars(data, 10, 28700 >> 12);
    bp->printBufferBlockOrder();
	
	//re-initialize the char array and get the next block of data
    initializeCharArray(10, data);
    bp->getBytes(data, 10, 16600);
    printChars(data, 10, 16600 >> 12);
    bp->printBufferBlockOrder();
	
	//close program
	cout << endl << endl;
	system("pause"); //this is fine i suppose
	return 0;
}
