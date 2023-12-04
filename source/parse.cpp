#pragma once

#include <iostream>

#include "lex.cpp"
#include "basic.cpp"
#include "run.cpp"

namespace parser {
    enum name_type {
        is_uninitialized,
        is_value_name,
        is_integer_literal,
        is_abstraction_name,
        is_offset,
        is_boolean_literal,
        is_instruction_literal,
    };

    class name {
    public:
        name_type p_name_type;
        std::string p_name_value;

        // default constructor
        name() {
            p_name_type = name_type::is_uninitialized;
            p_name_value = "";
        }

        // explicit constructor
        name(name_type type, std::string value) {
            p_name_type = type;
            p_name_value = value;
        }
    };

    enum statement_type {
        is_not_valid,
        is_abstraction_call,
        is_offset_declaration,
    };

    class statement {
    public:
        statement_type p_type;
        name p_name;
        std::vector<name> p_inputs;
        std::vector<name> p_outputs;

        // default constructor
        statement() {
            p_type = statement_type::is_not_valid;
            p_name = name();
            p_inputs = std::vector<name>();
            p_outputs = std::vector<name>();
        }

        // initialize statement except for io
        statement(name name_value) {
            p_name = name_value;
        }

        // initialize statement as abstraction call
        statement(name name_value, std::vector<name> inputs, std::vector<name> outputs) {
            p_name = name_value;
            p_inputs = inputs;
            p_outputs = outputs;
        }
    };

    enum abstraction_type {
        is_undefined,
        is_code_defined,
        is_compiler_defined,
    };

    class abstraction {
    public:
        abstraction_type p_type;
        statement p_header;
        std::vector<statement> p_scope;

        abstraction() {
            p_type = is_undefined;
            p_header = statement();
            p_scope = std::vector<statement>();
        }
    };

    class program {
    public:
        std::vector<abstraction> p_abstractions;
    };

    bool string_starts_with(std::string& string, std::string starts_with) {
        // check if length of start with is less than length of string
        if (string.length() <= starts_with.length()) {
            return false;
        }

        // check each character
        for (basic::u64 i = 0; i < starts_with.length(); i++) {
            // check if character is invalid
            if (starts_with[i] != string[i]) {
                // not a match
                return false;
            }
        }

        // match found
        return true;
    }

    bool string_contains_at(std::string& string, basic::u64 offset, std::string contains) {
        return string.substr(offset, contains.length()) == contains;
    }

    bool string_is_instruction_literal(lexer::lexling& lexling) {
        std::string instruction_prefix = "wave.instruction.";

        // check for prefix
        if (!string_contains_at(lexling.p_value, 0, instruction_prefix)) {
            return false;
        }

        // check for suffixes
        if (string_contains_at(lexling.p_value, instruction_prefix.length(), "type.size")) {
            return true;
        }
        if (string_contains_at(lexling.p_value, instruction_prefix.length(), "type.offset")) {
            return true;
        }
        if (string_contains_at(lexling.p_value, instruction_prefix.length(), "write_register_value.size")) {
            return true;
        }
        if (string_contains_at(lexling.p_value, instruction_prefix.length(), "write_register_value.offset")) {
            return true;
        }
        if (string_contains_at(lexling.p_value, instruction_prefix.length(), "input_0.size")) {
            return true;
        }
        if (string_contains_at(lexling.p_value, instruction_prefix.length(), "input_0.offset")) {
            return true;
        }
        if (string_contains_at(lexling.p_value, instruction_prefix.length(), "input_1.size")) {
            return true;
        }
        if (string_contains_at(lexling.p_value, instruction_prefix.length(), "input_1.offset")) {
            return true;
        }
        if (string_contains_at(lexling.p_value, instruction_prefix.length(), "input_2.size")) {
            return true;
        }
        if (string_contains_at(lexling.p_value, instruction_prefix.length(), "input_2.offset")) {
            return true;
        }
        if (string_contains_at(lexling.p_value, instruction_prefix.length(), "input_3.size")) {
            return true;
        }
        if (string_contains_at(lexling.p_value, instruction_prefix.length(), "input_3.offset")) {
            return true;
        }
        if (string_contains_at(lexling.p_value, instruction_prefix.length(), "output_0.size")) {
            return true;
        }
        if (string_contains_at(lexling.p_value, instruction_prefix.length(), "output_0.offset")) {
            return true;
        }
        if (string_contains_at(lexling.p_value, instruction_prefix.length(), "output_1.size")) {
            return true;
        }
        if (string_contains_at(lexling.p_value, instruction_prefix.length(), "output_1.offset")) {
            return true;
        }
        if (string_contains_at(lexling.p_value, instruction_prefix.length(), "output_2.size")) {
            return true;
        }
        if (string_contains_at(lexling.p_value, instruction_prefix.length(), "output_2.offset")) {
            return true;
        }
        if (string_contains_at(lexling.p_value, instruction_prefix.length(), "instruction_ID.size")) {
            return true;
        }
        if (string_contains_at(lexling.p_value, instruction_prefix.length(), "instruction_ID.offset")) {
            return true;
        }
        if (string_contains_at(lexling.p_value, instruction_prefix.length(), "size")) {
            return true;
        }

        return false;
    }

    // parse arguments
    std::vector<name> parse_arguments(lexer::lexlings& lexlings, int& lexling_index, bool& error_occured) {
        std::vector<name> output;
        std::string boolean_prefix = "wave.boolean.";
        std::string true_suffix = "true";
        std::string false_suffix = "false";

        // check for opener
        if (lexlings.p_lexlings[lexling_index].p_type == lexer::lexling_type::left_parenthesis) {
            // next lexling
            lexling_index++;

            // get arguments until end of arguments
            while (lexling_index < lexlings.count()) {
                // check for offset
                if (lexlings.p_lexlings[lexling_index].p_type == lexer::lexling_type::offset_marker && lexlings.p_lexlings[lexling_index + 1].p_type == lexer::lexling_type::name) {
                    // add argument
                    output.push_back(name(name_type::is_offset, lexlings.p_lexlings[lexling_index + 1].p_value));

                    // next lexling
                    lexling_index += 2;
                // check for integer literal
                } else if (string_starts_with(lexlings.p_lexlings[lexling_index].p_value, "wave.integer.") == true) {
                    // add argument
                    output.push_back(name(name_type::is_integer_literal, lexlings.p_lexlings[lexling_index].p_value));

                    // next argument
                    lexling_index++;
                // check for boolean literal
                } else if (string_starts_with(lexlings.p_lexlings[lexling_index].p_value, "wave.boolean.") == true && ((string_contains_at(lexlings.p_lexlings[lexling_index].p_value, boolean_prefix.length(), true_suffix) && boolean_prefix.length() + true_suffix.length() == lexlings.p_lexlings[lexling_index].p_value.length()) || (string_contains_at(lexlings.p_lexlings[lexling_index].p_value, boolean_prefix.length(), false_suffix) && boolean_prefix.length() + false_suffix.length() == lexlings.p_lexlings[lexling_index].p_value.length()))) {
                    // add argument
                    output.push_back(name(name_type::is_boolean_literal, lexlings.p_lexlings[lexling_index].p_value));

                    // next argument
                    lexling_index++;
                // check for instruction literal
                } else if (string_is_instruction_literal(lexlings.p_lexlings[lexling_index])) {
                    // add argument
                    output.push_back(name(name_type::is_instruction_literal, lexlings.p_lexlings[lexling_index].p_value));

                    // next argument
                    lexling_index++;
                // check for name
                } else if (lexlings.p_lexlings[lexling_index].p_type == lexer::lexling_type::name) {
                    // add argument
                    output.push_back(name(name_type::is_value_name, lexlings.p_lexlings[lexling_index].p_value));

                    // next lexling
                    lexling_index++;
                // invalid argument lexling, quit loop
                } else {
                    break;
                }
            }

            // skip past ending parenthesis
            if (lexlings.p_lexlings[lexling_index].p_type == lexer::lexling_type::right_parenthesis) {
                lexling_index++;
            } else {
                // display error
                std::cout << "Parse error, missing closing parenthesis." << std::endl;

                error_occured = true;
            }
        } else {
            // display error
            std::cout << "Parse error, missing opening parenthesis." << std::endl;

            error_occured = true;
        }

        return output;
    }

    // parse arguments
    std::vector<name> parse_header_arguments(lexer::lexlings& lexlings, int& lexling_index, bool& error_occured) {
        std::vector<name> output;

        // check for opener
        if (lexlings.p_lexlings[lexling_index].p_type == lexer::lexling_type::left_parenthesis) {
            // next lexling
            lexling_index++;

            // get arguments until end of arguments
            while (lexling_index < lexlings.count()) {
                // check for name
                if (lexlings.p_lexlings[lexling_index].p_type == lexer::lexling_type::name) {
                    // add argument
                    output.push_back(name(name_type::is_value_name, lexlings.p_lexlings[lexling_index].p_value));

                    // next lexling
                    lexling_index++;
                // end of arguments, quit loop
                } else if (lexlings.p_lexlings[lexling_index].p_type == lexer::lexling_type::right_parenthesis) {
                    break;
                // invalid argument lexling, raise error
                } else {
                    // set error
                    error_occured = true;

                    // inform user of error
                    std::cout << "Parse error, invalid header argument: " << lexlings.p_lexlings[lexling_index].p_value << std::endl;

                    // quit
                    return output;
                }
            }

            // skip past ending parenthesis
            if (lexlings.p_lexlings[lexling_index].p_type == lexer::lexling_type::right_parenthesis) {
                lexling_index++;
            } else {
                // display error
                std::cout << "Parse error, missing closing parenthesis." << std::endl;

                error_occured = true;
            }
        } else {
            // display error
            std::cout << "Parse error, missing opening parenthesis." << std::endl;

            error_occured = true;
        }

        return output;
    }

    // parse statement
    statement parse_statement(lexer::lexlings& lexlings, int& lexling_index, bool& error_occured) {
        statement output;

        // get abstraction call
        if (lexlings.p_lexlings[lexling_index].p_type == lexer::lexling_type::name) {
            // mark type
            output.p_type = statement_type::is_abstraction_call;

            // assign name
            output.p_name = name(name_type::is_abstraction_name, lexlings.p_lexlings[lexling_index].p_value);
            lexling_index++;

            // get inputs
            output.p_inputs = parse_arguments(lexlings, lexling_index, error_occured);

            // check error
            if (error_occured) {
                return output;
            }

            // get outputs
            output.p_outputs = parse_arguments(lexlings, lexling_index, error_occured);

            // check error
            if (error_occured) {
                return output;
            }
        // get offset marker
        } else if ((lexlings.count() > (lexling_index + 1)) && (lexlings.p_lexlings[lexling_index].p_type == lexer::lexling_type::offset_marker && lexlings.p_lexlings[lexling_index + 1].p_type == lexer::lexling_type::name)) {
            // mark type
            output.p_type = statement_type::is_offset_declaration;

            // get offset name
            output.p_name = name(name_type::is_offset, lexlings.p_lexlings[lexling_index + 1].p_value);

            // next lexling
            lexling_index += 2;
        // error
        } else {
            // mark type
            output.p_type = statement_type::is_not_valid;

            // display error
            std::cout << "Parse error, unrecognized statement type." << std::endl << "Current Lexling: " << lexlings.p_lexlings[lexling_index].p_value << std::endl;

            // set error
            error_occured = true;
        }

        return output;
    }

    // parse abstraction header
    statement parse_abstraction_header(lexer::lexlings& lexlings, int& lexling_index, bool& error_occured) {
        statement output;

        // get abstraction call
        if (lexlings.p_lexlings[lexling_index].p_type == lexer::lexling_type::name) {
            // assign name
            output.p_name = name(name_type::is_abstraction_name, lexlings.p_lexlings[lexling_index].p_value);
            lexling_index++;

            // get inputs
            output.p_inputs = parse_header_arguments(lexlings, lexling_index, error_occured);

            // check error
            if (error_occured) {
                return output;
            }

            // get outputs
            output.p_outputs = parse_header_arguments(lexlings, lexling_index, error_occured);

            // check error
            if (error_occured) {
                return output;
            }
        // error
        } else {
            // display error
            std::cout << "Parse error, invalid abstraction header." << std::endl << "Current Lexling: " << lexlings.p_lexlings[lexling_index].p_value << std::endl;

            // set error
            error_occured = true;
        }

        return output;
    }

    // parse abstraction
    abstraction parse_abstraction(lexer::lexlings& lexlings, int& lexling_index, bool& error_occured) {
        abstraction output;

        // get abstraction name
        output.p_header = parse_abstraction_header(lexlings, lexling_index, error_occured);

        // parse abstraction marker for abstraction
        if (lexlings.p_lexlings[lexling_index].p_type == lexer::lexling_type::abstraction_marker) {
            // set type
            output.p_type = abstraction_type::is_code_defined;

            // next lexling
            lexling_index++;

            // get scope
            if (lexlings.p_lexlings[lexling_index].p_type == lexer::lexling_type::left_curly_bracket) {
                // next scope
                lexling_index++;

                // get all statements
                while (lexlings.p_lexlings[lexling_index].p_type != lexer::lexling_type::right_curly_bracket) {
                    // parse statement
                    output.p_scope.push_back(parse_statement(lexlings, lexling_index, error_occured));

                    // check error
                    if (error_occured) {
                        return output;
                    }
                }

                // skip past scope end
                lexling_index++;
            } else {
                // print error
                std::cout << "Parse error, missing scope opener '{'." << std::endl;

                // set error
                error_occured = true;
            }
        }

        return output;
    }

    // parse file
    program parse_file(lexer::lexlings lexlings, bool& error_occured) {
        program output = program();
        int lexling_index = 0;

        // get abstractions and includes
        while (lexling_index < lexlings.count()) {
            // new abstraction
            output.p_abstractions.push_back(parse_abstraction(lexlings, lexling_index, error_occured));

            // check error
            if (error_occured) {
                return output;
            }
        }

        return output;
    }

    // print arguments
    void print_arguments(std::vector<name>& arguments) {
        // print starter
        std::cout << "(";

        // print arguments
        for (basic::u64 i = 0; i < arguments.size(); i++) {
            // if not the first argument
            if (i > 0) {
                // print the separator
                std::cout << " ";
            }

            // print the name
            std::cout << arguments[i].p_name_value;
        }

        // print closer
        std::cout << ")";

        return;
    }

    // print program
    void print_program(program program) {
        // print header
        std::cout << "Parse Tree:" << std::endl;

        // print all abstractions
        for (unsigned int abstraction = 0; abstraction < program.p_abstractions.size(); abstraction++) {
            // print abstraction type
            std::cout << "Abstraction ";
            switch (program.p_abstractions[abstraction].p_type) {
            case abstraction_type::is_code_defined:
                std::cout << "Function";
                break;
            case abstraction_type::is_compiler_defined:
                std::cout << "Instruction\n";
                break;
            default:
                std::cout << "Undefined\n";
                break;
            }
            std::cout << ": " << program.p_abstractions[abstraction].p_header.p_name.p_name_value;

            // print abstraction io
            print_arguments(program.p_abstractions[abstraction].p_header.p_inputs);
            print_arguments(program.p_abstractions[abstraction].p_header.p_outputs);

            // print statements header
            std::cout << "\nStatements:" << std::endl;

            // print abstraction statements
            for (unsigned int statement = 0; statement < program.p_abstractions[abstraction].p_scope.size(); statement++) {
                // print statments by type
                if (program.p_abstractions[abstraction].p_scope[statement].p_name.p_name_type == name_type::is_offset) {
                    // print offset
                    std::cout << "\t\tOffset Name: " << program.p_abstractions[abstraction].p_scope[statement].p_name.p_name_value << std::endl;
                } else {
                    // print name
                    std::cout << "\t" << program.p_abstractions[abstraction].p_scope[statement].p_name.p_name_value;

                    // print io
                    print_arguments(program.p_abstractions[abstraction].p_scope[statement].p_inputs);
                    print_arguments(program.p_abstractions[abstraction].p_scope[statement].p_outputs);

                    // print new line
                    std::cout << std::endl;
                }
            }
        }

        return;
    }
}
