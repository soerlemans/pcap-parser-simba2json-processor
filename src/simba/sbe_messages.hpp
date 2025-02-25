#ifndef SBE_MESSAGES_HPP
#define SBE_MESSAGES_HPP

// STL Includes:
#include <iosfwd>

// Relative Includes:
#include "sbe_types.hpp"

namespace simba {
// Enums:
/*!
 * For now just stick to these message ID's.
 * As the task
 */
enum class SbeMessageId {
  ORDER_UPDATE = 15,
  ORDER_EXECUTION = 16,
  ORDER_BOOK_SNAPSHOT = 17
};

// Structs:
// We dont need to handle nullable SIMBA SBE types.
// As we are just going to convert it to JSON anyway.
// And this takes just to much time to do now.
// Also for now follow the same capitalization as the documentation.
struct __attribute__((packed)) SbeMessageOrderUpdate {
  i64 m_MDEntryID;
  Decimal5 m_MDEntryPx;
  i64 m_MDEntrySize;
  MDFlagsSet m_MDFlags;
  MDFlags2Set m_MDFlags2;
  MDUpdateAction m_MDUpdateAction;
  MDEntryType m_MDEntryType;
};

struct __attribute__((packed)) SbeMessageOrderExecution {
  i64 m_MDEntryID;
  Decimal5 m_MDEntryPx;
  i64 m_EntrySize;
  Decimal5 m_LastPx;
  i64 m_LastQty;
  i64 m_TradeID;
  MDFlagsSet m_MDFlags;
  MDFlags2Set m_MDFlags2;
  i32 m_SecurityID;
  u32 m_RptSeq;
  MDUpdateAction m_MDUpdateAction;
  MDEntryType m_MDEntryType;
};

struct __attribute__((packed)) SbeMessageOrderBookSnapShot {
  i32 m_SecurityID;
  u32 m_LastMsgSeqNumProcessed;
  u32 m_RptSeq;
  u32 m_ExchangeTradingSessionID;
  GroupSize m_NoMDEntries;
  i64 m_MDEntryID;
  u64 m_TransactTime;
  Decimal5 m_MDEntryPx;
  i64 m_MDEntrySize;
  i64 m_TradeID;
  MDFlagsSet m_MDFlags;
  MDFlags2Set m_MDFlags2;
  MDEntryType m_MDEntryType;
};

// Functions:
/*!
 * This function is necessary as we need to cast the `void*`.
 * To the message type according to the @ref m_template_id.
 * There is definetly a more refined solution for this.
 * But I am under time pressure, and want to use `void*`.
 */
auto sbe_message_data2json(SbeMessageId t_id, const void* t_ptr,
                           std::ostream& t_os) -> void;
} // namespace simba

// Functions:
auto operator<<(std::ostream& t_os,
                const simba::SbeMessageOrderUpdate& t_message) -> std::ostream&;
auto operator<<(std::ostream& t_os,
                const simba::SbeMessageOrderExecution& t_message)
    -> std::ostream&;
auto operator<<(std::ostream& t_os,
                const simba::SbeMessageOrderBookSnapShot& t_message)
    -> std::ostream&;

#endif // SBE_MESSAGES_HPP
