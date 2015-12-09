#pragma once
#include "string_view.h"
#include <vector>
#include "error_codes.h"
#include "code_point.h"
class error_producer;

class error_handler
{
public:
    error_handler();
    virtual void handle_error(error_code code, code_point_info location, const std::string &msg) = 0;
    virtual void handle_warning(warning_code code, code_point_info location, const std::string &msg) = 0;
private:
};

class console_print_error_handler : public error_handler {
    virtual void handle_error(error_code code, code_point_info location, const std::string &msg);
    virtual void handle_warning(warning_code code, code_point_info location, const std::string &msg);
};


