#include "conservis.hpp"
#include <iostream>
#include <bitset>

#define ASSERT_EQUAL(a, b)                                   \
  {                                                          \
    auto ra = a;                                             \
    auto rb = b;                                             \
    if (ra != rb)                                            \
    {                                                        \
      std::cout << "Failed test expected: " << #a << " == "  \
                << rb << " got: " << ra << " in line "       \
                << __FILE__ << ":" << __LINE__ << std::endl; \
      return 1;                                              \
    }                                                        \
  }

int main(int argc, char const *argv[])
{
  // ABS
  ASSERT_EQUAL(conservis::abs(0), 0)
  ASSERT_EQUAL(conservis::abs(123), conservis::abs(-123))
  ASSERT_EQUAL(conservis::abs(-123), 123)

  ASSERT_EQUAL(conservis::abs((int8_t)123), conservis::abs((int8_t)-123))
  ASSERT_EQUAL((int)conservis::abs((__int128_t)123), (int)conservis::abs((__int128_t)-123))
  ASSERT_EQUAL((int)conservis::abs((__int128_t)-123), 123)
  ASSERT_EQUAL(conservis::abs((int8_t)123), conservis::abs((int8_t)-123))

  // popcnt
  ASSERT_EQUAL(conservis::countBitsSet(0), 0)
  ASSERT_EQUAL(conservis::countBitsSet(1), 1)
  ASSERT_EQUAL(conservis::countBitsSet((char)2), 1)
  ASSERT_EQUAL((int)conservis::countBitsSet((__uint128_t)12), 2)

  // size
  ASSERT_EQUAL(conservis::bitLength<int32_t>(), (size_t)32)
  ASSERT_EQUAL(conservis::bitLength<int8_t>(), (size_t)8)
  ASSERT_EQUAL(conservis::bitLength((__int128_t)0), (size_t)128)

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
  ASSERT_EQUAL(conservis::lsbSetIndex(0), -1);
  ASSERT_EQUAL(conservis::lsbSetIndex((uint16_t)0), -1);
  ASSERT_EQUAL(conservis::lsbSetIndex((int16_t)0), -1);
  ASSERT_EQUAL(conservis::lsbSetIndex((int16_t)2), 1);
  ASSERT_EQUAL(conservis::lsbSetIndex((uint16_t)2), 1);
  ASSERT_EQUAL(conservis::lsbSetIndex((int16_t)4), 2);
  ASSERT_EQUAL(conservis::lsbSetIndex((uint16_t)4), 2);
  ASSERT_EQUAL(conservis::lsbSetIndex((long)8), 3);
  ASSERT_EQUAL(conservis::lsbSetIndex((unsigned long)8), 3);
  ASSERT_EQUAL(conservis::lsbSetIndex(2), 1);
  ASSERT_EQUAL(conservis::lsbSetIndex(1 << 8), 8);
  ASSERT_EQUAL(conservis::lsbSetIndex<unsigned long>(1 << 8), 8);
  ASSERT_EQUAL(conservis::lsbSetIndex<unsigned long>(0), -1);
  ASSERT_EQUAL(conservis::lsbSetIndex<long>(0), (int)-1);

  //msb idx
  ASSERT_EQUAL(conservis::msbSetIndex((int16_t)2), 7);
  ASSERT_EQUAL(conservis::msbSetIndex((uint16_t)2), 14);
  ASSERT_EQUAL(conservis::msbSetIndex((int8_t)5), 2);
  ASSERT_EQUAL(conservis::msbSetIndex((uint8_t)5), 5);
  ASSERT_EQUAL(conservis::msbSetIndex((long)9), 29);
  ASSERT_EQUAL(conservis::msbSetIndex((unsigned long)9), 60);
  ASSERT_EQUAL(conservis::msbSetIndex(0), -1);
  ASSERT_EQUAL(conservis::msbSetIndex((uint16_t)0), -1);
  ASSERT_EQUAL(conservis::msbSetIndex((int16_t)0), -1);
  ASSERT_EQUAL(conservis::msbSetIndex<int16_t>(1 << 8), 0);
  ASSERT_EQUAL(conservis::msbSetIndex<unsigned long>(1 << 8), 55);
  ASSERT_EQUAL(conservis::msbSetIndex<unsigned long>(0), -1);
  ASSERT_EQUAL(conservis::msbSetIndex<long>(0), (int)-1);

  // mix max
  ASSERT_EQUAL(conservis::max((int)1, (ushort)2, (__int128_t)3), 3);
  // mix min
  ASSERT_EQUAL(conservis::min((int)1, (ushort)2, (__int128_t)3), 1);

  return 0;
}
