# Literals

All of the embedded data types that can be used.

## How To Use

There is only one instruction that accepts literals as a parameter, wave.set(1)(1).

The literal goes in the 0th input slot of wave.set(1)(1)

There are many types of literals.

## Integer Literal

An integer literal is a positive or negative integer number.

Positive integer literals are written like so:

> wave.set(wave.integer.0123456789)(destination_cell)

The 0123456789 represents any positive number that will fit into a 64 bit number.

In order to make it a negative number, simply place an 'n' before the number's digits.

Example:

> wave.set(wave.integer.n0123456789)(destination_cell)

Note, it is also possible to break up the numbers with underscores, like so:

> wave.set(wave.integer.1_000_000)(destination_cell)

## Binary Literal

A binary literal is a binary number written with zeros and ones.

Binary literals are written like so:

> wave.set(wave.binary.10101010)(destination_cell)

Note, it is also possible to break up the numbers with underscores, like so:

> wave.set(wave.binary.1000_1000)(destination_cell)

## Hexadecimal Literal

A hexadecimal literal is a positive number written in hexadecimal.

Hexadecimal literals are written like so:

> wave.set(wave.hexadecimal.0123456789abcdefABCDEF)(destination_cell)

Note, it is also possible to break up the numbers with underscores, like so:

> wave.set(wave.hexadecimal.00_cc_ff)(destination_cell)

## Boolean Literal

This is a literal that can be either true or false.

> wave.set(wave.boolean.false)(destination_cell)

> wave.set(wave.boolean.true)(destination_cell)

It is worth noting that true evaluates to "wave.integer.1" and false maps to "wave.integer.0".

## Instruction Literal

Instruction literals are literals that assist in the generation of wave instructions on a binary level.

They contain information like instruction layout, field sizes and opcodes.

### Instruction Layout

The literals that describe instruction layout are as follows:

They always start with "wave.instruction.".

Each field in the instruction has one literal to mark it's inter-instruction offset and one literal to mark the field's size in bytes.

The fields are:

- "type" : This is the opcode.
- "write_register_value" : For the set (write_cell) instruction, this contains the literal in binary.
- "input_0" : A cell_ID input.
- "input_1" : A cell_ID input.
- "input_2" : A cell_ID input.
- "input_3" : A cell_ID input.
- "output_0" : A cell_ID output.
- "output_1" : A cell_ID output.
- "output_2" : A cell_ID output.
- "jump_instruction_ID" : The instruction ID of a jump.

Offset Example:

> wave.set(wave.instruction.type.offset)(destination_cell)
> wave.set(wave.instruction.input_2.offset)(destination_cell)

Size Example:

> wave.set(wave.instruction.type.size)(destination_cell)
> wave.set(wave.instruction.input_2.size)(destination_cell)

There is also one last literal that is a part of the instruction metadata, the instruction's total size.

It is written as:

> wave.instruction.size

### Instruction Opcodes

Getting opcodes is very easy.

Simply take the instruction literal prefix "wave.instruction.", append "opcode." and append the binary name of the instruction (AKA, use the name in instructions.md without the parameter count).

Example:

> wave.set(wave.instruction.opcode.write_cell)(destination_register)
