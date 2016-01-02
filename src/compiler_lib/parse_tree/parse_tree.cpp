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
{ }

operation::operation(code_point cp) :
    branch_node<operation, 1, node_type::operation>(cp, cp),
    _operator(oper::invalid)
{ }

as_cast_expr::as_cast_expr(code_point cp) :
    branch_node<as_cast_expr, 2, node_type::as_cast_expr>(cp, cp)
{}

as_cast_expr::~as_cast_expr() {}


basic_type::basic_type(code_point cp) :
    branch_node<basic_type, 2, node_type::basic_type>(cp, cp)
{ }

basic_type::~basic_type()
{ }

template_type::template_type(code_point cp) :
    branch_node<template_type, 3, node_type::template_type>(cp, cp)
{ }

template_type::~template_type()
{ }

tuple_type::tuple_type(code_point cp_start, code_point cp_end) :
    variable_branch_node<tuple_type, node_type::tuple_type>(cp_start, cp_end)
{ }

tuple_type::~tuple_type() { }

void tuple_type::append_child(node *n)
{
    _child_nodes.push_back(n);
}

function_type::function_type(code_point cp) :
    branch_node<function_type, 3, node_type::function_type>(cp, cp)
{ }

function_type::~function_type()
{ }



block::block(code_point cp_start, code_point cp_end) :
    variable_branch_node<block, node_type::block>(cp_start, cp_end)
{
    _child_nodes.reserve(3);
    _child_nodes.resize(1);
}

block::~block()
{ }

void block::append_child(node *n)
{
    _child_nodes.push_back(n);
}

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
    branch_node<struct_decl, 4, node_type::struct_decl> (start_cp, end_cp)
{ }

struct_decl::~struct_decl() { }

trait_decl::trait_decl(code_point start_cp, code_point end_cp) :
    branch_node<trait_decl, 4, node_type::trait_decl>(start_cp, end_cp)
{ }

trait_decl::~trait_decl() { }

variable_decl::variable_decl(code_point cp) :
    branch_node<variable_decl, 3, node_type::variable_decl>(cp, cp)
{ }

variable_decl::~variable_decl() { }


impl_trait_decl::impl_trait_decl(code_point start_cp, code_point end_cp) :
    branch_node<impl_trait_decl, 5, node_type::impl_trait_decl>(start_cp, end_cp)
{ }

impl_trait_decl::~impl_trait_decl() { }

impl_decl::impl_decl(code_point start_cp, code_point end_cp) :
    branch_node<impl_decl, 3, node_type::impl_decl>(start_cp, end_cp)
{ }

impl_decl::~impl_decl() { }


for_stmt::for_stmt(code_point cp_start, code_point cp_end) :
    branch_node<for_stmt, 4, node_type::for_stmt>(cp_start, cp_end)
{ }

for_stmt::~for_stmt() { }

scoped_identifier::scoped_identifier(code_point cp_start, code_point cp_end) :
    variable_branch_node<scoped_identifier, node_type::scoped_identifier>(cp_start, cp_end)
{ }

scoped_identifier::~scoped_identifier() { }

void scoped_identifier::append_child(node *n)
{
    _child_nodes.push_back(n);
}

base_tree::base_tree(code_point start_cp, code_point end_cp) :
    branch_node<base_tree, 2, node_type::impl_decl>(start_cp, end_cp)
{ }

base_tree::~base_tree() { }

module_decl::module_decl(code_point cp) :
    branch_node<module_decl, 2, node_type::module_decl>(cp, cp)
{ }

module_decl::~module_decl() { }

namespace_decl::namespace_decl(code_point start_cp, code_point end_cp) :
    branch_node<namespace_decl, 3, node_type::namespace_decl> (start_cp, end_cp)
{ }

namespace_decl::~namespace_decl() { }


import_stmt::import_stmt(code_point cp) :
    branch_node<import_stmt, 3, node_type::import_stmt>(cp, cp)
{ }

import_stmt::~import_stmt() { }

splice_type::splice_type(code_point cp) :
    branch_node<splice_type, 2, node_type::array_type>(cp, cp)
{ }

splice_type::~splice_type() { }

owned_ptr_type::owned_ptr_type(code_point cp) :
    branch_node<owned_ptr_type, 2, node_type::owned_ptr_type>(cp, cp)
{ }

owned_ptr_type::~owned_ptr_type() { }

raw_ptr_type::raw_ptr_type(code_point cp) :
    branch_node<raw_ptr_type, 2, node_type::raw_ptr_type>(cp, cp)
{ }

raw_ptr_type::~raw_ptr_type() { }

ref_type::ref_type(code_point cp) :
    branch_node<ref_type, 2, node_type::ref_type>(cp, cp)
{ }

ref_type::~ref_type() { }

ref_ref_type::ref_ref_type(code_point cp) :
    branch_node<ref_ref_type, 2, node_type::ref_ref_type>(cp, cp)
{ }

ref_ref_type::~ref_ref_type() { }
