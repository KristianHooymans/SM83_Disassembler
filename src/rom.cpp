#include "rom.h"

#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <cstdint>
#include <fstream>


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


