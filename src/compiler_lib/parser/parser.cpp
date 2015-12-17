#include "parser.h"
#include "parser_error_producer.h"

parser::parser(const code &c) :
    _code(c),
    _err(new parser_error_producer(c))
{

}

parser::~parser()
{

}

