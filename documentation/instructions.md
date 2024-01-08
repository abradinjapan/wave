# Instruction List

## quit

wave.quit(0)(0)

- Quits the current program.
- Inputs:
    - (none)
- Outputs:
    - (none)

## write_cell

wave.set(1)(1)

- Writes a constant to a cell.
- Inputs:
    - value: The constant value to be written.
- Outputs:
    - destination: The cell ID of the value's destination.

## copy_cell

wave.copy(1)(1)

- Overwrites one cell with another cell.
- Inputs:
    - source: The source cell ID.
- Outputs:
    - destination: The destination cell ID.

## load_string

wave.load.string(1)(3)

- Duplicates a string into a buffer (note, the buffer from load_string must be deallocated).
- Note: When used as wave.load.string(1)(3), the input argument should be a string literal.
        But when used as the load_string instruction, the string_ID should be used.
- Inputs:
    - string_ID: The ID of the string being used.
- Outputs:
    - destination_start: The first address of the string buffer.
    - destination_end: The last address of the string buffer.
    - error: Returns true if the string could not be allocated.

## print_cell_as_number

wave.print.number(1)(0)

- Prints the cell as an integer number.
- Inputs:
    - source: The source cell ID.
- Outputs:
    - (none)

## print_cell_as_character

wave.print.character(1)(0)

- Prints the cell as an ASCII character.
- Inputs:
    - source: The source cell ID.
- Outputs:
    - (none)

## get_console_input

wave.get_console_input(0)(2)

- Gets one line of user input from the terminal.
- Inputs:
    - (none)
- Outputs:
    - start: The starting address of the text input buffer.
    - end: The ending address of the text input buffer.

## create_new_context

wave.create_new_context(1)(0)

- Creates a new context.
- Inputs:
    - cell_count: The amount of cells in the context.
- Outputs:
    - (none)

## restore_old_context

wave.restore_old_context(0)(0)

- Restores the previous context.
- Inputs:
    - (none)
- Outputs:
    - (none)

## pass_input

wave.pass_input(1)(0)

- Passes one cell to the top of the input stack.
- Inputs:
    - parameter: The cell ID of the parameter to be passed.
- Outputs:
    - (none)

## get_input

wave.get_input(0)(1)

- Gets the top cell from the input stack.
- Inputs:
    - (none)
- Outputs:
    - parameter: The cell ID of the parameter to be recieved.

## pass_output

wave.pass_output(1)(0)

- Passes one cell to the top of the output stack.
- Inputs:
    - parameter: The cell ID of the parameter to be passed.
- Outputs:
    - (none)

## get_output

wave.get_output(0)(1)

- Gets the top cell from the output stack.
- Inputs:
    - (none)
- Outputs:
    - parameter: The cell ID of the parameter to be recieved.

## jump_to_abstraction

wave.jump_to_abstraction(1)(0)

- Pushes the next instruction onto the return address stack and jumps to the designated abstraction.
- Inputs:
    - destination_instruction: The instruction ID being jumped to.
- Outputs:
    - (none)

## jump_from_abstraction


wave.jump_from_abstraction(0)(0)

- Pops the top return address from the return address stack and jumps to it.
- Inputs:
    - (none)
- Outputs:
    - (none)

## jump

wave.jump(2)(0)

- Conditionally jumps to any instruction.
- Inputs:
    - condition: A boolean that determines whether or not to jump.
    - instruction_ID: The instruction being jumped to.
- Outputs:
    - (none)

## get_instruction_index

wave.get_instruction_index(0)(1)

- Gets the current instruction ID and saves it into a cell.
- Inputs:
    - destination: The destination cell of the instruction ID.
- Outputs:
    - (none)

## request_memory

wave.request_memory(1)(3)

- Attempts to allocate memory, returning a buffer.
- Inputs:
    - length: The size of the buffer.
- Outputs:
    - start_address: The start of the new buffer.
    - end_address: The end of the new buffer.
    - error: An error boolean.

## return_memory

wave.return_memory(2)(0)

- Frees a buffer.
- Inputs:
    - start_address: The start of the buffer.
    - end_address: The end of the buffer.
- Outputs:
    - (none)

## cell_to_address

wave.cell_to_address(3)(1)

- Writes a specified amount of bytes from a cell to a buffer.
- Inputs:
    - source: The source cell ID.
    - byte_size: The amount of bytes to be copied (1 -> 8).
    - destination: The address being written to.
- Outputs:
    - error_code: An error boolean.

## address_to_cell

wave.address_to_cell(2)(2)

- Reads a specified amount of bytes from an address to a buffer.
- Inputs:
    - source: The address being read from.
    - byte_size: the amount of bytes being copied (1 -> 8).
- Outputs:
    - destination: The destination address.
    - error_code: An error boolean.

## buffer_to_file

wave.buffer_to_file(4)(1)

- Overwrites a file with a buffer.
- Inputs:
    - source_start: The start address of the data buffer.
    - source_end: The end address of the data buffer.
    - file_path_start: The start address of the file path buffer.
    - file_path_end: The end address of the file path buffer.
- Outputs:
    - error_code: An error boolean.

## file_to_buffer

wave.file_to_buffer(2)(3)

- Reads an entire file into a buffer.
- Inputs:
    - file_path_start: The start address of the file path buffer.
    - file_path_end: The end address of the file path buffer.
- Outputs:
    - source_start: The start address of the data buffer.
    - source_end: The end address of the data buffer.
    - error_code: An error boolean.

## integer_add

wave.integer.add(2)(1)

- Performs integer addition.
- Inputs:
    - source_1: The first parameter cell ID.
    - source_2: The second parameter cell ID.
- Outputs:
    - destination: The result cell ID.

## integer_subtract

wave.integer.subtract(2)(1)

- Performs integer subtraction.
- Inputs:
    - source_1: The first parameter cell ID.
    - source_2: The second parameter cell ID.
- Outputs:
    - destination: The result cell ID.

## integer_multiply

wave.integer.multiply(2)(1)

- Performs integer multiplication.
- Inputs:
    - source_1: The first parameter cell ID.
    - source_2: The second parameter cell ID.
- Outputs:
    - destination: The result cell ID.

## integer_divide

wave.integer.divide(2)(2)

- Performs integer division.
- Inputs:
    - source_1: The first parameter cell ID.
    - source_2: The second parameter cell ID.
- Outputs:
    - destination: The result cell ID.
    - error_code: Set to true if the divisor is zero.

## integer_modulous

wave.integer.modulous(2)(2)

- Performs integer modulous.
- Inputs:
    - source_1: The first parameter cell ID.
    - source_2: The second parameter cell ID.
- Outputs:
    - destination: The result cell ID.
    - error_code: Set to true if the divisor is zero.

## integer_within_range

wave.integer.within_range(3)(1)

- Checks if an integer is within an unsigned integer range.
- Inputs:
    - low_bound: The lowest (inclusive) possible value.
    - value: The value being checked.
    - high_bound: The highest (inclusive) possible value.
- Outputs:
    - destination: A boolean that says if the number was in range or not.

## boolean_not

wave.integer.boolean_not(1)(1)

- Inverts a boolean.
- Inputs:
    - source: The source boolean.
- Outputs:
    destination: The destination cell ID.

## get_context_input

wave.get_context_input(0)(2)

- Gets the context level input from the caller context.
- Inputs:
    - (none)
- Outputs:
    - destination_start: The starting address of the input.
    - destination_end: The ending address of the input.

## pass_context_output

wave.pass_context_output(2)(0)

- Gets the context level input from the caller context.
- Inputs:
    - (none)
- Outputs:
    - destination_start: The starting address of the input.
    - destination_end: The ending address of the input.

# run

wave.run(4)(3)

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
