//
// Created by Nik Wipper on 2019-07-27.
//

#ifndef CORE_8_INSTRUCTIONS_H
#define CORE_8_INSTRUCTIONS_H

#include <cstdint>
#include <string>

std::vector<std::string> split(std::string string);

void label(std::string line, int line_count);
uint16_t jp(std::string line);
uint16_t call(std::string line);
uint16_t se(std::string line);
uint16_t sne(std::string line);
uint16_t mov(std::string line);
uint16_t add(std::string line);
uint16_t sub(std::string line);
uint16_t subn(std::string line);
uint16_t or_(std::string line);
uint16_t xor_(std::string line);
uint16_t and_(std::string line);
uint16_t rnd(std::string line);
uint16_t put(std::string line);
uint16_t skp(std::string line);
uint16_t sknp(std::string line);

#endif //CORE_8_INSTRUCTIONS_H
