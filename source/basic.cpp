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
            // for each character
            for (u64 character = string.length(); character > 1; character--) {
                // go past dead space
                while (character > 1 && string[character - 1] == '_') {
                    // next character
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

            // adjust for twos compliment
            output = ~output;
            output++;

            return output;
        // if the number is positive
        } else {
            // for each character
            for (u64 character = string.length(); character > 0; character--) {
                // go past dead space
                while (character > 0 && string[character - 1] == '_') {
                    // next character
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

    u64 convert_hexadecimal_digit_to_binary(basic::character character, bool& error) {
        if (character >= '0' && character <= '9') {
            return character - '0';
        } else if (character >= 'a' && character <= 'f') {
            return character - 'a' + 10;
        } else if (character >= 'A' && character <= 'F') {
            return character - 'A' + 10;
        } else {
            error = true;
            return 0;
        }
    }

    u64 convert_hexadecimal_literal_to_binary_integer(std::string string, bool& error) {
        u64 output = 0;

        // check for blank string
        if (string == "") {
            // setup error
            error = true;

            return 0;
        }

        // pre check for all valid characters
        for (u64 character = 0; character < string.length(); character++) {
            // check character
            if (((string[character] >= '0' && string[character] <= '9') || (string[character] >= 'a' && string[character] <= 'f') || (string[character] >= 'A' && string[character] <= 'F') || string[character] == '_') == false) {
                // set error
                error = true;

                // return invalid
                return 0;
            }
        }

        // translate number
        // for each character
        for (u64 character = 0; character < string.length(); character++) {
            // skip dead space
            while (character < string.length() && string[character] == '_') {
                // next character
                character++;
            }

            // check for end of buffer
            if (character >= string.length()) {
                break;
            }

            // translate character
            u64 value = convert_hexadecimal_digit_to_binary(string[character], error);

            // write value to output
            output <<= 4;
            output &= ~15;
            output |= value;
        }

        return output;
    }
}
