import base64
import os

from falcon import falcon
from qblock.block import Block, GenesisBlock
from qblock.block_util import encode_secretkey, decode_secretkey, encode_pubkey, decode_pubkey
from qblock.chain import Chain
from qblock.const import *

from flask import Flask

def create_block(msg, prevHash, sk, pk2):
    print(f"\033[95m[\033[4m\x1b[1;32;40mCreating new block...\x1b[0m\033[95m]\033[0m")
    b = Block(msg, prevHash)
    b.sign(sk)
    b.publicKey = pk2
    print("\033[95m[\033[0mDone\033[95m]\033[0m\n")
    # print("\033[95m[Block detail]\033[0m\n\n")
    # print(b1)
    proof = b.mine(QBLOCK_DIFFICULTY, verbose=True)
    print("\n\n\033[93mNonce found!! 🎉\033[0m")
    print(proof, "\t", b.hash(), "\n")
    return b


# print(f"\033[95m[\033[4m\x1b[1;32;40mMining genesis block...\x1b[0m\033[95m]\033[0m")
# chain = Chain()
# print("\033[95m[\033[0mDone\033[95m]\033[0m\n")
# print(f"\033[95m[\033[4m\x1b[1;32;40mGenerating secret key...\x1b[0m\033[95m]\033[0m")
# sk = falcon.SecretKey(QBLOCK_N)
# pk = falcon.PublicKey(sk)
# pkb = encode_pubkey(pk.h)
# pk2h = decode_pubkey(pkb)
# pk2 = falcon.PublicKey(h=pk2h, n=QBLOCK_N)

# print("\033[95m[\033[0mDone\033[95m]\033[0m\n")

# data = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Phasellus eu quam ligula. Aliquam ornare, arcu eget placerat vulputate, eros mauris lobortis quam, at varius mauris eros in urna. Vestibulum tincidunt."
# data = "That's one small step for a man, a giant leap for mankind."
# for word in data.split(" "):
#     chain.add(create_block(word.encode("utf-8"), chain.blocks[-1], sk, pk2))

# print(chain)
    
# skb = encode_secretkey(sk)
# skpolys = decode_secretkey(skb)
# sk2 = falcon.SecretKey(QBLOCK_KEY_SIZE, polys=skpolys)
# pk2 = falcon.PublicKey(sk)
# pk2b = encode_pubkey(pk2.h)

# print(pkb)
# print(pk2b)
# print(pkb == pk2b)

# g = GenesisBlock()
# g.sign(sk)
# print(g)
# print(g.verify())
# print(f"Secret key: {str(skb)}")
# print(f"Public key: {str(pkb)}")

def create_app(test_config=None):
    # create and configure the app
    app = Flask(__name__, instance_relative_config=True)

    if test_config is None:
        # load the instance config, if it exists, when not testing
        app.config.from_pyfile('config.py', silent=True)
    else:
        # load the test config if passed in
        app.config.from_mapping(test_config)

    # ensure the instance folder exists
    try:
        os.makedirs(app.instance_path)
    except OSError:
        pass

    # a simple page that says hello
    @app.route('/hello')
    def hello():
        return 'Hello, World!'

    return app

print("\n\033[95m[Exit]\033[0m\n")

if __name__ == "__main__":
    app = create_app()
    app.run()