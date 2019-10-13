//
// Created by Nik Wipper on 2019-07-24.
//

#ifndef CHIP_8_CPU_H
#define CHIP_8_CPU_H

#include <cstdint>
#include <vector>

#include "../memory/ram.h"

// These are structs instead of normal typedefs because otherwise the compiler couldn't tell certain overloaded functions apart

typedef struct
{
    uint16_t _;
} addr;

typedef struct
{
    uint8_t _;
} nibble;

typedef uint8_t byte;

class CPU {
// for the tests
protected:
    RAM ram;
    uint16_t current_instruction; // pc
    int last_instruction;

    int8_t registers[16];
    int16_t I;
    uint8_t timer_delay;
    uint8_t stack_pointer; // not a real pointer, more an index, but it's not accessible from Core-8 programs

    std::vector<uint16_t> stack;

    // [X] treat as address; e.g. when register V3 holds address 0xA832, mov V3, 3 would set V3 to 3,
    // while mov [V3], 3 would set 0xA832 to 3. Also mov V2, V3 would set V2 to 0xA832,
    // while mov V2, [V3] would take the value at 0xA832 (in this case 3) and put it into V2

    void pass();                    //  ; simply blank line                                        : 0000

    void cls();                     // cls                                                         : 00E0
    void ret();                     // ret                                                         : 00EE

    void jp(addr a);                // jp a                                                        : 0aaa
    void jp(nibble x);              // jp Vx                                                       : 1x00

    void call(addr a);              // call a                                                      : 2aaa

    void se(nibble x, byte b);      // se Vx, b ; (Vx == b)                                        : 3xbb
    void se(nibble x, nibble y);    // se Vx, Vy ; (Vx == Vy)                                      : 4xy0

    void sne(nibble x, byte b);     // sne Vx, b ; (Vx != b)                                       : 5xbb
    void sne(nibble x, nibble y);   // sne Vx, Vy ; (Vx != Vy)                                     : 4xy1

    void mov(nibble x, byte b);     // mov Vx, b ; (Vx = b) | mov Vx, [y] ; (Vx = *y)              : 6xbb
    void mov(nibble x, nibble y);   // mov Vx, Vy ; (Vx = Vy)                                      : 4xy2
    void mov(nibble x);             // mov I, Vx ; (I = Vx)                                        : 1x01
    // void mov(addr a);               // mov I, a ; (I = a)                                          : 7aaa
    void mov_get_delay(nibble x);   // mov Vx, DT ; (Vx = DT)                                      : 1x02
    void mov_set_delay(nibble x);   // mov DT, Vx ; (DT = Vx)                                      : 1x03

    void store(nibble x);           // mov [I], Vx ; Stores Vx at the address in I                 : 1x04
    void load(nibble x);            // mov Vx, [I] ; Loads Vx from the address in I                : 1x05

    void get_key(nibble x);         // mov Vx, K ; waits for a key stroke and puts the key into Vx : 1x06

    void add(nibble x, byte b);     // add Vx, b ; (Vx = Vx + b)                                   : 7xbb
    void add(nibble x, nibble y);   // add Vx, Vy ; (Vx = Vx + Vy)                                 : 4xy3
    void add(nibble x);             // add I, Vx ; (I = I + Vx)                                    : 1x07

    void sub(nibble x, byte b);     // sub Vx, b ; (x = Vx - b)                                    : 8xbb
    void sub(nibble x, nibble y);   // sub Vx, Vy ; (Vx = Vx - Vy)                                 : 4xy4
    void sub(nibble x);             // sub I, Vx ; (I = I - Vx)                                    : 1x08

    void subn(nibble x, byte b);    // subn Vx, b ; (x = b - Vx)                                   : 9xbb
    void subn(nibble x, nibble y);  // subn Vx, y ; (x = y - Vx)                                   : 4xy5
    void subn(nibble x);            // subn I, Vx ; (I = Vx - I)                                   : 1x09

    void or_(nibble x, nibble y);   // or Vx, Vy ; (Vx = Vx | Vy)                                  : 4xy6
    void and_(nibble x, nibble y);  // and Vx, Vy ; (Vx = Vx & Vy)                                 : 4xy7
    void xor_(nibble x, nibble y);  // xor Vx, Vy ; (Vx = Vx ^ Vy)                                 : 4xy8

    void rnd(nibble x, byte seed);  // rnd Vx, s ; (Vx = random_byte & s)                          : Axss

    void put(byte b);               // put b ; writes a single char to stdout                      : 4aa9
    void put(nibble x);             // put Vx ; writes a single char from x to stdout              : 1x0A

    void skp(nibble x);             // skp x ; skip next instruction if key x is pressed           : 1x0B
    void sknp(nibble x);            // sknp x ; skip next instruction if key x is noz pressed      : 1x0C

public:
    CPU(uint16_t * code_start, uint16_t * code_end, int load_to = 512);
    bool instruct(uint16_t * instruction);
    int run();
};

#endif //CHIP_8_CPU_H
