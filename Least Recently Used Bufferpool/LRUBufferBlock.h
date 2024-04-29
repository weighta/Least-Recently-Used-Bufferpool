#pragma once

#include "BufferBlockADT.h"

#ifndef BUFFERBLOCK_H
#define BUFFERBLOCK_H


class BufferBlock : public BufferblockADT {
private:
    int blockID;
    char* block;
    int size;
public:
    BufferBlock() {
        blockID = NULL;
        block = NULL;
        size = NULL;
    }
    BufferBlock(char* data, int sz) {
        block = data;
        size = sz;
        blockID = -1;
    }
    ~BufferBlock() {}
    void getData(int pos, int sz, char* data) {
        pos %= size; //we need to reduce within range of 4096
        memcpy(data, block + pos, sz); //most likely the fastest-essential tool to use here
    }
    void setID(int id) {
        blockID = id;
    }
    int getID() const {
        return blockID;
    }
    int getBlocksize() const {
        return size;
    }
    char* getBlock() const {
        return block;
    }
    void setBlock(char* blk) {
        block = blk;
    }
};

#endif