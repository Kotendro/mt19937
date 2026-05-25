#include <boost/multiprecision/cpp_int.hpp>
#include <iostream>
#include <vector>
using namespace boost::multiprecision;

class MT
{
private:
    int pos_index_;
    uint32_t mt_[624];
    uint32_t twisted_mt_[624];

    void initialize(int seed)
    {
        cpp_int grow_seed = grow_input_seed(seed);
        sow_seeds(grow_seed);
    }

    cpp_int grow_input_seed(int seed)
    {
        cpp_int grow_seed = seed;
        for (int i=1; i<52; i++)
            grow_seed = 69069 * grow_seed + 1;

        return grow_seed;
    }

    void sow_seeds(cpp_int grow_seed)
    {
        for (pos_index_=0; pos_index_<624; pos_index_++)
        {
            grow_seed = 69069 * grow_seed + 1;
            mt_[pos_index_] = static_cast<uint32_t>(grow_seed) & 0xffffffff;
        }
    }

    void twist()
    {
        for (int i=0; i<624; i++)
        {
            uint32_t x = (mt_[i] & 0x80000000) | (mt_[(i+1) % 624] & 0x7fffffff);
            uint32_t xA = x >> 1;
            if (x & 1)
                xA = xA^0x9908B0DF;

            twisted_mt_[i]=mt_[(i+397) % 624]^xA;
        }

        for (int i = 0; i < 624; i++)
            mt_[i] = twisted_mt_[i];
    }

    uint32_t temper()
    {
        uint32_t y = mt_[pos_index_];
        
        y ^= (y >> 11);
        y ^= (y << 7) & 0x9D2C5680;
        y ^= (y << 15) & 0xEFC60000;
        y ^= (y >> 18);
        
        return y;
    }

public:
    MT (int seed) 
    {
        initialize(seed);
    }

    std::vector<uint32_t> runif (int n)
    {
        std::vector<uint32_t> result(n);

        for (int i=0; i<n; i++)
        {
            if (pos_index_ > 623)
            {
                twist();     
                pos_index_ = 0;
            }

            uint32_t output = temper();
            result[i] = output;
            pos_index_++;
        }

        return result;
    }
};


int main()
{
    MT mt19937(1958);
    auto res = mt19937.runif(3);
    for (int i=0; i<3; i++)
        std::cout << res[i] << ' ';
    std::cout << '\n';
}