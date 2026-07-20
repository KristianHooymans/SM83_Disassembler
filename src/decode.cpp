#include "decode.h"

#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <array>
#include <iomanip>

std::array<Opcode, 256> DECODE_TABLE;


void disassemble(const std::vector<uint8_t>& rom) {
  int pos = 0x0100;
  while (pos <= rom.size()) {
    DecodedInstruction d = decode(rom, pos);
    std::cout << std::hex << std::setw(4) << std::setfill('0') << pos << ": " << d.code << "\n";
    pos += d.length;
    if(pos + 2 >= rom.size()) break;
  } 
}


DecodedInstruction decode(const std::vector<uint8_t>& rom, int pos) {
  uint8_t opcode = rom[pos];
  Opcode entry = DECODE_TABLE[opcode];
  std::string code = entry.assemblyRepresentation;
  int length = entry.length;

  switch(entry.operandType) {
    case OperandType::NONE:
      break;
    
    case OperandType::IMM8:
      code += std::format("${:02X}", rom[pos+1]);
      break;
    
    case OperandType::IMM16: {
      int value {(rom[pos+2] << 8) + rom[pos+1]};
      code += std::format("${:04X}", value);
      break;
    }
    
    case OperandType::REL8:
      code += std::format("${:02X}", rom[pos+1]);
      break;
  }
  return {code, length};

}


void initDecodeTable() {

 DECODE_TABLE[0x00] = {1, "NOP", OperandType::NONE};
 DECODE_TABLE[0xAF] = {1, "XOR", OperandType::NONE};
 DECODE_TABLE[0xC9] = {1, "RET", OperandType::NONE};
 DECODE_TABLE[0x76] = {1, "HALT", OperandType::NONE};
 DECODE_TABLE[0x3E] = {2, "LD A, ", OperandType::IMM8};
 DECODE_TABLE[0x21] = {3, "LD HL, ", OperandType::IMM16};
 DECODE_TABLE[0xC3] = {3, "JP ", OperandType::IMM16};
 DECODE_TABLE[0xCD] = {3, "CALL ", OperandType::IMM16};
 DECODE_TABLE[0x18] = {2, "JR ", OperandType::REL8};

}
