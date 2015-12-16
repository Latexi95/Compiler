#pragma once
#include "../../common/string_view.h"
#include "../../common/code_point.h"

enum class token_type {
    t_identifier,
    t_float_literal,
    t_integer_literal,
    t_hex_literal,
    t_binary_literal,
    t_u8_string_literal,
    t_u16_string_literal,
    t_u32_string_literal,
    t_ascii_string_literal,
    t_target_comment,

    t_and,
    t_and_and,
    t_and_assign,
    t_assign,
    t_or,
    t_or_or,
    t_or_assign,
    t_xor,
    t_xor_assign,
    t_comma,
    t_colon,
    t_colon_colon,
    t_semicolon,
    t_divide,
    t_divide_assign,
    t_dot,
    t_dot_star,
    t_dot_ellipsis,
    t_equal,
    t_greater,
    t_greater_equal,
    t_minus,
    t_minus_assign,
    t_minus_minus,
    t_percent,
    t_percent_assign,
    t_not,
    t_not_equal,
    t_plus,
    t_plus_assign,
    t_plus_plus,
    t_question_mark,
    t_shift_left,
    t_shift_left_assign,
    t_shift_right,
    t_shift_right_assign,
    t_star,
    t_complement,           //~
    t_star_assign,          // *=
    t_pound,                //#
    t_pound_pound,          //##


    t_dollar,
    t_at,                   //@
    t_backslash,            //   \


    t_brace_left,
    t_brace_right,
    t_paren_left,
    t_paren_right,
    t_bracket_left,
    t_bracket_right,

    k_as,
    k_if,
    k_for,
    k_in,
    k_where,
    k_while,
    k_do,
    k_namespace,
    k_var,
    k_mut,
    k_match,
    k_enum,
    k_struct,
    k_use,
    k_break,
    k_trait,
    k_continue,
    k_extern,
    k_module,
    k_impl,
    k_public,
    k_protected,
    k_private,
    k_reinterpret_cast,
    k_compile_time,
    k_fn,
    k_return,

    invalid,
    last_token_type
};

class token
{
public:
    token(token_type t, string_view txt, code_point cp);
    token_type type() const { return _type; }
    string_view text() const { return _name; }
    code_point point() const { return _cp; }
    std::string to_string() const;

    static const char *token_type_name(token_type t);
private:
    token_type _type;
    string_view _name;
    code_point _cp;
};




