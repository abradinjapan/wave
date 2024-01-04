#pragma once

#include "basic.cpp"
#include "lex.cpp"

/* Code */
namespace literals {
    bool string_is_integer_literal(lexer::lexling& lexling) {
        bool output = false;
        bool error;
        std::string prefix = "wave.integer.";

        // check for the start of the literal
        if (basic::string_contains_at(lexling.p_value, 0, prefix) == false) {
            return output;
        }

        // convert integer literal to binary integer
        basic::convert_integer_literal_to_binary_integer(lexling.p_value.substr(prefix.length()), error);
        output = !error;

        return output;
    }

    bool string_is_boolean_literal(lexer::lexling& lexling) {
        return (lexling.p_value == "wave.boolean.true" || lexling.p_value == "wave.boolean.false");
    }

    bool string_is_instruction_literal(lexer::lexling& lexling) {
        std::string instruction_prefix = "wave.instruction.";
        std::vector<std::string> suffixes = {
            "type.size",
            "type.offset",
            "write_register_value.size",
            "write_register_value.offset",
            "input_0.size",
            "input_0.offset",
            "input_1.size",
            "input_1.offset",
            "input_2.size",
            "input_2.offset",
            "input_3.size",
            "input_3.offset",
            "output_0.size",
            "output_0.offset",
            "output_1.size",
            "output_1.offset",
            "output_2.size",
            "output_2.offset",
            "jump_instruction_ID.size",
            "jump_instruction_ID.offset",
            "size",
            "opcode.quit",
            "opcode.write_cell",
            "opcode.copy_cell",
            "opcode.print_cell_as_number",
            "opcode.print_cell_as_character",
            "opcode.get_console_input",
            "opcode.create_new_context",
            "opcode.restore_old_context",
            "opcode.pass_input",
            "opcode.get_input",
            "opcode.pass_output",
            "opcode.get_output",
            "opcode.jump_to_abstraction",
            "opcode.jump_from_abstraction",
            "opcode.jump",
            "opcode.get_instruction_index",
            "opcode.request_memory",
            "opcode.return_memory",
            "opcode.cell_to_address",
            "opcode.address_to_cell",
            "opcode.buffer_to_file",
            "opcode.file_to_buffer",
            "opcode.integer_add",
            "opcode.integer_subtract",
            "opcode.integer_multiply",
            "opcode.integer_divide",
            "opcode.integer_modulous",
            "opcode.integer_within_range",
            "opcode.boolean_not",
            "opcode.get_context_input",
            "opcode.pass_context_output",
            "opcode.run",
        };

        // check for prefix
        if (!basic::string_contains_at(lexling.p_value, 0, instruction_prefix)) {
            return false;
        }

        // check for suffixes
        for (basic::u64 suffix = 0; suffix < suffixes.size(); suffix++) {
            // check suffix
            if (basic::string_contains_at(lexling.p_value, instruction_prefix.length(), suffixes[suffix])) {
                // if found, return true
                return true;
            }
        }

        // suffix not found
        return false;
    }

    bool string_is_hexadecimal_literal(lexer::lexling& lexling) {
        std::string prefix = "wave.hexadecimal.";
        bool error;

        // check for prefix
        if (basic::string_contains_at(lexling.p_value, 0, prefix) == false) {
            return false;
        }

        // attempt conversion
        basic::convert_hexadecimal_literal_to_binary_integer(lexling.p_value.substr(prefix.length()), error);

        // return findings
        return !error;
    }
}
