#pragma once

/* Includes */
#include <vector>
#include <string>

#include "basic.cpp"

/* Code */
namespace lexer {
    // lexling types
    enum lexling_type {
        no_type,
        name,
        string_literal,
        left_parenthesis,
        right_parenthesis,
        left_curly_bracket,
        right_curly_bracket,
        offset_marker,
        abstraction_marker,
    };

    // lexling
    class lexling {
    public:
        lexling_type p_type;
        std::string p_value;

        // default constructor
        lexling() {
            p_type = lexling_type::no_type;
            p_value = "";
        }

        // constructor
        lexling(lexling_type type, std::string value) {
            p_type = type;
            p_value = value;
        }
    };

    // lexlings
    class lexlings {
    public:
        std::vector<lexling> p_lexlings;

        // constructor
        lexlings() {
            p_lexlings = std::vector<lexling>();
        }

        // return count
        int count() {
            return p_lexlings.size();
        }
    };

    // check if a character falls within a certain range
    bool check_character_range(basic::character low, basic::character character, basic::character high) {
        return (low <= character) && (character <= high);
    }

    // lex user code
    lexlings lex(std::string user_code, bool& error_occured) {
        lexlings output;
        unsigned int index;
        unsigned int comment_depth;
        unsigned int string_depth;
        unsigned int string_start;
        int length;

        // setup variables
        output = lexlings();
        index = 0;

        // get lexlings
        while (index < user_code.length()) {
            // set comment depth
            comment_depth = 0;

            // absorb whitespace
            while (index < user_code.length() && (user_code[index] == ' ' || user_code[index] == '\n' || user_code[index] == '\r' || user_code[index] == '\t' || user_code[index] == '[')) {
                // if comment
                if (user_code[index] == '[') {
                    // run through characters
                    while (index < user_code.length()) {
                        // increase comment depth
                        if (user_code[index] == '[') {
                            comment_depth++;
                        }

                        // decrease comment depth
                        if (user_code[index] == ']') {
                            comment_depth--;
                        }

                        // next character
                        index++;

                        // check for comment end
                        if (comment_depth == 0) {
                            break;
                        }
                    }
                // if normal whitespace
                } else {
                    // next index
                    index++;
                }
            }

            // set lexling length
            length = 0;

            // check for eof
            if (index < user_code.length()) {
                // check for name
                if ((check_character_range('a', user_code[index], 'z') || check_character_range('A', user_code[index], 'Z') || check_character_range('0', user_code[index], '9') || check_character_range('.', user_code[index], '.') || check_character_range('_', user_code[index], '_'))) {
                    // get characters
                    while (index < user_code.length() && (check_character_range('a', user_code[index], 'z') || check_character_range('A', user_code[index], 'Z') || check_character_range('0', user_code[index], '9') || check_character_range('.', user_code[index], '.') || check_character_range('_', user_code[index], '_'))) {
                        index++;
                        length++;
                    }

                    // add lexling
                    output.p_lexlings.push_back(lexling(lexling_type::name, user_code.substr(index - length, length)));
                // check for left parenthesis
                } else if (user_code[index] == '(') {
                    // add lexling
                    output.p_lexlings.push_back(lexling(lexling_type::left_parenthesis, "("));

                    // next index
                    index++;
                // check for right parenthesis
                } else if (user_code[index] == ')') {
                    // add lexling
                    output.p_lexlings.push_back(lexling(lexling_type::right_parenthesis, ")"));

                    // next index
                    index++;
                // check for left curly bracket
                } else if (user_code[index] == '{') {
                    // add lexling
                    output.p_lexlings.push_back(lexling(lexling_type::left_curly_bracket, "{"));

                    // next index
                    index++;
                // check for right curly bracket
                } else if (user_code[index] == '}') {
                    // add lexling
                    output.p_lexlings.push_back(lexling(lexling_type::right_curly_bracket, "}"));

                    // next index
                    index++;
                // check for offset marker
                } else if (user_code[index] == '@') {
                    // add lexling
                    output.p_lexlings.push_back(lexling(lexling_type::offset_marker, "@"));

                    // next index
                    index++;
                // check for abstraction marker
                } else if (user_code[index] == '=') {
                    // add lexling
                    output.p_lexlings.push_back(lexling(lexling_type::abstraction_marker, "="));

                    // next index
                    index++;
                // check for string
                } else if (user_code[index] == '\"' && user_code[index + 1] == '\'') {
                    // setup string start
                    string_start = index;

                    // setup string depth
                    string_depth = 1;

                    // skip past already read characters
                    index += 2;

                    // get string
                    while (string_depth > 0) {
                        // check for string opener
                        if (user_code[index] == '"' && user_code[index + 1] == '\'') {
                            // increase string depth
                            string_depth++;

                            // next character
                            index++;
                        }

                        // check for string closer
                        if (user_code[index] == '\'' && user_code[index + 1] == '"') {
                            // decrease string depth
                            string_depth--;

                            // next character
                            index++;
                        }

                        // next index
                        index++;
                    }

                    // push back string
                    output.p_lexlings.push_back(lexling(lexling_type::string_literal, user_code.substr(string_start + 2, (index - 2) - (string_start + 2))));
                // no lexling found, error
                } else {
                    // inform user of failure
                    std::cout << "Lexical error, invalid lexling / character detected: " << user_code[index] << std::endl;

                    // set error
                    error_occured = true;

                    // quit
                    return output;
                }
            }
        }

        return output;
    }

    void print_lexlings(lexlings& lexlings) {
        // print header
        std::cout << "Lexlings:" << std::endl;

        // print lexling
        for (unsigned int i = 0; i < lexlings.p_lexlings.size(); i++) {
            std::cout << "\tType: " << lexlings.p_lexlings[i].p_type << " Value: " << lexlings.p_lexlings[i].p_value << std::endl;
        }

        return;
    }
}
