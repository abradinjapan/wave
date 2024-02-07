#pragma once

#include "basic.cpp"

namespace compiler_errors {
    enum et {
        et__no_error,
        et__lexical_error,
        et__parsing_error,
        et__accounting_error,
        et__generation_error,
    };

    class error {
    public:
        et p_error_type;
        basic::file_index p_file_index;
        std::string p_error_information;

        error() {
            set_as_no_error();
        }

        bool error_occured() {
            return p_error_type != et__no_error;
        }

        void set_file_index(basic::file_index index) {
            p_file_index = index;
        }

        void set_as_no_error() {
            p_error_type = et::et__no_error;
            p_error_information = "[ no error ]";
        }

        void set_as_lexical_error(std::string error_information) {
            p_error_type = et::et__lexical_error;
            p_error_information = error_information;
        }

        void set_as_parsing_error(std::string error_information) {
            p_error_type = et::et__parsing_error;
            p_error_information = error_information;
        }

        void set_as_accounting_error(std::string error_information) {
            p_error_type = et::et__accounting_error;
            p_error_information = error_information;
        }

        void set_as_generation_error(std::string error_information) {
            p_error_type = et::et__generation_error;
            p_error_information = error_information;
        }

        void print_error() {
            std::cout << p_error_information << std::endl;
        }
    };
}
