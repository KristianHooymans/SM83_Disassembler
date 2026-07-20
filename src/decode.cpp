#include "decode.h"

#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <array>

std::array<Opcode, 256> DECODE_TABLE;

void initDecodeTable() {

 DECODE_TABLE[0x00] = {1, "NOP", OperandType::NONE};
 DECODE_TABLE[0xAF] = {1, "XOR", OperandType::NONE};
 DECODE_TABLE[0xC9] = {1, "RET", OperandType::NONE};
 DECODE_TABLE[0x76] = {1, "HALT", OperandType::NONE};
 DECODE_TABLE[0x3E] = {2, "LD A, n8", OperandType::IMM8};
 DECODE_TABLE[0x21] = {3, "LD HL, n16", OperandType::IMM16};
 DECODE_TABLE[0xC3] = {3, "JP a16", OperandType::IMM16};
 DECODE_TABLE[0xCD] = {3, "CALL a16", OperandType::IMM16};
 DECODE_TABLE[0x18] = {2, "JR e8", OperandType::REL8};

}
