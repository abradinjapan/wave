#pragma once

#include "parse.cpp"
#include "basic.cpp"

#include <vector>

namespace accounter {
    class header {
    public:
        std::string p_name;
        int p_input_count;
        int p_output_count;

        header() {
            p_name = "";
            p_input_count = 0;
            p_output_count = 0;
        }

        header(std::string name, int input_count, int output_count) {
            p_name = name;
            p_input_count = input_count;
            p_output_count = output_count;
        }
    };

    // check if two headers have matching signatures
    bool match_headers(header a, header b) {
        return (a.p_name == b.p_name) && (a.p_input_count == b.p_input_count) && (a.p_output_count == b.p_output_count);
    }

    // a type that keeps track of all header declarations
    class header_table {
    public:
        std::vector<header> p_headers;
        
        // check to see if a header is registered
        bool header_registered(header header) {
            // check each header
            for (basic::u64 i = 0; i < p_headers.size(); i++) {
                // check header for match
                if (match_headers(p_headers[i], header) == true) {
                    // match found
                    return true;
                }
            }

            // no matches found
            return false;
        }

        // attempt to register a new header, false means a duplicate was found, true means registration was successful
        bool try_register_header(header header) {
            // check to see if header already exists, if so, reject
            if (header_registered(header) == true) {
                // header could not be added
                return false;
            }

            // header was not already registered, add new header
            p_headers.push_back(header);

            // header added successfully
            return true;
        }
    };

    void get_header_table(parser::program& program, header_table& headers, bool& error_occured) {
        // add abstractions to header table
        for (basic::u64 i = 0; i < program.p_abstractions.size(); i++) {
            // try to add a header
            if (headers.try_register_header(header(program.p_abstractions[i].p_header.p_name.p_name_value, program.p_abstractions[i].p_header.p_inputs.size(), program.p_abstractions[i].p_header.p_outputs.size())) == false) {
                // error, header re-registered
                std::cout << "Accounting error, a header (with the same amounts of io) was defined more than once. : " << program.p_abstractions[i].p_header.p_name.p_name_value << std::endl;

                // set error
                error_occured = true;

                return;
            }
        }

        return;
    }

    void print_header_table(header_table table) {
        std::cout << "Header Table:" << std::endl;

        // print each header
        for (basic::u64 i = 0; i < table.p_headers.size(); i++) {
            // print header
            std::cout << "\tHeader: " << table.p_headers[i].p_name << "(" << table.p_headers[i].p_input_count << ")(" << table.p_headers[i].p_output_count << ")" << std::endl;
        }

        // new line
        std::cout << std::endl;
    }

    // verifies that all statements match a header
    bool verify_all_headers(header_table header_table, std::vector<parser::program>& programs) {
        // for each program
        for (basic::u64 program = 0; program < programs.size(); program++) {
            parser::program temp_program = programs[program];

            // for each abstraction
            for (basic::u64 abstraction = 0; abstraction < temp_program.p_abstractions.size(); abstraction++) {
                // for each statement in each abstraction
                for (basic::u64 statement = 0; statement < temp_program.p_abstractions[abstraction].p_scope.size(); statement++) {
                    // check header
                    if (temp_program.p_abstractions[abstraction].p_scope[statement].p_name.p_name_type == parser::name_type::is_abstraction_name && header_table.header_registered(header(temp_program.p_abstractions[abstraction].p_scope[statement].p_name.p_name_value, temp_program.p_abstractions[abstraction].p_scope[statement].p_inputs.size(), temp_program.p_abstractions[abstraction].p_scope[statement].p_outputs.size())) == false) {
                        // inform user of error
                        std::cout << "Accounting error, a header was not found during a header lookup: " << temp_program.p_abstractions[abstraction].p_scope[statement].p_name.p_name_value << "(" << temp_program.p_abstractions[abstraction].p_scope[statement].p_inputs.size() << ")" << "(" << temp_program.p_abstractions[abstraction].p_scope[statement].p_outputs.size() << ")" << std::endl;

                        // header not registered, error
                        return false;
                    }
                }
            }
        }

        // all headers matched correctly
        return true;
    }

    class variable {
    public:
        std::string p_name;
        basic::u64 p_declaration_index;

        variable(std::string name, basic::u64 declaration_index) {
            p_name = name;
            p_declaration_index = declaration_index;
        }
    };

    #define declared_as_input -2
    #define declared_as_output -1

    class variable_table {
    public:
        std::vector<variable> p_inputs;
        std::vector<variable> p_outputs;
        std::vector<variable> p_variables;

        // checks if the input list has a specific input
        bool table_contains_input(std::string name) {
            // for each variable
            for (basic::u64 input_ID = 0; input_ID < p_inputs.size(); input_ID++) {
                // check if it is a match
                if (p_inputs[input_ID].p_name == name) {
                    // match found, return found as true
                    return true;
                }
            }

            // not found
            return false;
        }

        // checks if the output list has a specific output
        bool table_contains_output(std::string name) {
            // for each variable
            for (basic::u64 output_ID = 0; output_ID < p_outputs.size(); output_ID++) {
                // check if it is a match
                if (p_outputs[output_ID].p_name == name) {
                    // match found, return found as true
                    return true;
                }
            }

            // not found
            return false;
        }

        // checks if the variable list has a specific variable
        bool table_contains_variable(std::string name) {
            // for each variable
            for (basic::u64 variable_ID = 0; variable_ID < p_variables.size(); variable_ID++) {
                // check if it is a match
                if (p_variables[variable_ID].p_name == name) {
                    // match found, return found as true
                    return true;
                }
            }

            // not found
            return false;
        }

        bool try_register_input(variable variable) {
            // check to see if the variable is already registered in the abstraction header
            if (table_contains_input(variable.p_name) == true || table_contains_output(variable.p_name) == true) {
                // if so, error
                return false;
            }

            // register input
            p_inputs.push_back(variable);

            // registered successfully
            return true;
        }

        bool try_register_output(variable variable) {
            // check to see if the variable is already registered in the abstraction header
            if (table_contains_input(variable.p_name) == true || table_contains_output(variable.p_name) == true) {
                // if so, error
                return false;
            }

            // register output
            p_outputs.push_back(variable);

            // registered successfully
            return true;
        }
        
        void register_variable(variable variable) {
            // check to see if the variable is already registered anywhere
            if (table_contains_input(variable.p_name) == true || table_contains_output(variable.p_name) == true || table_contains_variable(variable.p_name) == true) {
                // variable already declared
                return;
            }

            // register variable
            p_variables.push_back(variable);

            // registered successfully
            return;
        }
    };

    variable_table get_variable_table(parser::abstraction& abstraction, bool& error_occured) {
        variable_table output;

        // register the input variables
        for (basic::u64 input_ID = 0; input_ID < abstraction.p_header.p_inputs.size(); input_ID++) {
            // try to register input
            if (output.try_register_input(variable(abstraction.p_header.p_inputs[input_ID].p_name_value, declared_as_input)) == false) {
                // duplicate header argument error occured
                error_occured = true;

                // inform user of failure
                std::cout << "Accounting error, a duplicate input argument was detected. : " << abstraction.p_header.p_inputs[input_ID].p_name_value << std::endl;

                // return early
                return output;
            }
        }

        // register the output variables
        for (basic::u64 output_ID = 0; output_ID < abstraction.p_header.p_outputs.size(); output_ID++) {
            // try to register output
            if (output.try_register_output(variable(abstraction.p_header.p_outputs[output_ID].p_name_value, declared_as_output)) == false) {
                // duplicate header argument error occured
                error_occured = true;

                // inform user of failure
                std::cout << "Accounting error, a duplicate output argument was detected. : " << abstraction.p_header.p_outputs[output_ID].p_name_value << std::endl;

                // return early
                return output;
            }
        }

        // in each statement
        for (basic::u64 statement_ID = 0; statement_ID < abstraction.p_scope.size(); statement_ID++) {
            // ensure statement is an abstraction call
            if (abstraction.p_scope[statement_ID].p_type == parser::statement_type::is_abstraction_call) {
                // get variables
                for (basic::u64 output_ID = 0; output_ID < abstraction.p_scope[statement_ID].p_outputs.size(); output_ID++) {
                    // check to be sure that the name is a variable name
                    if (abstraction.p_scope[statement_ID].p_outputs[output_ID].p_name_type == parser::name_type::is_value_name) {
                        // try to add variable to variable list
                        output.register_variable(variable(abstraction.p_scope[statement_ID].p_outputs[output_ID].p_name_value, statement_ID));
                    }
                }
            }
        }

        // table created, return
        return output;
    }

    class offset {
    public:
        std::string p_name;
        basic::u64 p_statement_index;

        offset(std::string name, basic::u64 statement_index) {
            p_name = name;
            p_statement_index = statement_index;
        }
    };

    class offset_table {
    public:
        std::vector<offset> p_offsets;

        bool contains_offset(std::string name) {
            // check for offset
            for (basic::u64 i = 0; i < p_offsets.size(); i++) {
                // check for match
                if (p_offsets[i].p_name == name) {
                    // found match
                    return true;
                }
            }

            // no match found
            return false;
        }
    };

    offset_table get_offset_table(parser::abstraction& abstraction, bool& error_occured) {
        offset_table output;
        basic::u64 abstraction_call_ID = 0;
        basic::u64 offset_ID = 0;

        // for each statement
        for (basic::u64 statement_ID = 0; statement_ID < abstraction.p_scope.size(); statement_ID++) {
            // check to see if statement type is offset
            if (abstraction.p_scope[statement_ID].p_type == parser::statement_type::is_offset_declaration) {
                // check to see if offset is already declared
                if (output.contains_offset(abstraction.p_scope[statement_ID].p_name.p_name_value) == true) {
                    // if an offset is declared again, an error occured
                    error_occured = true;

                    // announce error
                    std::cout << "Accounting error, a duplicate offset declaration was found. : " << abstraction.p_scope[statement_ID].p_name.p_name_value << std::endl;

                    // quit
                    return output;
                }

                // otherwise declare offset
                output.p_offsets.push_back(offset(abstraction.p_scope[statement_ID].p_name.p_name_value, abstraction_call_ID));

                // next offset
                offset_ID++;
            // if the statement type is an abstraction call ID, advance to the next one
            } else if (abstraction.p_scope[statement_ID].p_type == parser::statement_type::is_abstraction_call) {
                // next abstraction call
                abstraction_call_ID++;
            }
        }

        return output;
    }

    enum literal_type {
        is_integer_literal,
        is_boolean_literal,
        is_instruction_literal,
        is_hexadecimal_literal,
        is_string_literal,
        is_invalid,
    };

    class literal {
    public:
        std::string p_name;
        literal_type p_type;
        basic::u64 p_integer_value;
        std::string p_string_value;
        int p_statement_ID;
        int p_argument_ID;

        literal(std::string name, parser::name_type type, int statement_ID, int argument_ID, bool& error_occured) {
            // set properties
            p_name = name;
            p_statement_ID = statement_ID;
            p_argument_ID = argument_ID;

            // set final type
            determine_literal_type(type, error_occured);
        }

        void determine_literal_type(parser::name_type type, bool& error_occured) {
            std::string integer_prefix = "wave.integer.";
            std::string boolean_prefix = "wave.boolean.";
            std::string instruction_prefix = "wave.instruction.";
            std::string hexadecimal_prefix = "wave.hexadecimal.";

            if (type == parser::name_type::is_string_literal) {
                p_string_value = p_name;

                p_type = literal_type::is_string_literal;
            } else if (type == parser::name_type::is_integer_literal) {
                p_integer_value = basic::convert_integer_literal_to_binary_integer(p_name.substr(integer_prefix.length()), error_occured);

                p_type = literal_type::is_integer_literal;
            } else if (type == parser::name_type::is_boolean_literal) {
                if (basic::string_contains_at(p_name, boolean_prefix.length(), "false")) {
                    p_integer_value = 0;
                } else if (basic::string_contains_at(p_name, boolean_prefix.length(), "true")) {
                    p_integer_value = 1;
                } else {
                    // error
                    std::cout << "Accounting error, unrecognized boolean literal type: " << p_name << std::endl;
                    error_occured = true;
                    p_integer_value = -1;
                }

                p_type = literal_type::is_boolean_literal;
            } else if (type == parser::name_type::is_instruction_literal) {
                p_integer_value = literals::convert_instruction_literal_suffix_to_value(p_name.substr(instruction_prefix.length()));

                p_type = literal_type::is_instruction_literal;
            } else if (type == parser::name_type::is_hexadecimal_literal) {
                p_integer_value = basic::convert_hexadecimal_literal_to_binary_integer(p_name.substr(hexadecimal_prefix.length()), error_occured);

                p_type = literal_type::is_hexadecimal_literal;
            } else {
                p_integer_value = -1;
                error_occured = true;
            }
        }
    };

    class literal_table {
    public:
        std::vector<literal> p_literals;
    };

    literal_table get_literal_table(parser::abstraction& abstraction, bool& error_occured) {
        literal_table output;

        // scan for literals
        // for each statement
        for (basic::u64 statement_ID = 0; statement_ID < abstraction.p_scope.size(); statement_ID++) {
            // check to see if statement is abstraction call type
            if (abstraction.p_scope[statement_ID].p_type == parser::statement_type::is_abstraction_call) {
                // check statement inputs for literals
                for (basic::u64 input_ID = 0; input_ID < abstraction.p_scope[statement_ID].p_inputs.size(); input_ID++) {
                    // check statement type
                    if (abstraction.p_scope[statement_ID].p_inputs[input_ID].p_name_type == parser::name_type::is_integer_literal || abstraction.p_scope[statement_ID].p_inputs[input_ID].p_name_type == parser::name_type::is_boolean_literal || abstraction.p_scope[statement_ID].p_inputs[input_ID].p_name_type == parser::name_type::is_instruction_literal || abstraction.p_scope[statement_ID].p_inputs[input_ID].p_name_type == parser::name_type::is_hexadecimal_literal || abstraction.p_scope[statement_ID].p_inputs[input_ID].p_name_type == parser::name_type::is_string_literal) {
                        // add literal
                        output.p_literals.push_back(literal(abstraction.p_scope[statement_ID].p_inputs[input_ID].p_name_value, abstraction.p_scope[statement_ID].p_inputs[input_ID].p_name_type, statement_ID, input_ID, error_occured));
                    }
                }

                // check statement outputs for illegal literals (literals cannot be outputs)
                for (basic::u64 output_ID = 0; output_ID < abstraction.p_scope[statement_ID].p_outputs.size(); output_ID++) {
                    // check for literal
                    if (abstraction.p_scope[statement_ID].p_outputs[output_ID].p_name_type != parser::name_type::is_value_name) {
                        // inform user of error
                        std::cout << "Accounting error, an non variable name was found in the outputs of an abstraction call.: " << abstraction.p_scope[statement_ID].p_outputs[output_ID].p_name_value << std::endl;
                        
                        // set error
                        error_occured = true;

                        // return
                        return output;
                    }
                }
            }
        }

        // success
        return output;
    }

    namespace skeleton {
        enum argument_type {
            is_invalid,
            is_input,
            is_output,
            is_variable,
            is_integer_literal,
            is_offset,
            is_boolean_literal,
            is_instruction_literal,
            is_hexadecimal_literal,
            is_string_literal,
        };

        class argument {
        public:
            argument_type p_type;
            int p_ID;

            argument(argument_type type, int ID) {
                p_type = type;
                p_ID = ID;
            }
        };

        class call {
        public:
            int p_header_ID;
            std::vector<argument> p_inputs;
            std::vector<argument> p_outputs;
        };

        enum statement_type {
            is_call_statement,
            is_offset_statement,
            is_invalid_statement,
        };

        class statement {
        public:
            statement_type p_type;
            int p_ID;

            statement(statement_type type, int ID) {
                p_type = type;
                p_ID = ID;
            }
        };

        class abstraction {
        public:
            variable_table p_variables;
            bool p_has_scope;
            offset_table p_offsets;
            literal_table p_literals;
            std::vector<call> p_calls;
            std::vector<statement> p_statement_map;

            // lookup variable in variable table
            argument lookup_variable_by_name(std::string name_value, bool& error_occured) {
                // check for match from inputs
                for (basic::u64 input_ID = 0; input_ID < p_variables.p_inputs.size(); input_ID++) {
                    // check for variable name
                    if (p_variables.p_inputs[input_ID].p_name == name_value) {
                        // match found
                        return argument(argument_type::is_input, input_ID);
                    }
                }

                // check for match from outputs
                for (basic::u64 output_ID = 0; output_ID < p_variables.p_outputs.size(); output_ID++) {
                    // check for variable name
                    if (p_variables.p_outputs[output_ID].p_name == name_value) {
                        // match found
                        return argument(argument_type::is_output, output_ID);
                    }
                }

                // check for match from variables
                for (basic::u64 variable_ID = 0; variable_ID < p_variables.p_variables.size(); variable_ID++) {
                    // check for variable name
                    if (p_variables.p_variables[variable_ID].p_name == name_value) {
                        // match found
                        return argument(argument_type::is_variable, variable_ID);
                    }
                }

                // no match found (should not be possible, but handled anyways)
                error_occured = true;

                // inform user of error
                std::cout << "Accounting error, a variable was not found during lookup: " << name_value << std::endl;

                // return invalid argument
                return argument(argument_type::is_invalid, -1);
            }

            // lookup offset in offset table
            argument lookup_offset_by_name(std::string name, bool& error_occured) {
                // lookup offset
                for (basic::u64 offset_ID = 0; offset_ID < p_offsets.p_offsets.size(); offset_ID++) {
                    // check for match
                    if (p_offsets.p_offsets[offset_ID].p_name == name) {
                        // match found
                        return argument(argument_type::is_offset, offset_ID);
                    }
                }

                // offset not found (should not be possible, but handled anyways)
                error_occured = true;

                // inform user of error
                std::cout << "Accounting error, an offset was not found during lookup. " << name << std::endl;

                // return invalid argument
                return argument(argument_type::is_invalid, -1);
            }

            argument_type convert_literal_type_to_argument_type(literal_type type) {
                switch (type) {
                case literal_type::is_integer_literal:
                    return argument_type::is_integer_literal;
                case literal_type::is_boolean_literal:
                    return argument_type::is_boolean_literal;
                case literal_type::is_instruction_literal:
                    return argument_type::is_instruction_literal;
                case literal_type::is_hexadecimal_literal:
                    return argument_type::is_hexadecimal_literal;
                case literal_type::is_string_literal:
                    return argument_type::is_string_literal;
                default:
                    return argument_type::is_invalid;
                }
            }

            argument lookup_literal_by_ID(int statement_ID, int io_ID, bool& error_occured) {
                // lookup literal
                for (basic::u64 literal_ID = 0; literal_ID < p_literals.p_literals.size(); literal_ID++) {
                    // check for match
                    if (p_literals.p_literals[literal_ID].p_statement_ID == statement_ID && p_literals.p_literals[literal_ID].p_argument_ID == io_ID) {
                        // return match
                        return argument(convert_literal_type_to_argument_type(p_literals.p_literals[literal_ID].p_type), literal_ID);
                    }
                }

                // literal not found (should not be possible, but handled anyways)
                error_occured = true;

                // inform user of error
                std::cout << "Accounting error, a literal was not found during lookup. [ " << statement_ID << " " << io_ID << " ]" << std::endl;

                // return invalid argument
                return argument(argument_type::is_invalid, -1);
            }
        };

        class skeleton {
        public:
            header_table p_header_table;
            std::vector<abstraction> p_abstractions;

            void get_skeleton(std::vector<parser::program>& programs, bool& error_occured) {
                // get predefined abstractions
                add_predefined_abstractions();

                // get header table from all files
                for (basic::u64 parse_tree = 0; parse_tree < programs.size(); parse_tree++) {
                    get_header_table(programs[parse_tree], p_header_table, error_occured);
                }

                // verify header table
                if (verify_all_headers(p_header_table, programs) == false) {
                    std::cout << "Accounting error, headers and statements do not all match." << std::endl;
                    error_occured = true;

                    return;
                }

                // get all the programs from each file
                for (basic::u64 i = 0; i < programs.size(); i++) {
                    parser::program program = programs[i];
                    
                    // get abstractions from file
                    for (basic::u64 j = 0; j < program.p_abstractions.size(); j++) {
                        // create new abstraction
                        p_abstractions.push_back(abstraction());

                        // check for scope
                        if (program.p_abstractions[j].p_type == parser::abstraction_type::is_code_defined) {
                            p_abstractions[p_abstractions.size() - 1].p_has_scope = true;
                        } else {
                            p_abstractions[p_abstractions.size() - 1].p_has_scope = false;
                        }

                        // get variable table
                        p_abstractions[p_abstractions.size() - 1].p_variables = get_variable_table(program.p_abstractions[j], error_occured);

                        // check for error
                        if (error_occured) {
                            return;
                        }

                        // get abstraction body
                        if (p_abstractions[p_abstractions.size() - 1].p_has_scope == true) {
                            // get offset table
                            p_abstractions[p_abstractions.size() - 1].p_offsets = get_offset_table(program.p_abstractions[j], error_occured);

                            // check for error
                            if (error_occured) {
                                return;
                            }

                            // get literal table
                            p_abstractions[p_abstractions.size() - 1].p_literals = get_literal_table(program.p_abstractions[j], error_occured);

                            // check for error
                            if (error_occured) {
                                return;
                            }

                            // get call table
                            p_abstractions[p_abstractions.size() - 1].p_calls = get_call_table(program.p_abstractions[j], p_abstractions.size() - 1, error_occured);

                            // check for error
                            if (error_occured) {
                                return;
                            }

                            // get statement map
                            p_abstractions[p_abstractions.size() - 1].p_statement_map = get_statement_map(program.p_abstractions[j], error_occured);

                            // check for error
                            if (error_occured) {
                                return;
                            }
                        }
                    }
                }

                return;
            }

            void print_skeleton() {
                // print skeleton
                std::cout << "Skeletons:" << std::endl;

                // print all abstractions
                for (basic::u64 abstraction_ID = 0; abstraction_ID < p_abstractions.size(); abstraction_ID++) {
                    // print abstraction name for clarity
                    std::cout << "\tAbstraction [ " << abstraction_ID << " ] " << p_header_table.p_headers[abstraction_ID].p_name << "(" << p_header_table.p_headers[abstraction_ID].p_input_count << ")" << "(" << p_header_table.p_headers[abstraction_ID].p_output_count << "):" << std::endl;

                    // print tables
                    if (p_abstractions[abstraction_ID].p_has_scope) {
                        print_variable_table(p_abstractions[abstraction_ID].p_variables);
                        print_offset_table(p_abstractions[abstraction_ID].p_offsets);
                        print_literal_table(p_abstractions[abstraction_ID].p_literals);
                        print_call_table(p_abstractions[abstraction_ID].p_calls);
                        print_statement_map(p_abstractions[abstraction_ID].p_statement_map);
                    }
                }
            }

        private:
            // add predefined abstraction
            void synthesize_header_only_abstraction(std::string name, int input_count, int output_count) {
                // create header
                p_header_table.try_register_header(header(name, input_count, output_count));

                // setup body
                p_abstractions.push_back(abstraction());
                p_abstractions[p_abstractions.size() - 1].p_has_scope = false;

                // setup inputs
                for (int64_t i = 0; i < input_count; i++) {
                    p_abstractions[p_abstractions.size() - 1].p_variables.p_inputs.push_back(variable(std::to_string(i), -2));
                }

                // setup outputs
                for (int64_t i = 0; i < output_count; i++) {
                    p_abstractions[p_abstractions.size() - 1].p_variables.p_outputs.push_back(variable(std::to_string(i), -1));
                }
            }

            // add predefined abstractions to tables
            void add_predefined_abstractions() {
                // add predefined abstractions to header table
                synthesize_header_only_abstraction("wave.quit", 0, 0);
                synthesize_header_only_abstraction("wave.set", 1, 1);
                synthesize_header_only_abstraction("wave.copy", 1, 1);
                synthesize_header_only_abstraction("wave.copy.string", 1, 3);
                synthesize_header_only_abstraction("wave.print.number", 1, 0);
                synthesize_header_only_abstraction("wave.print.character", 1, 0);
                synthesize_header_only_abstraction("wave.get_console_input", 0, 2);
                synthesize_header_only_abstraction("wave.create_new_context", 1, 0);
                synthesize_header_only_abstraction("wave.restore_old_context", 0, 0);
                synthesize_header_only_abstraction("wave.pass_input", 1, 0);
                synthesize_header_only_abstraction("wave.get_input", 0, 1);
                synthesize_header_only_abstraction("wave.pass_output", 1, 0);
                synthesize_header_only_abstraction("wave.get_output", 0, 1);
                synthesize_header_only_abstraction("wave.jump_to_abstraction", 1, 0);
                synthesize_header_only_abstraction("wave.jump_from_abstraction", 0, 0);
                synthesize_header_only_abstraction("wave.jump", 2, 0);
                synthesize_header_only_abstraction("wave.get_instruction_index", 0, 1);
                synthesize_header_only_abstraction("wave.request_memory", 1, 3);
                synthesize_header_only_abstraction("wave.return_memory", 2, 0);
                synthesize_header_only_abstraction("wave.cell_to_address", 3, 1);
                synthesize_header_only_abstraction("wave.address_to_cell", 2, 2);
                synthesize_header_only_abstraction("wave.buffer_to_file", 4, 1);
                synthesize_header_only_abstraction("wave.file_to_buffer", 2, 3);
                synthesize_header_only_abstraction("wave.integer.add", 2, 1);
                synthesize_header_only_abstraction("wave.integer.subtract", 2, 1);
                synthesize_header_only_abstraction("wave.integer.multiply", 2, 1);
                synthesize_header_only_abstraction("wave.integer.divide", 2, 2);
                synthesize_header_only_abstraction("wave.integer.modulous", 2, 2);
                synthesize_header_only_abstraction("wave.integer.within_range", 3, 1);
                synthesize_header_only_abstraction("wave.boolean.not", 1, 1);
                synthesize_header_only_abstraction("wave.get_context_input", 0, 2);
                synthesize_header_only_abstraction("wave.pass_context_output", 2, 0);
                synthesize_header_only_abstraction("wave.run", 4, 3);
            }

        public:
            // lookup header in header table
            int lookup_header_by_name(std::string header_name, bool& error_occured) {
                // lookup header
                for (basic::u64 header_ID = 0; header_ID < p_header_table.p_headers.size(); header_ID++) {
                    // match found
                    if (p_header_table.p_headers[header_ID].p_name == header_name) {
                        return header_ID;
                    }
                }

                // no match found (should not be possible, but here just in case)
                error_occured = true;

                // inform user of error
                std::cout << "Accounting error, a header was not found during a header lookup: " <<  header_name << std::endl;

                // return invalid argument
                return -1;
            }

        private:
            // get statement table
            std::vector<call> get_call_table(parser::abstraction& abstraction, int abstraction_ID, bool& error_occured) {
                std::vector<call> output;

                // get each abstraction call statement
                for (basic::u64 statement_ID = 0; statement_ID < abstraction.p_scope.size(); statement_ID++) {
                    // make sure that the statement is an abstraction call
                    if (abstraction.p_scope[statement_ID].p_type == parser::statement_type::is_abstraction_call) {
                        // create new abstraction call statement
                        output.push_back(call());

                        // get statement name
                        output[output.size() - 1].p_header_ID = lookup_header_by_name(abstraction.p_scope[statement_ID].p_name.p_name_value, error_occured);

                        // check for error
                        if (error_occured) {
                            return output;
                        }

                        // get inputs
                        for (basic::u64 input_ID = 0; input_ID < abstraction.p_scope[statement_ID].p_inputs.size(); input_ID++) {
                            // get argument by type
                            switch (abstraction.p_scope[statement_ID].p_inputs[input_ID].p_name_type) {
                            // is variable
                            case parser::name_type::is_value_name:
                                // look up variable
                                output[output.size() - 1].p_inputs.push_back(p_abstractions[abstraction_ID].lookup_variable_by_name(abstraction.p_scope[statement_ID].p_inputs[input_ID].p_name_value, error_occured));

                                // check for error
                                if (error_occured) {
                                    return output;
                                }

                                break;
                            // is offset
                            case parser::name_type::is_offset:
                                // lookup offset
                                output[output.size() - 1].p_inputs.push_back(p_abstractions[abstraction_ID].lookup_offset_by_name(abstraction.p_scope[statement_ID].p_inputs[input_ID].p_name_value, error_occured));

                                // check for error
                                if (error_occured) {
                                    return output;
                                }

                                break;
                            // is integer literal
                            case parser::name_type::is_integer_literal:
                                // lookup literal
                                output[output.size() - 1].p_inputs.push_back(p_abstractions[abstraction_ID].lookup_literal_by_ID(statement_ID, input_ID, error_occured));

                                // check for error
                                if (error_occured) {
                                    return output;
                                }

                                break;
                            // is boolean literal
                            case parser::name_type::is_boolean_literal:
                                // lookup literal
                                output[output.size() - 1].p_inputs.push_back(p_abstractions[abstraction_ID].lookup_literal_by_ID(statement_ID, input_ID, error_occured));

                                // check for error
                                if (error_occured) {
                                    return output;
                                }

                                break;
                            // is instruction literal
                            case parser::name_type::is_instruction_literal:
                                // lookup literal
                                output[output.size() - 1].p_inputs.push_back(p_abstractions[abstraction_ID].lookup_literal_by_ID(statement_ID, input_ID, error_occured));

                                // check for error
                                if (error_occured) {
                                    return output;
                                }

                                break;
                            // is hexadecimal literal
                            case parser::name_type::is_hexadecimal_literal:
                                // lookup literal
                                output[output.size() - 1].p_inputs.push_back(p_abstractions[abstraction_ID].lookup_literal_by_ID(statement_ID, input_ID, error_occured));

                                // check for error
                                if (error_occured) {
                                    return output;
                                }

                                break;
                            // is string literal
                            case parser::name_type::is_string_literal:
                                // lookup literal
                                output[output.size() - 1].p_inputs.push_back(p_abstractions[abstraction_ID].lookup_literal_by_ID(statement_ID, input_ID, error_occured));

                                // check for error
                                if (error_occured) {
                                    return output;
                                }

                                break;
                            // not valid
                            default:
                                std::cout << "Accounting error, an illegal name type was found in statement inputs." << std::endl;
                                error_occured = true;

                                return output;
                            }
                        }

                        // get outputs
                        for (basic::u64 output_ID = 0; output_ID < abstraction.p_scope[statement_ID].p_outputs.size(); output_ID++) {
                            // get argument by type
                            switch (abstraction.p_scope[statement_ID].p_outputs[output_ID].p_name_type) {
                            // is variable
                            case parser::name_type::is_value_name:
                                // look up variable
                                output[output.size() - 1].p_outputs.push_back(p_abstractions[abstraction_ID].lookup_variable_by_name(abstraction.p_scope[statement_ID].p_outputs[output_ID].p_name_value, error_occured));

                                // check for error
                                if (error_occured) {
                                    return output;
                                }

                                break;
                            // not valid
                            default:
                                std::cout << "Accounting error, an illegal name type was found in statement outputs." << std::endl;
                                return output;
                            }
                        }
                    }
                }

                // success
                return output;
            }

            // get statement map
            std::vector<statement> get_statement_map(parser::abstraction& parser_abstraction, bool& error_occured) {
                std::vector<statement> output;
                basic::u64 call_index = 0;
                basic::u64 offset_index = 0;

                // get each statement type
                for (basic::u64 statement_ID = 0; statement_ID < parser_abstraction.p_scope.size(); statement_ID++) {
                    // check for the statement type
                    if (parser_abstraction.p_scope[statement_ID].p_type == parser::statement_type::is_abstraction_call) {
                        // add the call statement
                        output.push_back(statement(statement_type::is_call_statement, call_index));

                        // next call
                        call_index++;
                    } else if (parser_abstraction.p_scope[statement_ID].p_type == parser::statement_type::is_offset_declaration) {
                        // add the offset statement
                        output.push_back(statement(statement_type::is_offset_statement, offset_index));

                        // next offset
                        offset_index++;
                    } else {
                        output.push_back(statement(statement_type::is_invalid_statement, statement_ID));

                        error_occured = true;

                        std::cout << "Accounting error, an illegal statement type was found while creating the statement map." << std::endl;

                        return output;
                    }
                }

                // success
                return output;
            }

            // print variable table
            void print_variable_table(variable_table& table) {
                // start information
                std::cout << "\t\tVariable Table:" << std::endl;

                // print variables
                std::cout << "\t\t\tAbstraction Variables:" << std::endl;
                for (basic::u64 i = 0; i < table.p_variables.size(); i++) {
                    std::cout << "\t\t\t\tVariable: " << table.p_variables[i].p_name << " [ " << (long long)table.p_variables[i].p_declaration_index << " ]" << std::endl;
                }
            }

            // print offset table
            void print_offset_table(offset_table& table) {
                // print header
                std::cout << "\t\tOffset Table:" << std::endl;

                // print offsets
                for (basic::u64 i = 0; i < table.p_offsets.size(); i++) {
                    // print offset
                    std::cout << "\t\t\t" << table.p_offsets[i].p_name << " [ " << table.p_offsets[i].p_statement_index << " ]" << std::endl;
                }
            }

            // print literal table
            void print_literal_table(literal_table& table) {
                // print header
                std::cout << "\t\tLiteral Table:" << std::endl;
        
                // print literals
                for (basic::u64 i = 0; i < table.p_literals.size(); i++) {
                    // print literal
                    std::cout << "\t\t\t" << table.p_literals[i].p_name << " ( ";

                    // if string literal
                    if (table.p_literals[i].p_type == literal_type::is_string_literal) {
                        std::cout << "\"\'" << table.p_literals[i].p_string_value << "\'\"";
                    // is integer based literal
                    } else {
                        std::cout << table.p_literals[i].p_integer_value;
                    }

                    // print remaining data
                    std::cout << " ); Found At: [ " << table.p_literals[i].p_statement_ID << " " << table.p_literals[i].p_argument_ID << " ]" << std::endl;
                }
            }

            // print call table
            void print_call_table(std::vector<call>& table) {
                // print header
                std::cout << "\t\tCall Table:" << std::endl;

                // print statements
                for (basic::u64 i = 0; i < table.size(); i++) {
                    // print statement header
                    std::cout << "\t\t\t" << table[i].p_header_ID << " : ";

                    // print inputs
                    for (basic::u64 j = 0; j < table[i].p_inputs.size(); j++) {
                        // start input
                        std::cout << "[ ";

                        // print input body
                        switch (table[i].p_inputs[j].p_type) {
                        case argument_type::is_input:
                            std::cout << "is_input" << " -> " << table[i].p_inputs[j].p_ID;
                            break;
                        case argument_type::is_output:
                            std::cout << "is_output" << " -> " << table[i].p_inputs[j].p_ID;
                            break;
                        case argument_type::is_variable:
                            std::cout << "is_variable" << " -> " << table[i].p_inputs[j].p_ID;
                            break;
                        case argument_type::is_offset:
                            std::cout << "is_offset" << " -> " << table[i].p_inputs[j].p_ID;
                            break;
                        case argument_type::is_integer_literal:
                            std::cout << "is_integer_literal" << " -> " << table[i].p_inputs[j].p_ID;
                            break;
                        case argument_type::is_boolean_literal:
                            std::cout << "is_boolean_literal" << " -> " << table[i].p_inputs[j].p_ID;
                            break;
                        case argument_type::is_instruction_literal:
                            std::cout << "is_instruction_literal" << " -> " << table[i].p_inputs[j].p_ID;
                            break;
                        case argument_type::is_hexadecimal_literal:
                            std::cout << "is_hexadecimal_literal" << " -> " << table[i].p_inputs[j].p_ID;
                            break;
                        case argument_type::is_string_literal:
                            std::cout << "is_string_literal" << " -> " << table[i].p_inputs[j].p_ID;
                            break;
                        default:
                            std::cout << "is_invalid" << " -> " << table[i].p_inputs[j].p_ID;
                            break;
                        }

                        // print input end
                        std::cout << " ]";
                    }

                    // print empty marker if there are no inputs
                    if (table[i].p_inputs.size() == 0) {
                        std::cout << "[ empty ]";
                    }

                    // continue statement header
                    std::cout << " : ";

                    // print outputs
                    for (basic::u64 j = 0; j < table[i].p_outputs.size(); j++) {
                        // start output
                        std::cout << "[ ";

                        switch (table[i].p_outputs[j].p_type) {
                        case argument_type::is_input:
                            std::cout << "is_input" << " -> " << table[i].p_outputs[j].p_ID;
                            break;
                        case argument_type::is_output:
                            std::cout << "is_output" << " -> " << table[i].p_outputs[j].p_ID;
                            break;
                        case argument_type::is_variable:
                            std::cout << "is_variable" << " -> " << table[i].p_outputs[j].p_ID;
                            break;
                        default:
                            std::cout << "is_invalid" << " -> " << table[i].p_outputs[j].p_ID;
                            break;
                        }

                        // print output end
                        std::cout << " ]";
                    }

                    // print empty marker if there are no outputs
                    if (table[i].p_outputs.size() == 0) {
                        std::cout << "[ empty ]";
                    }

                    // finish statement header
                    std::cout << std::endl;
                }
            }

            // print statement map
            void print_statement_map(std::vector<statement>& statements) {
                // print header
                std::cout << "\t\tStatement Map:" << std::endl;

                // print statements
                for (basic::u64 statement_ID = 0; statement_ID < statements.size(); statement_ID++) {
                    // print indentation
                    std::cout << "\t\t\t";

                    // print statement based on type
                    if (statements[statement_ID].p_type == accounter::skeleton::statement_type::is_call_statement) {
                        // print as call
                        std::cout << "Call [ " << statements[statement_ID].p_ID << " ]" << std::endl;
                    } else if (statements[statement_ID].p_type == accounter::skeleton::statement_type::is_offset_statement) {
                        // print as offset
                        std::cout << "Offset [ " << statements[statement_ID].p_ID << " ]" << std::endl;
                    }
                }
            }
        };
    }
}
