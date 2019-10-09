//
// Created by Nik Wipper on 2019-07-25.
//

#include "../src/cpu/cpu.h"

#include <iostream>

uint16_t buf[3] = {0x1337, 0x6969, 0x0};

class Tester : public CPU
{
public:
    Tester() : CPU(buf, buf + 2) {}
    void print()
    {
        this->put('H');
        this->put('e');
        this->put('l');
        this->put('l');
        this->put('o');
        this->put('\n');
    }

    void test_mov()
    {
        this->mov({4}, 23);
        this->mov({0xA}, (nibble){4});
        this->mov({0xA});
        assert(this->registers[4]==23); assert(this->registers[10]==23); assert(this->I==23);
    }

    void test_add_sub_n()
    {
        this->mov({4}, 23);
        assert(this->registers[4]==23);
        this->add({4}, 9);
        assert(this->registers[4]==32);
        this->sub({4}, 19);
        assert(this->registers[4]==13);
        this->subn({4}, 22);
        assert(this->registers[4]==9);
    }
};

int main()
{
    Tester tester = Tester();
    tester.print();
    tester.test_mov();
    tester.test_add_sub_n();
    return 0;
}
