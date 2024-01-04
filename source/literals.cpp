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
        std::string suffixes[] = {
            ""
        };

        // check for prefix
        if (!basic::string_contains_at(lexling.p_value, 0, instruction_prefix)) {
            return false;
        }

        // check for suffixes
        if (basic::string_contains_at(lexling.p_value, instruction_prefix.length(), "type.size")) {
            return true;
        }
        if (basic::string_contains_at(lexling.p_value, instruction_prefix.length(), "type.offset")) {
            return true;
        }
        if (basic::string_contains_at(lexling.p_value, instruction_prefix.length(), "write_register_value.size")) {
            return true;
        }
        if (basic::string_contains_at(lexling.p_value, instruction_prefix.length(), "write_register_value.offset")) {
            return true;
        }
        if (basic::string_contains_at(lexling.p_value, instruction_prefix.length(), "input_0.size")) {
            return true;
        }
        if (basic::string_contains_at(lexling.p_value, instruction_prefix.length(), "input_0.offset")) {
            return true;
        }
        if (basic::string_contains_at(lexling.p_value, instruction_prefix.length(), "input_1.size")) {
            return true;
        }
        if (basic::string_contains_at(lexling.p_value, instruction_prefix.length(), "input_1.offset")) {
            return true;
        }
        if (basic::string_contains_at(lexling.p_value, instruction_prefix.length(), "input_2.size")) {
            return true;
        }
        if (basic::string_contains_at(lexling.p_value, instruction_prefix.length(), "input_2.offset")) {
            return true;
        }
        if (basic::string_contains_at(lexling.p_value, instruction_prefix.length(), "input_3.size")) {
            return true;
        }
        if (basic::string_contains_at(lexling.p_value, instruction_prefix.length(), "input_3.offset")) {
            return true;
        }
        if (basic::string_contains_at(lexling.p_value, instruction_prefix.length(), "output_0.size")) {
            return true;
        }
        if (basic::string_contains_at(lexling.p_value, instruction_prefix.length(), "output_0.offset")) {
            return true;
        }
        if (basic::string_contains_at(lexling.p_value, instruction_prefix.length(), "output_1.size")) {
            return true;
        }
        if (basic::string_contains_at(lexling.p_value, instruction_prefix.length(), "output_1.offset")) {
            return true;
        }
        if (basic::string_contains_at(lexling.p_value, instruction_prefix.length(), "output_2.size")) {
            return true;
        }
        if (basic::string_contains_at(lexling.p_value, instruction_prefix.length(), "output_2.offset")) {
            return true;
        }
        if (basic::string_contains_at(lexling.p_value, instruction_prefix.length(), "jump_instruction_ID.size")) {
            return true;
        }
        if (basic::string_contains_at(lexling.p_value, instruction_prefix.length(), "jump_instruction_ID.offset")) {
            return true;
        }
        if (basic::string_contains_at(lexling.p_value, instruction_prefix.length(), "size")) {
            return true;
        }
        if (basic::string_contains_at(lexling.p_value, instruction_prefix.length(), "opcode.quit")) {
            return true;
        }
        if (basic::string_contains_at(lexling.p_value, instruction_prefix.length(), "opcode.write_cell")) {
            return true;
        }
        if (basic::string_contains_at(lexling.p_value, instruction_prefix.length(), "opcode.copy_cell")) {
            return true;
        }
        if (basic::string_contains_at(lexling.p_value, instruction_prefix.length(), "opcode.print_cell_as_number")) {
            return true;
        }
        if (basic::string_contains_at(lexling.p_value, instruction_prefix.length(), "opcode.print_cell_as_character")) {
            return true;
        }
        if (basic::string_contains_at(lexling.p_value, instruction_prefix.length(), "opcode.get_console_input")) {
            return true;
        }
        if (basic::string_contains_at(lexling.p_value, instruction_prefix.length(), "opcode.create_new_context")) {
            return true;
        }
        if (basic::string_contains_at(lexling.p_value, instruction_prefix.length(), "opcode.restore_old_context")) {
            return true;
        }
        if (basic::string_contains_at(lexling.p_value, instruction_prefix.length(), "opcode.pass_input")) {
            return true;
        }
        if (basic::string_contains_at(lexling.p_value, instruction_prefix.length(), "opcode.get_input")) {
            return true;
        }
        if (basic::string_contains_at(lexling.p_value, instruction_prefix.length(), "opcode.pass_output")) {
            return true;
        }
        if (basic::string_contains_at(lexling.p_value, instruction_prefix.length(), "opcode.get_output")) {
            return true;
        }
        if (basic::string_contains_at(lexling.p_value, instruction_prefix.length(), "opcode.jump_to_abstraction")) {
            return true;
        }
        if (basic::string_contains_at(lexling.p_value, instruction_prefix.length(), "opcode.jump_from_abstraction")) {
            return true;
        }
        if (basic::string_contains_at(lexling.p_value, instruction_prefix.length(), "opcode.jump")) {
            return true;
        }
        if (basic::string_contains_at(lexling.p_value, instruction_prefix.length(), "opcode.get_instruction_index")) {
            return true;
        }
        if (basic::string_contains_at(lexling.p_value, instruction_prefix.length(), "opcode.request_memory")) {
            return true;
        }
        if (basic::string_contains_at(lexling.p_value, instruction_prefix.length(), "opcode.return_memory")) {
            return true;
        }
        if (basic::string_contains_at(lexling.p_value, instruction_prefix.length(), "opcode.cell_to_address")) {
            return true;
        }
        if (basic::string_contains_at(lexling.p_value, instruction_prefix.length(), "opcode.address_to_cell")) {
            return true;
        }
        if (basic::string_contains_at(lexling.p_value, instruction_prefix.length(), "opcode.buffer_to_file")) {
            return true;
        }
        if (basic::string_contains_at(lexling.p_value, instruction_prefix.length(), "opcode.file_to_buffer")) {
            return true;
        }
        if (basic::string_contains_at(lexling.p_value, instruction_prefix.length(), "opcode.integer_add")) {
            return true;
        }
        if (basic::string_contains_at(lexling.p_value, instruction_prefix.length(), "opcode.integer_subtract")) {
            return true;
        }
        if (basic::string_contains_at(lexling.p_value, instruction_prefix.length(), "opcode.integer_multiply")) {
            return true;
        }
        if (basic::string_contains_at(lexling.p_value, instruction_prefix.length(), "opcode.integer_divide")) {
            return true;
        }
        if (basic::string_contains_at(lexling.p_value, instruction_prefix.length(), "opcode.integer_modulous")) {
            return true;
        }
        if (basic::string_contains_at(lexling.p_value, instruction_prefix.length(), "opcode.integer_within_range")) {
            return true;
        }
        if (basic::string_contains_at(lexling.p_value, instruction_prefix.length(), "opcode.boolean_not")) {
            return true;
        }
        if (basic::string_contains_at(lexling.p_value, instruction_prefix.length(), "opcode.get_context_input")) {
            return true;
        }
        if (basic::string_contains_at(lexling.p_value, instruction_prefix.length(), "opcode.pass_context_output")) {
            return true;
        }
        if (basic::string_contains_at(lexling.p_value, instruction_prefix.length(), "opcode.run")) {
            return true;
        }

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
