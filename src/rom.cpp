#include "rom.h"

#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <cstdint>
#include <fstream>


std::vector<uint8_t> loadRom(std::string filename) {
  std::ifstream file(filename, std::ios::binary);
  std::streampos fileSize = 0;
  if (file.is_open()) {
    file.seekg(0, std::ios::end);//get the size of the file
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg); //return pointer to the start of the file

    std::vector<uint8_t> romOutput(fileSize); // 
    
    for (auto i {0uz}; i < fileSize; ++i) {
      
    }

  }
  else {
    std::cerr << "Failed to open file" << '\n';
  }
}


