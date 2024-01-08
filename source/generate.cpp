#pragma once

#include <vector>

#include "account.cpp"
#include "run.cpp"
#include "basic.cpp"

namespace generator {
    class offset {
    public:
        basic::u64 p_instruction_ID;

        offset() {
            p_instruction_ID = 0;
        }

        offset(int instruction_ID) {
            p_instruction_ID = instruction_ID;
        }
    };

    class offsets {
    public:
        offset p_start;
        std::vector<offset> p_code_defined_offsets;
    };

    enum pass_type {
        pass_measure,
        pass_build,
    };

    class workspace {
    public:
        runner::program p_program;
        std::vector<offsets> p_abstraction_offsets;
        basic::u64 p_instruction_count;
        pass_type p_pass_type;

        void start_pass_measure(basic::u64 abstraction_count) {
            p_abstraction_offsets.resize(abstraction_count);
            p_pass_type = pass_type::pass_measure;
            p_instruction_count = 0;
            p_program.p_strings.clear();
        }

        void finish_pass_measure() {
            p_program.p_instructions.resize(p_instruction_count);
        }

        void start_pass_build() {
            p_program.p_strings.clear();
            p_instruction_count = 0;
            p_pass_type = pass_type::pass_build;
        }

        void finish_pass_build() {
            return;
        }

        basic::u64 get_offset() {
            return p_instruction_count;
        }
    };

    namespace write_instructions {
        void write__any_instruction(workspace& workspace, runner::opcode opcode, runner::cell write_register_value, runner::cell_ID input_0, runner::cell_ID input_1, runner::cell_ID input_2, runner::cell_ID input_3, runner::cell_ID output_0, runner::cell_ID output_1, runner::cell_ID output_2, runner::instruction_ID jump_instruction_ID) {
            runner::instruction temp;

            // create instruction
            if (workspace.p_pass_type == pass_type::pass_build) {
                // set data
                temp.p_opcode = opcode;
                temp.p_write_register_value = write_register_value;
                temp.p_input_0 = input_0;
                temp.p_input_1 = input_1;
                temp.p_input_2 = input_2;
                temp.p_input_3 = input_3;
                temp.p_output_0 = output_0;
                temp.p_output_1 = output_1;
                temp.p_output_2 = output_2;
                temp.p_jump_instruction_ID = jump_instruction_ID;

                // write instruction
                workspace.p_program.p_instructions[workspace.p_instruction_count] = temp;
            }

            // next instruction
            workspace.p_instruction_count++;

            return;
        }

        void write__quit(workspace& workspace) {
            write__any_instruction(workspace, runner::opcode::quit, 0, parameter_unused, parameter_unused, parameter_unused, parameter_unused, parameter_unused, parameter_unused, parameter_unused, parameter_unused);

            return;
        }

        void write__write_cell(workspace& workspace, runner::cell value, runner::cell_ID destination) {
            write__any_instruction(workspace, runner::opcode::write_cell, value, parameter_unused, parameter_unused, parameter_unused, parameter_unused, destination, parameter_unused, parameter_unused, parameter_unused);

            return;
        }

        void write__copy_cell(workspace& workspace, runner::cell_ID source, runner::cell_ID destination) {
            write__any_instruction(workspace, runner::opcode::copy_cell, parameter_unused, source, parameter_unused, parameter_unused, parameter_unused, destination, parameter_unused, parameter_unused, parameter_unused);

            return;
        }

        void write__load_string(workspace& workspace, runner::cell_ID string_ID, runner::cell_ID destination_start, runner::cell_ID destination_end, runner::cell_ID error) {
            write__any_instruction(workspace, runner::opcode::load_string, parameter_unused, string_ID, parameter_unused, parameter_unused, parameter_unused, destination_start, destination_end, error, parameter_unused);

            return;
        }

        void write__print_cell_as_number(workspace& workspace, runner::cell_ID source) {
            write__any_instruction(workspace, runner::opcode::print_cell_as_number, parameter_unused, source, parameter_unused, parameter_unused, parameter_unused, parameter_unused, parameter_unused, parameter_unused, parameter_unused);

            return;
        }

        void write__print_cell_as_character(workspace& workspace, runner::cell_ID source) {
            write__any_instruction(workspace, runner::opcode::print_cell_as_character, parameter_unused, source, parameter_unused, parameter_unused, parameter_unused, parameter_unused, parameter_unused, parameter_unused, parameter_unused);

            return;
        }

        void write__get_console_input(workspace& workspace, runner::cell_ID start, runner::cell_ID end) {
            write__any_instruction(workspace, runner::opcode::get_console_input, parameter_unused, parameter_unused, parameter_unused, parameter_unused, parameter_unused, start, end, parameter_unused, parameter_unused);

            return;
        }

        void write__create_new_context(workspace& workspace, runner::cell cell_count) {
            write__any_instruction(workspace, runner::opcode::create_new_context, cell_count, parameter_unused, parameter_unused, parameter_unused, parameter_unused, parameter_unused, parameter_unused, parameter_unused, parameter_unused);

            return;
        }

        void write__restore_old_context(workspace& workspace) {
            write__any_instruction(workspace, runner::opcode::restore_old_context, parameter_unused, parameter_unused, parameter_unused, parameter_unused, parameter_unused, parameter_unused, parameter_unused, parameter_unused, parameter_unused);

            return;
        }

        void write__pass_input(workspace& workspace, runner::cell_ID parameter) {
            write__any_instruction(workspace, runner::opcode::pass_input, parameter_unused, parameter, parameter_unused, parameter_unused, parameter_unused, parameter_unused, parameter_unused, parameter_unused, parameter_unused);

            return;
        }

        void write__get_input(workspace& workspace, runner::cell_ID parameter) {
            write__any_instruction(workspace, runner::opcode::get_input, parameter_unused, parameter_unused, parameter_unused, parameter_unused, parameter_unused, parameter, parameter_unused, parameter_unused, parameter_unused);

            return;
        }

        void write__pass_output(workspace& workspace, runner::cell_ID parameter) {
            write__any_instruction(workspace, runner::opcode::pass_output, parameter_unused, parameter, parameter_unused, parameter_unused, parameter_unused, parameter_unused, parameter_unused, parameter_unused, parameter_unused);

            return;
        }

        void write__get_output(workspace& workspace, runner::cell_ID parameter) {
            write__any_instruction(workspace, runner::opcode::get_output, parameter_unused, parameter_unused, parameter_unused, parameter_unused, parameter_unused, parameter, parameter_unused, parameter_unused, parameter_unused);

            return;
        }

        void write__jump_to_abstraction(workspace& workspace, runner::cell_ID destination_instruction) {
            write__any_instruction(workspace, runner::opcode::jump_to_abstraction, parameter_unused, destination_instruction, parameter_unused, parameter_unused, parameter_unused, parameter_unused, parameter_unused, parameter_unused, parameter_unused);

            return;
        }

        void write__jump_from_abstraction(workspace& workspace) {
            write__any_instruction(workspace, runner::opcode::jump_from_abstraction, parameter_unused, parameter_unused, parameter_unused, parameter_unused, parameter_unused, parameter_unused, parameter_unused, parameter_unused, parameter_unused);

            return;
        }

        void write__jump(workspace& workspace, runner::cell_ID condition, basic::u64 jump_instruction_ID) {
            write__any_instruction(workspace, runner::opcode::jump, parameter_unused, condition, parameter_unused, parameter_unused, parameter_unused, parameter_unused, parameter_unused, parameter_unused, jump_instruction_ID);

            return;
        }

        void write__get_instruction_index(workspace& workspace, runner::cell_ID destination) {
            write__any_instruction(workspace, runner::opcode::get_instruction_index, parameter_unused, parameter_unused, parameter_unused, parameter_unused, parameter_unused, destination, parameter_unused, parameter_unused, parameter_unused);

            return;
        }

        void write__request_memory(workspace& workspace, runner::cell_ID length, runner::cell_ID start_address, runner::cell_ID end_address, runner::cell_ID error) {
            write__any_instruction(workspace, runner::opcode::request_memory, parameter_unused, length, parameter_unused, parameter_unused, parameter_unused, start_address, end_address, error, parameter_unused);

            return;
        }

        void write__return_memory(workspace& workspace, runner::cell_ID start_address, runner::cell_ID end_address) {
            write__any_instruction(workspace, runner::opcode::return_memory, parameter_unused, start_address, end_address, parameter_unused, parameter_unused, parameter_unused, parameter_unused, parameter_unused, parameter_unused);

            return;
        }

        void write__cell_to_address(workspace& workspace, runner::cell_ID source, runner::cell_ID byte_size, runner::cell_ID destination, runner::cell_ID error_code) {
            write__any_instruction(workspace, runner::opcode::cell_to_address, parameter_unused, source, byte_size, destination, parameter_unused, error_code, parameter_unused, parameter_unused, parameter_unused);

            return;
        }

        void write__address_to_cell(workspace& workspace, runner::cell_ID source, runner::cell_ID byte_size, runner::cell_ID destination, runner::cell_ID error_code) {
            write__any_instruction(workspace, runner::opcode::address_to_cell, parameter_unused, source, byte_size, parameter_unused, parameter_unused, destination, error_code, parameter_unused, parameter_unused);

            return;
        }

        void write__buffer_to_file(workspace& workspace, runner::cell_ID source_start, runner::cell_ID source_end, runner::cell_ID file_path_start, runner::cell_ID file_path_end, runner::cell_ID error_code) {
            write__any_instruction(workspace, runner::opcode::buffer_to_file, parameter_unused, source_start, source_end, file_path_start, file_path_end, error_code, parameter_unused, parameter_unused, parameter_unused);

            return;
        }

        void write__file_to_buffer(workspace& workspace, runner::cell_ID file_path_start, runner::cell_ID file_path_end, runner::cell_ID source_start, runner::cell_ID source_end, runner::cell_ID error_code) {
            write__any_instruction(workspace, runner::opcode::file_to_buffer, parameter_unused, file_path_start, file_path_end, parameter_unused, parameter_unused, source_start, source_end, error_code, parameter_unused);

            return;
        }

        void write__copy_buffer(workspace& workspace, runner::cell_ID source_start, runner::cell_ID source_end, runner::cell_ID destination_start, runner::cell_ID destination_end, runner::cell_ID buffer_length_error, runner::cell_ID buffer_addresses_error) {
            write__any_instruction(workspace, runner::opcode::copy_buffer, parameter_unused, source_start, source_end, destination_start, destination_end, buffer_length_error, buffer_addresses_error, parameter_unused, parameter_unused);

            return;
        }

        void write__integer_add(workspace& workspace, runner::cell_ID source_1, runner::cell_ID source_2, runner::cell_ID destination) {
            write__any_instruction(workspace, runner::opcode::integer_add, parameter_unused, source_1, source_2, parameter_unused, parameter_unused, destination, parameter_unused, parameter_unused, parameter_unused);

            return;
        }

        void write__integer_subtract(workspace& workspace, runner::cell_ID source_1, runner::cell_ID source_2, runner::cell_ID destination) {
            write__any_instruction(workspace, runner::opcode::integer_subtract, parameter_unused, source_1, source_2, parameter_unused, parameter_unused, destination, parameter_unused, parameter_unused, parameter_unused);

            return;
        }

        void write__integer_multiply(workspace& workspace, runner::cell_ID source_1, runner::cell_ID source_2, runner::cell_ID destination) {
            write__any_instruction(workspace, runner::opcode::integer_multiply, parameter_unused, source_1, source_2, parameter_unused, parameter_unused, destination, parameter_unused, parameter_unused, parameter_unused);

            return;
        }

        void write__integer_divide(workspace& workspace, runner::cell_ID source_1, runner::cell_ID source_2, runner::cell_ID destination, runner::cell_ID error_code) {
            write__any_instruction(workspace, runner::opcode::integer_divide, parameter_unused, source_1, source_2, parameter_unused, parameter_unused, destination, error_code, parameter_unused, parameter_unused);

            return;
        }

        void write__integer_modulous(workspace& workspace, runner::cell_ID source_1, runner::cell_ID source_2, runner::cell_ID destination, runner::cell_ID error_code) {
            write__any_instruction(workspace, runner::opcode::integer_modulous, parameter_unused, source_1, source_2, parameter_unused, parameter_unused, destination, error_code, parameter_unused, parameter_unused);

            return;
        }

        void write__integer_within_range(workspace& workspace, runner::cell_ID low_bound, runner::cell_ID value, runner::cell_ID high_bound, runner::cell_ID destination) {
            write__any_instruction(workspace, runner::opcode::integer_within_range, parameter_unused, low_bound, value, high_bound, parameter_unused, destination, parameter_unused, parameter_unused, parameter_unused);

            return;
        }

        void write__boolean_not(workspace& workspace, runner::cell_ID source, runner::cell_ID destination) {
            write__any_instruction(workspace, runner::opcode::boolean_not, parameter_unused, source, parameter_unused, parameter_unused, parameter_unused, destination, parameter_unused, parameter_unused, parameter_unused);

            return;
        }

        void write__get_context_input(workspace& workspace, runner::cell_ID destination_start, runner::cell_ID destination_end) {
            write__any_instruction(workspace, runner::opcode::get_context_input, parameter_unused, parameter_unused, parameter_unused, parameter_unused, parameter_unused, destination_start, destination_end, parameter_unused, parameter_unused);

            return;
        }

        void write__pass_context_output(workspace& workspace, runner::cell_ID source_start, runner::cell_ID source_end) {
            write__any_instruction(workspace, runner::opcode::pass_context_output, parameter_unused, source_start, source_end, parameter_unused, parameter_unused, parameter_unused, parameter_unused, parameter_unused, parameter_unused);

            return;
        }

        void write__run(workspace& workspace, runner::cell_ID program_start, runner::cell_ID program_end, runner::cell_ID input_start, runner::cell_ID input_end, runner::cell_ID result_start, runner::cell_ID result_end, runner::cell_ID runner_error_occured) {
            write__any_instruction(workspace, runner::opcode::run, parameter_unused, program_start, program_end, input_start, input_end, result_start, result_end, runner_error_occured, parameter_unused);

            return;
        }
    }

    basic::u64 calculate_variable_index(accounter::skeleton::argument argument, accounter::skeleton::abstraction& abstraction) {
        switch (argument.p_type) {
        case accounter::skeleton::argument_type::is_input:
            return argument.p_ID;
        case accounter::skeleton::argument_type::is_output:
            return abstraction.p_variables.p_inputs.size() + argument.p_ID;
        case accounter::skeleton::argument_type::is_variable:
            return abstraction.p_variables.p_inputs.size() + abstraction.p_variables.p_outputs.size() + argument.p_ID;
        // offset is used for storing the temporary instruction number for function calls
        case accounter::skeleton::argument_type::is_offset:
            return abstraction.p_variables.p_inputs.size() + abstraction.p_variables.p_outputs.size() + abstraction.p_variables.p_variables.size();
        default:
            return abstraction.p_variables.p_inputs.size() + abstraction.p_variables.p_outputs.size() + abstraction.p_variables.p_variables.size() + 1;
        }
    }

    void generate_abstraction(workspace& workspace, accounter::skeleton::abstraction& abstraction, basic::u64 abstraction_ID, bool& error_occured) {
        basic::u64 variable_count;

        // determine variable count
        variable_count = calculate_variable_index(accounter::skeleton::argument(accounter::skeleton::argument_type::is_invalid, 0), abstraction);

        // setup offsets container
        if (workspace.p_pass_type == pass_type::pass_measure) {
            workspace.p_abstraction_offsets[abstraction_ID].p_code_defined_offsets.resize(abstraction.p_offsets.p_offsets.size());
        
            // setup function start offset
            workspace.p_abstraction_offsets[abstraction_ID].p_start = workspace.get_offset();
        }

        // generate function prologue
        // create context
        write_instructions::write__create_new_context(workspace, variable_count);

        // get inputs
        for (basic::u64 input_ID = abstraction.p_variables.p_inputs.size(); input_ID > 0; input_ID--) {
            // get input
            write_instructions::write__get_input(workspace, calculate_variable_index(accounter::skeleton::argument(accounter::skeleton::argument_type::is_input, input_ID - 1), abstraction));
        }

        // generate function body
        for (basic::u64 statement_ID = 0; statement_ID < abstraction.p_statement_map.size(); statement_ID++) {
            // generate statement
            if (abstraction.p_statement_map[statement_ID].p_type == accounter::skeleton::statement_type::is_call_statement) {
                // write code (NOTE: make sure that the switch cases are aligned with their respective run.cpp instructions!)
                switch (abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_header_ID) {
                // wave.quit(0)(0)
                case runner::opcode::quit:
                    // write code
                    write_instructions::write__quit(workspace);

                    break;
                // wave.write_cell(1)(1)
                case runner::opcode::write_cell:
                    // determine type of input
                    if (abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[0].p_type == accounter::skeleton::argument_type::is_integer_literal || abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[0].p_type == accounter::skeleton::argument_type::is_boolean_literal || abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[0].p_type == accounter::skeleton::argument_type::is_instruction_literal || abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[0].p_type == accounter::skeleton::argument_type::is_hexadecimal_literal || abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[0].p_type == accounter::skeleton::argument_type::is_binary_literal) {
                        // constant is an integer literal, write code
                        write_instructions::write__write_cell(workspace, abstraction.p_literals.p_literals[abstraction.lookup_literal_by_ID(statement_ID, 0, error_occured).p_ID].p_integer_value, calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_outputs[0], abstraction));
                    } else if (abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[0].p_type == accounter::skeleton::argument_type::is_offset) {
                        // if pass is measure
                        if (workspace.p_pass_type == pass_type::pass_measure) {
                            // write dummy instruction
                            write_instructions::write__write_cell(workspace, 0, calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_outputs[0], abstraction));
                        // if pass is build
                        } else {
                            // constant is an offset, write code
                            write_instructions::write__write_cell(workspace, workspace.p_abstraction_offsets[abstraction_ID].p_code_defined_offsets[abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[0].p_ID].p_instruction_ID, calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_outputs[0], abstraction));
                        }
                    } else {
                        // set error
                        error_occured = true;

                        // print error message
                        printf("Generation error, set has an invalid argument.\n");

                        return;
                    }

                    break;
                // wave.copy(1)(1)
                case runner::opcode::copy_cell:
                    // write code
                    write_instructions::write__copy_cell(workspace, calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[0], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_outputs[0], abstraction));

                    break;
                // wave.copy.string(1)(3)
                case runner::opcode::load_string:
                    // append string to program string list
                    workspace.p_program.p_strings.push_back(abstraction.p_literals.p_literals[abstraction.lookup_literal_by_ID(statement_ID, 0, error_occured).p_ID].p_string_value);

                    // write code
                    write_instructions::write__load_string(workspace, workspace.p_program.p_strings.size() - 1, calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_outputs[0], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_outputs[1], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_outputs[2], abstraction));

                    break;
                // wave.print_cell_as_number(1)(0)
                case runner::opcode::print_cell_as_number:
                    // write code
                    write_instructions::write__print_cell_as_number(workspace, calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[0], abstraction));

                    break;
                // wave.print_cell_as_character(1)(0)
                case runner::opcode::print_cell_as_character:
                    // write code
                    write_instructions::write__print_cell_as_character(workspace, calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[0], abstraction));

                    break;
                // wave.get_console_input(0)(2)
                case runner::opcode::get_console_input:
                    // write code
                    write_instructions::write__get_console_input(workspace, calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_outputs[0], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_outputs[1], abstraction));

                    break;
                // wave.create_new_context(1)(0)
                case runner::opcode::create_new_context:
                    // set error
                    error_occured = true;

                    // print error
                    std::cout << "Generation Error: Although create_new_context is a valid instruction, it is not available for general use." << std::endl;

                    break;
                // wave.restore_old_context(0)(0)
                case runner::opcode::restore_old_context:
                    // set error
                    error_occured = true;

                    // print error
                    std::cout << "Generation Error: Although restore_old_context is a valid instruction, it is not available for general use." << std::endl;

                    break;
                // wave.pass_input(1)(0)
                case runner::opcode::pass_input:
                    // set error
                    error_occured = true;

                    // print error
                    std::cout << "Generation Error: Although pass_input is a valid instruction, it is not available for general use." << std::endl;

                    break;
                // wave.get_input(0)(1)
                case runner::opcode::get_input:
                    // set error
                    error_occured = true;

                    // print error
                    std::cout << "Generation Error: Although get_input is a valid instruction, it is not available for general use." << std::endl;

                    break;
                // wave.pass_output(1)(0)
                case runner::opcode::pass_output:
                    // set error
                    error_occured = true;

                    // print error
                    std::cout << "Generation Error: Although pass_output is a valid instruction, it is not available for general use." << std::endl;

                    break;
                // wave.get_output(0)(1)
                case runner::opcode::get_output:
                    // set error
                    error_occured = true;

                    // print error
                    std::cout << "Generation Error: Although get_output is a valid instruction, it is not available for general use." << std::endl;

                    break;
                // wave.jump_to_abstraction(1)(0)
                case runner::opcode::jump_to_abstraction:
                    // set error
                    error_occured = true;

                    // print error
                    std::cout << "Generation Error: Although jump_to_abstraction is a valid instruction, it is not available for general use." << std::endl;

                    break;
                // wave.jump_from_abstraction(0)(0)
                case runner::opcode::jump_from_abstraction:
                    // set error
                    error_occured = true;

                    // print error
                    std::cout << "Generation Error: Although jump_from_abstraction is a valid instruction, it is not available for general use." << std::endl;

                    break;
                // wave.jump(2)(0)
                case runner::opcode::jump:
                    // if measure pass
                    if (workspace.p_pass_type == pass_type::pass_measure) {
                        // write dummy code
                        write_instructions::write__jump(workspace, calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[0], abstraction), 0);
                    // if build pass
                    } else {
                        // write actual code
                        write_instructions::write__jump(workspace, calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[0], abstraction), workspace.p_abstraction_offsets[abstraction_ID].p_code_defined_offsets[abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[1].p_ID].p_instruction_ID);
                    }

                    break;
                // wave.get_instruction_index(0)(1)
                case runner::opcode::get_instruction_index:
                    // write code
                    write_instructions::write__get_instruction_index(workspace, calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_outputs[0], abstraction));

                    break;
                // wave.request_memory(1)(3)
                case runner::opcode::request_memory:
                    // write code
                    write_instructions::write__request_memory(workspace, calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[0], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_outputs[0], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_outputs[1], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_outputs[2], abstraction));

                    break;
                // wave.return_memory(1)(0)
                case runner::opcode::return_memory:
                    // write code
                    write_instructions::write__return_memory(workspace, calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[0], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[1], abstraction));

                    break;
                // wave.cell_to_address(3)(1)
                case runner::opcode::cell_to_address:
                    // write code
                    write_instructions::write__cell_to_address(workspace, calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[0], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[1], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[2], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_outputs[0], abstraction));

                    break;
                // wave.address_to_cell(2)(2)
                case runner::opcode::address_to_cell:
                    // write code
                    write_instructions::write__address_to_cell(workspace, calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[0], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[1], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_outputs[0], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_outputs[1], abstraction));

                    break;
                // wave.buffer_to_file(4)(1)
                case runner::opcode::buffer_to_file:
                    // write code
                    write_instructions::write__buffer_to_file(workspace, calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[0], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[1], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[2], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[3], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_outputs[0], abstraction));

                    break;
                // wave.file_to_buffer(2)(3)
                case runner::opcode::file_to_buffer:
                    // write code
                    write_instructions::write__file_to_buffer(workspace, calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[0], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[1], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_outputs[0], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_outputs[1], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_outputs[2], abstraction));

                    break;
                // wave.copy.buffer(4)(2)
                case runner::opcode::copy_buffer:
                    // write code
                    write_instructions::write__copy_buffer(workspace, calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[0], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[1], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[2], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[3], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_outputs[0], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_outputs[1], abstraction));

                    break;
                // wave.integer.add(2)(1)
                case runner::opcode::integer_add:
                    // write code
                    write_instructions::write__integer_add(workspace, calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[0], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[1], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_outputs[0], abstraction));

                    break;
                // wave.integer.subtract(2)(1)
                case runner::opcode::integer_subtract:
                    // write code
                    write_instructions::write__integer_subtract(workspace, calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[0], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[1], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_outputs[0], abstraction));

                    break;
                // wave.integer.multiply(2)(1)
                case runner::opcode::integer_multiply:
                    // write code
                    write_instructions::write__integer_multiply(workspace, calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[0], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[1], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_outputs[0], abstraction));

                    break;
                // wave.integer.divide(2)(1)
                case runner::opcode::integer_divide:
                    // write code
                    write_instructions::write__integer_divide(workspace, calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[0], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[1], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_outputs[0], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_outputs[1], abstraction));

                    break;
                // wave.integer.modulous(2)(1)
                case runner::opcode::integer_modulous:
                    // write code
                    write_instructions::write__integer_modulous(workspace, calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[0], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[1], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_outputs[0], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_outputs[1], abstraction));

                    break;
                // wave.integer.within_range(3)(1)
                case runner::opcode::integer_within_range:
                    // write code
                    write_instructions::write__integer_within_range(workspace, calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[0], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[1], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[2], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_outputs[0], abstraction));

                    break;
                // wave.boolean.not(1)(1)
                case runner::opcode::boolean_not:
                    // write code
                    write_instructions::write__boolean_not(workspace, calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[0], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_outputs[0], abstraction));

                    break;
                // wave.get_context_input(0)(2)
                case runner::opcode::get_context_input:
                    // write code
                    write_instructions::write__get_context_input(workspace, calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_outputs[0], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_outputs[1], abstraction));

                    break;
                // wave.pass_context_output(2)(0)
                case runner::opcode::pass_context_output:
                    // write code
                    write_instructions::write__pass_context_output(workspace, calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[0], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[1], abstraction));

                    break;
                // wave.run(4)(3)
                case runner::opcode::run:
                    // write code
                    write_instructions::write__run(workspace, calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[0], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[1], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[2], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[3], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_outputs[0], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_outputs[1], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_outputs[2], abstraction));

                    break;
                // user defined statement call
                default:
                    // pass inputs
                    for (basic::u64 input_ID = 0; input_ID < abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs.size(); input_ID++) {
                        write_instructions::write__pass_input(workspace, calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[input_ID], abstraction));
                    }

                    // perform call
                    write_instructions::write__write_cell(workspace, workspace.p_abstraction_offsets[abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_header_ID].p_start.p_instruction_ID, calculate_variable_index(accounter::skeleton::argument(accounter::skeleton::argument_type::is_offset, 0), abstraction));
                    write_instructions::write__jump_to_abstraction(workspace, calculate_variable_index(accounter::skeleton::argument(accounter::skeleton::argument_type::is_offset, 0), abstraction));

                    // get outputs
                    for (basic::u64 output_ID = abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_outputs.size(); output_ID > 0; output_ID--) {
                        write_instructions::write__get_output(workspace, calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_outputs[output_ID - 1], abstraction));
                    }

                    break;
                }
            } else if (abstraction.p_statement_map[statement_ID].p_type == accounter::skeleton::statement_type::is_offset_statement) {
                // define offset
                if (workspace.p_pass_type == pass_type::pass_measure) {
                    // write offset
                    workspace.p_abstraction_offsets[abstraction_ID].p_code_defined_offsets[abstraction.p_statement_map[statement_ID].p_ID].p_instruction_ID = workspace.get_offset();
                }
            }
        }

        // create function epilogue
        // pass outputs
        for (basic::u64 output_ID = 0; output_ID < abstraction.p_variables.p_outputs.size(); output_ID++) {
            // pass outputs
            write_instructions::write__pass_output(workspace, calculate_variable_index(accounter::skeleton::argument(accounter::skeleton::argument_type::is_output, output_ID), abstraction));
        }

        // delete context
        write_instructions::write__restore_old_context(workspace);

        // return to caller
        write_instructions::write__jump_from_abstraction(workspace);
    }

    void generate_kickstarter(workspace& workspace, basic::u64 start_function_ID) {
        write_instructions::write__create_new_context(workspace, 1);
        write_instructions::write__write_cell(workspace, workspace.p_abstraction_offsets[start_function_ID].p_start.p_instruction_ID, 0);
        write_instructions::write__jump_to_abstraction(workspace, 0);
        write_instructions::write__quit(workspace);
    }

    // generate code from a program
    runner::program generate_program(accounter::skeleton::skeleton& skeleton, bool& error_occured) {
        workspace workspace;
        basic::u64 main_function_ID;

        // start pass
        workspace.start_pass_measure(skeleton.p_abstractions.size());

        // get main function ID
        main_function_ID = skeleton.lookup_header_by_name("main", error_occured);

        // check error
        if (error_occured) {
            return workspace.p_program;
        }

        // generate kickstarter
        generate_kickstarter(workspace, main_function_ID);

        // measure each abstraction
        for (basic::u64 abstraction_ID = 0; abstraction_ID < skeleton.p_abstractions.size(); abstraction_ID++) {
            // check if abstraction has scope
            if (skeleton.p_abstractions[abstraction_ID].p_has_scope) {
                // turn into function
                generate_abstraction(workspace, skeleton.p_abstractions[abstraction_ID], abstraction_ID, error_occured);

                // check error
                if (error_occured) {
                    return workspace.p_program;
                }
            }
        }

        // finish pass
        workspace.finish_pass_measure();

        // start pass
        workspace.start_pass_build();

        // generate kickstarter
        generate_kickstarter(workspace, main_function_ID);

        // check error
        if (error_occured) {
            return workspace.p_program;
        }

        // measure each abstraction
        for (basic::u64 abstraction_ID = 0; abstraction_ID < skeleton.p_abstractions.size(); abstraction_ID++) {
            // check if abstraction has scope
            if (skeleton.p_abstractions[abstraction_ID].p_has_scope) {
                // turn into function
                generate_abstraction(workspace, skeleton.p_abstractions[abstraction_ID], abstraction_ID, error_occured);

                // check error
                if (error_occured) {
                    return workspace.p_program;
                }
            }
        }

        return workspace.p_program;
    }
}
