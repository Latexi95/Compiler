#pragma once
namespace parse_tree {
class expr;
class operation;
class unary_expr;
class as_cast_expr;
class value_list;
class base_tree;
class block;
class for_stmt;
class while_stmt;
class do_while_stmt;
class use_stmt;
class use_alias_stmt;
class import_stmt;
class fn_decl;
class struct_decl;
class trait_decl;
class variable_decl;
class impl_decl;
class impl_trait_decl;
class module_decl;
class namespace_decl;
class basic_type;
class template_type;
class tuple_type;
class function_type;
class array_type;
class splice_type;
class owned_ptr_type;
class raw_ptr_type;
class ref_type;
class ref_ref_type;
class attribute;
class attribute_list;
class u8_string_literal;
class u16_string_literal;
class u32_string_literal;
class ascii_string_literal;
class integer_literal;
class float_literal;
class identifier;
class scoped_identifier;

#define VISITOR_VISIT(_class_) virtual void visit(_class_ *n)

class visitor
{
public:
    visitor();
    virtual ~visitor();

    VISITOR_VISIT(expr);
    VISITOR_VISIT(operation);
    VISITOR_VISIT(unary_expr);
    VISITOR_VISIT(as_cast_expr);
    VISITOR_VISIT(value_list);
    VISITOR_VISIT(base_tree);
    VISITOR_VISIT(block);
    VISITOR_VISIT(for_stmt);
    VISITOR_VISIT(while_stmt);
    VISITOR_VISIT(do_while_stmt);
    VISITOR_VISIT(use_stmt);
    VISITOR_VISIT(use_alias_stmt);
    VISITOR_VISIT(import_stmt);
    VISITOR_VISIT(fn_decl);
    VISITOR_VISIT(struct_decl);
    VISITOR_VISIT(trait_decl);
    VISITOR_VISIT(variable_decl);
    VISITOR_VISIT(impl_decl);
    VISITOR_VISIT(impl_trait_decl);
    VISITOR_VISIT(module_decl);
    VISITOR_VISIT(namespace_decl);
    VISITOR_VISIT(basic_type);
    VISITOR_VISIT(template_type);
    VISITOR_VISIT(tuple_type);
    VISITOR_VISIT(function_type);
    VISITOR_VISIT(array_type);
    VISITOR_VISIT(splice_type);
    VISITOR_VISIT(owned_ptr_type);
    VISITOR_VISIT(raw_ptr_type);
    VISITOR_VISIT(ref_type);
    VISITOR_VISIT(ref_ref_type);
    VISITOR_VISIT(attribute);
    VISITOR_VISIT(attribute_list);
    VISITOR_VISIT(u8_string_literal);
    VISITOR_VISIT(u16_string_literal);
    VISITOR_VISIT(u32_string_literal);
    VISITOR_VISIT(ascii_string_literal);
    VISITOR_VISIT(integer_literal);
    VISITOR_VISIT(float_literal);
    VISITOR_VISIT(identifier);
    VISITOR_VISIT(scoped_identifier);
};
#undef VISITOR_VISIT
}
