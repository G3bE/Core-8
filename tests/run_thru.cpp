//
// Created by Nik Wipper on 2019-07-25.
//

#include "../src/cpu/cpu.h"

#include <iostream>
#include <cstdint>

// ignore that shit
uint16_t buf[5] = {0x4489, 0x4659, 0x4799, 0x40A9, 0x0};

int main()
{
    CPU tester = CPU(buf, buf+8);
    tester.run();
    return 0;
}

