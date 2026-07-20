#include "rom.h"

#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <cstdint>
#include <fstream>
#include <unordered_map>

std::unordered_map<int, std::string> cartType {{0x00, "ROM ONLY"}, {0x01, "MBC1"}, {0x02, "MBC1+RAM"}, {0x03, "MBC1+RAM+BATTERY"}, {0x05, "MBC2"}, {0x06, "MBC2+BATTERY"}, {0x08, "ROM+RAM"}, {0x09, "ROM+RAM+BATTERY"}, {0x0B, "MMM01"}, {0x0C, "MMM01+RAM"}, {0x0D, "MMM01+RAM+BATTERY"}, {0x0F, "MBC3+TIMER+BATTERY"}, {0x10, "MBC3+TIMER+RAM+BATTERY"}, {0x11, "MBC3"}, {0x12, "MBC3+RAM"}, {0x13, "MBC3+RAM+BATTERY"}, {0x19, "MBC5"}, {0x1A, "MBC5+RAM"}, {0x1B, "MBC5+RAM+BATTERY"}, {0x1C, "MBC5+RUMBLE"}, {0x1D, "MBC5+RUMBLE+RAM"}, {0x1E, "MBC5+RUMBLE+RAM+BATTERY"}, {0x20, "MBC6"}, {0x22, "MBC7+SENSOR+RUMBLE+RAM+BATTERY"}, {0xFC, "POCKET CAMERA"}, {0xFD, "BANDAI TAMA5"}, {0xFE, "HuC3"}, {0xFF, "HuC1+RAM+BATTERY"}};

std::string romType(const std::vector<uint8_t>& hexDump) {
  int type = hexDump[0x147];
  auto it = cartType.find(type);
  if (it == cartType.end()) {
    std::cerr << "Error: Cart Type not found!" << '\n';
    return "";
  }
  else {
    return it->second;
  }
}

//in KiB
int romSize(const std::vector<uint8_t>& hexDump){
  int size = hexDump[0x148];
  return (1 << size) * 32;
 }


std::string romName(const std::vector<uint8_t>& hexDump) {
  std::string romNameOutput;
  for (auto i {0x134}; i <= 0x143; ++i) {
    if (std::isprint(hexDump[i])){
      romNameOutput += static_cast<char>(hexDump[i]);
    }
  }
  return romNameOutput;
}


std::vector<uint8_t> loadRom(std::string filename) {
  std::ifstream file(filename, std::ios::binary);
  if (file.is_open()) {
    
    std::vector<uint8_t> romOutput; // output vector
    
    char ch; // byte sized char 
    
    while (file.get(ch)) {
      uint8_t byte = static_cast<uint8_t>(ch);
      romOutput.push_back(byte); 
    }
    return romOutput;

  
  }
  else {
    std::cerr << "Failed to open file" << '\n';
    return {};
  }
}


