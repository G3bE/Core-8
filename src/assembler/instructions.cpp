//
// Created by Nik Wipper on 2019-07-27.
//

#include "instructions.h"

#include <vector>
#include <stdio.h>

std::vector<std::string> split(std::string string)
{
    std::vector<std::string> results;
    std::string buf;
    std::string::iterator i;
    for (i = string.begin(); i < string.end(); i++)
    {
        if (*i == ' ' || *i == ',')
        {
            results.push_back(buf);
            buf = "";
        } else {
            buf.push_back(*i);
        }
    }
    results.push_back(buf);
    return results;
}

uint16_t jp(std::string line)
{
    std::string where = split(line)[1];
    uint16_t instruction;
    if (where.find('V') == 0)
    {
        instruction = 0x1000;
        instruction += std::stoi(std::string({where[1]}), 0, 16) * 0x100;
    } else {
        instruction = 0x0000;
        instruction += std::stoi(where, 0, 16);
    }
    return instruction;
}

uint16_t call(std::string line)
{
    std::string where = split(line)[1];
    uint16_t instruction = 0x2000;
    instruction += std::stoi(where, 0, 16);
    return instruction;
}

uint16_t se(std::string line)
{
    std::string reg = {split(line)[1][1]};
    uint16_t instruction = std::stoi(reg, 0, 16) * 0x100;
    if (split(line)[2].find('V') == 0)
    {
        std::string reg2 = {split(line)[2][1]};
        instruction += 0x4000;
        instruction += std::stoi(reg2, 0, 16) * 0x10;
    } else {
        std::string byte = split(line)[2];
        instruction += 0x3000;
        instruction += std::stoi(byte);
    }
    return instruction;
}

uint16_t sne(std::string line)
{
    std::string reg = {split(line)[1][1]};
    uint16_t instruction;
    if (split(line)[2].find('V') == 0)
    {
        std::string reg2 = {split(line)[2][1]};
        instruction = 0x4000;
        instruction += std::stoi(reg, 0, 16) * 0x100;
        instruction += std::stoi(reg2, 0, 16) * 0x10;
        instruction += 0x1;
    } else {
        std::string byte = split(line)[2];
        instruction = 0x5000;
        instruction += std::stoi(reg, 0, 16) * 0x100;
        instruction += std::stoi(byte, 0, 0);
    }
    return instruction;
}

uint16_t mov(std::string line)
{
    std::string what = split(line)[1];
    std::string from = split(line)[2];
    uint16_t instruction = 0x0;
    if (what.find('V') == 0)
    {
        std::string reg = {what[1]};
        instruction += std::stoi(reg, 0, 16)*0x100;
    }else if (what == "I")
        instruction = 0x1001;
    else if (what == "DT")
        instruction = 0x1003;
    else if (what == "[I]")
        instruction = 0x1004;

    if (from.find('V') == 0)
    {
        std::string reg2 = {from[1]};
        instruction += std::stoi(reg2, 0, 16) * 0x10;
    }
    else if (from == "DT")
    {
        instruction += 0x1000;
        instruction += 0x2;
    }
    else if (from == "[I]")
    {
        instruction += 0x1000;
        instruction += 0x5;
    }
    else if (from == "K")
    {
        instruction += 0x1000;
        instruction += 0x6;
    } else {
        instruction += 0x6000;
        instruction += std::stoi(from);
    }
    if (instruction < 0x1000)
        instruction += 0x4002;

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
            instruction = 0x4003;
            instruction += std::stoi(reg, 0, 16) * 0x10;
        } else {
            instruction = 0x7000;
            instruction += std::stoi(reg) * 0x1;
        }
    } else {
        where = split(line)[2];
        std::string reg = {where[1]};
        instruction = 0x1007;
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
            instruction = 0x4003;
            instruction += std::stoi(reg, 0, 16) * 0x10;
        } else {
            instruction = 0x7000;
            instruction += std::stoi(reg) * 0x1;
        }
    } else {
        where = split(line)[2];
        std::string reg = {where[1]};
        instruction = 0x1007;
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
            instruction = 0x4003;
            instruction += std::stoi(reg, 0, 16) * 0x10;
        } else {
            instruction = 0x7000;
            instruction += std::stoi(reg) * 0x1;
        }
    } else {
        where = split(line)[2];
        std::string reg = {where[1]};
        instruction = 0x1007;
        instruction += std::stoi(reg, 0, 16) * 0x100;
    }
    return instruction;
}

uint16_t or_(std::string line)
{
    std::vector<std::string> splitt = split(line);
    std::string reg = {splitt[1][1]};
    std::string reg2 = {splitt[1][2]};

    uint16_t instruction = 0x4006;
    instruction += std::stoi(reg, 0, 0) * 0x100;
    instruction += std::stoi(reg2, 0, 0) * 0x10;
    return instruction;
}

uint16_t xor_(std::string line)
{
    std::vector<std::string> splitt = split(line);
    std::string reg = {splitt[1][1]};
    std::string reg2 = {splitt[1][2]};

    uint16_t instruction = 0x4006;
    instruction += std::stoi(reg, 0, 0) * 0x100;
    instruction += std::stoi(reg2, 0, 0) * 0x10;
    return instruction;
}

uint16_t and_(std::string line)
{
    std::vector<std::string> splitt = split(line);
    std::string reg = {splitt[1][1]};
    std::string reg2 = {splitt[1][2]};

    uint16_t instruction = 0x4006;
    instruction += std::stoi(reg, 0, 16) * 0x100;
    instruction += std::stoi(reg2, 0, 16) * 0x10;
    return instruction;
}

uint16_t rnd(std::string line)
{
    std::string reg = {split(line)[1][1]};
    std::string seed = {split(line)[2]};
    uint16_t instruction = 0xA000;
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
        instruction = 0x100A;
        instruction += std::stoi(reg, 0, 16) * 0x100;
    } else {
        instruction = 0x4009;
        instruction += std::stoi(what) * 0x10;
    }
    return instruction;
}

uint16_t skp(std::string line)
{
    std::string key = split(line)[1];
    uint16_t instruction = 0x100B;
    instruction += std::stoi(key);
    return instruction;
}

uint16_t sknp(std::string line)
{
    std::string key = split(line)[1];
    uint16_t instruction = 0x100C;
    instruction += std::stoi(key);
    return instruction;
}
