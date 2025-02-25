#include "pcap_buffer.hpp"

// STL Includes:
#include <exception>
#include <fstream>
#include <memory>
#include <vector>

// Absolute Includes:
#include "lib/view_cast.hpp"

// Internal:
namespace {
// Using Statements:
using pcap::ByteBuffer;
using pcap::PacketRegistery;
using pcap::PcapPacketRecord;
using std::filesystem::path;

// Variables:
constinit auto pcap_file_header_size{sizeof(pcap::PcapFileHeader)};
constinit auto pcap_record_size{sizeof(pcap::PcapPacketRecord)};

//! Copy whole file once to prevent slow down by repeated reading/copying.
inline auto init_file_buffer(const path &t_path) -> std::vector<char> {
  // Read complete file into memory (faster than repeated queries).
  std::ifstream ifs{t_path, std::ios::binary | std::ios::ate};
  if (ifs.fail()) {
    throw std::runtime_error{"Could not open strema to file."};
  }

  // Get the size of the entire file.
  const auto file_size{static_cast<std::size_t>(ifs.tellg())};
  ifs.seekg(0, std::ios::beg);

  // Using a char is required.
  std::vector<char> file_buffer{};
  file_buffer.resize(file_size);

  if (!ifs.read(file_buffer.data(), file_size)) {
    throw std::runtime_error{"Failed to write everything to memory buffer."};
  }

  return file_buffer;
}

inline auto init_registery(const ByteBuffer &t_buffer,
                           PacketRegistery &t_registery) -> void {
  std::size_t index{pcap_file_header_size};
  while (index < t_buffer.size()) {
    // Ref is optimized away by compiler.
    const auto &ref{t_buffer[index]};
    const auto *record{view_cast<PcapPacketRecord>(&ref)};
    const auto *data{view_cast<u32>(&ref + pcap_record_size)};

    // Increase index to next record.
    index += pcap_record_size + record->m_captured_packet_length;

    // Emplace entry.
    t_registery.emplace_back(record, data);
  }
}
} // namespace

namespace pcap {
// Public Methods:
PcapBuffer::PcapBuffer(ByteBuffer &&t_buffer)
    : m_buffer{std::move(t_buffer)},
      m_file_header{view_cast<PcapFileHeader>(m_buffer.data())},
      m_registery{} {
  // Verify pcap header is compatible/parseable.
  verify();

  // Rough optimization reserve the minimum required space on the heap.
  const auto reserve_size{m_buffer.size() / pcap_record_size};

  // -O2 enabled division will likely become >> 3.
  // We guestimate a good size to reserve.
  m_registery.reserve(reserve_size / 8);

  // Finish initialization.
  init_registery(m_buffer, m_registery);
}

auto PcapBuffer::verify() -> void {
  const auto magic_number{
      static_cast<PcapMagicNumber>(m_file_header->m_magic_number)};

  switch (magic_number) {
    case PcapMagicNumber::PCAP_MS:
      // TODO: Set flag.
      break;

    case PcapMagicNumber::PCAP_NS:
      // TODO: Set flag.
      break;

    default:
      // Prevent wasting time on parsing something that is not a pcap file.
      // Throw an exception even though they are expensive.
      // This is a critical failure so it is warranted.
      throw std::invalid_argument{"Unsupported MIME type for PcapBuffer."};
      break;
  }
}

auto PcapBuffer::header() const -> const PcapFileHeader * {
  return m_file_header;
}

auto PcapBuffer::begin() const -> PacketRegistery::const_iterator {
  return m_registery.cbegin();
}

auto PcapBuffer::end() const -> PacketRegistery::const_iterator {
  return m_registery.cend();
}

auto PcapBuffer::cbegin() const -> PacketRegistery::const_iterator {
  return m_registery.cbegin();
}

auto PcapBuffer::cend() const -> PacketRegistery::const_iterator {
  return m_registery.cend();
}

auto PcapBuffer::size() const -> std::size_t {
  return m_registery.size();
}

auto PcapBuffer::operator[](const std::size_t t_index) const
    -> const RecordPack & {
  return m_registery[t_index];
}

// Functions:
auto read_pcap_file(const path t_path) -> PcapBuffer {
  // RVO is applied here so we do not copy the std::vector.
  PcapBuffer pcap_buffer{init_file_buffer(t_path)};

  pcap_buffer.verify();

  return pcap_buffer;
}

} // namespace pcap
