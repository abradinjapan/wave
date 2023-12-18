# Instruction List

## quit

- Quits the current program.
- Inputs:
    - (none)
- Outputs:
    - (none)

## write_cell

- Writes a constant to a cell.
- Inputs:
    - value: The constant value to be written.
- Outputs:
    - destination: The cell ID of the value's destination.

## copy_cell

- Overwrites one cell with another cell.
- Inputs:
    - source: The source cell ID.
- Outputs:
    - destination: The destination cell ID.

## print_cell_as_number

- Prints the cell as an integer number.
- Inputs:
    - source: The source cell ID.
- Outputs:
    - (none)

## print_cell_as_character

- Prints the cell as an ASCII character.
- Inputs:
    - source: The source cell ID.
- Outputs:
    - (none)

## get_console_input

- Gets one line of user input from the terminal.
- Inputs:
    - (none)
- Outputs:
    - start: The starting address of the text input buffer.
    - end: The ending address of the text input buffer.

## create_new_context

- Creates a new context.
- Inputs:
    - cell_count: The amount of cells in the context.
- Outputs:
    - (none)

## restore_old_context

- Restores the previous context.
- Inputs:
    - (none)
- Outputs:
    - (none)

## pass_input

- Passes one cell to the top of the input stack.
- Inputs:
    - parameter: The cell ID of the parameter to be passed.
- Outputs:
    - (none)

## get_input

- Gets the top cell from the input stack.
- Inputs:
    - (none)
- Outputs:
    - parameter: The cell ID of the parameter to be recieved.

## pass_output

- Passes one cell to the top of the output stack.
- Inputs:
    - parameter: The cell ID of the parameter to be passed.
- Outputs:
    - (none)

## get_input

- Gets the top cell from the output stack.
- Inputs:
    - (none)
- Outputs:
    - parameter: The cell ID of the parameter to be recieved.

## jump_to_abstraction

- Pushes the next instruction onto the return address stack and jumps to the designated abstraction.
- Inputs:
    - destination_instruction: The instruction ID being jumped to.
- Outputs:
    - (none)

## jump_from_abstraction

- Pops the top return address from the return address stack and jumps to it.
- Inputs:
    - (none)
- Outputs:
    - (none)

## jump

- Conditionally jumps to any instruction.
- Inputs:
    - condition: A boolean that determines whether or not to jump.
    - instruction_ID: The instruction being jumped to.
- Outputs:
    - (none)

## get_instruction_index

- Gets the current instruction ID and saves it into a cell.
- Inputs:
    - destination: The destination cell of the instruction ID.
- Outputs:
    - (none)

## request_memory

- Attempts to allocate memory, returning a buffer.
- Inputs:
    - length: The size of the buffer.
- Outputs:
    - start_address: The start of the new buffer.
    - end_address: The end of the new buffer.
    - error: An error boolean.

## return_memory

- Frees a buffer.
- Inputs:
    - start_address: The start of the buffer.
    - end_address: The end of the buffer.
- Outputs:
    - (none)

## cell_to_address

- Writes a specified amount of bytes from a cell to a buffer.
- Inputs:
    - source: The source cell ID.
    - byte_size: The amount of bytes to be copied (1 -> 8).
    - destination: The address being written to.
- Outputs:
    - error_code: An error boolean.

## address_to_cell

- Reads a specified amount of bytes from an address to a buffer.
- Inputs:
    - source: The address being read from.
    - byte_size: the amount of bytes being copied (1 -> 8).
- Outputs:
    - destination: The destination address.
    - error_code: An error boolean.

## buffer_to_file

- Overwrites a file with a buffer.
- Inputs:
    - source_start: The start address of the data buffer.
    - source_end: The end address of the data buffer.
    - file_path_start: The start address of the file path buffer.
    - file_path_end: The end address of the file path buffer.
- Outputs:
    - error_code: An error boolean.

## file_to_buffer

- Reads an entire file into a buffer.
- Inputs:
    - file_path_start: The start address of the file path buffer.
    - file_path_end: The end address of the file path buffer.
- Outputs:
    - source_start: The start address of the data buffer.
    - source_end: The end address of the data buffer.
    - error_code: An error boolean.

## integer_add

- Performs integer addition.
- Inputs:
    - source_1: The first parameter cell ID.
    - source_2: The second parameter cell ID.
- Outputs:
    - destination: The result cell ID.

## integer_subtract

- Performs integer subtraction.
- Inputs:
    - source_1: The first parameter cell ID.
    - source_2: The second parameter cell ID.
- Outputs:
    - destination: The result cell ID.

## integer_multiply

- Performs integer multiplication.
- Inputs:
    - source_1: The first parameter cell ID.
    - source_2: The second parameter cell ID.
- Outputs:
    - destination: The result cell ID.

## integer_divide

- Performs integer division.
- Inputs:
    - source_1: The first parameter cell ID.
    - source_2: The second parameter cell ID.
- Outputs:
    - destination: The result cell ID.
    - error_code: Set to true if the divisor is zero.

## integer_modulous

- Performs integer modulous.
- Inputs:
    - source_1: The first parameter cell ID.
    - source_2: The second parameter cell ID.
- Outputs:
    - destination: The result cell ID.
    - error_code: Set to true if the divisor is zero.

## integer_within_range

- Checks if an integer is within an unsigned integer range.
- Inputs:
    - low_bound: The lowest (inclusive) possible value.
    - value: The value being checked.
    - high_bound: The highest (inclusive) possible value.
- Outputs:
    - destination: A boolean that says if the number was in range or not.

## boolean_not

- Inverts a boolean.
- Inputs:
    - source: The source boolean.
- Outputs:
    destination: The destination cell ID.

## get_context_input

- Gets the context level input from the caller context.
- Inputs:
    - (none)
- Outputs:
    - destination_start: The starting address of the input.
    - destination_end: The ending address of the input.

## pass_context_output

- Gets the context level input from the caller context.
- Inputs:
    - (none)
- Outputs:
    - destination_start: The starting address of the input.
    - destination_end: The ending address of the input.

# run

- Runs another program (single threaded).
- Inputs:
    - program_start: The program start address.
    - program_end: The program end address.
    - input_start: The context input start address.
    - input_end: The context input end address.
- Outputs:
    - result_start: The run called context start address.
    - result_end: The run called context end address.
    - runner_error_occured: Returned as true if there was an intepreter error.
