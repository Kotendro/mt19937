class MT:
    def __init__(self, seed: int):
        self.pos_index_ = 0
        self.mt_ = [0] * 624
        self.twisted_mt_ = [0] * 624
        self.initialize(seed)

    def initialize(self, seed: int):
        grow_seed = self.grow_input_seed(seed)
        self.sow_seeds(grow_seed)

    def grow_input_seed(self, seed: int) -> int:
        grow_seed = seed

        for _ in range(1, 52):
            grow_seed = 69069 * grow_seed + 1

        return grow_seed

    def sow_seeds(self, grow_seed: int):
        for i in range(624):
            grow_seed = 69069 * grow_seed + 1
            self.mt_[i] = grow_seed & 0xFFFFFFFF

        self.pos_index_ = 624

    def twist(self):
        for i in range(624):
            x = (
                (self.mt_[i] & 0x80000000)
                | (self.mt_[(i + 1) % 624] & 0x7FFFFFFF)
            )

            xA = x >> 1

            if x & 1:
                xA ^= 0x9908B0DF

            self.twisted_mt_[i] = self.mt_[(i + 397) % 624] ^ xA

        for i in range(624):
            self.mt_[i] = self.twisted_mt_[i]

    def temper(self) -> int:
        y = self.mt_[self.pos_index_]

        y ^= (y >> 11)
        y ^= (y << 7) & 0x9D2C5680
        y ^= (y << 15) & 0xEFC60000
        y ^= (y >> 18)

        return y & 0xFFFFFFFF

    def runif(self, n: int):
        result = []

        for _ in range(n):
            if self.pos_index_ > 623:
                self.twist()
                self.pos_index_ = 0

            output = self.temper()
            result.append(output)

            self.pos_index_ += 1

        return result
