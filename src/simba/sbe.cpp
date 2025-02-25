#include "sbe.hpp"

// Relative Includes:
#include "json_pair.hpp"
#include "simba.hpp"
#include "sbe_messages.hpp"

// Macros:
/*!
 * Bail when a message is incomplete.
 * Macros should be used sparingly.
 * Furthermore macros should have a `do { macro body... } while (false)`.
 * This makes the macro expand as a single statement.
 */
#define BAIL_IF_INCOMPLETE(t_size) \
  if (t_size > t_field.length()) { \
    break;                         \
  }

// Using Statements:
using simba::SbeHeader;
using simba::SbeMessage;
using simba::SbeMessageId;
using simba::SbeMessages;

namespace simba {
auto extract_sbe_messages_incremental(DataField& t_field) -> SbeMessages {
  SbeMessages messages{};

  // Reserve a rough estimate for how many messages we will receive.
  const auto reserve_size{t_field.length() / (sizeof(SbeHeader) * 2)};
  messages.reserve(reserve_size);

  while (!t_field.empty()) {
    SbeMessage message{};

    // Incomplete message bail.
    BAIL_IF_INCOMPLETE(sizeof(SbeHeader));

    message.m_header = t_field.consume<SbeHeader>();
    message.m_message_data = t_field.to<void>();

    // Consume the block length and continue to the next.
    const auto block_length{message.m_header->m_block_length};

    // Incomplete message bail.
    BAIL_IF_INCOMPLETE(block_length);

    // Consume the message if everything went well.
    t_field.consume(block_length);

    messages.emplace_back(std::move(message));
  }

  return messages;
}

// TODO: Parse repeating section header.
auto extract_sbe_messages_snapshot(DataField& t_field) -> SbeMessages {
  SbeMessages messages{};

  // Reserve a rough estimate for how many messages we will receive.
  const auto reserve_size{t_field.length() / (sizeof(SbeHeader) * 2)};
  messages.reserve(reserve_size);

  while (!t_field.empty()) {
    SbeMessage message{};

    // Incomplete message bail.
    BAIL_IF_INCOMPLETE(sizeof(SbeHeader));

    message.m_header = t_field.consume<SbeHeader>();
    message.m_message_data = t_field.to<void>();

    // Consume the block length and continue to the next.
    const auto block_length{message.m_header->m_block_length};

    // Incomplete message bail.
    BAIL_IF_INCOMPLETE(block_length);

    // Consume the message if everything went well.
    t_field.consume(block_length);

    messages.emplace_back(std::move(message));
  }

  return messages;
}

auto extract_sbe_messages(SimbaPacket& t_packet, DataField& t_field) -> void {
  // Parse
  if (t_packet.m_incremental_header) {
    t_packet.m_messages = extract_sbe_messages_incremental(t_field);
  } else {
    // t_packet.messages = extract_sbe_messages_snapshot(t_field);
  }
}
} // namespace simba

// Functions:
auto operator<<(std::ostream& t_os, const SbeHeader& t_header)
    -> std::ostream& {
  const auto& [block_length, template_id, schema_id, version] = t_header;

  // clang-format off
  t_os << "{ "
       << JSON_PAIR_NET(block_length) << ", "
       << JSON_PAIR_NET(template_id) << ", "
       << JSON_PAIR_NET(schema_id) << ", "
       << JSON_PAIR_NET(version)
       << " }";
  // clang-format on

  return t_os;
}

auto operator<<(std::ostream& t_os, const SbeMessage& t_message)
    -> std::ostream& {
  using simba::sbe_message_data2json;

  const auto& [sbe_header, message_data] = t_message;

  t_os << "{ " << JSON_PAIR_PTR(sbe_header) << ", \"message\": ";

  if (message_data) {
    const auto msg_id{static_cast<SbeMessageId>(sbe_header->m_template_id)};

    sbe_message_data2json(msg_id, message_data, t_os);
  } else {
    t_os << QUOTE_STR("incomplete");
  }

  t_os << " }";

  return t_os;
}

auto operator<<(std::ostream& t_os, const SbeMessages& t_messages)
    -> std::ostream& {
  t_os << "[";

  std::string_view sep{""};
  for (const auto& message : t_messages) {
    t_os << sep << message;
    sep = ", ";
  }

  t_os << "]";

  return t_os;
}
