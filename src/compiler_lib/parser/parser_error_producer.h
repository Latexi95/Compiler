#pragma once
#include "../../common/error_producer.h"
class code;
class parser_error_producer : public error_producer
{
public:
    parser_error_producer(const code &c);
    ~parser_error_producer();

    void expecting_identifier(int token_index);
private:
    const code &_code;
};

