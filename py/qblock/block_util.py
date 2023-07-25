# Note: b85 in base64 library is implemented with IPv6 standard
from base64 import b85encode, b85decode
from math import ceil
from qblock.const import *

def encode_vector(v, bits, offset=0):
    encoded = []
    for x in v:
        s = format(x+offset, 'b').zfill(bits)
        encoded.append(s)
    encoded = ''.join(encoded)
    encoded = int(encoded, 2).to_bytes(len(encoded) // 8, byteorder='big')
    return b85encode(encoded)

def decode_vector(vbytes, bits, offset=0):
    v = []
    vbytes = b85decode(vbytes)
    vbytes = format(int.from_bytes(vbytes, byteorder='big'), 'b')
    chunk_len = ceil(len(vbytes) / bits)
    vbytes = vbytes.zfill(chunk_len * bits)
    for i in range(0, len(vbytes), bits):
        v.append(int(vbytes[i:i+bits], 2) - offset)
    return v

def encode_secretkey(sk):
    skbytes = b""
    skbytes += encode_vector(sk.f, bits=8, offset=127)
    skbytes += b"/"
    skbytes += encode_vector(sk.g, bits=8, offset=127)
    skbytes += b"/"
    skbytes += encode_vector(sk.F, bits=8, offset=127)
    skbytes += b"/"
    skbytes += encode_vector(sk.G, bits=8, offset=127)
    return skbytes

def decode_secretkey(skbytes):
    polys = []
    for vbytes in skbytes.split(b"/"):
        polys.append(decode_vector(vbytes, bits=8, offset=127))
    return polys

def encode_pubkey(pk):
    return encode_vector(pk, bits=14)

def decode_pubkey(pkbytes):
    return decode_vector(pkbytes, bits=14)

def encode_signature(sigbytes):
    return b85encode(sigbytes)

def decode_signature(sigbytes):
    return b85decode(sigbytes)