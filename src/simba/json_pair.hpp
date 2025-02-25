#ifndef JSON_PAIR_HPP
#define JSON_PAIR_HPP

// STL Includes:
#include <concepts>
#include <iostream>
#include <string_view>

// Relative Includes:
#include "bit.hpp"

// Macros:
/*!
 * @warning This only works for string literals.
 */
#define QUOTE_STR(t_str) static_cast<std::string_view>("\"" t_str "\"")

/*!
 * We use this isntead of @ref std::quoted.
 * As this happens at compile time instead of @ref std::quoted().
 */
#define QUOTE(t_var) QUOTE_STR(#t_var)

//! Helper macro for printing a key value pair in JSON.
#define JSON_PAIR(t_var) \
  simba::JsonPair {      \
    QUOTE(t_var), t_var  \
  }

//! Helper macro for printing a key value pair in JSON.
#define JSON_PAIR_PTR(t_var) \
  simba::JsonPair {          \
    QUOTE(t_var), *t_var     \
  }

/*!
 * Helper macro for printing a key value pair in JSON.
 * Here we also convert the big endian to little endian.
 */
#define JSON_PAIR_NET(t_var)               \
  simba::JsonPair {                        \
    QUOTE(t_var), simba::be2native(t_var) \
  }

namespace simba {
// Concepts:
template <typename Type, typename... Args>
concept IsAnyOf = (std::same_as<Args, Type> || ...);

//! Helper struct for creating key value pair's in JSON.
template <typename T>
struct JsonPair {
  const std::string_view m_key;
  const T m_val;
};
} // namespace simba

template <typename T>
inline auto operator<<(std::ostream& t_os, simba::JsonPair<T>&& t_pair)
    -> std::ostream& {
  using simba::IsAnyOf;

  const auto& [key, val] = t_pair;

  t_os << key << ": ";

  // Prevent u8 from being treated as a char.
  if constexpr (IsAnyOf<T, u8, i8>) {
    t_os << static_cast<u16>(val);
  } else {
    t_os << val;
  }

  return t_os;
}

#endif // JSON_PAIR_HPP
