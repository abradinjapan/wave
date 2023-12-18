# Abstraction Calling

Abstraction calls are structured as follows:

- First, inputs are passed onto an input stack.
- Second, the instruction ID that is being jumped to is calculated (always the starting offset of a function).
- Third, the return address is pushed onto a return stack.
- Fourth, the function is jumped to.
- Fifth, the function context (variables) is created.
- Sixth, functions will get their input (in reverse order) using get_input instructions.
- Seventh, the function body will perform it's calculations.
- Eighth, the outputs will be passed on an output stack.
- Ninth, the current context is destroyed and the previous one is restored.
- Tenth, the program jumps to the return address.
- Eleventh, the outputs are pulled from the output stack (in reverse order).
