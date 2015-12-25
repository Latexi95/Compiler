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
    as_cast_expr,


    base_tree,
    block,

    for_stmt,
    while_stmt,
    do_while_stmt,
    use_stmt,
    use_alias_stmt,

    fn_decl,
    struct_decl,
    trait_decl,
    variable_decl,
    impl_decl,


    attribute,

    u8_string_literal,
    u16_string_literal,
    u32_string_literal,
    ascii_string_literal,
    integer_literal,
    float_literal,
    identifier
};

#define DEF_CHILD_NODE(_name_, _index_) \
    node *_name_ () const { return child_node(_index_); } \
    void set_ ## _name_ (node *n) { set_child_node(_index_, n); }

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

class identifier : public literal<string_view, node_type::identifier> {};

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

class attribute : public branch_node<attribute, 2, node_type::attribute> {
public:
    attribute(code_point cp);
    ~attribute();

    DEF_CHILD_NODE(identifier, 0)
    DEF_CHILD_NODE(param_list, 1)
};

class as_cast_expr : public branch_node<as_cast_expr, 2, node_type::as_cast_expr> {
public:
    as_cast_expr(code_point cp);
    ~as_cast_expr();

    DEF_CHILD_NODE(operand, 0)
    DEF_CHILD_NODE(target_type, 1)
};


class block : public variable_branch_node<block, node_type::block> {
public:
    block(code_point cp_start, code_point cp_end);
    ~block();

    virtual node *child_node(int i) const {
        assert(i >= 0 && i <= _child_nodes.size());
        return _child_nodes[i];
    }
    virtual void set_child_node(int i, node *n) {
        assert(i >= 0 && i <= _child_nodes.size());
        _child_nodes[i] = n;
    }
    virtual int child_node_count() const { return _child_nodes.size(); }
protected:
    std::vector<node*> _child_nodes;
};

class use_stmt : public branch_node<use_stmt, 2, node_type::use_stmt> {
public:
    use_stmt(code_point cp);
    ~use_stmt();

    DEF_CHILD_NODE(attribute, 0)
    DEF_CHILD_NODE(param, 1)
};

class use_alias_stmt : public branch_node<use_alias_stmt, 3, node_type::use_alias_stmt> {
public:
    use_alias_stmt(code_point cp_start, code_point cp_as);
    ~use_alias_stmt();

    DEF_CHILD_NODE(attribute, 0)
    DEF_CHILD_NODE(source, 1)
    DEF_CHILD_NODE(identifier, 2)

};

class while_stmt : public branch_node<while_stmt, 3, node_type::while_stmt> {
public:
    while_stmt(code_point cp_start, code_point cp_end);
    ~while_stmt();

    DEF_CHILD_NODE(attribute, 0)
    DEF_CHILD_NODE(condition, 1)
    DEF_CHILD_NODE(block, 2)
};

class do_while_stmt : public branch_node<do_while_stmt, 3, node_type::do_while_stmt> {
public:
    do_while_stmt(code_point cp_start, code_point cp_end);
    ~do_while_stmt();

    DEF_CHILD_NODE(attribute, 0)
    DEF_CHILD_NODE(block, 1)
    DEF_CHILD_NODE(condition, 2)
};

class for_stmt : public branch_node<for_stmt, 4, node_type::for_stmt> {
public:
    do_while_stmt(code_point cp_start, code_point cp_end);
    ~do_while_stmt();

    DEF_CHILD_NODE(attribute, 0)
    DEF_CHILD_NODE(variable, 1)
    DEF_CHILD_NODE(range, 2)
    DEF_CHILD_NODE(block, 3)
};


class fn_decl : public branch_node<fn_decl, 6, node_type::fn_decl> {
public:
    fn_decl(code_point cp_start, code_point cp_end);
    ~fn_decl();

    DEF_CHILD_NODE(attribute, 0)
    DEF_CHILD_NODE(identifier, 1)
    DEF_CHILD_NODE(template_param_list, 2)
    DEF_CHILD_NODE(param_list, 3)
    DEF_CHILD_NODE(ret_type, 4)
    DEF_CHILD_NODE(block, 5)
};

class struct_decl : public branch_node<struct_decl, 4, node_type::struct_decl> {
public:
    struct_decl(code_point start_cp, code_point end_cp);
    ~struct_decl();

    DEF_CHILD_NODE(attribute, 0)
    DEF_CHILD_NODE(identifier, 1)
    DEF_CHILD_NODE(template_param_list, 2)
    DEF_CHILD_NODE(variable_decls, 3)
};

class trait_decl : public branch_node<trait_decl, 5, node_type::trait_decl> {
    trait_decl(code_point start_cp, code_point end_cp);
    ~trait_decl();

    DEF_CHILD_NODE(attribute, 0)
    DEF_CHILD_NODE(template_param_list, 2)

    DEF_CHILD_NODE(identifier, 1)

    DEF_CHILD_NODE(fn_decls, 3)
    DEF_CHILD_NODE(use_stmts, 4)
};


}

