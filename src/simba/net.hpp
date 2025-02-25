#ifndef NET_HPP
#define NET_HPP

// Relative Includes:
#include "data_field.hpp"

namespace simba {
// Globals:
constexpr auto mac_address_size{6};

// Structs:
//! DIX/Ethernet II frame header.
struct __attribute__((packed)) Ethernet2Header {
  u8 m_dst_addr[mac_address_size];
  u8 m_src_addr[mac_address_size];
  u16 m_ether_type;
};

/*!
 * The actual frame, this is not packed as we do not map a direct pointer to it.
 * As we have a variadic amount of data.
 */
struct Ethernet2Frame {
  const Ethernet2Header* m_header;
  const u8* m_data;
};

/*!
 * This is the header containing the static (non variadic length) fields.
 */
struct __attribute__((packed)) Ipv4StaticHeader {
  u8 m_version_and_ihl;
  u8 m_dscp_and_ecn;
  u16 m_total_length;
  u16 m_identification;
  u16 m_flags_and_fragment_offset;
  u8 m_time_to_live;
  u8 m_protocol;
  u16 m_header_checksum;
  u32 m_src_addr;
  u32 m_dst_addr;
};

/*!
 * Ipv4 header is variadic length.
 * Where m_options is based on the size defined in the IHL field.
 */
struct Ipv4Header {
  const Ipv4StaticHeader* m_header;
  const u32* m_options;
};

//! The market data is always in a UDP header so we need to skip past it.
struct __attribute__((packed)) UdpHeader {
  u16 m_source_port;
  u16 m_dest_port;
  u16 m_length;
  u16 m_checksum;
};

//! The UDP frame with the actual data.
struct UdpFrame {
  const UdpHeader* m_header;
  const u8* m_data;
};

// Functions:
[[nodiscard("Pure method must use result.")]]
auto payload_is_ipv4(const Ethernet2Frame& t_frame) -> bool;
[[nodiscard("Pure method must use result.")]]
auto payload_is_udp(const Ipv4Header& t_frame) -> bool;

auto extract_ethernet2_frame(DataField& t_field) -> Ethernet2Frame;
auto extract_ipv4(DataField& t_field) -> Ipv4Header;
auto extract_udp(DataField& t_field) -> UdpFrame;
} // namespace simba

#endif // NET_HPP
