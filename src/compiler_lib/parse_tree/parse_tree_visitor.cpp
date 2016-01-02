#include "parse_tree_visitor.h"
#include "parse_tree.h"

#define VISIT_IMPL(_class_) void visitor::visit(_class_ *n) {\
        for (int i = 0; i < n->child_node_count(); ++i) { \
            if (n->child_node(i)) n->child_node(i)->accept(this); \
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
VISIT_IMPL(as_cast_expr)
VISIT_IMPL(value_list)
VISIT_IMPL(base_tree)
VISIT_IMPL(block)
VISIT_IMPL(for_stmt)
VISIT_IMPL(while_stmt)
VISIT_IMPL(do_while_stmt)
VISIT_IMPL(use_stmt)
VISIT_IMPL(use_alias_stmt)
VISIT_IMPL(import_stmt)
VISIT_IMPL(fn_decl)
VISIT_IMPL(struct_decl)
VISIT_IMPL(trait_decl)
VISIT_IMPL(variable_decl)
VISIT_IMPL(impl_decl)
VISIT_IMPL(impl_trait_decl)
VISIT_IMPL(module_decl)
VISIT_IMPL(namespace_decl)
VISIT_IMPL(basic_type)
VISIT_IMPL(template_type)
VISIT_IMPL(tuple_type)
VISIT_IMPL(function_type)
VISIT_IMPL(array_type)
VISIT_IMPL(splice_type)
VISIT_IMPL(owned_ptr_type)
VISIT_IMPL(raw_ptr_type)
VISIT_IMPL(ref_type)
VISIT_IMPL(ref_ref_type)
VISIT_IMPL(attribute)
VISIT_IMPL(attribute_list)
VISIT_IMPL(u8_string_literal)
VISIT_IMPL(u16_string_literal)
VISIT_IMPL(u32_string_literal)
VISIT_IMPL(ascii_string_literal)
VISIT_IMPL(integer_literal)
VISIT_IMPL(float_literal)
VISIT_IMPL(identifier)
VISIT_IMPL(scoped_identifier)
