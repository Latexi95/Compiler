#pragma once
#include <memory>
#include "ast_allocator.h"
#include "../../common/code_point.h"

namespace ast {

class node {


protected:
    code_point _cp;
};








class ast_base {
    ast_base();
    ~ast_base();


private:
    std::unique_ptr<ast_allocator> _allocator;
};



}


