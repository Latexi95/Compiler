#include "code_point.h"



std::string code_point_info::to_string() const
{
    return file_name + ':' + std::to_string(line) + ':' + std::to_string(column);
}
