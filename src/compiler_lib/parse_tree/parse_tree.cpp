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

as_cast_expr::as_cast_expr(code_point cp) :
    branch_node<as_cast_expr, 2, node_type::as_cast_expr>(cp, cp)
{}

as_cast_expr::~as_cast_expr() {}

block::block(code_point cp_start, code_point cp_end) :
    variable_branch_node<block, node_type::block>(cp_start, cp_end)
{}

block::~block()
{ }

attribute::attribute(code_point cp) :
    branch_node<attribute, 2, node_type::attribute>(cp, cp)
{
}

attribute::~attribute()
{
}

while_stmt::while_stmt(code_point cp_start, code_point cp_end) :
    branch_node<while_stmt, 3, node_type::while_stmt>(cp_start, cp_end)
{
}

while_stmt::~while_stmt()
{
}



do_while_stmt::do_while_stmt(code_point cp_start, code_point cp_end) :
    branch_node<do_while_stmt, 3, node_type::do_while_stmt>(cp_start, cp_end)
{
}

do_while_stmt::~do_while_stmt()
{
}

use_stmt::use_stmt(code_point cp) :
    branch_node<use_stmt, 2, node_type::use_stmt>(cp, cp)
{
}

use_stmt::~use_stmt() { }

use_alias_stmt::use_alias_stmt(code_point cp_start, code_point cp_as) :
    branch_node<use_alias_stmt, 3, node_type::use_alias_stmt>(cp_start, cp_as)
{ }

use_alias_stmt::~use_alias_stmt() { }

fn_decl::fn_decl(code_point cp_start, code_point cp_end) :
    branch_node<fn_decl, 6, node_type::fn_decl>(cp_start, cp_end)
{ }

fn_decl::~fn_decl() { }

struct_decl::struct_decl(code_point start_cp, code_point end_cp) :
    branch_node<trait_decl, 4, node_type::trait_decl> (start_cp, end_cp)
{ }

struct_decl::~struct_decl() { }

trait_decl::trait_decl(code_point start_cp, code_point end_cp) :
    branch_node<trait_decl, 5, node_type::trait_decl>(start_cp, end_cp)
{ }

trait_decl::~trait_decl() { }
