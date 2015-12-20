#include "parse_tree.h"
using namespace parse_tree;
expr::expr(code_point cp) :
    variable_branch_node<expr, node_type::expr>(cp, cp)
{
    _child_nodes.reserve(4);
    _child_nodes.resize(1);
    _child_nodes[0] = 0;
}

void expr::set_operations(const std::vector<node *> &operations)
{
    _child_nodes.resize(1 + operations.size());
    std::copy(operations.begin(), operations.end(), _child_nodes.begin() + 1);
}

unary_expr::unary_expr(code_point cp) :
    branch_node<unary_expr, 1, node_type::unary_expr>(cp, cp),
    _operator(unary_oper::invalid)
{}

operation::operation(code_point cp) :
    branch_node<operation, 1, node_type::operation>(cp, cp),
    _operator(oper::invalid)
{

}
