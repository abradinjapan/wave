#pragma once

#include <iostream>
#include <vector>
#include <stdint.h>

namespace runner {
    // defines
    typedef uint64_t cell; // register
    typedef uint64_t cell_ID;
    typedef void* address;

    // read buffer
    uint64_t read_buffer(address source, uint64_t byte_amount) {
        uint64_t output;

        // setup output
        output = 0;

        // read buffer
        for (uint64_t byte_index = 0; byte_index < byte_amount; byte_index += 1) {
            // get byte
            ((uint8_t*)&output)[byte_index] = ((uint8_t*)source)[byte_index];
        }

        // return output
        return output;
    }

    // write buffer
    void write_buffer(uint64_t source, uint64_t byte_amount, address destination) {
        // write data to buffer
        for (uint64_t byte_index = 0; byte_index < byte_amount; byte_index += 1) {
            // write byte
            ((uint8_t*)destination)[byte_index] = ((uint8_t*)&source)[byte_index];
        }
        
        return;
    }

    // create file from buffer
    void move_buffer_to_file(bool* error, address data, uint64_t length, address null_terminated_file_name) {
        FILE* file_handle;

        // setup error to no error to start
        *error = false;

        // open file
        file_handle = fopen((const char*)null_terminated_file_name, "w+b");

        // check if the file opened
        if (file_handle == 0) {
            // if not, return error
            *error = true;

            return;
        }

        // write buffer to file
        fwrite(data, length, 1, file_handle);

        // close file handle
        fclose(file_handle);

        // return
        return;
    }

    // create buffer from file
    address move_file_to_buffer(address null_terminated_file_name, uint64_t* output_length) {
        address output;
        FILE* file_handle;
        uint64_t file_size;

        // open file
        file_handle = fopen((const char*)null_terminated_file_name, "rb");

        // check if the file opened
        if (file_handle == 0) {
            // if not, return empty buffer
            return 0;
        }

        // get file size
        fseek(file_handle, 0, SEEK_END);
        file_size = ftell(file_handle);
        fseek(file_handle, 0, SEEK_SET);

        // allocate buffer
        output = malloc(file_size);

        // check if buffer allocated
        if (output == 0) {
            // close file handle
            fclose(file_handle);

            // return empty buffer
            *output_length = 0;

            return output;
        }

        // read file into buffer
        fread(output, file_size, 1, file_handle);

        // close file handle
        fclose(file_handle);

        // return buffer
        *output_length = file_size;

        return output;
    }

    class buffer {
    public:
        std::vector<cell> p_cells;
    };

    class context {
    public:
        buffer p_cells;

        // constructor
        context() {
            p_cells = buffer();
        }

        // constructor
        context(cell_ID cell_count) {
            // initialize blank context
            for (cell_ID i = 0; i < cell_count; i++) {
                // add cell
                p_cells.p_cells.push_back(0);
            }
        }
    };

    // instructions
    enum instruction_type {
        quit,
        write_cell,
        copy_cell,
        print_cell_as_number,
        print_cell_as_character,
        create_new_context,
        restore_old_context,
        pass_input,
        get_input,
        pass_output,
        get_output,
        jump_to_abstraction,
        jump_from_abstraction,
        //jump_to,
        //jump_if,
        jump,
        get_instruction_index,
        request_memory,
        return_memory,
        cell_to_address,
        address_to_cell,
        buffer_to_file,
        file_to_buffer,
        integer_add,
        integer_within_range,
    };

    class instruction {
    public:
        instruction_type p_type;
        cell p_write_register_value;
        cell_ID p_input_0;
        cell_ID p_input_1;
        cell_ID p_input_2;
        cell_ID p_output_0;
        cell_ID p_output_1;
        uint64_t p_instruction_ID;

        instruction() {
            p_type = instruction_type::quit;
            p_write_register_value = 0;
            p_input_0 = 0;
            p_input_1 = 0;
            p_input_2 = 0;
            p_output_0 = 0;
            p_output_1 = 0;
            p_instruction_ID = 0;
        }
    };

    class program {
    public:
        std::vector<instruction> p_instructions;
    };

    // run a program
    buffer run_program(program program, bool& error_occured) {
        buffer output;
        bool running = true;
        int current_instruction = 0;
        std::vector<context> context_stack;
        std::vector<int> return_stack;
        buffer inputs;
        buffer outputs;

        // process instructions
        while (running == true) {
            // process instruction
            switch (program.p_instructions[current_instruction].p_type) {
            case instruction_type::quit:
                // next instruction
                current_instruction++;

                // stop running loop
                running = false;

                break;
            case instruction_type::write_cell:
                // write data
                context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_0] = program.p_instructions[current_instruction].p_write_register_value;

                // next instruction
                current_instruction++;

                break;
            case instruction_type::copy_cell:
                // copy data
                context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_0] = context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_0];

                // next instruction
                current_instruction++;

                break;
            case instruction_type::print_cell_as_number:
                // print
                std::cout << context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_0];

                // next instruction
                current_instruction++;

                break;
            case instruction_type::print_cell_as_character:
                // print
                putchar((char)context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_0]);

                // next instruction
                current_instruction++;

                break;
            case instruction_type::create_new_context:
                // push a new context onto the stack
                context_stack.push_back(context(program.p_instructions[current_instruction].p_write_register_value));

                // next instruction
                current_instruction++;

                break;
            case instruction_type::restore_old_context:
                // restore the previous context
                context_stack.pop_back();

                // next instruction
                current_instruction++;

                break;
            case instruction_type::pass_input:
                // add input
                inputs.p_cells.push_back(context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_0]);

                // next instruction
                current_instruction++;

                break;
            case instruction_type::get_input:
                // retrieve input
                context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_0] = inputs.p_cells[inputs.p_cells.size() - 1];

                // clear last input
                inputs.p_cells.pop_back();

                // next instruction
                current_instruction++;

                break;
            case instruction_type::pass_output:
                // add output
                outputs.p_cells.push_back(context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_0]);

                // next instruction
                current_instruction++;

                break;
            case instruction_type::get_output:
                // retrieve output
                context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_0] = outputs.p_cells[outputs.p_cells.size() - 1];

                // clear last output
                outputs.p_cells.pop_back();

                // next instruction
                current_instruction++;

                break;
            case instruction_type::jump_to_abstraction:
                // push the instruction to be returned
                return_stack.push_back(current_instruction + 1);

                // jump
                current_instruction = context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_0];

                break;
            case instruction_type::jump_from_abstraction:
                // jump
                current_instruction = return_stack[return_stack.size() - 1];
                return_stack.pop_back();

                break;
            case instruction_type::jump:
                // if true
                if (context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_0] == 1) {
                    // jump via instruction number
                    current_instruction = program.p_instructions[current_instruction].p_instruction_ID;
                } else {
                    // next instruction
                    current_instruction++;
                }

                break;
            case instruction_type::get_instruction_index:
                // perform addition
                context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_0] = (cell)current_instruction;

                // next instruction
                current_instruction++;

                break;
            case instruction_type::request_memory:
                // perform allocation
                context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_0] = (uint64_t)malloc(context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_0]);

                // next instruction
                current_instruction++;

                break;
            case instruction_type::return_memory:
                // return memory
                free((void*)context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_0]);

                // next instruction
                current_instruction++;

                break;
            case instruction_type::cell_to_address:
                // do write
                write_buffer(context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_0], context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_1], (address)context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_2]);

                // next instruction
                current_instruction++;

                break;
            case instruction_type::address_to_cell:
                // do write
                context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_0] = read_buffer((address)context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_0], context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_1]);

                // next instruction
                current_instruction++;
                
                break;
            case instruction_type::buffer_to_file:
                // write a buffer to a file
                move_buffer_to_file(&error_occured, (runner::address)context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_0], context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_1], (runner::address)context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_2]);

                // next instruction
                current_instruction++;
                
                break;
            case instruction_type::file_to_buffer:
                // write a file to a buffer
                context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_0] = (runner::cell)move_file_to_buffer((runner::address)context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_0], (uint64_t*)&(context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_1]));

                // next instruction
                current_instruction++;
                
                break;
            case instruction_type::integer_add:
                // perform addition
                context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_0] = context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_0] + context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_1];

                // next instruction
                current_instruction++;

                break;
            case instruction_type::integer_within_range:
                // perform range check
                context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_0] = (cell)(uint64_t)((context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_1] >= context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_0]) && (context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_1] <= context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_2]));

                // next instruction
                current_instruction++;

                break;
            default:
                std::cout << "Runner Error: Invalid Instruction ID" << std::endl;

                error_occured = true;

                break;
            }
        }

        return output;
    }

    // print program
    void print_program(program& program) {
        // print header
        std::cout << "Instructions:" << std::endl;

        // print instructions
        for (uint64_t i = 0; i < program.p_instructions.size(); i++) {
            // print instruction
            std::cout << "\t" << program.p_instructions[i].p_type << std::endl;
        }

        return;
    }
}
