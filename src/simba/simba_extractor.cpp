#include "simba_extractor.hpp"

// STL Includes:
#include <fstream>
#include <iostream>

// Absolute Includes:
#include "lib/types.hpp"
#include "lib/view_cast.hpp"

namespace simba {
auto simba_extractor(const PcapBuffer& t_buffer, path t_path) -> void {
  // Change extension to json format.
  t_path.replace_extension(".json");

  // Upsize the stream buffer, to minimize disk IO/overhead.
  const size_t buffer_size = 256 * 1024;
  char buffer[buffer_size];

  std::ofstream ofs{t_path};
  ofs.rdbuf()->pubsetbuf(buffer, buffer_size);

  for (const auto& pack : t_buffer) {
    const auto* pcap_data{view_cast<u8>(pack.m_data)};
    const auto pcap_length{pack.m_record->m_captured_packet_length};

    DataField field{pcap_data, pcap_length};

    // These calls modify the field variable.
    const auto eth2_frame{extract_ethernet2_frame(field)};
    if (!payload_is_ipv4(eth2_frame)) {
      [[unlikely]];
      // If we are not dealing with an ethernet2 frame it is note SIMBA.
      // This prevents misrepresentation of the data.
      continue;
    }

    const auto ipv4_header{extract_ipv4(field)};
    if (!payload_is_udp(ipv4_header)) {
      [[unlikely]];
      // If we are not dealing with UDP it is not SIMBA.
      // This prevents misrepresentation of the data.
      continue;
    }

    extract_udp(field);

    // Actually get the underlying simba packet from pcap recording.
    const auto simba_packet{extract_simba(field)};

    ofs << simba_packet << '\n';
  }
}
} // namespace simba
