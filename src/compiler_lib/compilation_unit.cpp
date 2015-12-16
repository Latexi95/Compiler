#include "compilation_unit.h"
#include "common/code.h"
#include "parser/parser.h"
#include "lexer/lexer.h"
#include "lexer/lexer_error_producer.h"

compilation_unit::compilation_unit(const std::string &file_name) :
    _code(new code(file_name)),
    _lexer(new lexer(*_code)),
    _parser(new parser())
{

}

compilation_unit::~compilation_unit()
{

}

void compilation_unit::set_code(const std::string &code)
{
    _code->set_code(code);
}

void compilation_unit::add_error_handler(const std::shared_ptr<error_handler> &err_handler)
{
    _lexer->err_producer().add_handler(err_handler);
}

