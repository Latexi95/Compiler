#include "error_handler.h"
#include <iostream>

error_handler::error_handler()
{

}


void console_print_error_handler::handle_error(error_code code, code_point_info location, const std::string &msg)
{
    std::cerr << location.to_string() << " Error " << (int)code << ": " << msg;
}

void console_print_error_handler::handle_warning(warning_code code, code_point_info location, const std::string &msg)
{
    std::cerr << location.to_string() << " Warning " << (int)code << ": " << msg;
}
