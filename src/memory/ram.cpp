//
// Created by Nik Wipper on 2019-07-24.
//

#include "ram.h"

#include <cstdlib>

RAM::RAM(int size)
{
    this->size = size;
    this->at = malloc(this->size);
}

void * RAM::get(int addr)
{
    if (addr > this->size)
        return NULL;
    return (int *) this->at + addr;
}

int RAM::resolve(void * addr)
{
    return (uintptr_t)this->at - (uintptr_t)addr;
}
