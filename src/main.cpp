#include "rom.h"

#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <iomanip>


int main(int argc, char* argv[]) 
{
  if (argc == 2) {
    std::vector<uint8_t> rom = loadRom(argv[1]);
    int offset = 0; // for the loop, basically what line we are on right now in the ROM
    for (auto i {0uz}; i < 20; ++i) {
      std::cout << std::hex << std::setfill('0') << std::setw(7) << i << '0' << ": ";
     for (auto i {0uz}; j < 16; ++j) {
        std::cout << std::hex << std::setfill('0') << std::setw(2) <<  static_cast<int>(rom[j+offset]) << " ";
      }
     std::cout << '\n';
     offset += 16;
    }
    std::cout << std::dec;
  }
  else std::cout << "Input should be in the form: ./code /path/to/ROM/file\n";
}
