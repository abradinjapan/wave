#include <iostream>
#include <fstream>

#include "lex.cpp"
#include "parse.cpp"
#include "account.cpp"
#include "generate.cpp"
#include "run.cpp"

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

runner::program compile(std::string user_code, bool& compilation_error, bool print_debug) {
    runner::program output;
    lexer::lexlings lexlings;
    parser::program parse_tree;
    accounter::skeleton::skeleton skeleton;
    bool lex_error = false;
    bool parse_error = false;
    bool accounting_error = false;
    bool generation_error = false;

    // lex file
    lexlings = lexer::lex(user_code, lex_error);

    // print lexlings
    if (print_debug) {
        std::cout << "Lexlings:" << std::endl;
        for (unsigned int i = 0; i < lexlings.p_lexlings.size(); i++) {
            std::cout << "\tType: " << lexlings.p_lexlings[i].p_type << " Value: " << lexlings.p_lexlings[i].p_value << std::endl;
        }
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

void compile_and_run(std::string user_code, bool debug) {
    runner::program program;
    bool compilation_error = false;
    bool run_time_error = false;

    // inform user of compilation start
    std::cout << "----------\n" << "Compiling code." << std::endl;

    // if debug enabled
    if (debug) {
        // print original file
        std::cout << "Original File:" << std::endl << user_code << std::endl;
    }

    // compile program
    program = compile(user_code, compilation_error, debug);

    // run if no errors occured
    if (compilation_error == false) {
        // inform user of program start
        std::cout << "Starting program..." << std::endl;

        // run code
        runner::run_program(program, run_time_error);

        // inform user of program end
        std::cout << std::endl << "Ending program..." << std::endl;
    } else {
        std::cout << "Code not run, compilation error." << std::endl;
    }

    // close in display area
    std::cout << "----------" << std::endl;
}

int main(int argc, char** argv) {
    std::cout << "Starting up compiler." << std::endl;
    std::string file_path;
    std::string file_data;

    // get file from arguments
    if (argc > 1) {
        // get file path
        file_path = std::string(argv[1]);

        // get file data
        file_data = load_file(file_path);

        // run if file found
        if (file_data != "") {
            // run compiler
            compile_and_run(file_data, true);
        // if file not found
        } else {
            std::cout << "Invalid file path: " << file_path << std::endl;
        }
    // if file name is missing
    } else {
        std::cout << "Missing file name." << std::endl;
    }

    return 0;
}
