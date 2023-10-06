#pragma once

#include <iostream>
#include <vector>
#include <stdint.h>

namespace runner {
    // defines
    typedef uint64_t cell; // register
    typedef uint64_t cell_ID;
    typedef void* address;

    #define console_input_buffer_length 2048

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

    // open null terminated file name from file name buffer
    char* file_name_buffer_to_file_name_null_terminated(address start, address end) {
        char* output;
        uint64_t string_size;

        // calculate size
        string_size = (uint64_t)end - (uint64_t)start + 1;

        // allocate output
        output = (char*)malloc(string_size + 1);

        // copy string
        for (uint64_t i = 0; i < string_size; i++) {
            output[i] = ((char*)start)[i];
        }

        // setup null termination
        output[string_size] = 0;

        return output;
    }

    // create file from buffer
    void move_buffer_to_file(bool& error, address start, address end, address file_path_start, address file_path_end) {
        FILE* file_handle;
        uint64_t file_length;
        char* temp_file_name;

        // setup error to no error to start
        error = false;

        // setup file name
        temp_file_name = file_name_buffer_to_file_name_null_terminated(start, end);

        // calculate file length
        file_length = (uint64_t)start - (uint64_t)end + 1;

        // open file
        file_handle = fopen((const char*)temp_file_name, "w+b");

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
    void move_file_to_buffer(bool& error_occured, address file_name_start, address file_name_end, address* output_start, address* output_end) {
        FILE* file_handle;
        char* temp_file_name;
        uint64_t file_size;

        // set error code to no error
        error_occured = false;

        // setup null terminated file name
        temp_file_name = file_name_buffer_to_file_name_null_terminated(file_name_start, file_name_end);

        // open file
        file_handle = fopen((const char*)temp_file_name, "rb");

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
            *output_end = (*output_start) + (file_size - 1);
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
        address p_start;
        address p_end;

        allocation(address start, address end) {
            p_start = start;
            p_end = end;
        }
    };

    class allocations {
    private:
        std::vector<allocation> p_allocations;

        uint64_t get_allocation_number(allocation data) {
            for (uint64_t index = 0; index < p_allocations.size(); index++) {
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

        bool is_address_valid(address pointer) {
            for (uint64_t index = 0; index < p_allocations.size(); index++) {
                // check address
                if (p_allocations[index].p_start <= pointer && p_allocations[index].p_end >= pointer) {
                    return true;
                }
            }

            return false;
        }

        bool is_address_range_valid(address start, address end) {
            for (uint64_t index = 0; index < p_allocations.size(); index++) {
                // check addresses
                if ((p_allocations[index].p_start <= start && p_allocations[index].p_end >= start) && (p_allocations[index].p_start <= end && p_allocations[index].p_end >= end) && end <= start) {
                    return true;
                }
            }

            return false;
        }

        bool is_address_and_length_valid(address pointer, uint64_t length) {
            return is_address_range_valid(pointer, pointer + length - 1);
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
        uint64_t p_instruction_ID;

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
        allocations allocations;
        buffer inputs;
        buffer outputs;
        runner::address file_start;
        runner::address file_end;
        bool file_error_occured;
        char console_buffer[console_input_buffer_length];
        uint64_t console_buffer_length;

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
                for (uint64_t character = 0; character < console_input_buffer_length; character++) {
                    // clear one byte
                    console_buffer[character] = 0;
                }

                // get console input
                fgets((char*)&console_buffer, console_input_buffer_length, stdin);

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
                    allocations.add_allocation(allocation((runner::address)context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_0], (runner::address)context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_1]));

                    // copy string to new buffer
                    for (uint64_t i = 0; i < console_buffer_length; i++) {
                        // copy byte
                        ((char*)(context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_0]))[i] = console_buffer[i];
                    }

                    // setup null termination
                    ((char*)(context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_0]))[console_buffer_length] = 0;
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

                // if allocation succeded
                if (context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_0] != 0) {
                    // setup outputs
                    context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_1] = context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_0] + context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_0] - 1;
                    context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_2] = false;

                    // remember allocation
                    allocations.add_allocation(allocation((address)context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_0], (address)context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_1]));
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
                allocations.remove_allocation(allocation((address)context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_0], (address)context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_1]));

                // next instruction
                current_instruction++;

                break;
            case instruction_type::cell_to_address:
                // if valid request
                if (allocations.is_address_valid((address)context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_2])) {
                    // do write
                    write_buffer(context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_0], context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_1], (address)context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_2]);

                    // set error code
                    context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_0] = false;
                // if invalid request
                } else {
                    // set error code
                    context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_0] = true;
                }

                // next instruction
                current_instruction++;

                break;
            case instruction_type::address_to_cell:
                // if valid request
                if (allocations.is_address_valid((address)context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_0])) {
                    // do read
                    context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_0] = read_buffer((address)context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_0], context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_1]);

                    // set error code
                    context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_1] = false;
                // if invalid request
                } else {
                    // set error code
                    context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_1] = true;
                }

                // next instruction
                current_instruction++;
                
                break;
            case instruction_type::buffer_to_file:
                // write a buffer to a file
                move_buffer_to_file(file_error_occured, (runner::address)context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_0], (runner::address)context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_1], (runner::address)context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_2], (runner::address)context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_3]);

                // write error variable
                context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_0] = file_error_occured;

                // next instruction
                current_instruction++;
                
                break;
            case instruction_type::file_to_buffer:
                // write a file to a buffer
                move_file_to_buffer(file_error_occured, (runner::address)context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_0], (runner::address)context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_1], &file_start, &file_end);

                // write output variables
                context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_0] = (runner::cell)file_start;
                context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_1] = (runner::cell)file_end;
                context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_2] = file_error_occured;

                // register buffer with allocations
                if (file_start != 0) {
                    allocations.add_allocation(runner::allocation(file_start, file_end));
                }

                /*// DEBUG
                printf("File:\n");
                for (address current = file_start; current < file_end; current = current + sizeof(char)) {
                    putchar(*(char*)current);
                }
                putchar('\n');*/

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
                context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_0] = (cell)(uint64_t)((context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_1] >= context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_0]) && (context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_1] <= context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_2]));

                // next instruction
                current_instruction++;

                break;
            case instruction_type::boolean_not:
                // perform inversion
                context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_output_0] = (cell)!((uint64_t)(context_stack[context_stack.size() - 1].p_cells.p_cells[program.p_instructions[current_instruction].p_input_0]));

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
