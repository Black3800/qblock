import time
from base64 import b85encode, b85decode
from falcon import falcon
from Crypto.Hash import SHAKE256
from qblock.block_util import encode_pubkey, decode_pubkey
from qblock.const import QBLOCK_GENESIS_PUBKEY, QBLOCK_GENESIS_SIGNATURE

class Block:

    def __init__(self, msg, prevBlock):
        self.message = msg
        self.previousHash = prevBlock.hash().encode("utf-8")
        self.previousTimestamp = prevBlock.timestamp
        self.publicKey = ""
        self.signature = ""
        self.timestamp = 0.0
        self.proof = -1

    def __repr__(self):
        rep = f"\033[92mMessage\033[0m: {self.message}\n"
        rep += f"\033[92mpreviousHash\033[0m: {self.previousHash}\n"
        if self.publicKey == "":
            rep += f"\033[92mpublicKey\033[0m: n/a\n"
        else:
            rep += f"\033[92mpublicKey\033[0m: {encode_pubkey(self.publicKey.h)}\n"
        if self.signature == "":
            rep += f"\033[92msignature\033[0m: n/a\n"
        else:
            rep += f"\033[92msignature\033[0m: {b85encode(self.signature)}\n"
        return rep

    def sign(self, secretKey):
        self.publicKey = falcon.PublicKey(secretKey)
        self.signature = secretKey.sign(self.previousHash + self.message)

    def verify(self):
        return self.publicKey.verify(self.previousHash + self.message, self.signature)

    def hash(self, nonce=-1):
        if nonce == -1:
            nonce = self.proof
        shake = SHAKE256.new()
        shake.update(self.previousHash + str(self.previousTimestamp).encode("utf-8") + self.message + str(nonce).encode("utf-8"))
        return shake.read(32).hex()

    def mine(self, difficulty=1, verbose=False):
        nonce = -1
        currentHash = "ff"

        while currentHash[:difficulty] != "0"*difficulty:
            if verbose:
                print(f"\033[95m[\033[4m\x1b[1;32;40mMining block...\x1b[0m difficulty={difficulty}\033[95m]\033[0m", end="\t")
                print(nonce, "\t\t", currentHash, end="\r")
            nonce += 1
            currentHash = self.hash(nonce)
        self.proof = nonce
        self.timestamp = time.time()
        return nonce

class GenesisBlock(Block):

    def __init__(self):
        self.message = b'"Not only is the Universe stranger than we think, it is stranger than we can think." - Werner Heisenberg, Across the Frontiers'
        self.previousHash = b""
        self.previousTimestamp = 0
        self.publicKey = QBLOCK_GENESIS_PUBKEY
        self.signature = QBLOCK_GENESIS_SIGNATURE
        self.timestamp = 0.0
        self.proof = -1

    def __repr__(self):
        rep = f"\033[92mMessage\033[0m: {self.message}\n"
        rep += f"\033[92mpreviousHash\033[0m: {self.previousHash}\n"
        rep += f"\033[92mpublicKey\033[0m: {self.publicKey}\n"
        rep += f"\033[92msignature\033[0m: {self.signature}\n"
        return rep

    def verify():
        return True

    def hash(self, _nonce=-1):
        return "1122333344444444555555555555555566666666666666666666666666666666"
