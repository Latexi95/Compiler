#include "lexer.h"
#include "lexer_error_producer.h"
#include <unordered_map>

lexer::lexer(const code &c) :
    _c(c),
    _err(new lexer_error_producer(c)),
    _cp(c.begin())
{

}

bool lexer::parse()
{
    fatal_error = false;
    char c;
    while ((c = next_rc()) != '\0' && !fatal_error) {
        if (skip_spaces()) continue;
        if (handle_comments()) continue;
        if (handle_operators()) continue;
        if (handle_identifiers()) continue;
    }
    return true;
}

error_producer &lexer::err_producer()
{
    return *_err;
}

bool lexer::is_space(char c)
{
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

bool lexer::is_alpha(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool lexer::is_number(char c)
{
    return c >= '0' && c <= '9';
}

token_type lexer::is_keyword(code_point start, code_point end)
{
    /*
     * k_if,
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
     */
    static std::unordered_map<string_view, token_type> keywordmap = {
        {"for"_sv, token_type::k_for},
        {"in"_sv, token_type::k_in},
        {"where"_sv, token_type::k_where},
        {"while"_sv, token_type::k_while},
        {"do", token_type::k_do},
        {"namespace", token_type::k_namespace},
        {"var", token_type::k_var},
        {"mut", token_type::k_mut},
        {"match", token_type::k_match},
        {"enum"_sv, token_type::k_enum},
        {"struct", token_type::k_struct},
        {"use", token_type::k_use},
        {"break", token_type::k_break},
        {"trait", token_type::k_trait},
        {"continue", token_type::k_continue},
        {"extern", token_type::k_extern},
        {"module", token_type::k_module},
        {"impl", token_type::k_impl},
        {"public", token_type::k_public},
        {"protected", token_type::k_protected},
        {"private", token_type::k_private},
        {"reinterpret_cast", token_type::k_reinterpret_cast}
    };

    auto it = keywordmap.find(_c.view_range(start, end));
    if (it != keywordmap.end()) {
        return it->second;
    }
    return token_type::invalid;
}

void lexer::add_token(token_type t, code_point cp, code_point end)
{
    _tokens.emplace_back(t, _c.view_range(cp, end), cp);
}

bool lexer::skip_spaces()
{
    char c = ca();
    if (!is_space(c)) return false;
    do {
        c = next_rc();
    } while (is_space(c));
    return true;
}

void lexer::skip(size_t steps)
{
    for (size_t i = 0; i < steps; ++i) {
        next_rc();
    }
}

struct op_token {
    token_type type;
    const char *key;
};

bool lexer::handle_operators()
{
#define OP(token_t, _key) {token_type::token_t, _key}
    static op_token op_tokens[] = {
        OP(t_and_and, "&&"),
        OP(t_and_assign, "&="),
        OP(t_and, "&&"),

        OP(t_equal, "=="),
        OP(t_assign, "="),

        OP(t_or_or, "||"),
        OP(t_or_assign, "|="),
        OP(t_or, "|"),

        OP(t_xor_assign, "^="),
        OP(t_xor, "^"),

        OP(t_comma, ","),
        OP(t_colon, ":"),
        OP(t_colon_colon, "::"),
        OP(t_semicolon, ";"),

        OP(t_divide_assign, "/="),
        OP(t_divide, "/"),

        OP(t_dot_star, ".*"),
        OP(t_dot_ellipsis, ".~"),
        OP(t_dot, "."),

        OP(t_greater, ">"),
        OP(t_greater_equal, ">="),

        OP(t_minus_assign, "-="),
        OP(t_minus_minus, "--"),
        OP(t_minus, "-"),

        OP(t_percent, "%"),
        OP(t_percent_assign, "%="),

        OP(t_not_equal, "!="),
        OP(t_not, "!"),

        OP(t_plus_assign, "+="),
        OP(t_plus_plus, "++"),
        OP(t_plus, "+"),

        OP(t_question_mark, "?"),

        OP(t_shift_left_assign, "<<="),
        OP(t_shift_left, "<<"),

        OP(t_shift_right_assign, ">>="),
        OP(t_shift_right, ">>"),

        OP(t_star, "*"),
        OP(t_complement, "~"),           //~
        OP(t_star_assign, "*="),          // *=
        OP(t_pound,  "#"),               //#
        OP(t_pound_pound, "##"),          //##


        OP(t_dollar, "$"),
        OP(t_at, "@"),                   //@


        OP(t_brace_left, "{"),
        OP(t_brace_right, "}"),
        OP(t_paren_left, "("),
        OP(t_paren_right, ")"),
        OP(t_bracket_left, "["),
        OP(t_bracket_right, "]")
    };

    for (unsigned i = 0; i < sizeof(op_tokens); ++i) {
        int ci = 0;
        while (ca(ci) == op_tokens[i].key[ci]) {
            ++ci;
            if (op_tokens[i].key[ci] == 0) {
                add_token(op_tokens[i].type, _cp, _cp + ci);
                skip(ci - 1);
                return true;
            }
        }
    }
    return false;
}

bool lexer::handle_comments()
{
    code_point commentStart = _cp;
    if (ca() == '/') {
        if (ca(1) == '/') {
            skip(2);
            bool isTargettedComment = false;
            if (ca() == '/') {
                skip(1);
                isTargettedComment = true;
            }
            code_point start = _cp;
            code_point lastNonSpaceChar = _cp;
            char c = '\0';
            while ((c = next_rc()) != '\0' && c != '\n') {
                if (!is_space(c)) {
                    lastNonSpaceChar = _cp;
                }
            }

            if (isTargettedComment) {
                add_token(token_type::t_target_comment, start, lastNonSpaceChar + 1);
            }
            return true;
        }
        else if (ca(1) == '*') {
            skip(2);
            bool isTargettedComment = false;
            if (ca() == '*') {
                skip(1);
                isTargettedComment = true;
            }

            char c, lastC = '\0';
            code_point start = _cp;
            while ((c = next_rc()) != '\0' && !(lastC == '*' && c == '/')) {
                lastC = c;
            }
            if (c == '\0') {
                _err->missing_comment_ending(commentStart, _cp);
                return true;
            }

            if (isTargettedComment) {
                add_token(token_type::t_target_comment, start, _cp);
            }
            return true;
        }
    }
    return false;
}

bool lexer::handle_identifiers()
{
    char c = ca();
    code_point start = _cp;
    if (is_alpha(c) || c == '_') {
        while ((c = next_r()) && 
               (
                   is_alpha(c) ||
                   c == '_' ||
                   is_number(c))
               ) {
        }
        token_type keyword_type = is_keyword(start, _cp);
        if (keyword_type != token_type::invalid) {
            add_token(keyword_type, start, _cp);
        }
        
        add_token(token_type::t_identifier, start, _cp);
        return true;
    }
    return false;
}


char lexer::ca(code_point offset)
{
    return _c.c(_cp + offset);
}

char lexer::next_rc()
{
    char c = _c.c(_cp);
    _cp = _c.next(_cp);
    return c;
}

char lexer::next_r()
{
    _cp = _c.next(_cp);
    return _c.c(_cp);
}
