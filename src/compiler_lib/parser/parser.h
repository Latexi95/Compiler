#pragma once
#include "../../common/code_point.h"
#include "../../common/string_view.h"
#include <memory>

class code;
class parser_error_producer;

class parser
{
public:
    parser(const code &c);
    ~parser();



private:
    const code &_code;
    std::unique_ptr<parser_error_producer> _err;
};

