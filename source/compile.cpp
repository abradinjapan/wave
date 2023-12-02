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

    class file {
    public:
        std::string p_name;
        parser::program p_program;

        file(std::string name, parser::program program) {
            p_name = name;
            p_program = program;
        }
    };

    class collection {
    public:
        std::vector<file> p_files;

        uint64_t get_file_index(std::string name) {
            // browse all files
            for (uint64_t i = 0; i < p_files.size(); i++) {
                // check file
                if (p_files[i].p_name == name) {
                    // index found
                    return i;
                }
            }

            // index not found
            return p_files.size();
        }
    };

    runner::program compile_user_code(std::string user_code, bool& compilation_error, bool print_debug) {
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
    }

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

    collection find_all_files(std::string initial_file_path, bool& compilation_error, bool print_debug) {
        collection output;
        std::vector<std::string> unparsed_file_paths;

        // setup search
        unparsed_file_paths.push_back(initial_file_path);

        // collect files until empty
        while (unparsed_file_paths.size() > 0) {
            // load file
            std::string user_code = load_file(unparsed_file_paths[0]);

            // parse file
            parser::program parse_tree = dissect_user_code(user_code, compilation_error, print_debug);

            // check for error
            if (compilation_error) {
                return output;
            }

            // append file
            output.p_files.push_back(file(unparsed_file_paths[0], parse_tree));

            // remove parsed file path from list
            unparsed_file_paths.erase(unparsed_file_paths.begin());
        }

        return output;
    }

    runner::program compile(std::string initial_file_path, bool& compilation_error, bool print_debug) {
        runner::program output;
        collection file_collection;

        file_collection = find_all_files(initial_file_path, compilation_error, print_debug);
    }
}
