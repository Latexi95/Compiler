#pragma once
#include "../../common/code_point.h"
#include "../../common/string_view.h"
#include "parse_tree_visitor.h"
#include <cassert>
#include "../common/operators.h"
#include <boost/multiprecision/cpp_int.hpp>

using boost::multiprecision::int128_t;
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
    virtual void set_child_node(int, node *) = 0;
    virtual int child_node_count() const = 0;
    virtual code_point location() const = 0;
    virtual code_point start_location() const = 0;
    virtual code_point end_location() const = 0;

    virtual void accept(visitor *v) = 0;
protected:
};
template <typename T, node_type TYPE>
class leaf_node : public node {
public:
    static const node_type static_type = TYPE;
    leaf_node(code_point cp) : _cp(cp) {}
    virtual ~leaf_node() {}

    virtual node_type type() const { return TYPE; }

    virtual node *child_node(int) const { return 0; }
    virtual void set_child_node(int, node *) { }
    virtual int child_node_count() const { return 0; }

    virtual code_point location() const { return _cp; }
    virtual code_point start_location() const { return _cp; }
    virtual code_point end_location() const { return _cp; }

    virtual void accept(visitor *v) { v->visit(static_cast<T*>(this));}
protected:
    code_point _cp;
};

template <typename TARGET_T>
TARGET_T *node_cast(node *n) {
    assert(n->type() == TARGET_T::static_type);
    return static_cast<TARGET_T*>(n);
}

template <typename T, node_type TYPE>
class variable_branch_node : public node{
public:
    static const node_type static_type = TYPE;
    variable_branch_node(code_point start_cp, code_point end_cp) : _start_cp(start_cp), _end_cp(end_cp) {}
    ~variable_branch_node() {}

    virtual node_type type() const { return TYPE; }

    virtual code_point location() const { return _start_cp; }
    virtual code_point start_location() const { return _start_cp; }
    virtual code_point end_location() const { return _end_cp; }

    virtual void accept(visitor *v) { v->visit(static_cast<T*>(this));}
protected:
    code_point _start_cp, _end_cp;
};

template <typename T, int CHILD_NODE_COUNT, node_type TYPE>
class branch_node : public variable_branch_node<T, TYPE> {
public:
    branch_node(code_point start_cp, code_point end_cp) :
        variable_branch_node<T, TYPE>(start_cp, end_cp)
    {std::fill(_child_nodes, _child_nodes + CHILD_NODE_COUNT, nullptr);}
    ~branch_node() {}

    virtual node *child_node(int i) const {
        assert(i >= 0 && i <= CHILD_NODE_COUNT);
        return _child_nodes[i]; }
    virtual void set_child_node(int i, node *n) {
        assert(i >= 0 && i <= CHILD_NODE_COUNT);
        _child_nodes[i] = n; }
    virtual int child_node_count() const { return CHILD_NODE_COUNT; }
protected:
    node *_child_nodes[CHILD_NODE_COUNT];
};

template <typename T, node_type TYPE>
class literal : public branch_node<literal<T, TYPE>, 1, TYPE>{
public:
    literal(code_point cp) : branch_node<literal<T, TYPE>, 1, TYPE>(cp) {}
    ~literal() {}

    void set_value(T &&v) { _value = v; }
    T value() const { return _value; }
    node *suffix() const { return this->child_node(0); }
    void set_suffix(node *n) { this->set_child_node(0, n); }

protected:
    T _value;
};

class u8_string_literal : public literal<string_view, node_type::u8_string_literal> {};
class u16_string_literal : public literal<string_view, node_type::u16_string_literal> {};
class u32_string_literal : public literal<string_view, node_type::u32_string_literal> {};
class ascii_string_literal : public literal<string_view, node_type::ascii_string_literal> {};
class float_literal : public literal<double, node_type::float_literal> {};
class integer_literal : public literal<int128_t, node_type::integer_literal> {};


class unary_expr : public branch_node<unary_expr, 1, node_type::unary_expr> {
public:
    unary_expr(code_point cp);
    ~unary_expr();

    unary_oper op() const { return _operator; }
    void set_op(unary_oper o) { _operator = o; }
    node *operand() const { return this->child_node(0); }
    void set_operand(node *n) { this->set_child_node(0, n); }
protected:
    unary_oper _operator;
};

class operation : public branch_node<operation, 1, node_type::operation> {
public:
    operation(code_point cp);
    ~operation();
    void set_operand(node *n) { this->set_child_node(0, n); }
    node *operand() const { return this->child_node(0); }
    oper op() const { return _operator; }
    void set_op(oper o) { _operator = o; }
protected:
    oper _operator;
};

class expr : public variable_branch_node<expr, node_type::expr> {
public:
    expr(code_point cp);
    ~expr() {}
    virtual node *child_node(int i) const {
        assert(i >= 0 && i <= _child_nodes.size());
        return _child_nodes[i];
    }
    virtual void set_child_node(int i, node *n) {
        assert(i >= 0 && i <= _child_nodes.size());
        _child_nodes[i] = n;
    }

    virtual int child_node_count() const { return _child_nodes.size(); }

    node *first_operand() const { return _child_nodes[0]; }
    void set_operations(const std::vector<node*> &operations);
private:
    std::vector<node*> _child_nodes;
};

}

