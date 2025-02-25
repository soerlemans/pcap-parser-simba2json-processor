#include "net.hpp"

/*!
 *@file
 *
 * I decided that it would be good design to have the extract_* functions.
 * Actually return the headers/frames instead of just computing the offset.
 * Where the Simba packet starts.
 * As it might be desirable to actually be able to introspect.
 * Certain packet information before you go about converting the SIMBA to JSON.
 */

// Relative Includes:
#include "bit.hpp"

namespace simba {
// Functions:
auto payload_is_ipv4(const Ethernet2Frame& t_frame) -> bool {
  bool result{false};

  // Normally I would make this an enumeration but I have time constraints.
  constexpr u16 ipv4_ether_type{0x0800};

  // Must convert from big endian.
  const u16 ether_type{be2native(t_frame.m_header->m_ether_type)};
  result = (ipv4_ether_type == ether_type);

  return result;
}

auto payload_is_udp(const Ipv4Header& t_frame) -> bool {
  bool result{false};

  // Normally I would make this an enumeration but I have time constraints.
  constexpr u8 udp_protocol{0x11};

  // Dont need to convert from big endian as it is only a single byte.
  const u8 protocol{t_frame.m_header->m_protocol};
  result = (udp_protocol == protocol);

  return result;
}

auto extract_ethernet2_frame(DataField& t_field) -> Ethernet2Frame {
  Ethernet2Frame frame{};

  // Compute pointer offsets:
  frame.m_header = t_field.consume<Ethernet2Header>();
  frame.m_data = t_field.data();

  return frame;
}

auto extract_ipv4(DataField& t_field) -> Ipv4Header {
  Ipv4Header header{};

  // Compute pointer offsets:
  header.m_header = t_field.to<Ipv4StaticHeader>();
  header.m_options = t_field.to<u32>() + sizeof(Ipv4StaticHeader);

  // The IPv4 header size is based on the IHL field * sizeof(u32).
  const auto ihl_field{header.m_header->m_version_and_ihl};
  const auto header_size{(ihl_field & 0x0F) * sizeof(u32)};

  // Update t_field.
  t_field.consume(header_size);

  return header;
}

auto extract_udp(DataField& t_field) -> UdpFrame {
  // Compute pointer offsets.
  UdpFrame frame{};
  frame.m_header = t_field.consume<UdpHeader>();
  frame.m_data = t_field.data();

  return frame;
}
} // namespace simba
