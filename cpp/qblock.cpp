#include <iomanip>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

extern "C"
{
#include "falcon\falcon.h"
}

#include "block.hpp"
#include "chain.hpp"
#include "const.hpp"
#include "sync.hpp"

int main(void) {

    shake256_context rng;
    // if (shake256_init_prng_from_system(&rng) != 0)
    // {
    //     throw std::runtime_error("\033[91mRuntime error:\033[0m PRNG Initialization failed");
    // }
    uint8_t rng_seed[] = {0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7};
    shake256_init_prng_from_seed(&rng, rng_seed, 8);

    size_t pubkey_len = FALCON_PUBKEY_SIZE(QBLOCK_LOGN);
    size_t privkey_len = FALCON_PRIVKEY_SIZE(QBLOCK_LOGN);
    size_t tmpkg_len = FALCON_TMPSIZE_KEYGEN(QBLOCK_LOGN);
    std::vector<uint8_t> pubkey(pubkey_len);
    std::vector<uint8_t> privkey(privkey_len);
    std::vector<uint8_t> tmpkg(tmpkg_len);

    int r = falcon_keygen_make(&rng,
                               QBLOCK_LOGN,
                               &privkey.front(),
                               privkey_len,
                               &pubkey.front(),
                               pubkey_len,
                               &tmpkg.front(),
                               tmpkg_len);

    std::cout << BlockUtil::b85encode_vector(pubkey) << std::endl << std::endl;

    rng_seed[7] = 0x8;
    shake256_init_prng_from_seed(&rng, rng_seed, 8);

    r = falcon_keygen_make(&rng,
                           QBLOCK_LOGN,
                           &privkey.front(),
                           privkey_len,
                           &pubkey.front(),
                           pubkey_len,
                           &tmpkg.front(),
                           tmpkg_len);

    std::cout << BlockUtil::b85encode_vector(pubkey) << std::endl << std::endl;

    SyncHandler* sh = new SyncHandler();

    // Chain c = Chain();
    // Block* genesis = c.blocks.back();

    // std::vector<std::string> msg = {"That's", "one", "small", "step", "for", "a", "man,", "a", "giant", "leap", "for", "mankind."};

    // for (auto m : msg)
    // {
    //     Block* latest = c.blocks.back();
    //     Block* b = new Block();
    //     b->message = m;
    //     b->previousHash = latest->hash();
    //     b->previousTimestamp = latest->timestamp;
    //     b->publicKey = pubkey;
    //     b->sign(privkey);
    //     b->mine();
    //     c.add(b);
    // }


    // std::cout << c.toString() << std::endl;
    return 0;
}