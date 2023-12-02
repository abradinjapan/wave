#include <iostream>
#include <fstream>

#include "compile.cpp"

void compile_and_run(std::vector<std::string> user_codes, bool debug) {
    runner::program program;
    bool compilation_error = false;
    bool run_time_error = false;
    runner::allocation input;

    // inform user of compilation start
    std::cout << "----------\n" << "Compiling code." << std::endl;

    // compile program
    program = compiler::compile(user_codes, compilation_error, debug);

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
    std::vector<std::string> user_codes;

    // inform user of compilation start
    std::cout << "Starting up compiler." << std::endl;

    // get file from arguments
    if (argc > 1) {
        // load files
        for (basic::u64 file = 1; file < argc; file++) {
            // get file paths
            std::string file_path = std::string(argv[file]);
                
            // get file data
            std::string file_data = compiler::load_file(file_path);

            // append file
            user_codes.push_back(file_data);

            // run if file found
            if (file_data == "") {
                // print error
                std::cout << "Invalid file path or empty file: " << file_path << std::endl;
            }
        }

        // compile and run
        compile_and_run(user_codes, true);
    // if file name is missing
    } else {
        std::cout << "Missing file name." << std::endl;
    }

    return 0;
}
