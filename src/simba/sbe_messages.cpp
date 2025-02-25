#include "sbe_messages.hpp"

// Absolute Includes:
#include "lib/view_cast.hpp"

// Relative Includes:
#include "json_pair.hpp"

// Using Statements:
using simba::SbeMessageOrderBookSnapShot;
using simba::SbeMessageOrderExecution;
using simba::SbeMessageOrderUpdate;

namespace simba {
auto sbe_message_data2json(const SbeMessageId t_id, const void* t_ptr,
                           std::ostream& t_os) -> void {
  // If nullptr bail early.
  if (!t_ptr) {
    t_os << QUOTE_STR("null");

    return;
  }

  switch (t_id) {
    case SbeMessageId::ORDER_UPDATE: {
      const auto* message{view_cast<SbeMessageOrderUpdate>(t_ptr)};
      t_os << *message;
      break;
    }

    case SbeMessageId::ORDER_EXECUTION: {
      const auto* message{view_cast<SbeMessageOrderUpdate>(t_ptr)};
      t_os << *message;
      break;
    }

    case SbeMessageId::ORDER_BOOK_SNAPSHOT: {
      const auto* message{view_cast<SbeMessageOrderUpdate>(t_ptr)};
      t_os << *message;
      break;
    }

    default:
      // TODO: Error handle better?
      t_os << QUOTE_STR("unhandled");
      break;
  }
}
} // namespace simba

// Functions:
auto operator<<(std::ostream& t_os, const SbeMessageOrderUpdate& t_message)
    -> std::ostream& {
  // Am i fan of this solution? No, does it work? Yes.
  const auto& [MDEntryID, MDEntryPx, MDEntrySize, MDFlags, MDFlags2,
               MDUpdateAction, MDEntryType] = t_message;

  // clang-format off
  t_os << "{ "
       << JSON_PAIR_NET(MDEntryID) << ", "
       << JSON_PAIR(MDEntryPx) << ", "
       << JSON_PAIR_NET(MDEntrySize) << ", "
       << JSON_PAIR_NET(MDFlags) << ", "
       << JSON_PAIR_NET(MDFlags2) << ", "
       << JSON_PAIR(MDUpdateAction) << ", "
       << JSON_PAIR(MDEntryType)
       << " }";
  // clang-format on

  return t_os;
}

auto operator<<(std::ostream& t_os, const SbeMessageOrderExecution& t_message)
    -> std::ostream& {
  // Am i fan of this solution? No, does it work? Yes.
  const auto& [MDEntryID, MDEntryPx, MDEntrySize, LastPx, LastQty, TradeID,
               MDFlags, MDFlags2, SecurityID, RptSeq, MDUpdateAction,
               MDEntryType] = t_message;

  // clang-format off
  t_os << "{ "
       << JSON_PAIR_NET(MDEntryID) << ", "
       << JSON_PAIR(MDEntryPx) << ", "
       << JSON_PAIR_NET(MDEntrySize) << ", "
       << JSON_PAIR(LastPx) << ", "
       << JSON_PAIR_NET(LastQty) << ", "
       << JSON_PAIR_NET(TradeID) << ", "
       << JSON_PAIR_NET(MDFlags) << ", "
       << JSON_PAIR_NET(MDFlags2) << ", "
       << JSON_PAIR_NET(SecurityID) << ", "
       << JSON_PAIR_NET(RptSeq) << ", "
       << JSON_PAIR(MDUpdateAction) << ", "
       << JSON_PAIR(MDEntryType)
       << " }";
  // clang-format on

  return t_os;
}

auto operator<<(std::ostream& t_os,
                const SbeMessageOrderBookSnapShot& t_message) -> std::ostream& {
  // Am i fan of this solution? No, does it work? Yes.
  const auto& [SecurityID, LastMsgSeqNumProcessed, RptSeq,
               ExchangeTradingSessionID, NoMDEntries, MDEntryID, TransactTime,
               MDEntryPx, MDEntrySize, TradeID, MDFlags, MDFlags2,
               MDEntryType] = t_message;

  // clang-format off
  t_os << "{ "
       << JSON_PAIR_NET(SecurityID) << ", "
       << JSON_PAIR_NET(LastMsgSeqNumProcessed) << ", "
       << JSON_PAIR_NET(RptSeq) << ", "
       << JSON_PAIR_NET(ExchangeTradingSessionID) << ", "
       << JSON_PAIR(NoMDEntries) << ", "
       << JSON_PAIR_NET(MDEntryID) << ", "
       << JSON_PAIR_NET(TransactTime) << ", "
       << JSON_PAIR(MDEntryPx) << ", "
       << JSON_PAIR_NET(MDEntrySize) << ", "
       << JSON_PAIR_NET(TradeID) << ", "
       << JSON_PAIR_NET(MDFlags) << ", "
       << JSON_PAIR_NET(MDFlags2) << ", "
       << JSON_PAIR(MDEntryType)
       << " }";
  // clang-format on

  return t_os;
}
