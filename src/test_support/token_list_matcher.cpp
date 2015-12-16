#include "token_list_matcher.h"
#include <iostream>

bool token_list_matcher::does_match(const std::vector<token> &m) const
{
    std::size_t len = std::max(m.size(), _match_list.size());
    for (std::size_t i = 0; i < len; ++i) {
        token_type expected_type = _match_list[i];
        if (m[i].type() != _match_list[i]) {
            std::cerr << "Token " << (i + 1) << " is invalid. Expected type \""
                      << token::token_type_name(expected_type) << "\", got \""
                      << m[i].to_string() << "\"" << std::endl;
            return false;
        }
    }
    if (_match_list.size() < m.size()) {
        std::cerr << "Got extra tokens. Expected " << _match_list.size() << " tokens, got " << m.size() << std::endl;
        return false;
    }
    else if (m.size() < _match_list.size()) {
        std::cerr << "Expecting more tokens. Expected " << (_match_list.size() - m.size()) << " more tokens" << std::endl;
        return false;
    }
    return true;
}
