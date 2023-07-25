import time
from base64 import b85encode, b85decode
from falcon import falcon
from Crypto.Hash import SHAKE256
from qblock.block_util import encode_pubkey, decode_pubkey, encode_signature, decode_signature
from qblock.const import QBLOCK_GENESIS_HASH, QBLOCK_GENESIS_MESSAGE, QBLOCK_GENESIS_MESSAGE_HASH, QBLOCK_GENESIS_PREVIOUS_HASH, QBLOCK_GENESIS_PREVIOUS_TIMESTAMP, QBLOCK_GENESIS_PROOF, QBLOCK_GENESIS_PUBLIC_KEY, QBLOCK_GENESIS_SIGNATURE, QBLOCK_GENESIS_TIMESTAMP

class Block:

    def __init__(self, msg, *, prevBlock=None, prevHash=None, prevTimestamp=None, height=None):
        self.message = msg
        shake = SHAKE256.new()
        shake.update(msg)
        self.messageHash = shake.read(32).hex().encode("utf-8")
        self.selfHash = None
        if height != None:
            self.height = height
        else:
            self.height = 1
        if prevBlock != None:
            self.previousHash = prevBlock.hash()
            self.previousTimestamp = prevBlock.timestamp
        if prevHash != None:
            self.previousHash = prevHash
        if prevTimestamp != None:
            self.previousTimestamp = prevTimestamp
        self.publicKey = ""
        self.signature = ""
        self.timestamp = 0.0
        self.proof = -1
        self.hash()

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
        self.signature = encode_signature(secretKey.sign(self.previousHash + self.messageHash))

    def verify(self):
        return self.publicKey.verify(self.previousHash + self.messageHash, decode_signature(self.signature))

    def hash(self, nonce=-1, usePreviousHash=False):
        if not usePreviousHash:
            if nonce == -1:
                nonce = self.proof
            shake = SHAKE256.new()
            shake.update(self.previousHash + str(self.previousTimestamp).encode("utf-8") + self.message + str(nonce).encode("utf-8"))
            self.selfHash = shake.read(32).hex().encode("utf-8")
        return self.selfHash

    def mine(self, difficulty=1, verbose=False):
        nonce = -1
        currentHash = b"ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"

        while currentHash[:difficulty] != b"0"*difficulty:
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
        self.height = 1
        self.message = QBLOCK_GENESIS_MESSAGE
        self.messageHash = QBLOCK_GENESIS_MESSAGE_HASH
        self.previousHash = QBLOCK_GENESIS_PREVIOUS_HASH
        self.previousTimestamp = QBLOCK_GENESIS_PREVIOUS_TIMESTAMP
        self.publicKey = QBLOCK_GENESIS_PUBLIC_KEY
        self.signature = QBLOCK_GENESIS_SIGNATURE
        self.timestamp = QBLOCK_GENESIS_TIMESTAMP
        self.proof = QBLOCK_GENESIS_PROOF

    def verify(self):
        return True

    def hash(self, _nonce=-1, usePreviousHash=True):
        return QBLOCK_GENESIS_HASH
