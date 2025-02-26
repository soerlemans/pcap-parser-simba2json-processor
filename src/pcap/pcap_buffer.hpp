#ifndef PCAP_BUFFER_HPP
#define PCAP_BUFFER_HPP

// STL Includes:
#include <cctype>
#include <filesystem>
#include <vector>

// Absolute Includes:
#include "lib/enum2int.hpp"
#include "lib/types.hpp"

namespace pcap {
// Using Statements:
using std::filesystem::path;

// Forward Declarations:
struct PcapPacketRecord;
struct RecordPack;

// Aliases:
//! Contains the full memory of a file.
using ByteBuffer = std::vector<char>;

//! Vector of pointers to the actualy pcap records.
using PacketRegistery = std::vector<RecordPack>;

// Enums:
enum class PcapMagicNumber : u32 {
  PCAP_MS = 0xA1B2C3D4, // PCAP MIME type Packet records, are in Micro Seconds.
  PCAP_NS = 0xA1B23C4D  // PCAP MIME type Packet records, are in Nano Seconds.
};

// Structs:
/*!
 * Store a non owning set of pointers/references to data in the @ref
 * ByteBuffer. We dont use a direct
 */
struct __attribute__((packed)) PcapFileHeader {
  u32 m_magic_number;
  u16 m_major_version;
  u16 m_minor_version;
  u32 reserved1; // Do not access.
  u32 reserved2; // Do not access.
  u32 m_snap_len;
  u32 m_link_type; // Also contains FCS and f.
};

struct __attribute__((packed)) PcapPacketRecord {
  u32 m_timestamp_seconds;
  u32 m_timestamp_variadic;
  u32 m_captured_packet_length; // Actual size of the raw packet data.
  u32 m_original_packet_length;
};

/*!
 * Record pack as in a grouping of record information.
 * @ref PCapPacketRecord m_record stores the pcap header for a record.
 * m_data stores the raw packet data.
 */
struct RecordPack {
  const PcapPacketRecord *m_record;
  const u32 *m_data; // Variadic size.
};

// Classes:
/*!
 * Stores a PCAP file in memory, with quick access to helper methods.
 * We use non owning pointers to reference different areas in the pcap file.
 * This way we do not have to make any copies for the big pcap files.
 * This does make this data structure by default an immutable structure.
 * This does not have to be the case, but it is preferred.
 * As the job is not to modify the .pcap files, but to parse and extract simba.
 * Messages from them, as fast as possible.
 */
class PcapBuffer {
 private:
  ByteBuffer m_buffer;

  const PcapFileHeader *m_file_header;
  PacketRegistery m_registery;

 public:
  PcapBuffer() = delete;
  PcapBuffer(ByteBuffer &&t_buffer);

  auto header() const -> const PcapFileHeader *;

  auto begin() const -> PacketRegistery::const_iterator;
  auto end() const -> PacketRegistery::const_iterator;

  auto cbegin() const -> PacketRegistery::const_iterator;
  auto cend() const -> PacketRegistery::const_iterator;

  //! Print amount of
  auto size() const -> std::size_t;

  // Operators:
  auto operator[](std::size_t t_index) const -> const RecordPack &;

  virtual ~PcapBuffer() = default;
};

// Functions:
[[nodiscard("Pure method must use result.")]]
auto read_pcap_file(path t_path) -> PcapBuffer;
} // namespace pcap

#endif // PCAP_BUFFER_HPP
