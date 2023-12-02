#pragma once

/* Include */
#include <string>
#include <stdint.h>
#include <unistd.h>

/* Define */
#define define__current_working_directory_buffer_size 2048

/* Code */
namespace basic {
    typedef uint8_t u8;
    typedef uint16_t u16;
    typedef uint32_t u32;
    typedef uint64_t u64;
    typedef int8_t s8;
    typedef int16_t s16;
    typedef int32_t s32;
    typedef int64_t s64;

    typedef void* address;

    typedef char character;

    // get a current working directory
    std::string get_current_working_directory() {
        char buffer[define__current_working_directory_buffer_size];

        // initialize buffer
        for (uint64_t i = 0; i < define__current_working_directory_buffer_size; i++) {
            // initialize character
            buffer[i] = 0;
        }

        // get data
        getcwd(buffer, define__current_working_directory_buffer_size);

        // change to string
        std::string path = buffer;

        // return string
        return path;
    }
}
