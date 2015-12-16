#pragma once

#include "../compiler_lib/common/token.h"
#include "../compiler_lib/common/code.h"
#include <initializer_list>

class token_list_matcher {
public:
    bool does_match(const std::vector<token> &m) const;

    void set_match_list(const std::initializer_list<token_type> &list)  { _match_list.assign(list); }
    void set_match_list(const std::vector<token_type> &list) { _match_list = list;}
private:
    std::vector<token_type> _match_list;
};

