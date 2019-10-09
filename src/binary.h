//
// Created by Nikolas Wipper on 2019-07-24.
//

#ifndef CHIP_8_BINARY_H
#define CHIP_8_BINARY_H

#include <cstdint>

// This is a fragment of my attempt to load byte code from a separate file
// It defines the header for the separate file which stores where the code starts and where certain libraries should be loaded

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
