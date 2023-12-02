#pragma once

/* Include */
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <unistd.h>

#include "lex.cpp"
#include "parse.cpp"
#include "account.cpp"
#include "generate.cpp"

/* Code */
namespace compiler {
    std::string load_file(std::string file_path) {
        std::ifstream file(file_path);

        // if file opened
        if (file) {
            // return file contents
            return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        // if file not opened
        } else {
            // announce error
            std::cout << "File loading error, file was not opened: " << file_path << std::endl;

            // return empty string
            return "";
        }
    }

    /*runner::program compile_user_code(std::string user_code, bool& compilation_error, bool print_debug) {
        runner::program output;
        lexer::lexlings lexlings;
        parser::program parse_tree;
        accounter::skeleton::skeleton skeleton;
        bool lex_error = false;
        bool parse_error = false;
        bool accounting_error = false;
        bool generation_error = false;

        // print current working directory
        if (print_debug) {
            // print directory
            std::cout << "Current Working Directory: " << basic::get_current_working_directory() << std::endl;
        }

        // lex file
        lexlings = lexer::lex(user_code, lex_error);

        // print lexlings
        if (print_debug) {
            lexer::print_lexlings(lexlings);
        }

        // do not proceed if error occured
        if (lex_error) {
            compilation_error = true;

            return output;
        }

        // parse file
        parse_tree = parser::parse_file(lexlings, parse_error);

        // do not proceed if error occured
        if (parse_error) {
            compilation_error = true;

            return output;
        }

        // print parse tree
        if (print_debug) {
            parser::print_program(parse_tree);
        }

        // account program
        skeleton.get_skeleton(parse_tree, accounting_error);

        // do not proceed if error occured
        if (accounting_error) {
            compilation_error = true;

            return output;
        }

        // print program
        if (print_debug) {
            skeleton.print_skeleton();
        }

        // generate program code
        output = generator::generate_program(skeleton, generation_error);

        // do not proceed if error occured
        if (generation_error) {
            compilation_error = true;
            
            return output;
        }

        // print code
        if (print_debug) {
            print_program(output);
        }

        return output;
    }*/

    class compilation_unit {
    public:
        std::vector<std::string> p_files;
        std::vector<parser::program> p_parse_trees;
        accounter::skeleton::skeleton p_skeleton;
        bool p_accouting_error;
        bool p_generation_error;
    };

    parser::program dissect_user_code(std::string user_code, bool& compilation_error, bool print_debug) {
        parser::program output;
        lexer::lexlings lexlings;
        bool lex_error = false;
        bool parse_error = false;

        // lex file
        lexlings = lexer::lex(user_code, lex_error);

        // print lexlings
        if (print_debug) {
            print_lexlings(lexlings);
        }

        // do not proceed if error occured
        if (lex_error) {
            compilation_error = true;

            return output;
        }

        // parse file
        output = parser::parse_file(lexlings, parse_error);

        // do not proceed if error occured
        if (parse_error) {
            compilation_error = true;

            return output;
        }

        // print parse tree
        if (print_debug) {
            parser::print_program(output);
        }

        return output;
    }

    runner::program compile(std::vector<std::string> files, bool& compilation_error, bool print_debug) {
        runner::program output;
        compilation_unit unit;

        // setup errors
        unit.p_accouting_error = false;
        unit.p_generation_error = false;

        // add all files
        unit.p_files = files;

        // dissect all files
        for (basic::u64 file = 0; file < files.size(); file++) {
            // dissect file
            unit.p_parse_trees.push_back(dissect_user_code(files[file], compilation_error, print_debug));
        }

        // account for all files
        unit.p_skeleton.get_skeleton(unit.p_parse_trees, unit.p_accouting_error);

        // do not proceed if error occured
        if (unit.p_accouting_error) {
            compilation_error = true;

            return output;
        }

        // print program
        if (print_debug) {
            unit.p_skeleton.print_skeleton();
        }

        // generate program code
        output = generator::generate_program(unit.p_skeleton, unit.p_generation_error);

        // do not proceed if error occured
        if (unit.p_generation_error) {
            compilation_error = true;
            
            return output;
        }

        // print code
        if (print_debug) {
            print_program(output);
        }

        return output;
    }
}
