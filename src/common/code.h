#ifndef CODE_H
#define CODE_H
#include <vector>

#include "string_view.h"
#include "code_point.h"
#include "token.h"


class code
{
public:
    code(std::string file_name);

    void set_code(std::string code);
    const std::string &code_string() const { return _code; }

    void set_tokens(const std::vector<token> &tokens) { _tokens = tokens; }
    const std::vector<token> &tokens() const { return _tokens; }

    code_point begin() const { return 0; }
    code_point next(code_point cp) const { if (cp == _code.size()) return cp; return cp + 1; }

    char c(code_point cp) const { if (cp >= _code.size()) return '\0'; return _code[cp]; }
    string_view view(code_point cp, size_t size) const;
    string_view view_range(code_point cp, size_t cpEnd) const;

    code_point_info info(code_point cp) const;


private:
    std::string _code;
    std::string _file_name;
    std::vector<code_point> _line_changes;
    std::vector<token> _tokens;
};

#endif // CODE_H
