#ifndef CHAIN_H_
#define CHAIN_H_

#include "block.hpp"

class Chain
{
public:
    std::vector<Block*> blocks;

    Chain();

    int add(Block* &block);

    std::string toString();
};

#endif