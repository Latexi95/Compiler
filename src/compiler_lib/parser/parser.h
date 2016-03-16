#pragma once
#include "../../common/code_point.h"
#include "../../common/string_view.h"
#include "../common/token.h"
#include <memory>

namespace parse_tree {
     class base_tree;
     class node;
}
class token;
class code;
class parser_error_producer;

class parser
{
public:
    parser(const code &c);
    ~parser();

    bool parse();

    std::unique_ptr<parse_tree::base_tree> take_parse_tree();
    const parse_tree::base_tree &parse_tree() const;

private:
    const token &tok(int offset = 0);
    token_type tt(int offset = 0);
    const token &next();
    void skip(int count);

    parse_tree::node *try_struct_definition();
    parse_tree::node *expect_identifier();


    int _token_index;
    const code &_code;
    std::unique_ptr<parser_error_producer> _err;
    std::unique_ptr<parse_tree::base_tree> _base;
    token _invalid_token;
};

