#pragma once

#include "basic.cpp"
#include "lex.cpp"
#include "run.cpp"

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

    std::vector<std::string> get_all_instruction_literal_suffixes() {
        std::vector<std::string> suffixes = {
            "opcode.size",
            "opcode.offset",
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
            "opcode.load_string",
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
            "opcode.compile",
        };

        return suffixes;
    }

    std::vector<basic::u64> get_all_instruction_literal_values() {
        std::vector<basic::u64> values = {
            sizeof(runner::instruction::p_opcode),
            offsetof(runner::instruction, runner::instruction::p_opcode),
            sizeof(runner::instruction::p_write_register_value),
            offsetof(runner::instruction, runner::instruction::p_write_register_value),
            sizeof(runner::instruction::p_input_0),
            offsetof(runner::instruction, runner::instruction::p_input_0),
            sizeof(runner::instruction::p_input_1),
            offsetof(runner::instruction, runner::instruction::p_input_1),
            sizeof(runner::instruction::p_input_2),
            offsetof(runner::instruction, runner::instruction::p_input_2),
            sizeof(runner::instruction::p_input_3),
            offsetof(runner::instruction, runner::instruction::p_input_3),
            sizeof(runner::instruction::p_output_0),
            offsetof(runner::instruction, runner::instruction::p_output_0),
            sizeof(runner::instruction::p_output_1),
            offsetof(runner::instruction, runner::instruction::p_output_1),
            sizeof(runner::instruction::p_output_2),
            offsetof(runner::instruction, runner::instruction::p_output_2),
            sizeof(runner::instruction::p_jump_instruction_ID),
            offsetof(runner::instruction, runner::instruction::p_jump_instruction_ID),
            sizeof(runner::instruction),
            (basic::u64)runner::opcode::quit,
            (basic::u64)runner::opcode::write_cell,
            (basic::u64)runner::opcode::copy_cell,
            (basic::u64)runner::opcode::load_string,
            (basic::u64)runner::opcode::print_cell_as_number,
            (basic::u64)runner::opcode::print_cell_as_character,
            (basic::u64)runner::opcode::get_console_input,
            (basic::u64)runner::opcode::create_new_context,
            (basic::u64)runner::opcode::restore_old_context,
            (basic::u64)runner::opcode::pass_input,
            (basic::u64)runner::opcode::get_input,
            (basic::u64)runner::opcode::pass_output,
            (basic::u64)runner::opcode::get_output,
            (basic::u64)runner::opcode::jump_to_abstraction,
            (basic::u64)runner::opcode::jump_from_abstraction,
            (basic::u64)runner::opcode::jump,
            (basic::u64)runner::opcode::get_instruction_index,
            (basic::u64)runner::opcode::request_memory,
            (basic::u64)runner::opcode::return_memory,
            (basic::u64)runner::opcode::cell_to_address,
            (basic::u64)runner::opcode::address_to_cell,
            (basic::u64)runner::opcode::buffer_to_file,
            (basic::u64)runner::opcode::file_to_buffer,
            (basic::u64)runner::opcode::integer_add,
            (basic::u64)runner::opcode::integer_subtract,
            (basic::u64)runner::opcode::integer_multiply,
            (basic::u64)runner::opcode::integer_divide,
            (basic::u64)runner::opcode::integer_modulous,
            (basic::u64)runner::opcode::integer_within_range,
            (basic::u64)runner::opcode::boolean_not,
            (basic::u64)runner::opcode::get_context_input,
            (basic::u64)runner::opcode::pass_context_output,
            (basic::u64)runner::opcode::run,
            (basic::u64)runner::opcode::compile,
        };

        return values;
    }

    basic::u64 convert_instruction_literal_suffix_to_value(std::string suffix) {
        std::vector<std::string> suffixes = get_all_instruction_literal_suffixes();
        std::vector<basic::u64> values = get_all_instruction_literal_values();

        // sanity check
        if (suffixes.size() != values.size()) {
            // error
            std::cout << "Internal Error, value and suffix count did not match!" << std::endl;

            return -1;
        }

        // search for suffix
        for (basic::u64 i = 0; i < suffixes.size(); i++) {
            // check suffix
            if (suffixes[i] == suffix) {
                // found value
                return values[i];
            }
        }

        // suffix not found
        return -1;
    }

    bool string_is_instruction_literal(lexer::lexling& lexling) {
        std::string instruction_prefix = "wave.instruction.";
        std::vector<std::string> suffixes = get_all_instruction_literal_suffixes();

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
            // does not contain prefix
            return false;
        }

        // attempt conversion
        basic::convert_hexadecimal_literal_to_binary_integer(lexling.p_value.substr(prefix.length()), error);

        // return findings
        return !error;
    }

    bool string_is_binary_literal(lexer::lexling& lexling) {
        std::string prefix = "wave.binary.";
        bool error;

        // check for prefix
        if (basic::string_contains_at(lexling.p_value, 0, prefix) == false) {
            // does not contain prefix
            return false;
        }

        // attempt conversion
        basic::convert_binary_literal_to_binary_integer(lexling.p_value.substr(prefix.length()), error);

        // return findings
        return !error;
    }
}
