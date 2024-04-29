#pragma once
#include "BufferPoolADT.h"
#include "LRUBufferBlock.h"
#include <fstream>

#ifndef LRUBUFFERPOOL_H
#define LRUBUFFERPOOL_H

class LRUBufferPool : public BufferPoolADT {
private:
	// My private variables
	int poolSize;
	int blockSize;
	BufferBlock* bufferBlock;
	std::fstream input;
	int streamSize;

	// My private functions
	void pullBlock(char* block, int id);
	void openStream(char* filename);
	void instBufferBlocks();
	void bringIdForward(int pos);
	void setLRU(int id);
	int calcID(int pos);
	int indexOfID(int id);
	int getLRUBufferBlockID();
	
public:
	LRUBufferPool(char* filename, int pool_Size, int block_Size) {
		poolSize = pool_Size; //set the pool size private variable
		blockSize = block_Size; //set the block size private variable

		bufferBlock = new BufferBlock[pool_Size]; //Okay, this is fine
		//normally I wouldn't work this on the heap if we know it's 5 buffer blocks
		//but it also also provides dynamic size if for some reason needed

		openStream(filename); //open the stream with the filename
		instBufferBlocks(); //instantiate the buffer block array
	}
	~LRUBufferPool() {
		for (int i = 0; i < poolSize; i++) {
			delete[] bufferBlock[i].getBlock();
		}
		delete[] bufferBlock; //remove the buffer blocks from the heap
	}

	void getBytes(char* space, int sz, int pose) {
		if (pose + sz <= streamSize) { //make sure no overflow
			int id = calcID(pose); //find the id based on pose
			if (id == bufferBlock[0].getID()) {} //if first is used again, nothing shifts. this will save resources
			else { //otherwise do some shuffling
				int i = indexOfID(id); //see if the id exists already, -1 if otherwise
				char nExists = (i & 0x80000000) >> 31; //checks to see if number is negative
				if (nExists) { //if index is -1
					setLRU(id); //then we need to accomadate LRU and replace it with the new block containing it
				}
				else {
					bringIdForward(i); //brings the id forward but does not need a new block
				}
			}
			bufferBlock[0].getData(pose, sz, space); //regardless, we will always find that the first buffer block has our element we're looking for
		}
		else return; //do nothing if the position is outside the bounds of the stream
	}
	/// <summary>
	/// prints the buffer block order based on sequential ids
	/// </summary>
	void printBufferBlockOrder() {
		for (int i = 0; i < poolSize; i++) {
			std::cout << bufferBlock[i].getID(); //just grab the id, and print it
			if (i == poolSize - 1) { //formatting checks
				std::cout << '\n';
			}
			else {
				std::cout << " ";
			}
		}
	}
	/// <summary>
	/// reeturns the least recently used buffer block ID
	/// </summary>
	/// <returns></returns>
	int getLRUBlockID() {
		return bufferBlock[poolSize - 1].getID();
	}
};

/// <summary>
/// calculates the block id based on a position lookup
/// </summary>
/// <param name="pos"></param>
/// <returns></returns>
int LRUBufferPool::calcID(int pos) {
	return pos / blockSize; //in this case, div by an ambiguous integer requires '/' instead of '>>' if we dont know the divisor is log_2 equivalent
}

/// <summary>
/// pulls data from the stream and into the block
/// </summary>
/// <param name="block"></param>
/// <param name="id"></param>
void LRUBufferPool::pullBlock(char* block, int id) {
	int pos = id * blockSize; //the block position will be the id times the block size
	if (pos + blockSize <= streamSize) { //make sure we're not reading over the stream size
		input.seekg(pos, input.beg); //seek the data
		input.read(block, blockSize); //read and assign the data to block
	}
	else {
		block[0] = -1; //outside stream
	}
}

/// <summary>
/// instantiates all buffer blocks
/// </summary>
void LRUBufferPool::instBufferBlocks() {
	for (int i = 0; i < poolSize; i++) { //loop through all buffer blocks
		char* blockData = new char[blockSize]; //give a new character array for each buffer block
		pullBlock(blockData, i); //give the data matching ids 0, 1, 2, etc for each block
		bufferBlock[i] = BufferBlock(blockData, blockSize); //assign each buffer block to the bufferBlock[] array
		bufferBlock[i].setID(i); //finally set all buffer blocks their proper, sequential id
	}
}

/// <summary>
/// Opens the stream, discovers the filesize, awaits pulling data
/// </summary>
/// <param name="filename"></param>
void LRUBufferPool::openStream(char* filename) {
	input.open(filename, fstream::in | fstream::binary); //open the file through the stream with filename
	input.seekg(0, input.end); //seek the entire file
	streamSize = input.tellg(); //capture the full size for future overflow checking
}

/// <summary>
/// returns the index of the id assigned to in bufferBlock[]
/// </summary>
/// <param name="id"></param>
/// <returns></returns>
int LRUBufferPool::indexOfID(int id) {
	for (int i = 0; i < poolSize; i++) {
		if (id == bufferBlock[i].getID()) return i; //well loop through and find when the id matches, and return its index
	}
	return -1; //if no id matches, theres no index, so return -1
}

/// <summary>
/// brings the bufferBlock forward at pos and slides the rest of the buffer blocks back
/// </summary>
/// <param name="pos"></param>
void LRUBufferPool::bringIdForward(int pos) {
	BufferBlock tmp = bufferBlock[pos]; //create a temporary buffer block so that we can slide everything forward
	for (int i = pos; i >= 1; i--) {
		bufferBlock[i] = bufferBlock[i - 1]; //make sure to assign the one in front to the one behind it
	}
	bufferBlock[0] = tmp; //then put the temporary buffer block at the beginning
}

/// <summary>
/// sets the last buffer block to a new id and brings it forward
/// </summary>
/// <param name="id"></param>
void LRUBufferPool::setLRU(int id) {
	//e.g sets arry from
	//0, 4, 2, 3, 1
	//to
	//7, 0, 4, 2, 3
	BufferBlock last = bufferBlock[poolSize - 1]; //Make a temporary buffer block from the last buffer block
	for (int i = poolSize - 1; i >= 1; i--) {
		bufferBlock[i] = bufferBlock[i - 1]; //slide all buffer blocks back
	}
	bufferBlock[0] = last; //assign the block in front with the temporary one
	pullBlock(bufferBlock[0].getBlock(), id); //make the newly forwarded buffer block have newly corresponding data with its id
	bufferBlock[0].setID(id); //finally set the id of the buffer block
}

//Function to get the least recently used buffer block ID
int LRUBufferPool::getLRUBufferBlockID() {
	return bufferBlock[poolSize - 1].getID();
}

#endif