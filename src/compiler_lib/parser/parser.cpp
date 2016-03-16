#include "parser.h"
#include "parser_error_producer.h"
#include "../parse_tree/parse_tree.h"
#include "../common/code.h"
#include "../common/token.h"
#include "parser_error_producer.h"

using namespace parse_tree;
parser::parser(const code &c) :
    _token_index(0),
    _code(c),
    _err(new parser_error_producer(c)),
    _base(new base_tree(c.begin(), c.end())),
    _invalid_token(token_type::invalid, "", -1)
{

}

parser::~parser()
{ }

bool parser::parse()
{
    _token_index = 0;


    while (tt() != token_type::invalid) {

    }
}

std::unique_ptr<base_tree> parser::take_parse_tree()
{
    return std::move(_base);
}

const base_tree &parser::parse_tree() const
{
    return *_base;
}

const token &parser::tok(int offset)
{
    int index = _token_index + offset;
    if (index < 0 || (unsigned)index >= _code.tokens().size()) {
        return _invalid_token;
    }
    return _code.tokens()[index];
}

token_type parser::tt(int offset)
{
    return tok(offset).type();
}

const token &parser::next()
{
    ++_token_index;
    return tok();
}

node *parser::try_struct_definition()
{
    if (tt() != token_type::k_struct) return nullptr;
    next();
    node *id = expect_identifier();


}

node *parser::expect_identifier()
{
    if (tt() != token_type::t_identifier) {
        _err->expecting_identifier(_token_index);
        throw error_code::expecting_identifier;
    }

    identifier *id = new identifier(tok().point());
    id->set_value(tok().text());
    next();
    return id;
}

