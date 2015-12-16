#include "code.h"
#include <cassert>
#include <fstream>

code::code(std::string file_name) :
    _file_name(file_name)
{

}

void code::set_code(const std::string &code)
{
    _code = code;
    for (code_point i = 0; i < _code.size(); ++i) {
        if (_code[i] == '\n') {
            _line_changes.push_back(i);
        }
    }
}

string_view code::view(code_point cp, size_t size) const
{
    if (_code.size() >= cp + size)
        return string_view(&_code[cp], size);
    if (cp >= _code.size()) return string_view();

    code_point len = (_code.size() - cp);
    return string_view(&_code[cp], len);
}

string_view code::view_range(code_point cp, size_t cpEnd) const
{
    return view(cp, cpEnd - cp);
}

code_point_info code::info(code_point cp) const
{
    unsigned line = 1;
    code_point lastLineChange = 0;
    for (code_point lc : _line_changes) {
        if (cp < lc) {
            break;
        }
        lastLineChange = lc;
        ++line;
    }

    return code_point_info {
                (int)(cp - lastLineChange),
                (int)line,
                _file_name
    };
}

void code::add_string_literal(code_point location, const std::string &lit)
{
    _string_literals[location] = lit;
}

string_view code::string_literal(code_point location) const
{
    auto it = _string_literals.find(location);
    assert(it != _string_literals.end());
    return string_view(it->second);
}

void code::print_tokens()
{
    std::ofstream out("tokens.txt");



}

