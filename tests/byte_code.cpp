//
// Created by Nik Wipper on 2019-07-25.
//

#include "../src/cpu/cpu.h"

#include <iostream>
#include <cstdint>

// ignore that shit
uint16_t buf[3] = {0x1337, 0x6969, 0x0};

class Tester : public CPU
{
public:
    Tester() : CPU(buf, buf + 2) {}
    void print()
    {
        uint16_t print_H = 0x4489;
        uint16_t print_e = 0x4659;
        uint16_t print_y = 0x4799;
        uint16_t print_new_line = 0x40A9;

        this->instruct(&print_H);
        this->instruct(&print_e);
        this->instruct(&print_y);
        this->instruct(&print_new_line);
    }
};

int main()
{
    Tester tester = Tester();
    tester.print();
    //tester.test_mov();
    //tester.test_add_sub_n();
    return 0;
}

