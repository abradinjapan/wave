#pragma once

#include <iostream>
#include <vector>
#include <stdint.h>

#include "basic.cpp"

namespace runner {
    // defines
    typedef basic::u64 cell; // register
    typedef basic::u64 cell_ID;
    typedef cell cell_count;

    #define console_input_buffer_length 2048

    // read buffer
    basic::u64 read_buffer(basic::address source, basic::u64 byte_amount) {
        basic::u64 output;

        // setup output
        output = 0;

        // read buffer
        for (basic::u64 byte_index = 0; byte_index < byte_amount; byte_index += 1) {
            // get byte
            ((uint8_t*)&output)[byte_index] = ((uint8_t*)source)[byte_index];
        }

        // return output
        return output;
    }

    // write buffer
    void write_buffer(basic::u64 source, basic::u64 byte_amount, basic::address destination) {
        // write data to buffer
        for (basic::u64 byte_index = 0; byte_index < byte_amount; byte_index += 1) {
            // write byte
            ((uint8_t*)destination)[byte_index] = ((uint8_t*)&source)[byte_index];
        }
        
        return;
    }

    // open null terminated file name from file name buffer
    basic::character* file_name_buffer_to_file_name_null_terminated(basic::address start, basic::address end) {
        basic::character* output;
        basic::u64 string_size;

        // calculate size
        string_size = (basic::u64)end - (basic::u64)start + 1;

        // allocate output
        output = (basic::character*)malloc(string_size + 1);

        // copy string
        for (basic::u64 i = 0; i < string_size; i++) {
            output[i] = ((basic::character*)start)[i];
        }

        // setup null termination
        output[string_size] = 0;

        return output;
    }

    // create file from buffer
    void move_buffer_to_file(bool& error, basic::address start, basic::address end, basic::address file_path_start, basic::address file_path_end) {
        FILE* file_handle;
        basic::u64 file_length;
        basic::character* temp_file_name;

        // setup error to no error to start
        error = false;

        // setup file name
        temp_file_name = file_name_buffer_to_file_name_null_terminated(file_path_start, file_path_end);

        // calculate file length
        file_length = (basic::u64)start - (basic::u64)end + 1;

        // open file
        file_handle = fopen((const basic::character*)temp_file_name, "w+b");

        // check if the file opened
        if (file_handle == 0) {
            // clean up
            free(temp_file_name);

            // if not, return error
            error = true;

            return;
        }

        // write buffer to file
        fwrite(start, file_length, 1, file_handle);

        // clean up
        free(temp_file_name);
        fclose(file_handle);

        return;
    }

    // create buffer from file
    void move_file_to_buffer(bool& error_occured, basic::address file_name_start, basic::address file_name_end, basic::address* output_start, basic::address* output_end) {
        FILE* file_handle;
        basic::character* temp_file_name;
        basic::u64 file_size;

        // set error code to no error
        error_occured = false;

        // setup null terminated file name
        temp_file_name = file_name_buffer_to_file_name_null_terminated(file_name_start, file_name_end);

        // open file
        file_handle = fopen((const basic::character*)temp_file_name, "rb");

        // check if the file opened
        if (file_handle == 0) {
            // clean up
            free(temp_file_name);
            
            // if not, set error
            error_occured = true;

            // return empty buffer
            *output_start = 0;
            *output_end = 0;

            return;
        }

        // get file size
        fseek(file_handle, 0, SEEK_END);
        file_size = ftell(file_handle);
        fseek(file_handle, 0, SEEK_SET);

        // allocate buffer
        *output_start = malloc(file_size);

        // check if buffer allocated
        if (*output_start == 0) {
            // if not, set error
            error_occured = true;

            // close file handle
            fclose(file_handle);

            // return empty buffer
            *output_end = 0;

            return;
        // buffer allocated
        } else {
            // set buffer end
            *output_end = (basic::address)((basic::u64)(*output_start) + (file_size - 1));
        }

        // read file into buffer
        fread(*output_start, file_size, 1, file_handle);

        // clean up
        free(temp_file_name);
        fclose(file_handle);

        return;
    }

    class buffer {
    public:
        std::vector<cell> p_cells;
    };

    class allocation {
    public:
        basic::address p_start;
        basic::address p_end;

        allocation(basic::address start, basic::address end) {
            p_start = start;
            p_end = end;
        }

        allocation() {
            p_start = 0;
            p_end = 0;
        }

        void print() {
            basic::address current = p_start;

            // print each character
            while (current <= p_end) {
                // print character
                putchar(*(char*)current);

                // next character
                current = (char*)current + sizeof(char);
            }
        }
    };

    class allocations {
    private:
        std::vector<allocation> p_allocations;

        basic::u64 get_allocation_number(allocation data) {
            for (basic::u64 index = 0; index < p_allocations.size(); index++) {
                if (p_allocations[index].p_start == data.p_start && p_allocations[index].p_start == data.p_end) {
                    return index;
                }
            }

            return p_allocations.size();
        }

    public:
        bool allocation_exists(allocation data) {
            if (get_allocation_number(data) < p_allocations.size()) {
                return true;
            } else {
                return false;
            }
        }

        void add_allocation(allocation data) {
            if (allocation_exists(data) == false) {
                p_allocations.push_back(data);
            }
        }

        void remove_allocation(allocation data) {
            if (allocation_exists(data)) {
                p_allocations.erase(p_allocations.begin() + get_allocation_number(data));
            }
        }

        // warning, do not use if reading more than one byte!
        bool is_address_valid(basic::address pointer) {
            for (basic::u64 index = 0; index < p_allocations.size(); index++) {
                // check address
                if (p_allocations[index].p_start <= pointer && p_allocations[index].p_end >= pointer) {
                    return true;
                }
            }

            return false;
        }

        bool is_address_range_valid(basic::address start, basic::address end) {
            for (basic::u64 index = 0; index < p_allocations.size(); index++) {
                // check addresses
                if ((p_allocations[index].p_start <= start && p_allocations[index].p_end >= start) && (p_allocations[index].p_start <= end && p_allocations[index].p_end >= end) && start <= end) {
                    return true;
                }
            }

            return false;
        }

        bool is_address_and_length_valid(basic::address pointer, basic::u64 length) {
            return is_address_range_valid(pointer, (basic::address)((basic::u64)pointer + length - 1));
        }
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
        get_console_input,
        create_new_context,
        restore_old_context,
        pass_input,
        get_input,
        pass_output,
        get_output,
        jump_to_abstraction,
        jump_from_abstraction,
        jump,
        get_instruction_index,
        request_memory,
        return_memory,
        cell_to_address,
        address_to_cell,
        buffer_to_file,
        file_to_buffer,
        integer_add,
        integer_subtract,
        integer_multiply,
        integer_divide,
        integer_modulous,
        integer_within_range,
        boolean_not,
        get_context_input,
        pass_context_output,
        run,
    };

    class instruction {
    public:
        instruction_type p_type;
        cell p_write_register_value;
        cell_ID p_input_0;
        cell_ID p_input_1;
        cell_ID p_input_2;
        cell_ID p_input_3;
        cell_ID p_output_0;
        cell_ID p_output_1;
        cell_ID p_output_2;
        basic::u64 p_jump_instruction_ID;

        instruction() {
            p_type = instruction_type::quit;
            p_write_register_value = 0;
            p_input_0 = 0;
            p_input_1 = 0;
            p_input_2 = 0;
            p_input_3 = 0;
            p_output_0 = 0;
            p_output_1 = 0;
            p_output_2 = 0;
            p_jump_instruction_ID = 0;
        }
    };

    class program {
    public:
        std::vector<instruction> p_instructions;

        bool create_from_allocation(allocations& allocations, allocation program) {
            basic::address current;

            // clear program
            p_instructions.clear();

            // check for remaining room
            if (!allocations.is_address_range_valid(program.p_start, program.p_end)) {
                // DEBUG
                printf("Program Creation Error: Allocation does not exist [ %lu %lu ].\n", program.p_start, program.p_end);

                return false;
            }
            if (((runner::cell)program.p_end - (runner::cell)program.p_start + 1) % sizeof(runner::instruction) != 0) {
                // DEBUG
                printf("Program Creation Error: Program is invalid size.\n");

                return false;
            }

            // setup current
            current = program.p_start;

            // append instructions until at the end of the list
            while (current < program.p_end) {
                // add instruction
                p_instructions.push_back(*(instruction*)current);

                // next instruction
                current = current + sizeof(runner::instruction);
            }

            return true;
        }
    };

    // run code
    allocation run_code(program program, allocation input, allocations& allocations, bool& error_occured) {
        allocation output;
        bool running = true;
        int current_instruction = 0;
        std::vector<context> context_stack;
        std::vector<int> return_stack;
        buffer inputs;
        buffer outputs;
        basic::address file_start;
        basic::address file_end;
        bool file_error_occured;
        char console_buffer[console_input_buffer_length];
        basic::u64 console_buffer_length;
        
        // run instruction variables
        runner::program temp_program;
        runner::allocation temp_code;
        runner::allocation temp_input;
        runner::allocation temp_result;
        bool runner_error_occured;

        // process instructions
        while (running == true) {
            // check for instruction out of bounds
            if ((size_t)current_instruction >= program.p_instructions.size()) {
                // set error
                error_occured = true;

                // announce error
                std::cout << "Runner Error: Ran out of instructions before quit was executed, quitting program." << std::endl;

                return output;
            }

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

                // clear cache
                fflush(stdout);

                // next instruction
                current_instruction++;

                break;
            case instruction_type::print_cell_as_character:
                // print
                putchar((char)context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_0]);

                // clear cache
                fflush(stdout);

                // next instruction
                current_instruction++;

                break;
            case instruction_type::get_console_input:
                // clear console input
                for (basic::u64 character = 0; character < console_input_buffer_length; character++) {
                    // clear one byte
                    console_buffer[character] = 0;
                }

                // get console input
                fgets((basic::character*)&console_buffer, console_input_buffer_length, stdin);

                // get string length
                console_buffer_length = 0;
                while (console_buffer_length < console_input_buffer_length - 1 && console_buffer[console_buffer_length] != '\n') {
                    // next character
                    console_buffer_length += sizeof(char);
                }

                // allocate new buffer
                context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_0] = (runner::cell)malloc(console_buffer_length + 1);

                // if allocation succeded
                if (context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_0] != 0) {
                    // setup buffer end
                    context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_1] = context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_0] + console_buffer_length;

                    // register buffer with allocations
                    allocations.add_allocation(allocation((basic::address)context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_0], (basic::address)context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_1]));

                    // copy string to new buffer
                    for (basic::u64 i = 0; i < console_buffer_length; i++) {
                        // copy byte
                        ((basic::character*)(context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_0]))[i] = console_buffer[i];
                    }

                    // setup null termination
                    ((basic::character*)(context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_0]))[console_buffer_length] = 0;
                // if allocation failed
                } else {
                    // setup null buffer
                    context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_1] = 0;
                }

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
                    current_instruction = program.p_instructions[current_instruction].p_jump_instruction_ID;
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
                context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_0] = (basic::u64)malloc(context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_0]);

                // if allocation succeded
                if (context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_0] != 0) {
                    // setup outputs
                    context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_1] = context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_0] + context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_0] - 1;
                    context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_2] = false;

                    // remember allocation
                    allocations.add_allocation(allocation((basic::address)context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_0], (basic::address)context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_1]));
                // if allocation failed
                } else {
                    // setup outputs
                    context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_1] = 0;
                    context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_2] = true;
                }

                // next instruction
                current_instruction++;

                break;
            case instruction_type::return_memory:
                // return memory
                free((void*)context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_0]);

                // remove allocation marker
                allocations.remove_allocation(allocation((basic::address)context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_0], (basic::address)context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_1]));

                // next instruction
                current_instruction++;

                break;
            case instruction_type::cell_to_address:
                // DEBUG
                //printf("Writing allocation: [ %lu, %lu ]\n", context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_2], context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_1]);

                // if valid request
                if (allocations.is_address_and_length_valid((basic::address)context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_2], context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_1])) {
                    // do write
                    write_buffer(context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_0], context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_1], (basic::address)context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_2]);

                    // DEBUG
                    //printf("Allocation write, value: [ %lu ]\n", context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_0]);

                    // set error code
                    context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_0] = false;
                // if invalid request
                } else {
                    // DEBUG
                    //printf("Allocation not written. Values: [ %lu, %lu ]\n", context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_2], context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_1]);

                    // set error code
                    context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_0] = true;
                }

                // next instruction
                current_instruction++;

                break;
            case instruction_type::address_to_cell:
                // DEBUG
                //printf("Reading allocation: [ %lu, %lu ]\n", context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_0], context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_1]);

                // if valid request
                if (allocations.is_address_and_length_valid((basic::address)context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_0], context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_1])) {
                    // do read
                    context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_0] = read_buffer((basic::address)context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_0], context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_1]);

                    // DEBUG
                    //printf("Allocation read, value: [ %lu ]\n", context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_0]);

                    // set error code
                    context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_1] = false;
                // if invalid request
                } else {
                    // DEBUG
                    //printf("Allocation not read.\n");

                    // set error code
                    context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_1] = true;
                }

                // next instruction
                current_instruction++;
                
                break;
            case instruction_type::buffer_to_file:
                // write a buffer to a file
                move_buffer_to_file(file_error_occured, (basic::address)context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_0], (basic::address)context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_1], (basic::address)context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_2], (basic::address)context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_3]);

                // write error variable
                context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_0] = file_error_occured;

                // next instruction
                current_instruction++;
                
                break;
            case instruction_type::file_to_buffer:
                // write a file to a buffer
                move_file_to_buffer(file_error_occured, (basic::address)context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_0], (basic::address)context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_1], &file_start, &file_end);

                // write output variables
                context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_0] = (runner::cell)file_start;
                context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_1] = (runner::cell)file_end;
                context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_2] = file_error_occured;

                // register buffer with allocations
                if (file_start != 0) {
                    allocations.add_allocation(runner::allocation(file_start, file_end));
                }

                // next instruction
                current_instruction++;
                
                break;
            case instruction_type::integer_add:
                // perform addition
                context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_0] = context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_0] + context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_1];

                // next instruction
                current_instruction++;

                break;
            case instruction_type::integer_subtract:
                // perform subtraction
                context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_0] = context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_0] - context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_1];

                // next instruction
                current_instruction++;

                break;
            case instruction_type::integer_multiply:
                // perform multiplication
                context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_0] = context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_0] * context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_1];

                // next instruction
                current_instruction++;

                break;
            case instruction_type::integer_divide:
                // check for valid denominator
                if (context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_1] != 0) {
                    // perform division
                    context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_0] = context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_0] / context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_1];

                    // setup error code
                    context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_1] = false;
                // demoninator is invalid (0)
                } else {
                    // setup error code
                    context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_1] = true;
                }

                // next instruction
                current_instruction++;

                break;
            case instruction_type::integer_modulous:
                // check for valid denominator
                if (context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_1] != 0) {
                    // perform division
                    context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_0] = context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_0] % context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_1];

                    // setup error code
                    context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_1] = false;
                // demoninator is invalid (0)
                } else {
                    // setup error code
                    context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_1] = true;
                }

                // next instruction
                current_instruction++;

                break;
            case instruction_type::integer_within_range:
                // perform range check
                context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_0] = (cell)(basic::u64)((context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_1] >= context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_0]) && (context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_1] <= context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_2]));

                // next instruction
                current_instruction++;

                break;
            case instruction_type::boolean_not:
                // perform inversion
                context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_0] = (cell)!((basic::u64)(context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_0]));

                // next instruction
                current_instruction++;

                break;
            case instruction_type::get_context_input:
                // get data
                context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_0] = (cell)input.p_start;
                context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_1] = (cell)input.p_end;

                // next instruction
                current_instruction++;

                break;
            case instruction_type::pass_context_output:
                // get data
                output.p_start = (basic::address)context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_0];
                output.p_end = (basic::address)context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_1];

                // next instruction
                current_instruction++;

                break;
            case instruction_type::run:
                // setup error
                runner_error_occured = false;

                // get code
                temp_code.p_start = (basic::address)context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_0];
                temp_code.p_end = (basic::address)context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_1];

                // get input
                temp_input.p_start = (basic::address)context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_2];
                temp_input.p_end = (basic::address)context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_3];

                // convert buffer to program
                if (temp_program.create_from_allocation(allocations, temp_code)) {
                    // run program
                    temp_result = run_code(temp_program, temp_input, allocations, runner_error_occured);

                    // setup outputs
                    context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_0] = (runner::cell)temp_result.p_start;
                    context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_1] = (runner::cell)temp_result.p_end;
                    context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_2] = (runner::cell)runner_error_occured;

                    // clean up
                    temp_program.p_instructions.clear();
                // conversion failed
                } else {
                    // setup error
                    context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_0] = 0;
                    context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_1] = 0;
                    context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_2] = true;
                }

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

    // run a program
    allocation run_program(program program, allocation input, bool& error_occured) {
        allocations allocations;

        // add the input buffer to the allocations
        if (input.p_start != 0) {
            allocations.add_allocation(input);
        }

        // run program and return result
        return run_code(program, input, allocations, error_occured);
    }

    // print program
    void print_program(program& program) {
        // print header
        std::cout << "Instructions:" << std::endl;

        // print instructions
        for (basic::u64 i = 0; i < program.p_instructions.size(); i++) {
            // print instruction name
            std::cout << "\t";
            switch (program.p_instructions[i].p_type) {
            case runner::instruction_type::quit:
                std::cout << "quit";
                break;
            case runner::instruction_type::write_cell:
                std::cout << "write_cell";
                break;
            case runner::instruction_type::copy_cell:
                std::cout << "copy_cell";
                break;
            case runner::instruction_type::print_cell_as_number:
                std::cout << "print_cell_as_number";
                break;
            case runner::instruction_type::print_cell_as_character:
                std::cout << "print_cell_as_character";
                break;
            case runner::instruction_type::get_console_input:
                std::cout << "get_console_input";
                break;
            case runner::instruction_type::create_new_context:
                std::cout << "create_new_context";
                break;
            case runner::instruction_type::restore_old_context:
                std::cout << "restore_old_context";
                break;
            case runner::instruction_type::pass_input:
                std::cout << "pass_input";
                break;
            case runner::instruction_type::get_input:
                std::cout << "get_input";
                break;
            case runner::instruction_type::pass_output:
                std::cout << "pass_output";
                break;
            case runner::instruction_type::get_output:
                std::cout << "get_output";
                break;
            case runner::instruction_type::jump_to_abstraction:
                std::cout << "jump_to_abstraction";
                break;
            case runner::instruction_type::jump_from_abstraction:
                std::cout << "jump_from_abstraction";
                break;
            case runner::instruction_type::jump:
                std::cout << "jump";
                break;
            case runner::instruction_type::get_instruction_index:
                std::cout << "get_instruction_index";
                break;
            case runner::instruction_type::request_memory:
                std::cout << "request_memory";
                break;
            case runner::instruction_type::return_memory:
                std::cout << "return_memory";
                break;
            case runner::instruction_type::cell_to_address:
                std::cout << "cell_to_address";
                break;
            case runner::instruction_type::address_to_cell:
                std::cout << "address_to_cell";
                break;
            case runner::instruction_type:: buffer_to_file:
                std::cout << "buffer_to_file";
                break;
            case runner::instruction_type::file_to_buffer:
                std::cout << "file_to_buffer";
                break;
            case runner::instruction_type::integer_add:
                std::cout << "integer_add";
                break;
            case runner::instruction_type::integer_subtract:
                std::cout << "integer_subtract";
                break;
            case runner::instruction_type::integer_multiply:
                std::cout << "integer_multiply";
                break;
            case runner::instruction_type::integer_divide:
                std::cout << "integer_divide";
                break;
            case runner::instruction_type::integer_modulous:
                std::cout << "integer_modulous";
                break;
            case runner::instruction_type::integer_within_range:
                std::cout << "integer_within_range";
                break;
            case runner::instruction_type::boolean_not:
                std::cout << "boolean_not";
                break;
            case runner::instruction_type::get_context_input:
                std::cout << "get_context_input";
                break;
            case runner::instruction_type::pass_context_output:
                std::cout << "pass_context_output";
                break;
            case runner::instruction_type::run:
                std::cout << "run";
                break;
            default:
                std::cout << "[ invalid instruction ]";
                break;
            }

            // print instruction ID
            std::cout << "[" << program.p_instructions[i].p_type << ']' << std::endl;
        }

        return;
    }
}
