#ifndef _CONVERSIV_HPP
#define _CONVERSIV_HPP 1

#include <string>

#define _conservis_builtin_int(T, C) sizeof(T) C sizeof(unsigned int)
#define _conservis_builtin_long(T, C) sizeof(T) C sizeof(unsigned long)
#define _conservis_builtin_long_long(T, C) sizeof(T) C sizeof(unsigned long long)
#define _conservis_is_signed(T) (((T)-1) < 0)

// _conservis_has_hasbuiltin allows some preprocessor safety as __has_builtin is not support by widely used GCC versions
#ifndef _conservis_disable_builtins
#if __clang__
#define _conservis_builtin(f) __has_builtin(f)
#elif __GNUC__ > 9
#define _conservis_builtin(f) __has_builtin(f)
#else
#define _conservis_builtin(f) true
#pragma message "__has_builtin check disabled, your compiler doesn't support it, use clang or gcc10+ to ensure functionallity or define _conservis_disable_builtins"
#endif
#else
#define _conservis_builtin(f) false
#pragma message "builtin function are disabled, this might reduce performance"
#endif

#define _conservis_builtin_possibility_cmp(T, B, V, C) \
  if constexpr (_conservis_builtin_int(T, C))          \
    return B(V);                                       \
                                                       \
  if constexpr (_conservis_builtin_long(T, C))         \
    return B##l(V);                                    \
                                                       \
  if constexpr (_conservis_builtin_long_long(T, C))    \
    return B##ll(V);

#define _conservis_builtin_possibility(T, B, V) _conservis_builtin_possibility_cmp(T, B, V, <=)

namespace conservis
{
  template <typename Number>
  static inline bool isBitSet(const Number number, const Number bit)
  {
    return (number & (((Number)1) << bit));
  }

  template <typename T>
  static constexpr inline auto bitLength()
  {
    return sizeof(T) * 8;
  }

  template <typename T>
  static constexpr inline auto bitLength(const T n)
  {
    return bitLength<T>();
  }

  template <typename Number>
  static Number countBitsSet(Number n)
  {
#if _conservis_builtin(__builtin_popcount)
    _conservis_builtin_possibility(Number, __builtin_popcount, n);

#ifdef _GLIBCXX_USE_INT128
#if __SIZEOF_LONG__ == 8
    if constexpr (sizeof(Number) == sizeof(__int128_t))
      return __builtin_popcountl(n >> 64) + __builtin_popcountl(n);
#elif __SIZEOF_LONG_LONG__ == 8
    if constexpr (sizeof(Number) == sizeof(__int128_t))
      return __builtin_popcountll(n >> 64) + __builtin_popcountll(n);
#endif
#endif
#endif

    if constexpr (_conservis_is_signed(Number))
      n = abs(n);

    Number cnt = 0;
    while (n)
    {
      cnt += n & 1;
      n >>= 1;
    }
    return cnt;
  }

  template <typename Number>
  static Number getMsbSetIndex(Number n)
  {
#if _conservis_builtin(__builtin_clz)
    _conservis_builtin_possibility_cmp(Number, __builtin_clz, n, ==);
#ifdef _GLIBCXX_USE_INT128
#if __SIZEOF_LONG__ == 8
    if constexpr (sizeof(Number) == sizeof(__int128_t))
    {
      Number first = __builtin_clzl(n);
      if (first)
        return first;
      Number second = __builtin_clzl(n >> 64);
      if (second)
        return second + 64;
      return 0;
    }
#elif __SIZEOF_LONG_LONG__ == 8
    if constexpr (sizeof(Number) == sizeof(__int128_t))
    {
      Number first = __builtin_clzll(n);
      if (first)
        return first;
      Number second = __builtin_clzll(n >> 64);
      if (second)
        return second + 64;
      return 0;
    }
#endif
#endif
#endif
    if (n == 0)
      return -1;

    if constexpr (_conservis_is_signed(Number))
      n = abs(n);
    Number cnt = 0;
    Number max = abs(((Number)1) << (bitLength<Number>() - 1));
    while (!(n & (max >> cnt)))
      cnt++;

    return cnt;
  }

  template <typename Number>
  static Number getLsbSetIndex(Number n)
  {
#if _conservis_builtin(__builtin_ctz)
    _conservis_builtin_possibility(Number, __builtin_ctz, n);
#ifdef _GLIBCXX_USE_INT128
#if __SIZEOF_LONG__ == 8
    if constexpr (sizeof(Number) == sizeof(__int128_t))
    {
      Number first = __builtin_ctzl(n);
      if (first)
        return first;
      Number second = __builtin_ctzl(n >> 64);
      if (second)
        return second + 64;
      return 0;
    }
#elif __SIZEOF_LONG_LONG__ == 8
    if constexpr (sizeof(Number) == sizeof(__int128_t))
    {
      Number first = __builtin_ctzll(n);
      if (first)
        return first;
      Number second = __builtin_ctzll(n >> 64);
      if (second)
        return second + 64;
      return 0;
    }
#endif
#endif
#endif
    if (n != 0)
      return countBitsSet((n & -n) - 1);

    return -1;
  }

  template <typename Number>
  static Number abs(const Number n)
  {
    if constexpr (!_conservis_is_signed(Number))
      return n;

#if _conservis_builtin(__builtin_abs)
    if constexpr (_conservis_builtin_int(Number, <=))
      return __builtin_abs(n);
#endif

    return n < 0 ? -n : n;
  }

  template <typename ReturnNumType>
  static inline auto max(ReturnNumType i) { return i; }

  /**
   * @brief Mixed type max function 
   */
  template <typename ReturnNumType, typename... Number>
  static inline auto max(ReturnNumType a, const Number... bs)
  {
    ReturnNumType b = max(bs...);
    return a < b ? b : a;
  }

  template <typename ReturnNumType>
  static inline auto min(ReturnNumType i) { return i; }

  /**
   * @brief Mixed type min function 
   */
  template <typename ReturnNumType, typename... Number>
  static inline auto min(ReturnNumType a, const Number... bs)
  {
    ReturnNumType b = min(bs...);
    return b < a ? b : a;
  }

  template <typename Number>
  static std::string printBinaryLsbR(Number n)
  {
    std::string out;
    if (n < 0)
      out += "-";

    n = abs(n);

    Number nBits = bitLength<Number>();
    for (Number i = nBits - 1; i + 1; i--)
    {
      out += '0' + isBitSet(n, i);
    }
    return out;
  }

  template <typename Number>
  static std::string printBinaryLsbR(const Number *n)
  {
    return printBinaryLsbR(*n);
  }

  template <typename Number>
  static std::string printBinaryLsbL(Number n)
  {
    std::string out;
    if (n < 0)
      out += "-";

    n = abs(n);

    Number nBits = bitLength<Number>();
    for (Number i = 0; i < nBits; i++)
    {
      out += '0' + isBitSet(n, i);
    }
    return out;
  }

  template <typename Number>
  static std::string printBinaryLsbL(const Number *n)
  {
    return printBinaryLsbL(*n);
  }

} //namespace conservis
#undef _conservis_builtin
#undef _conservis_builtin_int
#undef _conservis_builtin_long
#undef _conservis_builtin_long_long
#undef _conservis_builtin_possibility
#undef _conservis_is_signed
#endif //_CONVERSIV_HPP