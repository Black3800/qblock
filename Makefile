CC = g++
CFLAGS = -std=c++20 -lWs2_32 #--verbose
FALCON_DEPS = falcon/falcon.o falcon/codec.o falcon/common.o falcon/fft.o falcon/fpr.o falcon/keygen.o falcon/rng.o falcon/shake.o falcon/sign.o falcon/vrfy.o
# CRYPTOPP_DEPS = cryptopp/*.o 
Z85_DEPS = z85/z85.o z85/z85_impl.o
CRYPTOPP_DEPS = cryptopp/algparam.o cryptopp/allocate.o cryptopp/asn.o cryptopp/basecode.o cryptopp/cpu.o cryptopp/cryptlib.o cryptopp/dll.o cryptopp/ec2n.o cryptopp/ecp.o cryptopp/filters.o cryptopp/fips140.o cryptopp/gf2n.o cryptopp/gfpcrypt.o cryptopp/hex.o cryptopp/hmac.o cryptopp/hrtimer.o cryptopp/integer.o cryptopp/iterhash.o cryptopp/keccak_core.o cryptopp/keccak.o cryptopp/misc.o cryptopp/modes.o cryptopp/mqueue.o cryptopp/nbtheory.o cryptopp/oaep.o cryptopp/osrng.o cryptopp/pubkey.o cryptopp/queue.o cryptopp/randpool.o cryptopp/rijndael.o cryptopp/rdtables.o cryptopp/rng.o cryptopp/sha.o cryptopp/shake.o

chain: chain.cpp
	$(CC) -o qblock qblock.cpp block.cpp chain.cpp sync.cpp $(FALCON_DEPS) $(CRYPTOPP_DEPS) $(Z85_DEPS) $(CFLAGS)