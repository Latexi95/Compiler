#pragma once
#include "../../common/error_producer.h"
#include "../common/code.h"

class lexer_error_producer : public error_producer
{
public:
    lexer_error_producer(const code &c);

    void missing_comment_ending(code_point commentStart, code_point fileEnd);
    void invalid_character_in_raw_string_literal_delimiter(code_point cp, char character);
    void too_long_raw_string_literal_delimiter(code_point cp);
    void missing_ending_of_string_literal(code_point cp, code_point start);
    void invalid_string_literal_escape_sequence(code_point cp, const std::string &sequence);
    void invalid_number_literal_unexpected_char(code_point cp, char unexpected);
private:
    const code &_code;
};

