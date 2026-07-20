#ifndef DECODEH
#define DECODEH

#include <string>
#include <array>
#include <cstdint>


enum class OperandType {NONE, IMM8, IMM16, REL8};

struct Opcode {
  int length = 1;
  std::string assemblyRepresentation = "ERROR";
  OperandType operandType;
};

extern std::array<Opcode, 256> DECODE_TABLE;

void initDecodeTable();

struct DecodedInstruction {
  std::string code;
  int length;
};

DecodedInstruction decode(const std::vector<uint8_t>& rom, int pos);

#endif
