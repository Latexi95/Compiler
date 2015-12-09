#pragma once
#include "../common/error_producer.h"
#include "../common/code.h"

class lexer_error_producer : public error_producer
{
public:
    lexer_error_producer(const code &c);

    void missing_comment_ending(code_point commentStart, code_point fileEnd);
private:
    const code &_code;
};

