//
// Created by Nik Wipper on 2019-07-24.
//

#include "cpu.h"

#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <vector>

CPU::CPU(uint16_t * code_start, uint16_t * code_end, int load_to) :
ram(4096)
{
    int offs = code_end - code_start;
    memcpy(this->ram.get(load_to), code_start, offs);

    this->current_instruction = load_to;
    this->last_instruction = load_to+offs;

    for (signed char & i : this->registers) {i = 0;}
}

std::vector<int> get_digits(int x)
{
    std::vector<int> digits;
    while (x >= 0x10)
    {
        digits.push_back(x % 0x10);
        x /= 0x10;
    }
    digits.push_back(x);
    std::reverse(digits.begin(), digits.end());
    return digits;
}

bool CPU::instruct(uint16_t * instruction)
{
    if (*instruction < 0x1000)
    {
        switch (*instruction)
        {
        case 0x0:
            this->pass();
            break;
        case 0xE0:
            this->cls();
            break;
        case 0xEE:
            this->ret();
            break;
        default:
            addr a = {*instruction};
            this->jp(a);
        }
    }
    else if (*instruction < 0x2000)
    {
        std::vector<int> digits = get_digits(*instruction);
        nibble n;
        n._ = digits[1];
        switch (digits[3])
        {
        case 0x0:
            this->jp(n);
            break;
        case 0x1:
            this->mov(n);
            break;
        case 0x2:
            this->mov_get_delay(n);
            break;
        case 0x3:
            this->mov_set_delay(n);
            break;
        case 0x4:
            this->store(n);
            break;
        case 0x5:
            this->load(n);
            break;
        case 0x6:
            this->get_key(n);
            break;
        case 0x7:
            this->add(n);
            break;
        case 0x8:
            this->sub(n);
            break;
        case 0x9:
            this->subn(n);
            break;
        case 0xA:
            this->put(n);
            break;
        case 0xB:
            this->skp(n);
            break;
        case 0xC:
            this->sknp(n);
            break;
        }
    }
    else if (*instruction < 0x3000)
    {
        addr ad;
        ad._ = *instruction - 0x2000;
        this->call(ad);
    }
    else if (*instruction < 0x4000)
    {
        std::vector<int> digits = get_digits(*instruction);
        nibble n;
        n._ = digits[1];
        byte b;
        b = (digits[2] * 0x10) + digits[3];
        this->se(n, b);
    }
    else if (*instruction < 0x5000)
    {
        std::vector<int> digits = get_digits(*instruction);
        nibble n;
        n._ = digits[1];
        nibble m;
        m._ = digits[2];

        byte b = n._ * 0x10 + m._;
        switch (digits[3])
        {
        case 0x0:
            this->se(n, m);
            break;
        case 0x1:
            this->sne(n, m);
            break;
        case 0x2:
            this->mov(n, m);
            break;
        case 0x3:
            this->add(n, m);
            break;
        case 0x4:
            this->sub(n, m);
            break;
        case 0x5:
            this->subn(n, m);
            break;
        case 0x6:
            this->or_(n, m);
            break;
        case 0x7:
            this->and_(n, m);
            break;
        case 0x8:
            this->xor_(n, m);
            break;
        case 0x9:
            this->put(b);
            break;
        }
    }
    else if (*instruction < 0x6000)
    {
        std::vector<int> digits = get_digits(*instruction);
        nibble n;
        n._ = digits[1];
        byte b;
        b = (digits[2] * 0x10) + digits[3];
        this->sne(n, b);
    }
    else if (*instruction < 0x7000)
    {
        std::vector<int> digits = get_digits(*instruction);
        nibble n;
        n._ = digits[1];
        byte b;
        b = (digits[2] * 0x10) + digits[3];
        this->mov(n, b);
    }
    else if (*instruction < 0x8000)
    {
        std::vector<int> digits = get_digits(*instruction);
        nibble n;
        n._ = digits[1];
        byte b;
        b = (digits[2] * 0x10) + digits[3];
        this->add(n, b);
    }
    else if (*instruction < 0x9000)
    {
        std::vector<int> digits = get_digits(*instruction);
        nibble n;
        n._ = digits[1];
        byte b;
        b = (digits[2] * 0x10) + digits[3];
        this->sub(n, b);
    }
    else if (*instruction < 0xA000)
    {
        std::vector<int> digits = get_digits(*instruction);
        nibble n;
        n._ = digits[1];
        byte b;
        b = (digits[2] * 0x10) + digits[3];
        this->subn(n, b);
    }
    else if (*instruction < 0xB000)
    {
        std::vector<int> digits = get_digits(*instruction);
        nibble n;
        n._ = digits[1];
        byte b;
        b = (digits[2] * 0x10) + digits[3];
        this->rnd(n, b);
    }

    return true;
}

int CPU::run()
{
    while (this->current_instruction >= 512 and this->current_instruction <= this->last_instruction)
    {
        auto real_instr = (uint16_t *) this->ram.get(this->current_instruction);
        this->instruct(real_instr);
        this->current_instruction += 2;
    }
}

// instructions

void CPU::pass()
{
}

void CPU::cls()
{
    // clear via termbox or smth
}

void CPU::ret()
{
    this->current_instruction = this->stack[this->stack_pointer];
}

// all the minus 1 because we always increment the PC

void CPU::jp(addr a)
{
    this->current_instruction = a._;
    this->current_instruction -= 2;

}

void CPU::jp(nibble x)
{
    this->current_instruction = this->registers[x._];
    this->current_instruction -= 2;
}

void CPU::call(addr a)
{
    this->stack_pointer++;
    this->stack[this->stack_pointer] = this->current_instruction;
    this->current_instruction = a._;
    this->current_instruction -= 2;
}

void CPU::se(nibble x, byte b)
{
    if (this->registers[x._] == b)
        this->current_instruction += 2;
}

void CPU::se(nibble x, nibble y)
{
    if (this->registers[x._] == this->registers[y._])
        this->current_instruction += 2;
}

void CPU::sne(nibble x, byte b)
{
    if (this->registers[x._] != b)
        this->current_instruction += 2;
}

void CPU::sne(nibble x, nibble y)
{
    if (this->registers[x._] != this->registers[y._])
        this->current_instruction += 2;
}

void CPU::mov(nibble x, byte b)
{
    this->registers[x._] = b;
}

void CPU::mov(nibble x, nibble y)
{
    this->registers[x._] = this->registers[y._];
}

void CPU::mov(nibble x)
{
    this->I = this->registers[x._];
}

/*
void CPU::mov(addr a)
{
    this->I = a._;
}
*/

void CPU::mov_get_delay(nibble x)
{
    this->registers[x._] = this->timer_delay;
}

void CPU::mov_set_delay(nibble x)
{
    this->timer_delay = this->registers[x._];
}

void CPU::store(nibble x)
{
    auto ad = (uint16_t *) this->ram.get(this->I);
    *ad = this->registers[x._];
}

void CPU::load(nibble x)
{
    auto ad = (uint16_t *) this->ram.get(this->I);
    this->registers[x._] = *ad;
}

void CPU::get_key(nibble x)
{
    byte in = getchar();
    this->registers[x._] = in;
}

void CPU::add(nibble x, byte a)
{
    this->registers[x._] += a;
}

void CPU::add(nibble x, nibble y)
{
    this->registers[x._] += this->registers[y._];
}

void CPU::add(nibble x)
{
    this->I += this->registers[x._];
}

void CPU::sub(nibble x, byte a)
{
    this->registers[x._] -= a;
}

void CPU::sub(nibble x, nibble y)
{
    this->registers[x._] -= this->registers[y._];
}

void CPU::sub(nibble x)
{
    this->I -= this->registers[x._];
}

void CPU::subn(nibble x, byte a)
{
    this->registers[x._] = a - this->registers[x._];
}

void CPU::subn(nibble x, nibble y)
{
    this->registers[x._] = this->registers[y._] - this->registers[x._];
}

void CPU::subn(nibble x)
{
    this->I = this->registers[x._ - I];
}

void CPU::or_(nibble x, nibble y)
{
    this->registers[x._] |= this->registers[y._];
}

void CPU::and_(nibble x, nibble y)
{
    this->registers[x._] &= this->registers[y._];
}

void CPU::xor_(nibble x, nibble y)
{
    this->registers[x._] ^= this->registers[y._];
}

void CPU::rnd(nibble x, byte seed)
{
    this->registers[x._] = (uint8_t)(random() % 256) & seed;
}

void CPU::put(byte a)
{
    std::cout.put(a);
}

void CPU::put(nibble x)
{
    std::cout.put(this->registers[x._]);
}

void CPU::skp(nibble x)
{
    // termbox can hopefully do that
}

void CPU::sknp(nibble x)
{
    // termbox can hopefully do that
}
