import base64
import datetime
import os
import pymongo

from flask import Flask, request
from pymongo.errors import ConnectionFailure

def create_app(test_config=None):
    # create and configure the app
    app = Flask(__name__, instance_relative_config=True)
    print(f"\033[95m[\033[4m\x1b[1;32;40mStarting QBlock client...\x1b[0m\033[95m]\033[0m")
    mongoclient = pymongo.MongoClient("mongodb://mongodb:27017/?directConnection=true")
    try:
        mongoclient.admin.command('ping')
    except ConnectionFailure:
        print('no mongo')
        return 'no mongo'
        
    cold = mongoclient["qblock"]["cold"]

    if test_config is None:
        # load the instance config, if it exists, when not testing
        app.config.from_pyfile('config.py', silent=True)
    else:

        app.config.from_mapping(test_config)

    try:
        os.makedirs(app.instance_path)
    except OSError:
        pass

    @app.get('/block')
    def get_block_all():
        print(cold)
        return "all blocks"

    @app.post('/block/<string:block_hash>')
    def post_block(block_hash=None):
        data = request.json
        message = data.get('message')
        signature = data.get('signature')
        publicKey = data.get('publicKey')
        submitted_time = datetime.datetime.now(tz=datetime.timezone.utc)
        block = {
            "message": message,
            "signature": signature,
            "publicKey": publicKey,
            "submitted_time": submitted_time
        }
        # latest = qblock_chain.latest_block()
        # qblock_chain.add(create_block(message, latest.hash().encode('utf-8'), latest.timestamp, signature, publicKey))
        bid = cold.insert_one(block).inserted_id
        print(bid)
        print(cold)
        return 'success'

    return app

print("\n\033[95m[Exit]\033[0m\n")

if __name__ == "__main__":
    app = create_app()
    app.run(host='0.0.0.0', port=23714, debug=True)