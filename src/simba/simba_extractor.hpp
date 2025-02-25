#ifndef SIMBA_PROCESS_HPP
#define SIMBA_PROCESS_HPP

// STL Includes:
#include <filesystem>
#include <iosfwd>
#include <optional>

// Absolute Includes:
#include "pcap/pcap_buffer.hpp"

// Relative Includes:
#include "net.hpp"
#include "simba.hpp"

/*!
 *@file
 *
 * In order to get to the SIMBA protocol.
 * We need to step through several layers of encapsulation.
 * Ethernet II frame -> IPv4 -> UDP -> SIMBA.
 * We do not care about the data in Ethernet II, IPv4 and UDP.
 * As we only need to parse the offsets to get to SIMBA.
 * But its still important to do this neatly.
 *
 * Another important thing to note, is that network protocols use Big Endian.
 * I developed this on on a Little Endian system.
 * So when retrieving fields that are larger then u8.
 * You manually have to account for the endianess
 * But since we ignore most of the fields, and care about speed.
 * We only convert the endianness when this is required.
 */

namespace simba {
// Using Statements:
using pcap::PcapBuffer;
using std::filesystem::path;

// Functions:
/*!
 * Extract the SIMBA messages from the @ref PcapBuffer datastructure.
 * And write to a JSON file.
 */
auto simba_extractor(const PcapBuffer& t_buffer, path t_path) -> void;
} // namespace simba

#endif // SIMBA_PROCESS_HPP
