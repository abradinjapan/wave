#pragma once

#include <vector>

#include "account.cpp"
#include "run.cpp"

namespace generator {
    class offset {
    public:
        uint64_t p_instruction_ID;

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
        uint64_t p_instruction_count;
        pass_type p_pass_type;

        void start_pass_measure(uint64_t abstraction_count) {
            // DEBUG
            //std::cout << "Starting pass measure, abstraction count: " << abstraction_count << std::endl;

            p_abstraction_offsets.resize(abstraction_count);
            p_pass_type = pass_type::pass_measure;
            p_instruction_count = 0;
        }

        void finish_pass_measure() {
            // DEBUG
            //std::cout << "Finishing pass measure." << std::endl;

            p_program.p_instructions.resize(p_instruction_count);
        }

        void start_pass_build() {
            // DEBUG
            //std::cout << "Starting pass build." << std::endl;

            p_instruction_count = 0;
            p_pass_type = pass_type::pass_build;
        }

        void finish_pass_build() {
            // DEBUG
            //std::cout << "Finishing pass build." << std::endl;
        }

        uint64_t get_offset() {
            return p_instruction_count;
        }
    };

    namespace write_instructions {
        void write__quit(workspace& workspace) {
            runner::instruction temp_instruction;

            // create instruction
            if (workspace.p_pass_type == pass_type::pass_build) {
                // set type
                temp_instruction.p_type = runner::instruction_type::quit;

                // write instruction
                workspace.p_program.p_instructions[workspace.p_instruction_count] = temp_instruction;
            }

            // next instruction
            workspace.p_instruction_count++;
        }

        void write__write_cell(workspace& workspace, runner::cell value, runner::cell_ID destination) {
            runner::instruction temp_instruction;

            // create instruction
            if (workspace.p_pass_type == pass_type::pass_build) {
                // set type
                temp_instruction.p_type = runner::instruction_type::write_cell;
                temp_instruction.p_write_register_value = value;
                temp_instruction.p_output_0 = destination;

                // write instruction
                workspace.p_program.p_instructions[workspace.p_instruction_count] = temp_instruction;
            }

            // next instruction
            workspace.p_instruction_count++;
        }

        void write__copy_cell(workspace& workspace, runner::cell_ID source, runner::cell_ID destination) {
            runner::instruction temp_instruction;

            // create instruction
            if (workspace.p_pass_type == pass_type::pass_build) {
                // set type
                temp_instruction.p_type = runner::instruction_type::copy_cell;
                temp_instruction.p_input_0 = source;
                temp_instruction.p_output_0 = destination;

                // write instruction
                workspace.p_program.p_instructions[workspace.p_instruction_count] = temp_instruction;
            }

            // next instruction
            workspace.p_instruction_count++;
        }

        void write__print_cell_as_number(workspace& workspace, runner::cell_ID source) {
            runner::instruction temp_instruction;

            // create instruction
            if (workspace.p_pass_type == pass_type::pass_build) {
                // set type
                temp_instruction.p_type = runner::instruction_type::print_cell_as_number;
                temp_instruction.p_input_0 = source;

                // write instruction
                workspace.p_program.p_instructions[workspace.p_instruction_count] = temp_instruction;
            }

            // next instruction
            workspace.p_instruction_count++;
        }

        void write__print_cell_as_character(workspace& workspace, runner::cell_ID source) {
            runner::instruction temp_instruction;

            // create instruction
            if (workspace.p_pass_type == pass_type::pass_build) {
                // set type
                temp_instruction.p_type = runner::instruction_type::print_cell_as_character;
                temp_instruction.p_input_0 = source;

                // write instruction
                workspace.p_program.p_instructions[workspace.p_instruction_count] = temp_instruction;
            }

            // next instruction
            workspace.p_instruction_count++;
        }

        void write__create_new_context(workspace& workspace, runner::cell cell_count) {
            runner::instruction temp_instruction;

            // create instruction
            if (workspace.p_pass_type == pass_type::pass_build) {
                // set type
                temp_instruction.p_type = runner::instruction_type::create_new_context;
                temp_instruction.p_write_register_value = cell_count;

                // write instruction
                workspace.p_program.p_instructions[workspace.p_instruction_count] = temp_instruction;
            }

            // next instruction
            workspace.p_instruction_count++;
        }

        void write__restore_old_context(workspace& workspace) {
            runner::instruction temp_instruction;

            // create instruction
            if (workspace.p_pass_type == pass_type::pass_build) {
                // set type
                temp_instruction.p_type = runner::instruction_type::restore_old_context;

                // write instruction
                workspace.p_program.p_instructions[workspace.p_instruction_count] = temp_instruction;
            }

            // next instruction
            workspace.p_instruction_count++;
        }

        void write__pass_input(workspace& workspace, runner::cell_ID parameter) {
            runner::instruction temp_instruction;

            // create instruction
            if (workspace.p_pass_type == pass_type::pass_build) {
                // set type
                temp_instruction.p_type = runner::instruction_type::pass_input;
                temp_instruction.p_input_0 = parameter;

                // write instruction
                workspace.p_program.p_instructions[workspace.p_instruction_count] = temp_instruction;
            }

            // next instruction
            workspace.p_instruction_count++;
        }

        void write__get_input(workspace& workspace, runner::cell_ID parameter) {
            runner::instruction temp_instruction;

            // create instruction
            if (workspace.p_pass_type == pass_type::pass_build) {
                // set type
                temp_instruction.p_type = runner::instruction_type::get_input;
                temp_instruction.p_output_0 = parameter;

                // write instruction
                workspace.p_program.p_instructions[workspace.p_instruction_count] = temp_instruction;
            }

            // next instruction
            workspace.p_instruction_count++;
        }

        void write__pass_output(workspace& workspace, runner::cell_ID parameter) {
            runner::instruction temp_instruction;

            // create instruction
            if (workspace.p_pass_type == pass_type::pass_build) {
                // set type
                temp_instruction.p_type = runner::instruction_type::pass_output;
                temp_instruction.p_input_0 = parameter;

                // write instruction
                workspace.p_program.p_instructions[workspace.p_instruction_count] = temp_instruction;
            }

            // next instruction
            workspace.p_instruction_count++;
        }

        void write__get_output(workspace& workspace, runner::cell_ID parameter) {
            runner::instruction temp_instruction;

            // create instruction
            if (workspace.p_pass_type == pass_type::pass_build) {
                // set type
                temp_instruction.p_type = runner::instruction_type::get_output;
                temp_instruction.p_output_0 = parameter;

                // write instruction
                workspace.p_program.p_instructions[workspace.p_instruction_count] = temp_instruction;
            }

            // next instruction
            workspace.p_instruction_count++;
        }

        void write__jump_to_abstraction(workspace& workspace, runner::cell_ID destination_instruction) {
            runner::instruction temp_instruction;

            // create instruction
            if (workspace.p_pass_type == pass_type::pass_build) {
                // set data
                temp_instruction.p_type = runner::instruction_type::jump_to_abstraction;
                temp_instruction.p_input_0 = destination_instruction;

                // write instruction
                workspace.p_program.p_instructions[workspace.p_instruction_count] = temp_instruction;
            }

            // next instruction
            workspace.p_instruction_count++;
        }

        void write__jump_from_abstraction(workspace& workspace) {
            runner::instruction temp_instruction;

            // create instruction
            if (workspace.p_pass_type == pass_type::pass_build) {
                // set data
                temp_instruction.p_type = runner::instruction_type::jump_from_abstraction;

                // write instruction
                workspace.p_program.p_instructions[workspace.p_instruction_count] = temp_instruction;
            }

            // next instruction
            workspace.p_instruction_count++;
        }

        void write__jump_to(workspace& workspace, runner::cell_ID source) {
            runner::instruction temp_instruction;

            // create instruction
            if (workspace.p_pass_type == pass_type::pass_build) {
                // set type
                temp_instruction.p_type = runner::instruction_type::jump_to;
                temp_instruction.p_input_0 = source;

                // write instruction
                workspace.p_program.p_instructions[workspace.p_instruction_count] = temp_instruction;
            }

            // next instruction
            workspace.p_instruction_count++;
        }

        void write__jump_if(workspace& workspace, runner::cell_ID condition, runner::cell_ID instruction_ID) {
            runner::instruction temp_instruction;

            // create instruction
            if (workspace.p_pass_type == pass_type::pass_build) {
                // set type
                temp_instruction.p_type = runner::instruction_type::jump_if;
                temp_instruction.p_input_0 = condition;
                temp_instruction.p_input_1 = instruction_ID;

                // write instruction
                workspace.p_program.p_instructions[workspace.p_instruction_count] = temp_instruction;
            }

            // next instruction
            workspace.p_instruction_count++;
        }

        void write__get_instruction_index(workspace& workspace, runner::cell_ID destination) {
            runner::instruction temp_instruction;

            // create instruction
            if (workspace.p_pass_type == pass_type::pass_build) {
                // set type
                temp_instruction.p_type = runner::instruction_type::get_instruction_index;
                temp_instruction.p_output_0 = destination;

                // write instruction
                workspace.p_program.p_instructions[workspace.p_instruction_count] = temp_instruction;
            }

            // next instruction
            workspace.p_instruction_count++;
        }

        void write__request_memory(workspace& workspace, runner::cell_ID length, runner::cell_ID address) {
            runner::instruction temp_instruction;

            // create instruction
            if (workspace.p_pass_type == pass_type::pass_build) {
                // set type
                temp_instruction.p_type = runner::instruction_type::request_memory;
                temp_instruction.p_input_0 = length;
                temp_instruction.p_output_0 = address;

                // write instruction
                workspace.p_program.p_instructions[workspace.p_instruction_count] = temp_instruction;
            }

            // next instruction
            workspace.p_instruction_count++;
        }

        void write__return_memory(workspace& workspace, runner::cell_ID address) {
            runner::instruction temp_instruction;

            // create instruction
            if (workspace.p_pass_type == pass_type::pass_build) {
                // set type
                temp_instruction.p_type = runner::instruction_type::return_memory;
                temp_instruction.p_input_0 = address;

                // write instruction
                workspace.p_program.p_instructions[workspace.p_instruction_count] = temp_instruction;
            }

            // next instruction
            workspace.p_instruction_count++;
        }

        void write__cell_to_address(workspace& workspace, runner::cell_ID source, runner::cell_ID byte_size, runner::cell_ID destination) {
            runner::instruction temp_instruction;

            // create instruction
            if (workspace.p_pass_type == pass_type::pass_build) {
                // set type
                temp_instruction.p_type = runner::instruction_type::cell_to_address;
                temp_instruction.p_input_0 = source;
                temp_instruction.p_input_1 = byte_size;
                temp_instruction.p_input_2 = destination;

                // write instruction
                workspace.p_program.p_instructions[workspace.p_instruction_count] = temp_instruction;
            }

            // next instruction
            workspace.p_instruction_count++;
        }

        void write__address_to_cell(workspace& workspace, runner::cell_ID source, runner::cell_ID byte_size, runner::cell_ID destination) {
            runner::instruction temp_instruction;

            // create instruction
            if (workspace.p_pass_type == pass_type::pass_build) {
                // set type
                temp_instruction.p_type = runner::instruction_type::address_to_cell;
                temp_instruction.p_input_0 = source;
                temp_instruction.p_input_1 = byte_size;
                temp_instruction.p_output_0 = destination;

                // write instruction
                workspace.p_program.p_instructions[workspace.p_instruction_count] = temp_instruction;
            }

            // next instruction
            workspace.p_instruction_count++;
        }

        void write__buffer_to_file(workspace& workspace, runner::cell_ID source_buffer, runner::cell_ID source_buffer_length, runner::cell_ID null_terminated_file_path) {
            runner::instruction temp_instruction;

            // create instruction
            if (workspace.p_pass_type == pass_type::pass_build) {
                // set type
                temp_instruction.p_type = runner::instruction_type::buffer_to_file;
                temp_instruction.p_input_0 = source_buffer;
                temp_instruction.p_input_1 = source_buffer_length;
                temp_instruction.p_output_0 = null_terminated_file_path;

                // write instruction
                workspace.p_program.p_instructions[workspace.p_instruction_count] = temp_instruction;
            }

            // next instruction
            workspace.p_instruction_count++;
        }

        void write__file_to_buffer(workspace& workspace, runner::cell_ID null_terminated_file_path, runner::cell_ID source_buffer, runner::cell_ID source_buffer_length) {
            runner::instruction temp_instruction;

            // create instruction
            if (workspace.p_pass_type == pass_type::pass_build) {
                // set type
                temp_instruction.p_type = runner::instruction_type::file_to_buffer;
                temp_instruction.p_input_0 = null_terminated_file_path;
                temp_instruction.p_output_0 = source_buffer;
                temp_instruction.p_output_1 = source_buffer_length;

                // write instruction
                workspace.p_program.p_instructions[workspace.p_instruction_count] = temp_instruction;
            }

            // next instruction
            workspace.p_instruction_count++;
        }

        void write__integer_add(workspace& workspace, runner::cell_ID source_1, runner::cell_ID source_2, runner::cell_ID destination) {
            runner::instruction temp_instruction;

            // create instruction
            if (workspace.p_pass_type == pass_type::pass_build) {
                // set type
                temp_instruction.p_type = runner::instruction_type::integer_add;
                temp_instruction.p_input_0 = source_1;
                temp_instruction.p_input_1 = source_2;
                temp_instruction.p_output_0 = destination;

                // write instruction
                workspace.p_program.p_instructions[workspace.p_instruction_count] = temp_instruction;
            }

            // next instruction
            workspace.p_instruction_count++;
        }

        void write__integer_within_range(workspace& workspace, runner::cell_ID low_bound, runner::cell_ID value, runner::cell_ID high_bound, runner::cell_ID destination) {
            runner::instruction temp_instruction;

            // create instruction
            if (workspace.p_pass_type == pass_type::pass_build) {
                // set type
                temp_instruction.p_type = runner::instruction_type::integer_within_range;
                temp_instruction.p_input_0 = low_bound;
                temp_instruction.p_input_1 = value;
                temp_instruction.p_input_2 = high_bound;
                temp_instruction.p_output_0 = destination;

                // write instruction
                workspace.p_program.p_instructions[workspace.p_instruction_count] = temp_instruction;
            }

            // next instruction
            workspace.p_instruction_count++;
        }
    }

    enum variable_type {
        variable_input,
        variable_output,
        variable_variable,
        variable_COUNT,
    };

    uint64_t calculate_variable_index(accounter::skeleton::argument argument, accounter::skeleton::abstraction& abstraction) {
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

    void generate_abstraction(workspace& workspace, accounter::skeleton::abstraction& abstraction, uint64_t abstraction_ID, bool& error_occured) {
        uint64_t variable_count;

        // determine variable count
        variable_count = calculate_variable_index(accounter::skeleton::argument(accounter::skeleton::argument_type::is_invalid, 0), abstraction);

        // setup offsets container
        if (workspace.p_pass_type == pass_type::pass_measure) {
            workspace.p_abstraction_offsets[abstraction_ID].p_code_defined_offsets.resize(abstraction.p_offsets.p_offsets.size());
        
            // setup function start offset
            workspace.p_abstraction_offsets[abstraction_ID].p_start = workspace.get_offset();
        // DEBUG
        }/* else {
            std::cout << "Offset Count: " << workspace.p_abstraction_offsets[abstraction_ID].p_code_defined_offsets.size() << std::endl;
        }*/

        // generate function prologue
        // create context
        write_instructions::write__create_new_context(workspace, variable_count);

        // get inputs
        for (uint64_t input_ID = abstraction.p_variables.p_inputs.size(); input_ID > 0; input_ID--) {
            // get input
            write_instructions::write__get_input(workspace, calculate_variable_index(accounter::skeleton::argument(accounter::skeleton::argument_type::is_input, input_ID - 1), abstraction));
        }

        // generate function body
        for (uint64_t statement_ID = 0; statement_ID < abstraction.p_statement_map.size(); statement_ID++) {
            // generate statement
            if (abstraction.p_statement_map[statement_ID].p_type == accounter::skeleton::statement_type::is_call_statement) {
                // write code (NOTE: make sure that the switch cases are aligned with their respective run.cpp instructions!)
                switch (abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_header_ID) {
                // pirate.quit(0)(0)
                case runner::instruction_type::quit:
                    // write code
                    write_instructions::write__quit(workspace);

                    break;
                // pirate.write_cell(1)(1)
                case runner::instruction_type::write_cell:
                    // determine type of input
                    if (abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[0].p_type == accounter::skeleton::argument_type::is_integer_literal) {
                        // constant is an integer literal, write code
                        write_instructions::write__write_cell(workspace, abstraction.p_literals.p_literals[abstraction.lookup_literal_by_ID(statement_ID, 0, error_occured).p_ID].p_integer_value, calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_outputs[0], abstraction));
                    } else if (abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[0].p_type == accounter::skeleton::argument_type::is_offset) {
                        // if pass is measure
                        if (workspace.p_pass_type == pass_type::pass_measure) {
                            // write dummy instruction
                            write_instructions::write__write_cell(workspace, 0, calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_outputs[0], abstraction));
                        // if pass is build
                        } else {
                            // DEBUG
                            //std::cout << "Offset ID #" << abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[0].p_ID << std::endl;

                            // constant is an offset, write code
                            write_instructions::write__write_cell(workspace, workspace.p_abstraction_offsets[abstraction_ID].p_code_defined_offsets[abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[0].p_ID].p_instruction_ID, calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_outputs[0], abstraction));
                        }
                    }

                    break;
                // pirate.copy(1)(1)
                case runner::instruction_type::copy_cell:
                    // write code
                    write_instructions::write__copy_cell(workspace, calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[0], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_outputs[0], abstraction));

                    break;
                // pirate.print_cell_as_number(1)(0)
                case runner::instruction_type::print_cell_as_number:
                    // write code
                    write_instructions::write__print_cell_as_number(workspace, calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[0], abstraction));

                    break;
                // pirate.print_cell_as_character(1)(0)
                case runner::instruction_type::print_cell_as_character:
                    // write code
                    write_instructions::write__print_cell_as_character(workspace, calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[0], abstraction));

                    break;
                // pirate.create_new_context(1)(0)
                case runner::instruction_type::create_new_context:
                    // set error
                    error_occured = true;

                    // print error
                    std::cout << "Generation Error: Although create_new_context is a valid instruction, it is not available for general use." << std::endl;

                    break;
                // pirate.restore_old_context(0)(0)
                case runner::instruction_type::restore_old_context:
                    // set error
                    error_occured = true;

                    // print error
                    std::cout << "Generation Error: Although restore_old_context is a valid instruction, it is not available for general use." << std::endl;

                    break;
                // pirate.pass_input(1)(0)
                case runner::instruction_type::pass_input:
                    // set error
                    error_occured = true;

                    // print error
                    std::cout << "Generation Error: Although pass_input is a valid instruction, it is not available for general use." << std::endl;

                    break;
                // pirate.get_input(0)(1)
                case runner::instruction_type::get_input:
                    // set error
                    error_occured = true;

                    // print error
                    std::cout << "Generation Error: Although get_input is a valid instruction, it is not available for general use." << std::endl;

                    break;
                // pirate.pass_output(1)(0)
                case runner::instruction_type::pass_output:
                    // set error
                    error_occured = true;

                    // print error
                    std::cout << "Generation Error: Although pass_output is a valid instruction, it is not available for general use." << std::endl;

                    break;
                // pirate.get_output(0)(1)
                case runner::instruction_type::get_output:
                    // set error
                    error_occured = true;

                    // print error
                    std::cout << "Generation Error: Although get_output is a valid instruction, it is not available for general use." << std::endl;

                    break;
                // pirate.jump_to_abstraction(1)(0)
                case runner::instruction_type::jump_to_abstraction:
                    // set error
                    error_occured = true;

                    // print error
                    std::cout << "Generation Error: Although jump_to_abstraction is a valid instruction, it is not available for general use." << std::endl;

                    break;
                // pirate.jump_from_abstraction(0)(0)
                case runner::instruction_type::jump_from_abstraction:
                    // set error
                    error_occured = true;

                    // print error
                    std::cout << "Generation Error: Although jump_from_abstraction is a valid instruction, it is not available for general use." << std::endl;

                    break;
                // pirate.jump_to(1)(0)
                case runner::instruction_type::jump_to:
                    // write code
                    write_instructions::write__jump_to(workspace, calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[0], abstraction));

                    break;
                // pirate.jump_if(2)(0)
                case runner::instruction_type::jump_if:
                    // write code
                    write_instructions::write__jump_if(workspace, calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[0], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[1], abstraction));

                    break;
                // pirate.get_instruction_index(0)(1)
                case runner::instruction_type::get_instruction_index:
                    // write code
                    write_instructions::write__get_instruction_index(workspace, calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_outputs[0], abstraction));

                    break;
                // pirate.request_memory(1)(1)
                case runner::instruction_type::request_memory:
                    // write code
                    write_instructions::write__request_memory(workspace, calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[0], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_outputs[0], abstraction));

                    break;
                // pirate.return_memory(1)(0)
                case runner::instruction_type::return_memory:
                    // write code
                    write_instructions::write__return_memory(workspace, calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[0], abstraction));

                    break;
                // pirate.cell_to_address(3)(0)
                case runner::instruction_type::cell_to_address:
                    // write code
                    write_instructions::write__cell_to_address(workspace, calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[0], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[1], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[2], abstraction));

                    break;
                // pirate.address_to_cell(2)(1)
                case runner::instruction_type::address_to_cell:
                    // write code
                    write_instructions::write__address_to_cell(workspace, calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[0], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[1], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_outputs[0], abstraction));

                    break;
                // pirate.buffer_to_file(2)(1)
                case runner::instruction_type::buffer_to_file:
                    // write code
                    write_instructions::write__buffer_to_file(workspace, calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[0], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[1], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_outputs[0], abstraction));

                    break;
                // pirate.file_to_buffer(1)(2)
                case runner::instruction_type::file_to_buffer:
                    // write code
                    write_instructions::write__file_to_buffer(workspace, calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[0], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_outputs[0], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_outputs[1], abstraction));

                    break;
                // pirate.integer_add(2)(1)
                case runner::instruction_type::integer_add:
                    // write code
                    write_instructions::write__integer_add(workspace, calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[0], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[1], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_outputs[0], abstraction));

                    break;
                // pirate.integer_within_range(3)(1)
                case runner::instruction_type::integer_within_range:
                    // write code
                    write_instructions::write__integer_within_range(workspace, calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[0], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[1], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[2], abstraction), calculate_variable_index(abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_outputs[0], abstraction));

                    break;
                // user defined statement call
                default:
                    // pass inputs
                    for (uint64_t input_ID = 0; input_ID < abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs.size(); input_ID++) {
                        write_instructions::write__pass_input(workspace, abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_inputs[input_ID].p_ID);
                    }

                    // perform call
                    write_instructions::write__write_cell(workspace, workspace.p_abstraction_offsets[abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_header_ID].p_start.p_instruction_ID, calculate_variable_index(accounter::skeleton::argument(accounter::skeleton::argument_type::is_offset, 0), abstraction));
                    write_instructions::write__jump_to_abstraction(workspace, calculate_variable_index(accounter::skeleton::argument(accounter::skeleton::argument_type::is_offset, 0), abstraction));

                    // get outputs
                    for (uint64_t output_ID = abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_outputs.size(); output_ID > 0; output_ID--) {
                        write_instructions::write__get_output(workspace, abstraction.p_calls[abstraction.p_statement_map[statement_ID].p_ID].p_outputs[output_ID - 1].p_ID);
                    }

                    break;
                }
            } else if (abstraction.p_statement_map[statement_ID].p_type == accounter::skeleton::statement_type::is_offset_statement) {
                // define offset
                if (workspace.p_pass_type == pass_type::pass_measure) {
                    // DEBUG
                    //std::cout << "Defined abstraction offset #" << abstraction.p_statement_map[statement_ID].p_ID << " at instruction #" << workspace.get_offset() << std::endl;

                    // write offset
                    workspace.p_abstraction_offsets[abstraction_ID].p_code_defined_offsets[abstraction.p_statement_map[statement_ID].p_ID].p_instruction_ID = workspace.get_offset();
                }
            }
        }

        // create function epilogue
        // pass outputs
        for (uint64_t output_ID = 0; output_ID < abstraction.p_variables.p_outputs.size(); output_ID++) {
            // pass outputs
            write_instructions::write__pass_output(workspace, calculate_variable_index(accounter::skeleton::argument(accounter::skeleton::argument_type::is_output, output_ID), abstraction));
        }

        // delete context
        write_instructions::write__restore_old_context(workspace);

        // return to caller
        write_instructions::write__jump_from_abstraction(workspace);
    }

    void generate_kickstarter(workspace& workspace, uint64_t start_function_ID) {
        write_instructions::write__create_new_context(workspace, 1);
        write_instructions::write__write_cell(workspace, workspace.p_abstraction_offsets[start_function_ID].p_start.p_instruction_ID, 0);
        write_instructions::write__jump_to_abstraction(workspace, 0);
        write_instructions::write__quit(workspace);
    }

    // generate code from a program
    runner::program generate_program(accounter::skeleton::skeleton& skeleton, bool& error_occured) {
        workspace workspace;

        // start pass
        workspace.start_pass_measure(skeleton.p_abstractions.size());

        // generate kickstarter
        generate_kickstarter(workspace, skeleton.lookup_header_by_name("pirate.sail", error_occured));

        // check error
        if (error_occured) {
            return workspace.p_program;
        }

        // measure each abstraction
        for (uint64_t abstraction_ID = 0; abstraction_ID < skeleton.p_abstractions.size(); abstraction_ID++) {
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
        generate_kickstarter(workspace, skeleton.lookup_header_by_name("pirate.sail", error_occured));

        // check error
        if (error_occured) {
            return workspace.p_program;
        }

        // measure each abstraction
        for (uint64_t abstraction_ID = 0; abstraction_ID < skeleton.p_abstractions.size(); abstraction_ID++) {
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
