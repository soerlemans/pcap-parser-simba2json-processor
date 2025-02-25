#ifndef SIMBA_HPP
#define SIMBA_HPP

// Relative Includes:
#include "sbe.hpp"

namespace simba {
// Enums:
/*!
 * I always try to use `enum class` as it enforces stricer rules.
 * As well as preventing name collisions due to the scoping.
 */
enum class SimbaMarketDataFlags : u8 {
  LAST_FRAGMENT = 0x01,
  START_OF_SNAPSHOT = 0x02,
  END_OF_SNAPSHOT = 0x04,
  INCREMENTAL_PACKET = 0x08,
  POSS_DUP_FLAG = 0x10
};

// Structs:
//! Byte order of encoding is little endian for this struct.
struct __attribute__((packed)) SimbaMarketDataPacketHeader {
  u32 m_msg_seq_num;
  u16 m_msg_size;
  u16 m_msg_flags;
  u64 m_sending_time;
};

//! Byte order of encoding is little endian for this struct.
struct __attribute__((packed)) SimbaIncrementalPacketHeader {
  u64 m_transact_time;
  u32 m_exchange_trading_session_id;
};

//! * Simba packet.
struct SimbaPacket {
  const SimbaMarketDataPacketHeader* m_header;
  const SimbaIncrementalPacketHeader* m_incremental_header; // Can be nullptr.

  // Single message or multiple, depends on if it is incremental.
  SbeMessages m_messages;
};

// Functions:
auto extract_simba(DataField& t_field) -> SimbaPacket;
} // namespace simba

// Functions:
// For now default is just printing JSON.
auto operator<<(std::ostream& t_os,
                const simba::SimbaMarketDataPacketHeader& t_header)
    -> std::ostream&;
auto operator<<(std::ostream& t_os,
                const simba::SimbaIncrementalPacketHeader& t_header)
    -> std::ostream&;
auto operator<<(std::ostream& t_os, const simba::SimbaPacket& t_packet)
    -> std::ostream&;

#endif // SIMBA_HPP
