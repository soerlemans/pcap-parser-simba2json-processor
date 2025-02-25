#ifndef SBE_HPP
#define SBE_HPP

// STl Includes:
#include <iosfwd>
#include <vector>

// Relative Includes:
#include "data_field.hpp"
#include "sbe_messages.hpp"

namespace simba {
// Forward Declarations:
struct SbeMessage;
struct SimbaPacket;

// Aliases:
using SbeMessages = std::vector<SbeMessage>;

struct __attribute__((packed)) SbeHeader {
  u16 m_block_length;
  u16 m_template_id; // This is the msg id.
  u16 m_schema_id;
  u16 m_version;
};

struct SbeMessage {
  const SbeHeader* m_header;

  // Normally I would use std::variant.
  // But this is a performance critical parser, so casting from void* is faster.
  const void* m_message_data;
};

// Functions
auto extract_sbe_messages_incremental(DataField& t_field) -> SbeMessages;
auto extract_sbe_messages_snapshot(DataField& t_field) -> SbeMessages;
auto extract_sbe_messages(SimbaPacket& t_packet, DataField& t_field) -> void;
} // namespace simba

// Functions:
// For now default is just printing JSON.
auto operator<<(std::ostream& t_os, const simba::SbeHeader& t_header)
    -> std::ostream&;
auto operator<<(std::ostream& t_os, const simba::SbeMessage& t_message)
    -> std::ostream&;
auto operator<<(std::ostream& t_os, const simba::SbeMessages& t_messages)
    -> std::ostream&;

#endif // SBE_HPP
