#ifndef LEXER_H
#define LEXER_H
#include <vector>
#include "../common/token.h"
#include "../common/code.h"
#include <memory>

class lexer_error_producer;
class error_producer;

class lexer
{
public:
    lexer(const code &c);

    bool parse();

    const std::vector<token> &tokens() { return _tokens; }

    error_producer &err_producer();
private:
    static bool is_space(char c);
    static bool is_alpha(char c);
    static bool is_number(char c);
    
    token_type is_keyword(code_point start, code_point end);

    void add_token(token_type t, code_point cp, code_point end);
    bool skip_spaces();
    void skip(size_t steps);
    bool handle_operators();
    bool handle_comments();
    bool handle_identifiers();

    char ca(code_point offset = 0);
    char next_rc();
    char next_r();

    const code &_c;
    std::unique_ptr<lexer_error_producer> _err;
    std::vector<token> _tokens;
    code_point _cp;
    bool fatal_error;
};

#endif // LEXER_H
