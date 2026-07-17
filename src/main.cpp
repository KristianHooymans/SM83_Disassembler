#include "rom.h"

#include <iostream>
#include <vector>
#include <string>
#include <cstdint>


int main(int argc, char* argv[]) 
{
  if (argc == 2) {
    std::vector<uint8_t> rom = loadRom(argv[1]);
    std::cout << rom.size();
  }
  else std::cout << "Input should be in the form: ./code /path/to/ROM/file\n";
}
