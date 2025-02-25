#ifndef DATA_FIELD_HPP
#define DATA_FIELD_HPP

// Absolute Includes:
#include "lib/types.hpp"

namespace simba {
// Classes:
/*!
 * Class for partritioning/walking through a memory field.
 */
class DataField {
 private:
  const u8* m_data;
  std::size_t m_length;

 public:
  explicit DataField(const u8* t_data, std::size_t t_length);

  auto data() const -> const u8*;
  auto length() const -> std::size_t;
  auto empty() const -> bool;

  //! Cast to a const view of the preferred type.
  template <typename T>
  auto to() const -> const T* {
    return reinterpret_cast<const T*>(m_data);
  }

  //! Advance the data pointer, and make the size smaller.
  auto consume(std::size_t t_size) -> void;

  /*!
   * Cast to a const view of the preferred type.
   * And then also move it by that many bytes.
   */
  template <typename T>
  auto consume() -> const T* {
    const auto* ptr{to<T>()};

    consume({sizeof(T)});

    return ptr;
  }

  virtual ~DataField() = default;
};
} // namespace simba

#endif // DATA_FIELD_HPP
