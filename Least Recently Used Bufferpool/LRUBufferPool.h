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
	void foundLRU(int pos);
	void setLRU(int id);
	int calcID(int pos);
	int indexOfID(int id);
	int getLRUBufferBlockID();
	char existsInBuffer(int id);
	
public:
	LRUBufferPool(char* filename, int pool_Size, int block_Size) {
		poolSize = pool_Size; //set the pool size private variable
		blockSize = block_Size; //set the block size private variable

		bufferBlock = new BufferBlock[pool_Size]; //Okay, this is fine, normally I wouldn't put this on the heap

		openStream(filename);
		instBufferBlocks();
	}
	~LRUBufferPool() {
		delete[] bufferBlock;
	}

	void getBytes(char* space, int sz, int pose) {
		if (pose + sz <= streamSize) { //make sure no overflow
			int id = calcID(pose);
			if (id == bufferBlock[0].getID()) {} //In the case the first is used again, nothing shifts
			else {
				int i = indexOfID(id);
				char nExists = (i & 0x80000000) >> 24;
				if (nExists) {
					setLRU(id);
				}
				else {
					foundLRU(i);
				}
			}
			bufferBlock[0].getData(pose, sz, space);
		}
		else return;
	}
	void printBufferBlockOrder() {
		for (int i = 0; i < poolSize; i++) {
			std::cout << bufferBlock[i].getID();
			if (i == poolSize - 1) {
				std::cout << '\n';
			}
			else {
				std::cout << " ";
			}
		}
	}
	int getLRUBlockID() {
		return bufferBlock[poolSize - 1].getID();
	}
};

int LRUBufferPool::calcID(int pos) {
	return pos / blockSize;
}


char LRUBufferPool::existsInBuffer(int id) {
	for (int i = 0; i < poolSize; i++) {
		if (bufferBlock->getID() == id) return 1;
	}
	return 0;
}


void LRUBufferPool::pullBlock(char* block, int id) {
	int pos = id * blockSize;
	if (pos + blockSize <= streamSize) {
		input.seekg(pos, input.beg);
		input.read(block, blockSize);
	}
	else {
		block[0] = -1; //outside stream
	}
}

void LRUBufferPool::instBufferBlocks() {
	for (int i = 0; i < poolSize; i++) {
		char* blockData = new char[blockSize];
		pullBlock(blockData, i);
		bufferBlock[i] = BufferBlock(blockData, blockSize);
		bufferBlock[i].setID(i);
	}
}

void LRUBufferPool::openStream(char* filename) {
	input.open(filename, fstream::in | fstream::binary);
	input.seekg(0, input.end);
	streamSize = input.tellg();
}

int LRUBufferPool::indexOfID(int id) {
	for (int i = 0; i < poolSize; i++) {
		if (id == bufferBlock[i].getID()) return i;
	}
	return -1;
}

void LRUBufferPool::foundLRU(int pos) {
	BufferBlock tmp = bufferBlock[pos];
	for (int i = pos; i >= 1; i--) {
		bufferBlock[i] = bufferBlock[i - 1];
	}
	bufferBlock[0] = tmp;
}

void LRUBufferPool::setLRU(int id) {
	//e.g sets arry from
	//0, 4, 2, 3, 1
	//to
	//7, 0, 4, 2, 3
	BufferBlock last = bufferBlock[poolSize - 1];
	for (int i = poolSize - 1; i >= 1; i--) {
		bufferBlock[i] = bufferBlock[i - 1];
	}
	bufferBlock[0] = last;
	pullBlock(bufferBlock[0].getBlock(), id);
	bufferBlock[0].setID(id);
}

int LRUBufferPool::getLRUBufferBlockID() {
	return bufferBlock[poolSize - 1].getID();
}

#endif