#pragma once
#include <vector>
#include <memory>
#include "error_codes.h"
#include "code_point.h"

class error_handler;

class error_producer
{
public:
    error_producer();
    virtual ~error_producer();

    void add_handler(const std::shared_ptr<error_handler> &handler);

    virtual void error(error_code code, code_point_info location, const std::string &message);
    virtual void warning(warning_code code, code_point_info location, const std::string &message);

    int warning_count() const { return _warning_count; }
    int error_count() const { return _error_count; }

protected:
    std::vector<std::shared_ptr<error_handler>> _handlers;
    int _error_count;
    int _warning_count;
};

