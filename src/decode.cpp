#include "decode.h"

#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <array>
#include <iomanip>
#include <format>
#include <iterator>

std::array<Opcode, 256> DECODE_TABLE;
std::array<std::string, 256> CB_TABLE;


void disassemble(const std::vector<uint8_t>& rom) {
  int pos = 0x0100;
  while (pos <  std::ssize(rom)) {
    DecodedInstruction d = decode(rom, pos);
    std::cout << std::hex << std::setw(4) << std::setfill('0') << pos << ": " << d.code << "\n";
    pos += d.length;
    if(pos + 2 >= std::ssize(rom)) break;
  } 
}


DecodedInstruction decode(const std::vector<uint8_t>& rom, int pos) {
  uint8_t opcode = rom[pos];

  if (opcode == 0xCB) return {CB_TABLE[rom[pos + 1]], 2}; // use CB_TABLE rather than regular table (CB prefix)
  
  Opcode entry = DECODE_TABLE[opcode];
  std::string code = entry.assemblyRepresentation;
  int length = entry.length;
  
  if (pos + length > std::ssize(rom)){
    return {std::format("DB ${:02X}", rom[pos]), 1};
  }

  if (!entry.valid) {
    code += std::format(" ${:02X}", rom[pos]);
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
  code += entry.suffix;

  return {code, length};

}


void initDecodeTable() {
  
  DECODE_TABLE[0x00] = {1, "NOP",          OperandType::NONE,  true};
  DECODE_TABLE[0x10] = {2, "STOP",         OperandType::NONE,  true};
  DECODE_TABLE[0x20] = {2, "JR NZ, ",      OperandType::REL8,  true};
  DECODE_TABLE[0x30] = {2, "JR NC, ",      OperandType::REL8,  true};
  DECODE_TABLE[0x40] = {1, "LD B, B",      OperandType::NONE,  true};
  DECODE_TABLE[0x50] = {1, "LD D, B",      OperandType::NONE,  true};
  DECODE_TABLE[0x60] = {1, "LD H, B",      OperandType::NONE,  true};
  DECODE_TABLE[0x70] = {1, "LD (HL), B",   OperandType::NONE,  true};
  DECODE_TABLE[0x80] = {1, "ADD A, B",     OperandType::NONE,  true};
  DECODE_TABLE[0x90] = {1, "SUB A, B",     OperandType::NONE,  true};
  DECODE_TABLE[0xA0] = {1, "AND A, B",     OperandType::NONE,  true};
  DECODE_TABLE[0xB0] = {1, "OR A, B",      OperandType::NONE,  true};
  DECODE_TABLE[0xC0] = {1, "RET NZ",       OperandType::NONE,  true};
  DECODE_TABLE[0xD0] = {1, "RET NC",       OperandType::NONE,  true};
  DECODE_TABLE[0xE0] = {2, "LDH (",        OperandType::IMM8,  true, "), A"};
  DECODE_TABLE[0xF0] = {2, "LDH A, (",     OperandType::IMM8,  true, ")"};
 
  DECODE_TABLE[0x01] = {3, "LD BC, ",      OperandType::IMM16, true};
  DECODE_TABLE[0x11] = {3, "LD DE, ",      OperandType::IMM16, true};
  DECODE_TABLE[0x21] = {3, "LD HL, ",      OperandType::IMM16, true};
  DECODE_TABLE[0x31] = {3, "LD SP, ",      OperandType::IMM16, true};
  DECODE_TABLE[0x41] = {1, "LD B, C",      OperandType::NONE,  true};
  DECODE_TABLE[0x51] = {1, "LD D, C",      OperandType::NONE,  true};
  DECODE_TABLE[0x61] = {1, "LD H, C",      OperandType::NONE,  true};
  DECODE_TABLE[0x71] = {1, "LD (HL), C",   OperandType::NONE,  true};
  DECODE_TABLE[0x81] = {1, "ADD A, C",     OperandType::NONE,  true};
  DECODE_TABLE[0x91] = {1, "SUB A, C",     OperandType::NONE,  true};
  DECODE_TABLE[0xA1] = {1, "AND A, C",     OperandType::NONE,  true};
  DECODE_TABLE[0xB1] = {1, "OR A, C",      OperandType::NONE,  true};
  DECODE_TABLE[0xC1] = {1, "POP BC",       OperandType::NONE,  true};
  DECODE_TABLE[0xD1] = {1, "POP DE",       OperandType::NONE,  true};
  DECODE_TABLE[0xE1] = {1, "POP HL",       OperandType::NONE,  true};
  DECODE_TABLE[0xF1] = {1, "POP AF",       OperandType::NONE,  true};
 
  DECODE_TABLE[0x02] = {1, "LD (BC), A",   OperandType::NONE,  true};
  DECODE_TABLE[0x12] = {1, "LD (DE), A",   OperandType::NONE,  true};
  DECODE_TABLE[0x22] = {1, "LD (HL+), A",  OperandType::NONE,  true};
  DECODE_TABLE[0x32] = {1, "LD (HL-), A",  OperandType::NONE,  true};
  DECODE_TABLE[0x42] = {1, "LD B, D",      OperandType::NONE,  true};
  DECODE_TABLE[0x52] = {1, "LD D, D",      OperandType::NONE,  true};
  DECODE_TABLE[0x62] = {1, "LD H, D",      OperandType::NONE,  true};
  DECODE_TABLE[0x72] = {1, "LD (HL), D",   OperandType::NONE,  true};
  DECODE_TABLE[0x82] = {1, "ADD A, D",     OperandType::NONE,  true};
  DECODE_TABLE[0x92] = {1, "SUB A, D",     OperandType::NONE,  true};
  DECODE_TABLE[0xA2] = {1, "AND A, D",     OperandType::NONE,  true};
  DECODE_TABLE[0xB2] = {1, "OR A, D",      OperandType::NONE,  true};
  DECODE_TABLE[0xC2] = {3, "JP NZ, ",      OperandType::IMM16, true};
  DECODE_TABLE[0xD2] = {3, "JP NC, ",      OperandType::IMM16, true};
  DECODE_TABLE[0xE2] = {1, "LDH (C), A",   OperandType::NONE,  true};
  DECODE_TABLE[0xF2] = {1, "LDH A, (C)",   OperandType::NONE,  true};
 
  DECODE_TABLE[0x03] = {1, "INC BC",       OperandType::NONE,  true};
  DECODE_TABLE[0x13] = {1, "INC DE",       OperandType::NONE,  true};
  DECODE_TABLE[0x23] = {1, "INC HL",       OperandType::NONE,  true};
  DECODE_TABLE[0x33] = {1, "INC SP",       OperandType::NONE,  true};
  DECODE_TABLE[0x43] = {1, "LD B, E",      OperandType::NONE,  true};
  DECODE_TABLE[0x53] = {1, "LD D, E",      OperandType::NONE,  true};
  DECODE_TABLE[0x63] = {1, "LD H, E",      OperandType::NONE,  true};
  DECODE_TABLE[0x73] = {1, "LD (HL), E",   OperandType::NONE,  true};
  DECODE_TABLE[0x83] = {1, "ADD A, E",     OperandType::NONE,  true};
  DECODE_TABLE[0x93] = {1, "SUB A, E",     OperandType::NONE,  true};
  DECODE_TABLE[0xA3] = {1, "AND A, E",     OperandType::NONE,  true};
  DECODE_TABLE[0xB3] = {1, "OR A, E",      OperandType::NONE,  true};
  DECODE_TABLE[0xC3] = {3, "JP ",          OperandType::IMM16, true};
  DECODE_TABLE[0xF3] = {1, "DI",           OperandType::NONE,  true};
 
  DECODE_TABLE[0x04] = {1, "INC B",        OperandType::NONE,  true};
  DECODE_TABLE[0x14] = {1, "INC D",        OperandType::NONE,  true};
  DECODE_TABLE[0x24] = {1, "INC H",        OperandType::NONE,  true};
  DECODE_TABLE[0x34] = {1, "INC (HL)",     OperandType::NONE,  true};
  DECODE_TABLE[0x44] = {1, "LD B, H",      OperandType::NONE,  true};
  DECODE_TABLE[0x54] = {1, "LD D, H",      OperandType::NONE,  true};
  DECODE_TABLE[0x64] = {1, "LD H, H",      OperandType::NONE,  true};
  DECODE_TABLE[0x74] = {1, "LD (HL), H",   OperandType::NONE,  true};
  DECODE_TABLE[0x84] = {1, "ADD A, H",     OperandType::NONE,  true};
  DECODE_TABLE[0x94] = {1, "SUB A, H",     OperandType::NONE,  true};
  DECODE_TABLE[0xA4] = {1, "AND A, H",     OperandType::NONE,  true};
  DECODE_TABLE[0xB4] = {1, "OR A, H",      OperandType::NONE,  true};
  DECODE_TABLE[0xC4] = {3, "CALL NZ, ",    OperandType::IMM16, true};
  DECODE_TABLE[0xD4] = {3, "CALL NC, ",    OperandType::IMM16, true};
 
  DECODE_TABLE[0x05] = {1, "DEC B",        OperandType::NONE,  true};
  DECODE_TABLE[0x15] = {1, "DEC D",        OperandType::NONE,  true};
  DECODE_TABLE[0x25] = {1, "DEC H",        OperandType::NONE,  true};
  DECODE_TABLE[0x35] = {1, "DEC (HL)",     OperandType::NONE,  true};
  DECODE_TABLE[0x45] = {1, "LD B, L",      OperandType::NONE,  true};
  DECODE_TABLE[0x55] = {1, "LD D, L",      OperandType::NONE,  true};
  DECODE_TABLE[0x65] = {1, "LD H, L",      OperandType::NONE,  true};
  DECODE_TABLE[0x75] = {1, "LD (HL), L",   OperandType::NONE,  true};
  DECODE_TABLE[0x85] = {1, "ADD A, L",     OperandType::NONE,  true};
  DECODE_TABLE[0x95] = {1, "SUB A, L",     OperandType::NONE,  true};
  DECODE_TABLE[0xA5] = {1, "AND A, L",     OperandType::NONE,  true};
  DECODE_TABLE[0xB5] = {1, "OR A, L",      OperandType::NONE,  true};
  DECODE_TABLE[0xC5] = {1, "PUSH BC",      OperandType::NONE,  true};
  DECODE_TABLE[0xD5] = {1, "PUSH DE",      OperandType::NONE,  true};
  DECODE_TABLE[0xE5] = {1, "PUSH HL",      OperandType::NONE,  true};
  DECODE_TABLE[0xF5] = {1, "PUSH AF",      OperandType::NONE,  true};
 
  DECODE_TABLE[0x06] = {2, "LD B, ",       OperandType::IMM8,  true};
  DECODE_TABLE[0x16] = {2, "LD D, ",       OperandType::IMM8,  true};
  DECODE_TABLE[0x26] = {2, "LD H, ",       OperandType::IMM8,  true};
  DECODE_TABLE[0x36] = {2, "LD (HL), ",    OperandType::IMM8,  true};
  DECODE_TABLE[0x46] = {1, "LD B, (HL)",   OperandType::NONE,  true};
  DECODE_TABLE[0x56] = {1, "LD D, (HL)",   OperandType::NONE,  true};
  DECODE_TABLE[0x66] = {1, "LD H, (HL)",   OperandType::NONE,  true};
  DECODE_TABLE[0x76] = {1, "HALT",         OperandType::NONE,  true};
  DECODE_TABLE[0x86] = {1, "ADD A, (HL)",  OperandType::NONE,  true};
  DECODE_TABLE[0x96] = {1, "SUB A, (HL)",  OperandType::NONE,  true};
  DECODE_TABLE[0xA6] = {1, "AND A, (HL)",  OperandType::NONE,  true};
  DECODE_TABLE[0xB6] = {1, "OR A, (HL)",   OperandType::NONE,  true};
  DECODE_TABLE[0xC6] = {2, "ADD A, ",      OperandType::IMM8,  true};
  DECODE_TABLE[0xD6] = {2, "SUB A, ",      OperandType::IMM8,  true};
  DECODE_TABLE[0xE6] = {2, "AND A, ",      OperandType::IMM8,  true};
  DECODE_TABLE[0xF6] = {2, "OR A, ",       OperandType::IMM8,  true};
 
  DECODE_TABLE[0x07] = {1, "RLCA",         OperandType::NONE,  true};
  DECODE_TABLE[0x17] = {1, "RLA",          OperandType::NONE,  true};
  DECODE_TABLE[0x27] = {1, "DAA",          OperandType::NONE,  true};
  DECODE_TABLE[0x37] = {1, "SCF",          OperandType::NONE,  true};
  DECODE_TABLE[0x47] = {1, "LD B, A",      OperandType::NONE,  true};
  DECODE_TABLE[0x57] = {1, "LD D, A",      OperandType::NONE,  true};
  DECODE_TABLE[0x67] = {1, "LD H, A",      OperandType::NONE,  true};
  DECODE_TABLE[0x77] = {1, "LD (HL), A",   OperandType::NONE,  true};
  DECODE_TABLE[0x87] = {1, "ADD A, A",     OperandType::NONE,  true};
  DECODE_TABLE[0x97] = {1, "SUB A, A",     OperandType::NONE,  true};
  DECODE_TABLE[0xA7] = {1, "AND A, A",     OperandType::NONE,  true};
  DECODE_TABLE[0xB7] = {1, "OR A, A",      OperandType::NONE,  true};
  DECODE_TABLE[0xC7] = {1, "RST $00",      OperandType::NONE,  true};
  DECODE_TABLE[0xD7] = {1, "RST $10",      OperandType::NONE,  true};
  DECODE_TABLE[0xE7] = {1, "RST $20",      OperandType::NONE,  true};
  DECODE_TABLE[0xF7] = {1, "RST $30",      OperandType::NONE,  true};
 
  DECODE_TABLE[0x08] = {3, "LD (",         OperandType::IMM16, true, "), SP"};
  DECODE_TABLE[0x18] = {2, "JR ",          OperandType::REL8,  true};
  DECODE_TABLE[0x28] = {2, "JR Z, ",       OperandType::REL8,  true};
  DECODE_TABLE[0x38] = {2, "JR C, ",       OperandType::REL8,  true};
  DECODE_TABLE[0x48] = {1, "LD C, B",      OperandType::NONE,  true};
  DECODE_TABLE[0x58] = {1, "LD E, B",      OperandType::NONE,  true};
  DECODE_TABLE[0x68] = {1, "LD L, B",      OperandType::NONE,  true};
  DECODE_TABLE[0x78] = {1, "LD A, B",      OperandType::NONE,  true};
  DECODE_TABLE[0x88] = {1, "ADC A, B",     OperandType::NONE,  true};
  DECODE_TABLE[0x98] = {1, "SBC A, B",     OperandType::NONE,  true};
  DECODE_TABLE[0xA8] = {1, "XOR A, B",     OperandType::NONE,  true};
  DECODE_TABLE[0xB8] = {1, "CP A, B",      OperandType::NONE,  true};
  DECODE_TABLE[0xC8] = {1, "RET Z",        OperandType::NONE,  true};
  DECODE_TABLE[0xD8] = {1, "RET C",        OperandType::NONE,  true};
  DECODE_TABLE[0xE8] = {2, "ADD SP, ",     OperandType::IMM8,  true};  // signed operand, prints unsigned
  DECODE_TABLE[0xF8] = {2, "LD HL, SP+",   OperandType::IMM8,  true};  // signed operand, prints unsigned
 
  DECODE_TABLE[0x09] = {1, "ADD HL, BC",   OperandType::NONE,  true};
  DECODE_TABLE[0x19] = {1, "ADD HL, DE",   OperandType::NONE,  true};
  DECODE_TABLE[0x29] = {1, "ADD HL, HL",   OperandType::NONE,  true};
  DECODE_TABLE[0x39] = {1, "ADD HL, SP",   OperandType::NONE,  true};
  DECODE_TABLE[0x49] = {1, "LD C, C",      OperandType::NONE,  true};
  DECODE_TABLE[0x59] = {1, "LD E, C",      OperandType::NONE,  true};
  DECODE_TABLE[0x69] = {1, "LD L, C",      OperandType::NONE,  true};
  DECODE_TABLE[0x79] = {1, "LD A, C",      OperandType::NONE,  true};
  DECODE_TABLE[0x89] = {1, "ADC A, C",     OperandType::NONE,  true};
  DECODE_TABLE[0x99] = {1, "SBC A, C",     OperandType::NONE,  true};
  DECODE_TABLE[0xA9] = {1, "XOR A, C",     OperandType::NONE,  true};
  DECODE_TABLE[0xB9] = {1, "CP A, C",      OperandType::NONE,  true};
  DECODE_TABLE[0xC9] = {1, "RET",          OperandType::NONE,  true};
  DECODE_TABLE[0xD9] = {1, "RETI",         OperandType::NONE,  true};
  DECODE_TABLE[0xE9] = {1, "JP HL",        OperandType::NONE,  true};
  DECODE_TABLE[0xF9] = {1, "LD SP, HL",    OperandType::NONE,  true};
 
  DECODE_TABLE[0x0A] = {1, "LD A, (BC)",   OperandType::NONE,  true};
  DECODE_TABLE[0x1A] = {1, "LD A, (DE)",   OperandType::NONE,  true};
  DECODE_TABLE[0x2A] = {1, "LD A, (HL+)",  OperandType::NONE,  true};
  DECODE_TABLE[0x3A] = {1, "LD A, (HL-)",  OperandType::NONE,  true};
  DECODE_TABLE[0x4A] = {1, "LD C, D",      OperandType::NONE,  true};
  DECODE_TABLE[0x5A] = {1, "LD E, D",      OperandType::NONE,  true};
  DECODE_TABLE[0x6A] = {1, "LD L, D",      OperandType::NONE,  true};
  DECODE_TABLE[0x7A] = {1, "LD A, D",      OperandType::NONE,  true};
  DECODE_TABLE[0x8A] = {1, "ADC A, D",     OperandType::NONE,  true};
  DECODE_TABLE[0x9A] = {1, "SBC A, D",     OperandType::NONE,  true};
  DECODE_TABLE[0xAA] = {1, "XOR A, D",     OperandType::NONE,  true};
  DECODE_TABLE[0xBA] = {1, "CP A, D",      OperandType::NONE,  true};
  DECODE_TABLE[0xCA] = {3, "JP Z, ",       OperandType::IMM16, true};
  DECODE_TABLE[0xDA] = {3, "JP C, ",       OperandType::IMM16, true};
  DECODE_TABLE[0xEA] = {3, "LD (",         OperandType::IMM16, true, "), A"};
  DECODE_TABLE[0xFA] = {3, "LD A, (",      OperandType::IMM16, true, ")"};
 
  DECODE_TABLE[0x0B] = {1, "DEC BC",       OperandType::NONE,  true};
  DECODE_TABLE[0x1B] = {1, "DEC DE",       OperandType::NONE,  true};
  DECODE_TABLE[0x2B] = {1, "DEC HL",       OperandType::NONE,  true};
  DECODE_TABLE[0x3B] = {1, "DEC SP",       OperandType::NONE,  true};
  DECODE_TABLE[0x4B] = {1, "LD C, E",      OperandType::NONE,  true};
  DECODE_TABLE[0x5B] = {1, "LD E, E",      OperandType::NONE,  true};
  DECODE_TABLE[0x6B] = {1, "LD L, E",      OperandType::NONE,  true};
  DECODE_TABLE[0x7B] = {1, "LD A, E",      OperandType::NONE,  true};
  DECODE_TABLE[0x8B] = {1, "ADC A, E",     OperandType::NONE,  true};
  DECODE_TABLE[0x9B] = {1, "SBC A, E",     OperandType::NONE,  true};
  DECODE_TABLE[0xAB] = {1, "XOR A, E",     OperandType::NONE,  true};
  DECODE_TABLE[0xBB] = {1, "CP A, E",      OperandType::NONE,  true};
  //DECODE_TABLE[0xCB] = {2, "CB ",          OperandType::IMM8,  true};  //CB page done so this doesn't do anything.
  DECODE_TABLE[0xFB] = {1, "EI",           OperandType::NONE,  true};
 
  DECODE_TABLE[0x0C] = {1, "INC C",        OperandType::NONE,  true};
  DECODE_TABLE[0x1C] = {1, "INC E",        OperandType::NONE,  true};
  DECODE_TABLE[0x2C] = {1, "INC L",        OperandType::NONE,  true};
  DECODE_TABLE[0x3C] = {1, "INC A",        OperandType::NONE,  true};
  DECODE_TABLE[0x4C] = {1, "LD C, H",      OperandType::NONE,  true};
  DECODE_TABLE[0x5C] = {1, "LD E, H",      OperandType::NONE,  true};
  DECODE_TABLE[0x6C] = {1, "LD L, H",      OperandType::NONE,  true};
  DECODE_TABLE[0x7C] = {1, "LD A, H",      OperandType::NONE,  true};
  DECODE_TABLE[0x8C] = {1, "ADC A, H",     OperandType::NONE,  true};
  DECODE_TABLE[0x9C] = {1, "SBC A, H",     OperandType::NONE,  true};
  DECODE_TABLE[0xAC] = {1, "XOR A, H",     OperandType::NONE,  true};
  DECODE_TABLE[0xBC] = {1, "CP A, H",      OperandType::NONE,  true};
  DECODE_TABLE[0xCC] = {3, "CALL Z, ",     OperandType::IMM16, true};
  DECODE_TABLE[0xDC] = {3, "CALL C, ",     OperandType::IMM16, true};
 
  DECODE_TABLE[0x0D] = {1, "DEC C",        OperandType::NONE,  true};
  DECODE_TABLE[0x1D] = {1, "DEC E",        OperandType::NONE,  true};
  DECODE_TABLE[0x2D] = {1, "DEC L",        OperandType::NONE,  true};
  DECODE_TABLE[0x3D] = {1, "DEC A",        OperandType::NONE,  true};
  DECODE_TABLE[0x4D] = {1, "LD C, L",      OperandType::NONE,  true};
  DECODE_TABLE[0x5D] = {1, "LD E, L",      OperandType::NONE,  true};
  DECODE_TABLE[0x6D] = {1, "LD L, L",      OperandType::NONE,  true};
  DECODE_TABLE[0x7D] = {1, "LD A, L",      OperandType::NONE,  true};
  DECODE_TABLE[0x8D] = {1, "ADC A, L",     OperandType::NONE,  true};
  DECODE_TABLE[0x9D] = {1, "SBC A, L",     OperandType::NONE,  true};
  DECODE_TABLE[0xAD] = {1, "XOR A, L",     OperandType::NONE,  true};
  DECODE_TABLE[0xBD] = {1, "CP A, L",      OperandType::NONE,  true};
  DECODE_TABLE[0xCD] = {3, "CALL ",        OperandType::IMM16, true};
 
  DECODE_TABLE[0x0E] = {2, "LD C, ",       OperandType::IMM8,  true};
  DECODE_TABLE[0x1E] = {2, "LD E, ",       OperandType::IMM8,  true};
  DECODE_TABLE[0x2E] = {2, "LD L, ",       OperandType::IMM8,  true};
  DECODE_TABLE[0x3E] = {2, "LD A, ",       OperandType::IMM8,  true};
  DECODE_TABLE[0x4E] = {1, "LD C, (HL)",   OperandType::NONE,  true};
  DECODE_TABLE[0x5E] = {1, "LD E, (HL)",   OperandType::NONE,  true};
  DECODE_TABLE[0x6E] = {1, "LD L, (HL)",   OperandType::NONE,  true};
  DECODE_TABLE[0x7E] = {1, "LD A, (HL)",   OperandType::NONE,  true};
  DECODE_TABLE[0x8E] = {1, "ADC A, (HL)",  OperandType::NONE,  true};
  DECODE_TABLE[0x9E] = {1, "SBC A, (HL)",  OperandType::NONE,  true};
  DECODE_TABLE[0xAE] = {1, "XOR A, (HL)",  OperandType::NONE,  true};
  DECODE_TABLE[0xBE] = {1, "CP A, (HL)",   OperandType::NONE,  true};
  DECODE_TABLE[0xCE] = {2, "ADC A, ",      OperandType::IMM8,  true};
  DECODE_TABLE[0xDE] = {2, "SBC A, ",      OperandType::IMM8,  true};
  DECODE_TABLE[0xEE] = {2, "XOR A, ",      OperandType::IMM8,  true};
  DECODE_TABLE[0xFE] = {2, "CP A, ",       OperandType::IMM8,  true};
 
  DECODE_TABLE[0x0F] = {1, "RRCA",         OperandType::NONE,  true};
  DECODE_TABLE[0x1F] = {1, "RRA",          OperandType::NONE,  true};
  DECODE_TABLE[0x2F] = {1, "CPL",          OperandType::NONE,  true};
  DECODE_TABLE[0x3F] = {1, "CCF",          OperandType::NONE,  true};
  DECODE_TABLE[0x4F] = {1, "LD C, A",      OperandType::NONE,  true};
  DECODE_TABLE[0x5F] = {1, "LD E, A",      OperandType::NONE,  true};
  DECODE_TABLE[0x6F] = {1, "LD L, A",      OperandType::NONE,  true};
  DECODE_TABLE[0x7F] = {1, "LD A, A",      OperandType::NONE,  true};
  DECODE_TABLE[0x8F] = {1, "ADC A, A",     OperandType::NONE,  true};
  DECODE_TABLE[0x9F] = {1, "SBC A, A",     OperandType::NONE,  true};
  DECODE_TABLE[0xAF] = {1, "XOR A, A",     OperandType::NONE,  true};
  DECODE_TABLE[0xBF] = {1, "CP A, A",      OperandType::NONE,  true};
  DECODE_TABLE[0xCF] = {1, "RST $08",      OperandType::NONE,  true};
  DECODE_TABLE[0xDF] = {1, "RST $18",      OperandType::NONE,  true};
  DECODE_TABLE[0xEF] = {1, "RST $28",      OperandType::NONE,  true};
  DECODE_TABLE[0xFF] = {1, "RST $38",      OperandType::NONE,  true};


  const std::string OPS[8] = {"RLC", "RRC", "RL", "RR", "SLA", "SRA", "SWAP", "SRL"};
  const std::string REGS[8] = {"B", "C", "D", "E", "H", "L", "(HL)", "A"};
  const std::string FAM[4] = {"", "BIT", "RES", "SET"};

  for (int i = 0; i < 256; ++i) {
    int fam = i >> 6;
    int mid = (i >> 3) & 7;
    int reg = i & 7;

    if (fam == 0) {
      CB_TABLE[i] = OPS[mid] + " " + REGS[reg];
    }
    else {
      CB_TABLE[i] = std::format("{} {}, {}", FAM[fam], mid, REGS[reg]);
    }
  }
}
