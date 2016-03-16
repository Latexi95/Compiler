#ifndef CODE_H
#define CODE_H
#include <vector>
#include <map>

#include "../../common/string_view.h"
#include "../../common/code_point.h"
#include "token.h"

/**
 * @brief The code class contains source files in the text form and lexer tokens.
 *
 * The code class is used to pass source files to the lexer and tokens to the parser.
 * It also handles mapping between code_point and code_point_info.
 */
class code
{
public:
    code(std::string file_name);

    void set_code(const std::string &code);
    const std::string &code_string() const { return _code; }

    void add_token(const token &token) { _tokens.push_back(token); }
    void set_tokens(const std::vector<token> &tokens) { _tokens = tokens; }
    const std::vector<token> &tokens() const { return _tokens; }

    code_point begin() const { return 0; }
    code_point end() const { return _code.size(); }
    code_point next(code_point cp) const { return cp + 1; }

    char c(code_point cp) const { if (cp >= _code.size()) return '\0'; return _code[cp]; }
    string_view view(code_point cp, size_t size) const;
    string_view view_range(code_point cp, size_t cpEnd) const;

    code_point_info info(code_point cp) const;
    code_point_info info_tok(int index) const;

    token tok(int index) const;

    std::string tok_str(int index) const;


    void add_string_literal(code_point location, const std::string &lit);
    string_view string_literal(code_point location) const;

    void print_tokens();
private:
    std::string _code;
    std::string _file_name;
    std::vector<code_point> _line_changes;
    std::vector<token> _tokens;
    std::map<code_point, std::string> _string_literals;
};

#endif // CODE_H
