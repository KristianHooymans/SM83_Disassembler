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

  if (!entry.valid) {
    code += std::format(" ${:02X}", rom[pos+1]);
  }

  else {
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
      {
        int8_t offset = rom[pos+1];

        code += std::format("${:04X}", pos + 2 + offset); // jump is 2's compliment (signed distance 8 bytes) 
        break;
      }
    }
  }
  return {code, length};

}


void initDecodeTable() {

 DECODE_TABLE[0x00] = {1, "NOP", OperandType::NONE, true};
 DECODE_TABLE[0xAF] = {1, "XOR", OperandType::NONE, true};
 DECODE_TABLE[0xC9] = {1, "RET", OperandType::NONE, true};
 DECODE_TABLE[0x76] = {1, "HALT", OperandType::NONE, true};
 DECODE_TABLE[0x3E] = {2, "LD A, ", OperandType::IMM8, true};
 DECODE_TABLE[0x21] = {3, "LD HL, ", OperandType::IMM16, true};
 DECODE_TABLE[0xC3] = {3, "JP ", OperandType::IMM16, true};
 DECODE_TABLE[0xCD] = {3, "CALL ", OperandType::IMM16, true};
 DECODE_TABLE[0x18] = {2, "JR ", OperandType::REL8, true};
 DECODE_TABLE[0x10] = {2, "STOP", OperandType::IMM8, true};
 DECODE_TABLE[0x20] = {2, "JR NZ, ", OperandType::REL8, true};
 DECODE_TABLE[0x30] = {2, "JR NC, ", OperandType::REL8, true};
 DECODE_TABLE[0x40] = {1, "LD B, B", OperandType::NONE, true};
 DECODE_TABLE[0x50] = {1, "LD D, B", OperandType::NONE, true};
 DECODE_TABLE[0x60] = {1, "LD H, B", OperandType::NONE, true};
 DECODE_TABLE[0x70] = {1, "LD HL, B", OperandType::NONE, true};
 DECODE_TABLE[0x80] = {1, "ADD A, B", OperandType::NONE, true};
 DECODE_TABLE[0x90] = {1, "SUB A, B", OperandType::NONE, true};
 DECODE_TABLE[0xA0] = {1, "AND A, B", OperandType::NONE, true};
 DECODE_TABLE[0xB0] = {1, "OR A, B", OperandType::NONE, true};
 DECODE_TABLE[0xC0] = {1, "RET NZ", OperandType::NONE, true};
 DECODE_TABLE[0xD0] = {1, "RET NC", OperandType::NONE, true};
 DECODE_TABLE[0xE0] = {2, "LDH (a8), A", OperandType::IMM8, true}; //TODO: LDH formatting
 DECODE_TABLE[0xF0] = {2, "LDH A, ", OperandType::IMM8, true};
 DECODE_TABLE[0x01] = {3, "LD BC, ", OperandType::IMM16, true};
 DECODE_TABLE[0x11] = {3, "LD DE, ", OperandType::IMM16, true};
 DECODE_TABLE[0x21] = {3, "LD HL, ", OperandType::IMM16, true};
 DECODE_TABLE[0x31] = {3, "LD SP, ", OperandType::IMM16, true};
 DECODE_TABLE[0x41] = {1, "LD B, C", OperandType::NONE, true};
 DECODE_TABLE[0x51] = {1, "LD D, C", OperandType::NONE, true};
 DECODE_TABLE[0x61] = {1, "LD H, C", OperandType::NONE, true};
 DECODE_TABLE[0x71] = {1, "LD (HL), C", OperandType::NONE, true};
 DECODE_TABLE[0x81] = {1, "ADD A, C", OperandType::NONE, true};
 DECODE_TABLE[0x91] = {1, "SUB A, C", OperandType::NONE, true};
 DECODE_TABLE[0xA1] = {1, "AND A, C", OperandType::NONE, true};


}
