#pragma once

enum class error_code {
    //Lexer error codes
    missing_comment_ending,
    invalid_character_in_raw_string_literal_delimiter,
    too_long_raw_string_literal_delimiter,
    missing_ending_of_string_literal,
    invalid_string_literal_escape_sequence,
    invalid_number_literal,

    unexpected_character,

    //Parser error codes

    expecting_identifier,
};

enum class warning_code {
    ignored_string_literal_escape_sequence,
};

