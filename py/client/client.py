import base64
import time
import os
import pymongo

from Crypto.Hash import SHAKE256
from falcon import falcon
from qblock.block_util import encode_pubkey, encode_secretkey, encode_signature, decode_secretkey
from qblock.const import QBLOCK_N
from flask import Flask, request
from flask_cors import CORS, cross_origin
from pymongo.errors import ConnectionFailure

def create_app(test_config=None):
    # create and configure the app
    app = Flask(__name__, instance_relative_config=True)
    cors = CORS(app)
    app.config['CORS_HEADERS'] = 'Content-Type'
    print(f"\033[95m[\033[4m\x1b[1;32;40mStarting QBlock client...\x1b[0m\033[95m]\033[0m")
    mongo_client = pymongo.MongoClient('mongodb://mongodb:27017/?directConnection=true')
    mongo_down = True
    while mongo_down:
        try:
            mongo_client.admin.command('ping')
            mongo_down = False
        except ConnectionFailure:
            continue
    
    qblock = mongo_client.qblock
    cold = qblock.cold
    hot = qblock.hot
    warm = qblock.warm
    meta = qblock.meta

    if test_config is None:
        # load the instance config, if it exists, when not testing
        app.config.from_pyfile('config.py', silent=True)
    else:

        app.config.from_mapping(test_config)

    try:
        os.makedirs(app.instance_path)
    except OSError:
        pass

    @app.get('/key')
    @cross_origin()
    def get_key():
        sk = falcon.SecretKey(QBLOCK_N)
        pk = falcon.PublicKey(sk)
        return {
            'secret_key': encode_secretkey(sk).decode('utf-8'),
            'public_key': encode_pubkey(pk.h).decode('utf-8')
        }

    @app.post('/sign')
    @cross_origin()
    def post_sign():
        data = request.json
        message = data.get('message').encode('utf-8')
        previous_hash = meta.find().sort("height", pymongo.DESCENDING).limit(1)[0]['latest_hash']
        secret_key = data.get('secret_key').encode('utf-8')
        shake = SHAKE256.new()
        shake.update(message)
        message_hash = shake.read(32).hex().enocde('utf-8')
        sk = falcon.SecretKey(QBLOCK_N, polys=decode_secretkey(secret_key))
        signature = encode_signature(sk.sign(previous_hash + message_hash))
        return {
            "signature": signature
        }

    @app.get('/hot')
    @cross_origin()
    def get_hot():
        blocks = hot.find()
        res = []
        for b in blocks:
            res.append({
                "_id": b["_id"].decode("utf-8"),
                "height": b["height"],
                "message": b["message"].decode("utf-8"),
                "message_hash": b["message_hash"].decode("utf-8"),
                "previous_hash": b["previous_hash"].decode("utf-8"),
                "previous_timestamp": b["previous_timestamp"],
                "proof": b["proof"],
                "signature": b["signature"].decode("utf-8"),
                "public_key": b["public_key"].decode("utf-8"),
                "timestamp": b["timestamp"]
            })
        print(res)
        return {
            "blocks": res
        }

    @app.post('/cold')
    @cross_origin()
    def post_cold():
        data = request.json
        message = data.get('message').encode('utf-8')
        signature = data.get('signature').encode('utf-8')
        publicKey = data.get('publicKey').encode('utf-8')
        submitted_time = time.time()
        meta_block = meta.find().sort("height", pymongo.DESCENDING).limit(1)[0]
        block = {
            "message": message,
            "signature": signature,
            "public_key": publicKey,
            "submitted_time": submitted_time
        }
        meta.update_one(
            {
                "_id": meta_block["_id"]
            },
            {
                "$inc": {
                    "cold_size": 1
                }
            }
        )
        cold.insert_one(block)
        return 'success'

    @app.post('/cold_with_sk')
    @cross_origin()
    def post_cold_with_sk():
        data = request.json
        message = data.get('message').encode('utf-8')
        previous_hash = meta.find().sort("height", pymongo.DESCENDING).limit(1)[0]['latest_hash']
        secret_key = data.get('secret_key').encode('utf-8')
        shake = SHAKE256.new()
        shake.update(message)
        message_hash = shake.read(32).hex().encode('utf-8')
        sk = falcon.SecretKey(QBLOCK_N, polys=decode_secretkey(secret_key))
        pk = falcon.PublicKey(sk)
        pk = encode_pubkey(pk.h)
        signature = encode_signature(sk.sign(previous_hash + message_hash))
        submitted_time = time.time()
        meta_block = meta.find().sort("height", pymongo.DESCENDING).limit(1)[0]
        block = {
            "message": message,
            "signature": signature,
            "public_key": pk,
            "submitted_time": submitted_time
        }
        meta.update_one(
            {
                "_id": meta_block["_id"]
            },
            {
                "$inc": {
                    "cold_size": 1
                }
            }
        )
        cold.insert_one(block)
        return 'success'

    @app.post('/hot')
    @cross_origin()
    def post_hot():
        data = request.json
        _id = data.get('_id').encode('utf-8')
        height = data.get('height')
        message = data.get('message').encode('utf-8')
        message_hash = data.get('message_hash').encode('utf-8')
        previous_hash = data.get('previous_hash').encode('utf-8')
        previous_timestamp = data.get('previous_timestamp')
        proof = data.get('proof')
        signature = data.get('signature').encode('utf-8')
        public_key = data.get('public_key').encode('utf-8')
        timestamp = data.get('timestamp')
        block_dict = {
            "_id": _id,
            "height": height,
            "message": message,
            "message_hash": message_hash,
            "previous_hash": previous_hash,
            "previous_timestamp": previous_timestamp,
            "proof": proof,
            "signature": signature,
            "public_key": public_key,
            "timestamp": timestamp
        }
        print(block_dict)
        warm.insert_one(block_dict)
        return 'waiting for verification'

    return app

print("\n\033[95m[Exit]\033[0m\n")

if __name__ == "__main__":
    app = create_app()
    app.run(host='0.0.0.0', port=23614, debug=True)