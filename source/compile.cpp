#pragma once

/* Include */
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <unistd.h>

#include "compiler_errors.cpp"
#include "lex.cpp"
#include "parse.cpp"
#include "account.cpp"
#include "generate.cpp"

/* Code */
namespace compiler {
    class compilation_unit {
    public:
        std::vector<std::string> p_files;
        std::vector<parser::program> p_parse_trees;
        accounter::skeleton::skeleton p_skeleton;
        compiler_errors::error p_error;
    };

    parser::program dissect_user_code(std::string user_code, compilation_unit& compilation_unit, bool print_debug) {
        parser::program output;
        lexer::lexlings lexlings;

        // lex file
        lexlings = lexer::lex(user_code, compilation_unit.p_error);

        // print lexlings
        if (print_debug) {
            print_lexlings(lexlings);
        }

        // do not proceed if error occured
        if (compilation_unit.p_error.error_occured()) {
            return output;
        }

        // parse file
        output = parser::parse_file(lexlings, compilation_unit.p_error);

        // do not proceed if error occured
        if (compilation_unit.p_error.error_occured()) {
            return output;
        }

        // print parse tree
        if (print_debug) {
            parser::print_program(output);
        }

        return output;
    }

    runner::program compile(std::vector<std::string> files, compilation_unit& unit, bool print_debug) {
        runner::program output;

        // initialize as no error
        unit.p_error.set_as_no_error();

        // add all files
        unit.p_files = files;

        // dissect all files
        for (basic::u64 file = 0; file < files.size(); file++) {
            // dissect file
            unit.p_parse_trees.push_back(dissect_user_code(files[file], unit, print_debug));

            // check for error
            if (unit.p_error.error_occured()) {
                return output;
            }
        }

        // account for all files
        unit.p_skeleton.get_skeleton(unit.p_parse_trees, unit.p_error);

        // do not proceed if error occured
        if (unit.p_error.error_occured()) {
            return output;
        }

        // print program
        if (print_debug) {
            unit.p_skeleton.print_skeleton();
        }

        // generate program code
        output = generator::generate_program(unit.p_skeleton, unit.p_error);

        // do not proceed if error occured
        if (unit.p_error.error_occured()) {
            return output;
        }

        // print code
        if (print_debug) {
            print_program(output);
        }

        return output;
    }
}
