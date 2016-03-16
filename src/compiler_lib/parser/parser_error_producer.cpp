#include "parser_error_producer.h"
#include "../common/code.h"
#include <boost/format.hpp>

parser_error_producer::parser_error_producer(const code &c) :
    _code(c)
{

}

parser_error_producer::~parser_error_producer()
{

}

void parser_error_producer::expecting_identifier(int token_index)
{
    error(error_code::expecting_identifier,
          _code.info_tok(token_index),
          (boost::format("Expecting an identifier after %1%") % _code.tok_str(token_index - 1)).str());
}
