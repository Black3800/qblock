#include "chain.hpp"
#include "block.hpp"

#include <string>

Chain::Chain()
{
    GenesisBlock* genesis = new GenesisBlock();
    blocks.push_back(genesis);
}

int Chain::add(Block* &block)
{
    if (block->previousHash != blocks.back()->hash()) return QBLOCK_CHAIN_ADD_PREVHASH_MISMATCH;
    if (block->verify() != 0) return QBLOCK_CHAIN_ADD_VERIFY_FAILED;
    blocks.push_back(block);
    return 0;
}

std::string Chain::toString()
{
    std::string stringRepresentation = "[";
    for (auto b : blocks)
    {
        stringRepresentation += (*b).toString() + ",\n";
    }
    stringRepresentation += "]";
    return stringRepresentation;
}