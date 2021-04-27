#include "conservis.hpp"
#include <iostream>
#include <bitset>

#define ASSERT_EQUAL(a, b)                                                               \
    {                                                                                    \
        auto ra = a;                                                                     \
        auto rb = b;                                                                     \
        if (ra != rb)                                                                    \
        {                                                                                \
            std::cout << "Failed test expected: " << rb << " got: " << ra << " in line " \
                      << __FILE__ << ":" << __LINE__ << std::endl;                       \
            return 1;                                                                    \
        }                                                                                \
    }

int main(int argc, char const *argv[])
{
    const int a = 12;
    const std::string b = "asd";
    // ABS
    ASSERT_EQUAL(conservis::abs(123), conservis::abs(-123))
    ASSERT_EQUAL(conservis::abs(-123), 123)

    ASSERT_EQUAL(conservis::abs((int8_t)123), conservis::abs((int8_t)-123))
    ASSERT_EQUAL((int)conservis::abs((__int128_t)123), (int)conservis::abs((__int128_t)-123))
    ASSERT_EQUAL((int)conservis::abs((__int128_t)-123), 123)
    ASSERT_EQUAL(conservis::abs((int8_t)123), conservis::abs((int8_t)-123))

    // popcnt
    ASSERT_EQUAL(conservis::countBitsSet(1), 1)
    ASSERT_EQUAL(conservis::countBitsSet((char)2), 1)
    ASSERT_EQUAL((int)conservis::countBitsSet((__uint128_t)12), 2)

    // size
    ASSERT_EQUAL(conservis::bitLength<int32_t>(), 32)
    ASSERT_EQUAL(conservis::bitLength<int8_t>(), 8)
    ASSERT_EQUAL((int)conservis::bitLength((__int128_t)0), 128)

    // printbinary
    std::string l_one_8 = "10000000";
    std::string l_one_32 = "10000000000000000000000000000000";
    std::string r_one_8 = "00000001";
    std::string r_one_32 = "00000000000000000000000000000001";
    ASSERT_EQUAL(conservis::printBinaryLsbL((int8_t)1), l_one_8);
    ASSERT_EQUAL(conservis::printBinaryLsbL((int32_t)1), l_one_32);
    ASSERT_EQUAL(conservis::printBinaryLsbL((int8_t)-1), "-" + l_one_8);
    ASSERT_EQUAL(conservis::printBinaryLsbL((int32_t)-1), "-" + l_one_32);
    ASSERT_EQUAL(conservis::printBinaryLsbR((int8_t)1), r_one_8);
    ASSERT_EQUAL(conservis::printBinaryLsbR((int32_t)1), r_one_32);
    ASSERT_EQUAL(conservis::printBinaryLsbR((int8_t)-1), "-" + r_one_8);
    ASSERT_EQUAL(conservis::printBinaryLsbR((int32_t)-1), "-" + r_one_32);

    //lsb idx
    ASSERT_EQUAL(conservis::getLsbSetIndex(2), 1);
    ASSERT_EQUAL(conservis::getLsbSetIndex(1 << 8), 8);

    //msb idx
    ASSERT_EQUAL(conservis::getMsbSetIndex((int16_t)2), 14);
    ASSERT_EQUAL(conservis::getMsbSetIndex<int16_t>(1 << 8), 7);

    // mix max
    ASSERT_EQUAL(conservis::max((int)1, (ushort)2, (__int128_t)3), 3);
    // mix min
    ASSERT_EQUAL(conservis::min((int)1, (ushort)2, (__int128_t)3), 1);

    return 0;
}
