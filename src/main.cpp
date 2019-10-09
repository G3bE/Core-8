//
// Created by Nikolas Wipper on 2019-07-24.
//

#include <iostream>
#include <fstream>

#include "binary.h"
#include "cpu/cpu.h"
#include "assembler/assembler.h"

#define VERSION_MAYOR 1
#define VERSION_MINOR 0


std::string read_file(std::string path)
{
    std::string line;
    std::string res;

    std::ifstream file ("example.txt");
    if (file.is_open())
    {
        while ( getline (file,line) )
        {
            res.append(line + '\n');
        }
        file.close();
    } else return "";
    return res;
}


int main(int argc, char * argv[])
{
    std::string version = std::to_string(VERSION_MAYOR) + "." + std::to_string(VERSION_MINOR);
    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];
        if (arg.find('-') == 0)
        {
            if (arg == "-help" || arg == "--help")
            {
                std::cout << "AT Core-8 interpreter" << std::endl;
                std::cout << "Usage: " << argv[0] << " [options] <input_file>" << std::endl;
                std::cout << "Options:" << std::endl;
                std::cout << "--------" << std::endl;
                std::cout << "--help:   Shows this message" << std::endl;
            }
            continue;
        }
        // This is a fragment of my attempt to load byte code from a separate file
        // It checks if the header of the file has the same version as the current interpreter
        /*
        auto h = (header *) read_file(arg).c_str();
        if (h->version_mayor != VERSION_MAYOR or h->version_minor != VERSION_MINOR)
        {
            std::string message = "Binary file was build for version ";
            message.append(std::to_string(h->version_mayor));
            message.append(".");
            message.append(std::to_string(h->version_minor));
            message.append(" of this interpreter but this is version ");
            message.append(version);
            std::cerr << message << std::endl;
            return 1;
        }
         */
        std::vector<uint16_t> instructions = proccess_file(arg);
        uint16_t * code_start = instructions.data();
        CPU * cpu = new CPU(code_start, code_start + instructions.size()*2);
        cpu->run();
    }
    return 0;
}