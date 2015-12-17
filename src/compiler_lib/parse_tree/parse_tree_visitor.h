#pragma once
namespace parse_tree {
class expr;

class visitor
{
public:
    visitor();
    virtual ~visitor();

    void visit(expr *n);
};

}
