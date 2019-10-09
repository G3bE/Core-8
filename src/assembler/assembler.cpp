//
// Created by Nik Wipper on 2019-10-09.
//

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "instructions.h"

#include "assembler.h"

constexpr unsigned int hash(const char * s, int off = 0) {
    return !s[off] ? 5381 : hash(s, off+1)*33 ^ s[off];
}

std::vector<uint16_t> proccess_file(std::string path)
{
    std::string line;
    std::vector<uint16_t> res;

    std::ifstream file(path);
    if (file.is_open())
    {
        while (getline(file, line))
        {
            std::vector<std::string> tokens = split(line);
            uint16_t instruction;

            switch (hash(tokens[0].c_str()))
            {
            case hash("cls"):
                instruction = 0x00E0;
                break;
            case hash("ret"):
                instruction = 0x00EE;
                break;
            case hash("jp"):
                instruction = jp(line);
                break;
            case hash("call"):
                instruction = call(line);
                break;
            case hash("se"):
                instruction = se(line);
                break;
            case hash("sne"):
                instruction = sne(line);
                break;
            case hash("mov"):
                instruction = mov(line);
                break;
            case hash("add"):
                instruction = add(line);
                break;
            case hash("sub"):
                instruction = sub(line);
                break;
            case hash("subn"):
                instruction = subn(line);
                break;
            case hash("or"):
                instruction = or_(line);
                break;
            case hash("xor"):
                instruction = xor_(line);
                break;
            case hash("and"):
                instruction = and_(line);
                break;
            case hash("rnd"):
                instruction = rnd(line);
                break;
            case hash("put"):
                instruction = put(line);
                break;
            case hash("skp"):
                instruction = skp(line);
                break;
            case hash("sknp"):
                instruction = sknp(line);
                break;
            }
            res.push_back(instruction);
        }
        file.close();
    } else return {};
    return res;
}
