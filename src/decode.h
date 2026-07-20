#ifndef DECODEH
#define DECODEH

#include <string>
#include <array>


enum class OperandType {NONE, IMM8, IMM16, REL8};

struct Opcode {
  int length = 1;
  std::string assemblyRepresentation = "ERROR";
  OperandType operandType;
};

extern std::array<Opcode, 256> DECODE_TABLE;

void initDecodeTable();

#endif
