#include "parse_tree_visitor.h"
#include "parse_tree.h"

#define VISIT_IMPL(_class_) void visitor::visit(_class_ *n) {\
        for (int i = 0; i < n->child_node_count(); ++i) { \
            n->child_node(i)->accept(this); \
        } \
    }

using namespace parse_tree;
visitor::visitor()
{

}

visitor::~visitor()
{

}

VISIT_IMPL(expr)
VISIT_IMPL(operation)
VISIT_IMPL(unary_expr)
VISIT_IMPL(integer_literal)
VISIT_IMPL(float_literal)
VISIT_IMPL(u8_string_literal)
VISIT_IMPL(u16_string_literal)
VISIT_IMPL(u32_string_literal)
VISIT_IMPL(ascii_string_literal)
