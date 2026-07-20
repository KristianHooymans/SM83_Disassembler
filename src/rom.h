#ifndef ROMH
#define ROMH
#include <string>
#include <cstdint>
#include <vector>

std::vector<uint8_t> loadRom(std::string filename);
std::string romName(const std::vector<uint8_t>& hexDump);
std::string romType(const std::vector<uint8_t>& hexDump);
int romSize(const std::vector<uint8_t>& hexDump);

#endif 
