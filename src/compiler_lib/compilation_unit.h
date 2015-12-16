#pragma once
#include <memory>
class lexer;
class parser;
class code;
class error_handler;

class compilation_unit
{
public:
    compilation_unit(const std::string &file_name);
    ~compilation_unit();
    void set_code(const std::string &code);

    void add_error_handler(const std::shared_ptr<error_handler> &err_handler);

private:
    std::unique_ptr<code> _code;
    std::unique_ptr<lexer> _lexer;
    std::unique_ptr<parser> _parser;
};

