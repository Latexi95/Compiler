#pragma once
#include "../../common/code_point.h"
#include "parse_tree_visitor.h"
#include <cassert>

namespace parse_tree {
enum class node_type {
    expr,
    operation,
    unary_expr,

    u8_string_literal,
    u16_string_literal,
    u32_string_literal,
    ascii_string_literal,
    integer_literal,
    float_literal,

};

class node {
public:
    node() {}
    virtual ~node() {}
    virtual node_type type() const = 0;
    virtual node *child_node(int) const = 0;
    virtual int child_node_count() const = 0;
    virtual node *location() const = 0;
    virtual node *start_location() const = 0;
    virtual node *end_location() const = 0;

    virtual void accept(visitor *v) = 0;
protected:
};
template <typename T, node_type TYPE>
class leaf_node {
public:
    static const node_type static_type = TYPE;
    leaf_node(code_point cp) : _cp(cp) {}
    virtual ~leaf_node() {}

    virtual node_type type() const { return TYPE; }

    virtual node *child_node(int) const { return 0; }
    virtual int child_node_count() const { return 0; }

    virtual node *location() const { return _cp; }
    virtual node *start_location() const { return _cp; }
    virtual node *end_location() const { return _cp; }

    virtual void accept(visitor *v) { v->visit(static_cast<T*>(this));}
protected:
    code_point _cp;
};


template <typename T, node_type TYPE>
class variable_branch_node {
public:
    static const node_type static_type = TYPE;
    variable_branch_node(code_point start_cp, code_point end_cp) : _start_cp(start_cp), _end_cp(end_cp) {}

    virtual node_type type() const { return TYPE; }

    virtual node *location() const { return _start_cp; }
    virtual node *start_location() const { return _start_cp; }
    virtual node *end_location() const { return _end_cp; }

    virtual void accept(visitor *v) { v->visit(static_cast<T*>(this));}
protected:
    code_point _start_cp, _end_cp;
};

template <typename T, int CHILD_NODE_COUNT, node_type TYPE>
class branch_node : public variable_branch_node<T, TYPE> {
public:
    branch_node(code_point start_cp, code_point end_cp) : variable_branch_node<T, TYPE>(start_cp, end_cp) {}

    virtual node *child_node(int i) const {
        assert(i >= 0 && i <= CHILD_NODE_COUNT);
        return child_nodes[i]; }
    virtual int child_node_count() const { return CHILD_NODE_COUNT; }
protected:
    node *child_nodes[CHILD_NODE_COUNT];
};



class expr : variable_branch_node<expr, node_type::expr> {

};

}

