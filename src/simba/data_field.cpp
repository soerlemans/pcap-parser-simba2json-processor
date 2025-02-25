#include "data_field.hpp"

// STL Includes:
#include <exception>
#include <sstream>

namespace simba {
DataField::DataField(const u8* t_data, const std::size_t t_length)
    : m_data{t_data}, m_length{t_length} {
}

auto DataField::data() const -> const u8* {
  return m_data;
}

auto DataField::length() const -> std::size_t {
  return m_length;
}

auto DataField::empty() const -> bool {
  return m_length == 0;
}

auto DataField::consume(const std::size_t t_size) -> void {
  // Consume the give amount of bytes.
  if (m_length >= t_size) {
    m_data += t_size;
    m_length -= t_size;
  } else {
    std::stringstream ss{};

    ss << "Tried to consume more bytes than there is space left.\n"
       << "(" << m_length << " - " << t_size << ") < 0.";

    throw std::runtime_error{ss.str()};
  }
}
} // namespace simba
