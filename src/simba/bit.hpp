#ifndef BIT_HPP
#define BIT_HPP

// STL Includes:
#include <bit>

namespace simba {
// Functions:
/*!
 * Network protocols are usually big endian, convert to native endian if needed.
 * @note: Function name is short for `Big Endian to Native`.
 */
template <typename T>
inline constexpr auto be2native(const T t_int) -> T {
  using std::endian;

  T result{t_int};

  if constexpr (endian::native == endian::little) {
    result = std::byteswap(t_int);
  } else if constexpr (endian::native != endian::big) {
    // TODO: Error handle, unhandeld endianess.
  }

  return result;
}
} // namespace simba

#endif // BIT_HPP
