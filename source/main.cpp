#include <iostream>
#include <string.h>

#include "compile.cpp"

void compile_and_run(std::vector<std::string> user_codes, bool debug) {
    runner::program program;
    compiler::compilation_unit compilation_unit;
    bool run_time_error = false;

    // compile program
    program = compiler::compile(user_codes, compilation_unit, debug);

    // run if no errors occured
    if (compilation_unit.p_error.error_occured() == false) {
        // run code
        runner::run_program(program, basic::buffer(), run_time_error);
    } else {
        std::cout << "Code not run, compilation error." << std::endl;
    }

    return;
}

int main(int argc, char** argv) {
    std::vector<std::string> user_codes;

    // get file from arguments
    if (argc > 1) {
        // load files
        for (basic::u64 file = 1; file < (basic::u64)argc; file++) {
            // get file paths
            std::string file_path = std::string(argv[file]);
                
            // get file data
            std::string file_data = basic::load_file(file_path);

            // append file
            user_codes.push_back(file_data);

            // run if file found
            if (file_data == "") {
                // print error
                std::cout << "Invalid file path or empty file: " << file_path << std::endl;
            }
        }

        // compile and run
        compile_and_run(user_codes, false);
    // if file name is missing
    } else {
        std::cout << "Missing file name(s)." << std::endl;
    }

    return 0;
}
