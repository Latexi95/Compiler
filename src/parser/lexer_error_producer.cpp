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

