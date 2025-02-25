#ifndef ENUM2INT_HPP
#define ENUM2INT_HPP

// STL Includes:
#include <type_traits>

// Functions
//! Refer to integers in general not just @ref int.
template <typename T>
inline constexpr auto enum2int(const T t_value) -> std::underlying_type_t<T> {
  return static_cast<std::underlying_type_t<T>>(t_value);
}

#endif // ENUM2INT_HPP
