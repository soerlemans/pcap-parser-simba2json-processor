#ifndef SBE_TYPES_HPP
#define SBE_TYPES_HPP

// STL Includes:
#include <iosfwd>
#include <string_view>

// Absolute Includes:
#include "lib/types.hpp"

namespace simba {
// Aliases:
// Temp, defining everything for MDFlagsset will take to long.
// As well as we need to account for endianess for the enum related fields.
using MDFlagsSet = u64;
using MDFlags2Set = u64;
using MDUpdateAction = u8;
using MDEntryType = u8;

// Enums:
/*
//! I am not defining all values due to time constraints:
enum class MDFlagsSet : u64 { UNDEFINED = 0x0 };
enum class MDFlags2Set : u64 { UNDEFINED = 0x0 };

enum class MDUpdateAction : u8 {
NEW = 0x0,
CHANGE = 0x1,
DELETE = 0x3,
};

enum class MDEntryType : u8 { BID = '0', OFFER = '1', EMPTYBOOK = 'J' };
*/

// Structs:
struct __attribute__((packed)) GroupSize {
  u16 m_block_length;
  u8 m_num_in_group;
};

struct __attribute__((packed)) Decimal5 {
  i64 m_mantissa;
  i8 m_exponent;
};

// Functions:
/*
[[nodiscard("Pure method must use results.")]]
auto MDUpdateAction2str(MDUpdateAction t_update_action) -> std::string_view;
[[nodiscard("Pure method must use results.")]]
auto MDEntryType2str(MDEntryType t_entry_type) -> std::string_view;
*/
} // namespace simba

// Functions:
// auto operator<<(std::ostream& t_os, const simba::MDUpdateAction
// t_update_action) -> std::ostream&; auto operator<<(std::ostream& t_os, const
// simba::MDEntryType t_entry_type) -> std::ostream&;
auto operator<<(std::ostream& t_os, const simba::GroupSize t_group_size)
    -> std::ostream&;
auto operator<<(std::ostream& t_os, const simba::Decimal5 t_decimal5)
    -> std::ostream&;

#endif // SBE_TYPES_HPP
