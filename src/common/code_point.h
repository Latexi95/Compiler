#pragma once
#include "string_view.h"

struct code_point_info
{
    int column;
    int line;
    std::string file_name;

    std::string to_string() const;
};

typedef unsigned code_point;

