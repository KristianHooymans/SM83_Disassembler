#include "rom.h"

#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <cstdint>
#include <fstream>


void loadRom(std::string filename) {
  std::ifstream file(filename, std::ios::binary);
  if (file.is_open()) {
    std::cout << filename << '\n';
  }
  else {
    std::cout << "Filename not found or unable to open file\n";
  }
}


