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

    // power function
    u64 power(u64 base, u64 exponent) {
        u64 output = 1;

        if (exponent == 0) {
            return output;
        }

        while (exponent > 0) {
            output *= base;

            exponent--;
        }

        return output;
    }

    // convert an integer literal to a binary integer
    u64 convert_integer_literal_to_binary_integer(std::string string, bool& error) {
        u64 output = 0;
        u64 digit = 0;

        // check for blank string
        if (string == "") {
            // setup error
            error = true;

            return 0;
        }

        // pre check for all valid characters
        for (u64 character = 0; character < string.length(); character++) {
            // check character
            if (((string[character] >= '0' && string[character] <= '9') || string[character] == '_' || (character == 0 && string[character] == 'n')) == false) {
                // set error
                error = true;

                // return invalid
                return 0;
            }
        }

        // translate the number
        // if the number is negative
        if (string[0] == 'n') {
            // TODO
            error = true;
            return 0;
        // if the number is positive
        } else {
            // for each character
            for (u64 character = string.length(); character > 0; character--) {
                // go past dead space
                while (character > 0 && string[character - 1] == '_') {
                    // DEBUG
                    printf("Skipping Underscore!\n");

                    character--;
                }

                // check for end of buffer
                if (character == 0) {
                    break;
                }

                // apply digit
                output += power(10, digit) * (u64)(string[character - 1] - '0');

                // next digit
                digit++;
            }

            return output;
        }
    }
}
