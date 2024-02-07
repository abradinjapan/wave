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

        // lexical error information
        basic::character_index p_lexing__character_index;
        basic::line_index p_lexing__line_index;

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
            p_lexing__character_index = 0;
            p_lexing__line_index = 0;
        }

        void set_as_lexical_error(basic::character_index character_index, basic::line_index line_index) {
            p_error_type = et::et__lexical_error;
            p_lexing__character_index = character_index;
            p_lexing__line_index = line_index;
        }

        void set_as_parsing_error() {
            p_error_type = et::et__parsing_error;
        }

        void set_as_accounting_error() {
            p_error_type = et::et__accounting_error;
        }

        void set_as_generation_error() {
            p_error_type = et::et__generation_error;
        }
    };
}
