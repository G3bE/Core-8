//
// Created by Nikolas Wipper on 2019-07-24.
//

#ifndef CHIP_8_BINARY_H
#define CHIP_8_BINARY_H

#include <stdint.h>

typedef struct
{
    char name[51];
    char hash[33];
    uint32_t copy; // in virtual ram
} linker_entry;

typedef struct
{
    uint8_t version_mayor;
    uint8_t version_minor;

    uint8_t type; // 0 executable, 1 library

    uint16_t linker_table_size;
    linker_entry * first_entry; // offset

    uint16_t * code_start; // offset
    uint16_t * code_end; // offset
} header;

#endif //CHIP_8_BINARY_H
