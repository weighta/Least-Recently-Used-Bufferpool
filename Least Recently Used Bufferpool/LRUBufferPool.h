#pragma once
#include "BufferPoolADT.h"
#include "LRUBufferBlock.h"
#include <fstream>

#ifndef LRUBUFFERPOOL_H
#define LRUBUFFERPOOL_H

class LRUBufferPool : public BufferPoolADT {
private:
	int poolSize;
	int blockSize;
	int* blockIDList;
	BufferBlock* bufferBlock;
	std::fstream input;
	int streamSize;

	void pullBlock(char* block, int id);
	void instBufferBlocks(char* filename);
	int calcID(int pos);
	char existsInBuffer(int id);

public:
	LRUBufferPool(char* filename, int pool_Size, int block_Size) {
		poolSize = pool_Size;
		blockSize = block_Size;

		bufferBlock = new BufferBlock[pool_Size];
		blockIDList = new int[pool_Size];
		input.open(filename, fstream::in | fstream::binary);
		input.seekg(0, input.end);
		int len = input.tellg();
		instBufferBlocks(filename);
	}
	~LRUBufferPool() {
		delete[] bufferBlock;
		delete[] blockIDList;
	}
	void getBytes(char* space, int sz, int pose) {
		int id = calcID(pose);
		bufferBlock[id].getData(pose, sz, space);
	}
	void printBufferBlockOrder() {
		for (int i = 0; i < poolSize; i++) {
			std::cout << blockIDList[i];
			if (i == poolSize - 1) {}
			else {
				std::cout << " ";
			}
		}
	}
	int getLRUBlockID() {
		return blockIDList[poolSize - 1];
	}
};

int LRUBufferPool::calcID(int pos) {
	return blockSize / pos;
}

char LRUBufferPool::existsInBuffer(int id) {
	for (int i = 0; i < poolSize; i++) {
		if (blockIDList[i] == id) return 1;
	}
	return 0;
}

void LRUBufferPool::pullBlock(char* block, int id) {

	input.seekg(0, input.beg);

	char* buffer = new char[len];
	input.read(buffer, len);
}

void LRUBufferPool::instBufferBlocks(char* filename) {
	

	char randata[4096];
	for (int i = 0; i < poolSize; i++) {
		bufferBlock[i] = BufferBlock(randata, blockSize);
	}
}

#endif