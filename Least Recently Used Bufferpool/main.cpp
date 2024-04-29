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

using namespace std;

int main() {
	sayl("Alexander Weight -- CSIS 215 Programming Assignment 3 -- Least Recently Used Bufferpool\n");


    //initialize buffer pool
	char filename[0xFF];
	memset(filename, 0x0, 0xFF);
	memcpy(filename, &"mydatafile.txt", 15);
    LRUBufferPool* bp = new LRUBufferPool(filename, POOL_SIZE, BLOCKSIZE);
    
    //get data from the buffer
    char* data = new char[10];
    bp->getBytes(data, 10, 5030);
    printChars(data, 10, 5030/BLOCKSIZE);
    bp->printBufferBlockOrder();
	/*Output should be something like the following:
		My data for block 1 is: "ment all o"
		My buffer block order from most recently used to LRU is:
			1, 0, 2, 3, 4,
	*/
    
	//re-initialize the char array and get the next block of data
    initializeCharArray(10, data);
    bp->getBytes(data, 10, 16500);
    printChars(data, 10, 16500/BLOCKSIZE);
    bp->printBufferBlockOrder();
	/*Output should be something like the following:
		My data for block 4 is: "e for the "
		My buffer block order from most recently used to LRU is:
			4, 1, 0, 2, 3,
	*/
	

	//re-initialize the char array and get the next block of data
    initializeCharArray(10, data);
    bp->getBytes(data, 10, 24640);
    printChars(data, 10, 24640/BLOCKSIZE);
    bp->printBufferBlockOrder();
	/*Output should be something like the following:
		My data for block 6 is: "ent a Buff"
		My buffer block order from most recently used to LRU is:
			6, 4, 1, 0, 2,
	*/
	
	//re-initialize the char array and get the next block of data
    initializeCharArray(10, data);
    bp->getBytes(data, 10, 28700);
    printChars(data, 10, 28700/BLOCKSIZE);
    bp->printBufferBlockOrder();
	
	//re-initialize the char array and get the next block of data
    initializeCharArray(10, data);
    bp->getBytes(data, 10, 16600);
    printChars(data, 10, 16600/BLOCKSIZE);
    bp->printBufferBlockOrder();
	
	//close program
	cout << endl << endl;
	system("pause");
	return 0;
}
