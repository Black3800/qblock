#ifndef BLOCK_H_
#define BLOCK_H_

#include <string>
#include <vector>

extern "C"
{
#include "falcon\falcon.h"
}

#define LOGN 10

class BlockUtil
{
public:
    static std::string b85encode_vector(std::vector<uint8_t> v);
    static std::vector<uint8_t> b85decode_vector(std::string s);
};

class BufferSize
{
public:
    static int logn;
    static size_t pubkey, privkey, sig, sigpad, sigct, expkey;
    static size_t tmp_keygen, tmp_makepub, tmp_signdyn, tmp_signtree, tmp_verify, tmp_expandpriv;

    static void setLogn(int logn);
};

class Block
{
public:
    std::string message;
    std::string selfHash;
    std::string previousHash;
    uint64_t previousTimestamp;
    uint64_t timestamp;
    std::vector<uint8_t> publicKey;
    std::vector<uint8_t> signature;
    uint32_t proof;
    static shake256_context rng;
    static bool isRngInitiated;

    Block();
    std::string hash();
    int sign(std::vector<uint8_t> secretKey);
    uint32_t mine();

    std::string toString();
};

class GenesisBlock : public Block
{
public:
    GenesisBlock();

    std::string hash();
    uint32_t mine();

    std::string toString();
};

#endif