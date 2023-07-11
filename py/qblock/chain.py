from qblock.block import Block, GenesisBlock

class Chain:

    def __init__(self):
        self.blocks = []
        genesis = GenesisBlock()
        self.blocks.append(genesis)

    def __repr__(self):
        rep = "\033[95m[Chain detail]\033[0m\n\n"
        for i in range(0, len(self.blocks)):
            rep += f"\033[95m[Block {i}]\033[0m\n"
            rep += f"\033[92mMessage\033[0m: {self.blocks[i].message}\n"
            rep += f"\033[92mHash\033[0m: {self.blocks[i].hash()}\n"
            rep += f"\033[92mPrevious hash\033[0m: {self.blocks[i].previousHash.decode('utf-8')}\n"
            rep += f"\033[92mNonce\033[0m: {self.blocks[i].proof}\n"
            rep += f"\033[92mTimestamp\033[0m: {self.blocks[i].timestamp}\n"
        return rep

    def add(self, newBlock):
        if len(self.blocks) < 1:
            print("blocks empty")
            return False

        if self.blocks[-1].hash().encode("utf-8") != newBlock.previousHash:
            print("hash incorrect")
            return False

        if not newBlock.verify():
            print("signature invalid")
            return False

        self.blocks.append(newBlock)

        return True