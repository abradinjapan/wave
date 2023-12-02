#include <iostream>
#include <fstream>

#include "compile.cpp"

void compile_and_run(std::string user_code, bool debug) {
    runner::program program;
    bool compilation_error = false;
    bool run_time_error = false;
    runner::allocation input;

    // inform user of compilation start
    std::cout << "----------\n" << "Compiling code." << std::endl;

    // if debug enabled
    if (debug) {
        // print original file
        std::cout << "Original File:" << std::endl << user_code << std::endl;
    }

    // compile program
    program = compiler::compile_user_code(user_code, compilation_error, debug);

    // run if no errors occured
    if (compilation_error == false) {
        // inform user of program start
        std::cout << "Starting program..." << std::endl;

        // run code
        runner::run_program(program, input, run_time_error);

        // inform user of program end
        std::cout << std::endl << "Ending program..." << std::endl;
    } else {
        std::cout << "Code not run, compilation error." << std::endl;
    }

    // close in display area
    std::cout << "----------" << std::endl;
}

int main(int argc, char** argv) {
    std::string file_path;
    std::string file_data;

    // inform user of compilation start
    std::cout << "Starting up compiler." << std::endl;

    // get file from arguments
    if (argc > 1) {
        // get file path
        file_path = std::string(argv[1]);

        // get file data
        file_data = compiler::load_file(file_path);

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
