#include "lexer_error_producer.h"
#include <boost/format.hpp>

lexer_error_producer::lexer_error_producer(const code &c) :
    _code(c)
{

}

void lexer_error_producer::missing_comment_ending(code_point commentStart, code_point fileEnd)
{
    error(error_code::missing_comment_ending,
          _code.info(fileEnd),
          (boost::format("Missing ending '*/' for multiline commend starting from %1%") % _code.info(commentStart).to_string()).str());
}

void lexer_error_producer::invalid_character_in_raw_string_literal_delimiter(code_point cp, char character)
{
    error(error_code::invalid_character_in_raw_string_literal_delimiter,
          _code.info(cp),
          (boost::format("Invalid character in raw string literal delimiter: '%1%'") % character).str());
}

void lexer_error_producer::too_long_raw_string_literal_delimiter(code_point cp)
{
    error(error_code::too_long_raw_string_literal_delimiter,
          _code.info(cp),
          "Too long raw string literal delimiter");
}

void lexer_error_producer::missing_ending_of_string_literal(code_point cp, code_point start)
{
    error(error_code::missing_ending_of_string_literal,
          _code.info(cp),
          "Missing ending of a string literal starting " + _code.info(start).to_string());
}

void lexer_error_producer::invalid_string_literal_escape_sequence(code_point cp, const std::string &sequence)
{
    error(error_code::invalid_string_literal_escape_sequence,
          _code.info(cp),
          "Invalid string literal escape sequence: " + sequence);
}

void lexer_error_producer::invalid_number_literal_unexpected_char(code_point cp, char unexpected)
{
    error(error_code::invalid_number_literal,
          _code.info(cp),
          (boost::format("Invalid number literal. Unexpected character '%1%'") % unexpected).str());
}

void lexer_error_producer::unexpected_character(code_point cp, char character)
{
    error(error_code::unexpected_character,
          _code.info(cp),
          (boost::format("Unexpected character '%1%'") % character).str());
}

