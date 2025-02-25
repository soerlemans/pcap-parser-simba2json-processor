#include "sbe_types.hpp"

// Relative Includes:
#include "bit.hpp"
#include "json_pair.hpp"

// Using Statements:
using simba::Decimal5;
using simba::GroupSize;

/*
namespace simba {
auto MDUpdateAction2str(const MDUpdateAction t_update_action) ->
std::string_view {
}

auto MDEntryType2str(const MDEntryType t_entry_type) -> std::string_view {
}
} // namespace simba
*/

// Functions:
auto operator<<(std::ostream& t_os, const simba::GroupSize t_group_size)
    -> std::ostream& {
  const auto& [block_length, num_in_group] = t_group_size;

  // Little endian was not specified.
  // clang-format off
  t_os << "{ "
       << JSON_PAIR_NET(block_length) << ", "
       << JSON_PAIR_NET(num_in_group)
       << " }";
  // clang-format on

  return t_os;
}
auto operator<<(std::ostream& t_os, const simba::Decimal5 t_decimal5)
    -> std::ostream& {
  const auto& [mantissa, exponent] = t_decimal5;

  // Little endian was not specified.
  // clang-format off
  t_os << "{ "
       << JSON_PAIR_NET(mantissa) << ", "
       << JSON_PAIR_NET(exponent)
       << " }";
  // clang-format on

  return t_os;
}
