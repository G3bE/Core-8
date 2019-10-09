//
// Created by Nik Wipper on 2019-07-24.
//

#ifndef CHIP_8_RAM_H
#define CHIP_8_RAM_H

class RAM {
    void * at;
    int size; // in bytes
public:
    RAM(int size);
    void * get(int addr);
    int resolve(void * addr);
};

#endif //CHIP_8_RAM_H
