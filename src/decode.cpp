#include "opcodes.h"

#include <iostream>
#include <cmath>
#include <vector>


struct assemblyCode {
  std::string opcode;
  std::string operand;
};


std::array<assemblyCode> DECODE_TABLE(256);

