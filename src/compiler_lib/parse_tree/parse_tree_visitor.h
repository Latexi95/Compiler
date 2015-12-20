#pragma once
namespace parse_tree {
class expr;
class operation;
class unary_expr;
class integer_literal;
class float_literal;
class u8_string_literal;
class u16_string_literal;
class u32_string_literal;
class ascii_string_literal;

#define VISITOR_VISIT(_class_) virtual void visit(_class_ *n)

class visitor
{
public:
    visitor();
    virtual ~visitor();

    VISITOR_VISIT(expr);
    VISITOR_VISIT(operation);
    VISITOR_VISIT(unary_expr);
    VISITOR_VISIT(integer_literal);
    VISITOR_VISIT(float_literal);
    VISITOR_VISIT(u8_string_literal);
    VISITOR_VISIT(u16_string_literal);
    VISITOR_VISIT(u32_string_literal);
    VISITOR_VISIT(ascii_string_literal);
};

}
