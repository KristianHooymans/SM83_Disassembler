#include "rom.h"

#include <iostream>
#include <vector>
#include <string>
#include <cstdint>


int main(int argc, char* argv[]) 
{
  if (argc == 2) {
    loadRom(argv[1]);
  }
  else std::cout << "Input should be in the form: ./code /path/to/ROM/file\n";
}
