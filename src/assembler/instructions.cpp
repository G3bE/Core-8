//
// Created by Nik Wipper on 2019-07-27.
//

#include "instructions.h"

#include <vector>
#include <map>
#include <iostream>

// byte codes an their availability

#define FULL_FREE_1 0x0000
#define FULL_FREE_3 0x2000
#define FULL_FREE_4 0x7000

#define FULL_SPLIT_1 0x3000
#define FULL_SPLIT_2 0x5000
#define FULL_SPLIT_3 0x6000
#define FULL_SPLIT_4 0x8000
#define FULL_SPLIT_5 0x9000
#define FULL_SPLIT_6 0xA000

#define HALF_FREE 0x4000

#define SINGLE_FREE 0x1000

#define BASE_ADDRESS 512 // address to which the bytecode gets loaded

static std::map<std::string, int> labels;

std::vector<std::string> split(std::string string)
{
    std::vector<std::string> results;
    std::string buf;
    std::string::iterator i;
    for (i = string.begin(); i < string.end(); i++)
    {
        if (*i == ';')
        {
            break;
        }
        else if (*i == ' ' || *i == ',')
        {
            if (!buf.empty())
                results.push_back(buf);
            buf = "";
        } else {
            buf.push_back(*i);
        }
    }
    results.push_back(buf);
    return results;
}

bool is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

char read_byte(std::string token)
{
    if (is_number(token))
        return std::stoi(token);
    if (token[0] == '\\')
    {
        switch (token[1])
        {
        case 'b':
            return '\b';
        case 'e':
            return '\e';
        case 'f':
            return '\f';
        case 'n':
            return '\n';
        case 'r':
            return '\r';
        case 't':
            return '\t';
        case 'v':
            return '\v';
        case '\\':
            return '\\';
        case '\'':
            return '\'';
        case '"':
            return '"';
        case '?':
            return '\?';
        }
    }
    return token[0];
}

int read_address(std::string token)
{
    if (is_number(token))
        return std::stoi(token);
    int address = (*labels.find(token)).second * 2 + BASE_ADDRESS;
    return address;
}

void label(std::string line, int line_count)
{
    std::string label_name = line.substr(0, line.length()-1);
    labels.insert(std::pair<std::string, int>(label_name, line_count));
}

uint16_t jp(std::string line)
{
    std::string where = split(line)[1];
    uint16_t instruction;
    if (where.find('V') == 0)
    {
        instruction = SINGLE_FREE;
        instruction += std::stoi(std::string({where[1]}), 0, 16) * 0x100;
    } else {
        instruction = FULL_FREE_1;
        instruction += read_address(where);
    }
    return instruction;
}

uint16_t call(std::string line)
{
    std::string where = split(line)[1];
    uint16_t instruction = FULL_FREE_3;
    instruction += read_address(where);
    return instruction;
}

uint16_t se(std::string line)
{
    std::string reg = {split(line)[1][1]};
    uint16_t instruction = std::stoi(reg, 0, 16) * 0x100;
    if (split(line)[2].find('V') == 0)
    {
        std::string reg2 = {split(line)[2][1]};
        instruction += HALF_FREE;
        instruction += std::stoi(reg2, 0, 16) * 0x10;
    } else {
        std::string byte = split(line)[2];
        instruction += FULL_SPLIT_1;
        instruction += std::stoi(byte);
    }
    return instruction;
}

uint16_t sne(std::string line)
{
    std::string reg = {split(line)[1][1]};
    uint16_t instruction = std::stoi(reg, 0, 16) * 0x100;;
    if (split(line)[2].find('V') == 0)
    {
        std::string reg2 = {split(line)[2][1]};
        instruction = HALF_FREE + 0x1;
        instruction += std::stoi(reg2, 0, 16) * 0x10;
    } else {
        std::string byte = split(line)[2];
        instruction = FULL_SPLIT_2;
        instruction += std::stoi(byte, 0, 0);
    }
    return instruction;
}

uint16_t mov(std::string line)
{
    std::string what = split(line)[1];
    std::string from = split(line)[2];
    uint16_t instruction;
    if (what.find('V') == 0)
    {
        std::string reg = {what[1]};
        instruction += std::stoi(reg, 0, 16)*0x100;
    }else if (what == "I")
        instruction = SINGLE_FREE + 0x1;
    else if (what == "DT")
        instruction = SINGLE_FREE + 0x3;
    else if (what == "[I]")
        instruction = SINGLE_FREE + 0x4;

    if (from.find('V') == 0)
    {
        std::string reg2 = {from[1]};
        instruction += std::stoi(reg2, 0, 16) * 0x10;
    }
    else if (from == "DT")
    {
        instruction += SINGLE_FREE + 0x2;
    }
    else if (from == "[I]")
    {
        instruction += SINGLE_FREE + 0x5;
    }
    else if (from == "K")
    {
        instruction += SINGLE_FREE + 0x6;
    } else {
        instruction += FULL_SPLIT_3;
        instruction += std::stoi(from);
    }
    if (instruction < SINGLE_FREE)
        instruction += HALF_FREE + 0x2;

    return instruction;
}

uint16_t add(std::string line)
{
    std::string where = split(line)[1];
    uint16_t instruction;
    if (where.find('V') == 0)
    {
        std::string reg = {where[1]};
        instruction = std::stoi(reg, 0, 16) * 0x100;

        std::string what = split(line)[2];
        if (what.find('V') == 0)
        {
            std::string reg2 = {what[1]};
            instruction += HALF_FREE + 0x3;
            instruction += std::stoi(reg, 0, 16) * 0x10;
        } else {
            instruction += FULL_FREE_4;
            instruction += std::stoi(reg) * 0x1;
        }
    } else {
        where = split(line)[2];
        std::string reg = {where[1]};
        instruction = SINGLE_FREE + 0x7;
        instruction += std::stoi(reg, 0, 16) * 0x100;
    }
    return instruction;
}

uint16_t sub(std::string line)
{
    std::string where = split(line)[1];
    uint16_t instruction;
    if (where.find('V') == 0)
    {
        std::string reg = {where[1]};
        instruction = std::stoi(reg, 0, 16) * 0x100;

        std::string what = split(line)[2];
        if (what.find('V') == 0)
        {
            std::string reg2 = {what[1]};
            instruction += HALF_FREE + 0x4;
            instruction += std::stoi(reg, 0, 16) * 0x10;
        } else {
            instruction += FULL_SPLIT_4;
            instruction += std::stoi(reg) * 0x1;
        }
    } else {
        where = split(line)[2];
        std::string reg = {where[1]};
        instruction = SINGLE_FREE + 0x8;
        instruction += std::stoi(reg, 0, 16) * 0x100;
    }
    return instruction;
}

uint16_t subn(std::string line)
{
    std::string where = split(line)[1];
    uint16_t instruction;
    if (where.find('V') == 0)
    {
        std::string reg = {where[1]};
        instruction = std::stoi(reg, 0, 16) * 0x100;

        std::string what = split(line)[2];
        if (what.find('V') == 0)
        {
            std::string reg2 = {what[1]};
            instruction += HALF_FREE + 0x5;
            instruction += std::stoi(reg, 0, 16) * 0x10;
        } else {
            instruction += FULL_SPLIT_5;
            instruction += std::stoi(reg) * 0x1;
        }
    } else {
        where = split(line)[2];
        std::string reg = {where[1]};
        instruction = SINGLE_FREE + 0x9;
        instruction += std::stoi(reg, 0, 16) * 0x100;
    }
    return instruction;
}

uint16_t or_(std::string line)
{
    std::vector<std::string> splitt = split(line);
    std::string reg = {splitt[1][1]};
    std::string reg2 = {splitt[1][2]};

    uint16_t instruction = HALF_FREE + 0x6;
    instruction += std::stoi(reg, 0, 0) * 0x100;
    instruction += std::stoi(reg2, 0, 0) * 0x10;
    return instruction;
}

uint16_t xor_(std::string line)
{
    std::vector<std::string> splitt = split(line);
    std::string reg = {splitt[1][1]};
    std::string reg2 = {splitt[1][2]};

    uint16_t instruction = HALF_FREE + 0x7;
    instruction += std::stoi(reg, 0, 0) * 0x100;
    instruction += std::stoi(reg2, 0, 0) * 0x10;
    return instruction;
}

uint16_t and_(std::string line)
{
    std::vector<std::string> splitt = split(line);
    std::string reg = {splitt[1][1]};
    std::string reg2 = {splitt[1][2]};

    uint16_t instruction = HALF_FREE + 0x8;
    instruction += std::stoi(reg, 0, 16) * 0x100;
    instruction += std::stoi(reg2, 0, 16) * 0x10;
    return instruction;
}

uint16_t rnd(std::string line)
{
    std::string reg = {split(line)[1][1]};
    std::string seed = {split(line)[2]};
    uint16_t instruction = FULL_SPLIT_6;
    instruction += std::stoi(reg, 0, 16) * 0x100;
    instruction += std::stoi(reg, 0, 0);
    return instruction;
}

uint16_t put(std::string line)
{
    std::string what = split(line)[1];
    uint16_t instruction;
    if (what.find('V') == 0)
    {
        std::string reg = {what[1]};
        instruction = SINGLE_FREE + 0xA;
        instruction += (char)std::stoi(reg, 0, 16) * 0x100;
    } else {
        instruction = HALF_FREE + 0x9;
        instruction += read_byte(what) * 0x10;
    }
    return instruction;
}

uint16_t skp(std::string line)
{
    std::string key = split(line)[1];
    uint16_t instruction = SINGLE_FREE + 0xB;
    instruction += std::stoi(key);
    return instruction;
}

uint16_t sknp(std::string line)
{
    std::string key = split(line)[1];
    uint16_t instruction = SINGLE_FREE + 0xC;
    instruction += std::stoi(key);
    return instruction;
}
