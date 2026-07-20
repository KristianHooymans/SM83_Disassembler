#ifndef ROMH
#define ROMH
#include <string>
#include <cstdint>
#include <vector>

std::vector<uint8_t> loadRom(std::string filename);
std::string romName(std::vector<uint8_t> hexDump);
std::string romType(std::vector<uint8_t> hexDump);
int romSize(std::vector<uint8_t> hexDump);

#endif 
