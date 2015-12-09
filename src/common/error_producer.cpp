#include "error_producer.h"
#include "error_handler.h"

error_producer::error_producer() :
    _error_count(0),
    _warning_count(0)
{

}

error_producer::~error_producer()
{

}

void error_producer::add_handler(const std::shared_ptr<error_handler> &handler)
{
    _handlers.push_back(handler);
}

void error_producer::error(error_code code, code_point_info location, const std::string &message)
{
    ++_error_count;
    for (const auto &handler : _handlers) {
        handler->handle_error(code, location, message);
    }
}

void error_producer::warning(warning_code code, code_point_info location, const std::string &message)
{
    ++_warning_count;
    for (const auto &handler : _handlers) {
        handler->handle_warning(code, location, message);
    }
}

