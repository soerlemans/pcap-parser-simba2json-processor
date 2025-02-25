#include "simba.hpp"

// Absolute Includes:
#include "lib/enum2int.hpp"

// Relative Includes:
#include "json_pair.hpp"

// Using Statements:
using simba::SimbaIncrementalPacketHeader;
using simba::SimbaMarketDataPacketHeader;
using simba::SimbaPacket;

namespace simba {
auto extract_simba(DataField& t_field) -> SimbaPacket {
  // Compute pointer offsets.
  SimbaPacket packet{};
  packet.m_header = t_field.consume<SimbaMarketDataPacketHeader>();
  packet.m_incremental_header = nullptr;

  // Determine if there is an incremental header.
  const auto flags{packet.m_header->m_msg_flags};
  constexpr auto mask{enum2int(SimbaMarketDataFlags::INCREMENTAL_PACKET)};
  if (flags & mask) {
    packet.m_incremental_header =
        t_field.consume<SimbaIncrementalPacketHeader>();
  }

  // Extract the Simba SBE messages.
  extract_sbe_messages(packet, t_field);

  return packet;
}
} // namespace simba

// Functions:
auto operator<<(std::ostream& t_os, const SimbaMarketDataPacketHeader& t_header)
    -> std::ostream& {
  const auto& [msg_seq_num, msg_size, msg_flags, sending_time] = t_header;

  // clang-format off
  t_os << "{ "
       << JSON_PAIR(msg_seq_num) << ", "
       << JSON_PAIR(msg_size) << ", "
       << JSON_PAIR(msg_flags) << ", "
       << JSON_PAIR(sending_time)
       << " }";
  // clang-format on

  return t_os;
}

auto operator<<(std::ostream& t_os,
                const SimbaIncrementalPacketHeader& t_header) -> std::ostream& {
  const auto& [transact_time, exchange_trading_session_id] = t_header;

  // clang-format off
  t_os << "{ "
       << JSON_PAIR(transact_time) << ", "
       << JSON_PAIR(exchange_trading_session_id)
       << " }";
  // clang-format on

  return t_os;
}

auto operator<<(std::ostream& t_os, const SimbaPacket& t_packet)
    -> std::ostream& {
  const auto& [header, incremental_header, messages] = t_packet;

  t_os << "{ \"header\": ";

  // If the header pointer is null something wen seriously wrong.
  if (header) {
    t_os << *header << ", ";

    if (incremental_header) {
      t_os << JSON_PAIR_PTR(incremental_header) << ", ";
    }

    t_os << JSON_PAIR(messages);
  } else {
    // This should never happen or something must have gone wrong.
    t_os << "null";
  }

  t_os << " }";

  return t_os;
}
