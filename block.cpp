#include "block.hpp"

#include <stdexcept>
#include <iostream>
#include <format>

#include "cryptopp\cryptlib.h"
#include "cryptopp\filters.h"
#include "cryptopp\hex.h"
#include "cryptopp\shake.h"

#include "z85\z85.hpp"

#define DIFFICULTY 4

int BufferSize::logn = 0;
size_t BufferSize::pubkey, BufferSize::privkey, BufferSize::sig, BufferSize::sigpad, BufferSize::sigct, BufferSize::expkey;
size_t BufferSize::tmp_keygen, BufferSize::tmp_makepub, BufferSize::tmp_signdyn, BufferSize::tmp_signtree, BufferSize::tmp_verify, BufferSize::tmp_expandpriv;

shake256_context Block::rng;
bool Block::isRngInitiated = false;

std::string BlockUtil::b85encode_vector(std::vector<uint8_t> v)
{
    // while (v.size() % 4 != 0)
    // {
    //     v.push_back(0);
    // }
    return z85::encode_with_padding((char *)&v.front(), v.size());
}

std::vector<uint8_t> BlockUtil::b85decode_vector(std::string s)
{
    std::string decoded = z85::decode_with_padding(s.data(), s.length());
    const std::vector<uint8_t> v(decoded.begin(), decoded.end());
    return v;
}

void BufferSize::setLogn(int logn)
{
    pubkey = FALCON_PUBKEY_SIZE(LOGN);
    privkey = FALCON_PRIVKEY_SIZE(LOGN);
    sig = FALCON_SIG_COMPRESSED_MAXSIZE(LOGN);
    sigpad = FALCON_SIG_PADDED_SIZE(LOGN);
    sigct = FALCON_SIG_CT_SIZE(LOGN);
    expkey = FALCON_EXPANDEDKEY_SIZE(LOGN);

    tmp_keygen = FALCON_TMPSIZE_KEYGEN(LOGN);
    tmp_makepub = FALCON_TMPSIZE_MAKEPUB(LOGN);
    tmp_signdyn = FALCON_TMPSIZE_SIGNDYN(LOGN);
    tmp_signtree = FALCON_TMPSIZE_SIGNTREE(LOGN);
    tmp_verify = FALCON_TMPSIZE_VERIFY(LOGN);
    tmp_expandpriv = FALCON_TMPSIZE_EXPANDPRIV(LOGN);
}

Block::Block()
{
    proof = 0;
    if (!isRngInitiated)
    {
        if (shake256_init_prng_from_system(&rng) != 0)
        {
            throw std::runtime_error("\033[91mRuntime error:\033[0m PRNG Initialization failed");
        }
        isRngInitiated = true;
    }
    if (BufferSize::logn == 0)
    {
        BufferSize::setLogn(LOGN);
    }
}

std::string Block::hash()
{
    std::string data = "";
    data += previousHash;
    data += std::to_string(previousTimestamp);
    data += message;
    data += std::to_string(proof);

    std::string digest;

    CryptoPP::SHAKE256 hash;
    hash.Update((CryptoPP::byte*) data.data(), data.size());
    digest.resize(hash.DigestSize());
    hash.Final((CryptoPP::byte*) &digest[0]);

    std::string encoded = "";
    CryptoPP::StringSource ss((CryptoPP::byte*) &digest[0], sizeof(digest), true,
                              new CryptoPP::HexEncoder(
                                  new CryptoPP::StringSink(encoded),
                                  false, // uppercase
                                  64,    // grouping
                                  ""     // separator
                                  )      // HexDecoder
    );
    return encoded;
}

int Block::sign(std::vector<uint8_t> privateKey)
{
    size_t sig_len = BufferSize::sig;
    size_t privkey_len = BufferSize::privkey;
    size_t tmp_len = BufferSize::tmp_signdyn;

    signature.resize(sig_len);

    std::string data = previousHash + message;

    std::vector<uint8_t> tmp(tmp_len);

    int r = falcon_sign_dyn(&rng,
                            &signature.front(),
                            &sig_len,
                            FALCON_SIG_COMPRESSED,
                            &privateKey.front(),
                            privkey_len,
                            data.data(),
                            data.length(),
                            &tmp.front(),
                            tmp_len);
    
    return r;
}

uint32_t Block::mine()
{
    std::string prefix = "xxx";
    std::string target = std::string(DIFFICULTY, '0');
    while (prefix != target)
    {
        proof++;
        prefix = hash().substr(0, DIFFICULTY);
        // std::cout << "\r\033[95m[\033[4m\x1b[1;32;40mMining block...\x1b[0m difficulty=" << 5 << "\033[95m]\033[0m\t";
        // std::cout << proof << "\t\t" << hash();
    }
    // std::cout << proof << "\t" << hash();
    return proof;
}

std::string Block::toString()
{
    std::string stringRepresentation = "";
    stringRepresentation += std::format("\033[92mMessage\033[0m:\t{}\n", message);
    stringRepresentation += std::format("\033[92mhash\033[0m:\t\t{}\n", hash());
    stringRepresentation += std::format("\033[92mpreviousHash\033[0m:\t{}\n", previousHash);
    stringRepresentation += std::format("\033[92mnonce\033[0m:\t\t{}\n", proof);
    if (publicKey.empty())
    {
        stringRepresentation += "\033[92mpublicKey\033[0m:\tN/A\n";
    }
    else
    {
        stringRepresentation += std::format("\033[92mpublicKey\033[0m:\t{}\n", BlockUtil::b85encode_vector(publicKey));
    }

    // if (publicKey.size() == 0)
    // {
    //     stringRepresentation += "\033[92mpublicKey\033[0m: n/a\n";
    // }
    // else
    // {
    //     // stringRepresentation += std::format("\033[92mpublicKey\033[0m: {}\n", std::string(publicKey.begin(), publicKey.end()));
    //     stringRepresentation += "\033[92mpublicKey\033[0m: ";
    //     stringRepresentation += publicKey;
    // }

    // if (signature.size() == 0)
    // {
    //     stringRepresentation += "\033[92msignature\033[0m: n/a\n";
    // }
    // else
    // {
    //     // stringRepresentation += std::format("\033[92msignature\033[0m: {}\n", std::string(signature.begin(), signature.end()));
    //     stringRepresentation += "\033[92msignature\033[0m: ";
    //     stringRepresentation += signature;
    // }
    return stringRepresentation;
}

GenesisBlock::GenesisBlock() : Block::Block()
{
    message = "Genesis block";
    previousHash = "";
    previousTimestamp = 0;
    timestamp = 0;
    proof = 0;
}

std::string GenesisBlock::hash()
{
    return "1122333344444444555555555555555566666666666666666666666666666666";
}

uint32_t GenesisBlock::mine()
{
    return 0;
}

std::string GenesisBlock::toString()
{
    std::string stringRepresentation = "";
    stringRepresentation += std::format("\033[92mMessage\033[0m:\t{}\n", message);
    stringRepresentation += std::format("\033[92mhash\033[0m:\t\t{}\n", hash());
    stringRepresentation += "\033[92mpreviousHash\033[0m:\tN/A\n";
    stringRepresentation += std::format("\033[92mnonce\033[0m:\t\t{}\n", proof);
    return stringRepresentation;
}