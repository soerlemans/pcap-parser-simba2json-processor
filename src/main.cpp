// STL Includes:
#include <array>
#include <bit>
#include <iostream>
#include <string_view>

// Relative Includes:
#include "pcap/pcap_buffer.hpp"
#include "simba/simba_extractor.hpp"

// Using Statements:
using namespace std::literals::string_view_literals;

// Globals:
constinit std::array pcap_test_files = {"../test/2023-10-09.1849-1906.pcap"sv,
                                        "../test/2023-10-09.2349-2355.pcap"sv,
                                        "../test/2023-10-10.0439-0450.pcap"sv,
                                        "../test/2023-10-10.0845-0905.pcap"sv,
                                        "../test/2023-10-10.0959-1005.pcap"sv,
                                        "../test/2023-10-10.1359-1406.pcap"sv,
                                        "../test/2023-10-10.1849-1906.pcap"sv};

auto pcap2json(const std::string_view pcap_path) -> void {
  using pcap::read_pcap_file;
  using simba::simba_extractor;

  const auto pcap_buffer{read_pcap_file(pcap_path)};

  std::cout << "file: " << pcap_path
            << ", packets captured: " << pcap_buffer.size() << '\n';

  simba_extractor(pcap_buffer, pcap_path);
}

// Loop through test values and convert them.
auto test_default() -> void {
  for (const auto &pcap_path : pcap_test_files) {
    pcap2json(pcap_path);
  }
}

auto main(const int argc, char *argv[]) -> int {
  using std::endian;

  // Edge case but it is good to account for this.
  constexpr bool is_little_endian{endian::native == endian::little};
  static_assert(
      is_little_endian,
      "The parser currently only supports systems using little endian.");

  // Quick and dirty way for the reviewer to test functionality.
  try {
    if (argc > 1) {
      std::cout << "Pcap files to convert: " << (argc - 1) << '\n';

      // We need to skip the first entry as this is the program itself.
      for (auto index{1}; index < argc; index++) {
        pcap2json(argv[index]);
      }
    } else {
      std::cout << "No arguments given running defaults.\n";

      test_default();
    }
  } catch (const std::exception &e) {
    std::cout << "Error: " << e.what() << '\n';

    return 1;
  }

  return 0;
}
