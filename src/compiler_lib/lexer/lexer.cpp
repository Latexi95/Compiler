#include "lexer.h"
#include "lexer_error_producer.h"
#include "lexer_settings.h"
#include <unordered_map>


lexer::lexer(code &c) :
    _c(c),
    _err(new lexer_error_producer(c)),
    _cp(c.begin())
{

}

lexer::~lexer()
{

}

bool lexer::tokenize()
{
    _fatal_error = false;
    _cp = 0;
    while (ch() != '\0' && !_fatal_error) {
        if (skip_spaces()) continue;
        if (handle_comments()) continue;
        if (handle_operators()) continue;
        if (handle_string_literals()) continue;
        if (handle_identifiers()) continue;
        if (handle_numbers()) continue;

        _err->unexpected_character(_cp, ch());
        break;
    }
    return _err->error_count() == 0;
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

bool lexer::is_hex(char c)
{
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

token_type lexer::is_keyword(code_point start, code_point end)
{
    static std::unordered_map<string_view, token_type> keywordmap = {
        {"as", token_type::k_as},
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
        {"reinterpret_cast", token_type::k_reinterpret_cast},
        {"compile_time", token_type::k_compile_time},
        {"fn", token_type::k_fn},
        {"return", token_type::k_return}
    };

    auto it = keywordmap.find(_c.view_range(start, end));
    if (it != keywordmap.end()) {
        return it->second;
    }
    return token_type::invalid;
}

void lexer::add_token(token_type t, code_point cp, code_point end)
{
    _c.add_token(token(t, _c.view_range(cp, end), cp));
}

bool lexer::skip_spaces()
{
    char c = ch();
    if (!is_space(c)) return false;
    while ((c = next()) && is_space(c)) {

    }
    return true;
}

void lexer::skip(size_t steps)
{
    for (size_t i = 0; i < steps; ++i) {
        next();
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
        OP(t_backslash, "\\"),
        OP(t_dot_dot, ".."),


        OP(t_brace_left, "{"),
        OP(t_brace_right, "}"),
        OP(t_paren_left, "("),
        OP(t_paren_right, ")"),
        OP(t_bracket_left, "["),
        OP(t_bracket_right, "]")
    };

    for (unsigned i = 0; i < sizeof(op_tokens) / sizeof(op_tokens[0]); ++i) {
        int ci = 0;
        while (ch(ci) == op_tokens[i].key[ci]) {
            ++ci;
            if (op_tokens[i].key[ci] == 0) {
                add_token(op_tokens[i].type, _cp, _cp + ci);
                skip(ci);
                return true;
            }
        }
    }
    return false;
}

bool lexer::handle_comments()
{
    code_point commentStart = _cp;
    if (ch() == '/') {
        if (ch(1) == '/') {
            skip(2);
            bool isTargettedComment = false;
            if (ch() == '/') {
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
        else if (ch(1) == '*') {
            skip(2);
            bool isTargettedComment = false;
            if (ch() == '*') {
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
                add_token(token_type::t_target_comment, start, _cp - 2);
            }
            return true;
        }
    }
    return false;
}

bool lexer::handle_identifiers()
{
    char c = ch();
    code_point start = _cp;
    if (is_alpha(c) || c == '_') {
        while ((c = next()) &&
               (
                   is_alpha(c) ||
                   c == '_' ||
                   is_number(c))
               ) {
        }
        token_type keyword_type = is_keyword(start, _cp);
        if (keyword_type != token_type::invalid) {
            add_token(keyword_type, start, _cp);
            return true;
        }
        
        add_token(token_type::t_identifier, start, _cp);
        return true;
    }
    return false;
}

bool lexer::handle_string_literals()
{
    token_type string_literal_type = token_type::t_u8_string_literal;
    code_point offset = 0;
    code_point start = _cp;
    if (ch() == 'a' || ch() == 'A') {
        ++offset;
        string_literal_type = token_type::t_ascii_string_literal;
    }
    else if ((ch() == 'u' || ch() == 'U') && ch(1) == '8') {
        offset += 2;
        string_literal_type = token_type::t_u8_string_literal;
    }
    else if ((ch() == 'u' || ch() == 'U') && ch(1) == '1' && ch(2) == '6') {
        offset += 3;
        string_literal_type = token_type::t_u16_string_literal;
    }
    else if ((ch() == 'u' || ch() == 'U') && ch(1) == '3' && ch(2) == '2') {
        offset += 3;
        string_literal_type = token_type::t_u32_string_literal;
    }

    //Raw string literal
    if (ch(offset) == 'r' && ch(offset + 1) == '"') {
        skip(offset + 1);
        code_point delimiterStart = _cp + 1;
        char c = '\0';
        while ((c = next()) && c != '(') {
            if (is_space(c) || c == '\\' || c == ')') {
                _err->invalid_character_in_raw_string_literal_delimiter(_cp, c);
                _fatal_error = true;
                return true;
            }
            if (_cp - delimiterStart >= lexer_settings::MAX_RAW_LITERAL_DELIMITER_LENGTH) {
                _err->too_long_raw_string_literal_delimiter(delimiterStart);
                _fatal_error = true;
                return true;
            }
        }

        if (c == '\0') {
            _err->missing_comment_ending(_cp, start);
            _fatal_error = true;
            return true;
        }
        string_view delimiter = _c.view_range(delimiterStart, _cp);

        code_point literalStart = _cp + 1;
        code_point literalEnd = 0;
        while ((c = next())) {
            if (c == ')') {
                if (_c.view(_cp + 1, delimiter.size()) == delimiter && _c.c(_cp + 1 + delimiter.size()) == '"') {
                    literalEnd = _cp;
                    skip(delimiter.size() + 1);
                    break;
                }
            }
        }
        if (!literalEnd) {
            _err->missing_comment_ending(_cp, start);
            _fatal_error = true;
            return true;
        }
        _c.add_string_literal(start, _c.view_range(literalStart, literalEnd));
        add_token(string_literal_type, start, _cp);
        return true;
    }
    else if (ch(offset) == '"') {
        skip(offset);
        std::string literal;
        char c = '\0';
        bool escape = false;
        while ((c = next()) && (c != '"' || escape)) {
            if (escape) {
                switch (c) {
                case '\'': literal += '\''; break;
                case '"': literal += '"'; break;
                case '\\': literal += '\\'; break;
                case 'a': literal += '\a'; break;
                case 'b': literal += '\b'; break;
                case 'f': literal += '\f'; break;
                case 'n': literal += '\n'; break;
                case 'r': literal += '\r'; break;
                case 't': literal += '\t'; break;
                case 'v': literal += '\v'; break;
                case 'x': {
                    if (!is_hex(ch(1)) || !is_hex(ch(2))) {
                        _err->invalid_string_literal_escape_sequence(_cp, std::string("\\x") + ch(1) + ch(2));
                    }
                    else {
                        char hex_str[3] = {ch(1), ch(2), 0};
                        char *end_ptr = 0;
                        skip(2);
                        unsigned long charVal = std::strtoul(hex_str, &end_ptr, 16);
                        literal += (char)charVal;
                    }
                    break;
                }
                }
                escape = false;
            }
            else if (c == '\\'){
                escape = true;
            }
            else {
                literal += c;
            }
        }

        if (c == 0) {
            _err->missing_ending_of_string_literal(_cp, start);
            _fatal_error = true;
            return true;
        }
        next();
        _c.add_string_literal(start, literal);
        add_token(string_literal_type, start, _cp);
        return true;
    }
    return false;
}

bool lexer::handle_numbers()
{
    char c = ch();
    code_point number_start = _cp;
    if (c == '0' && (ch(1) == 'x' && ch(1) == 'X')) {
        next();
        while ((c == next()) && is_hex(c)) { }
        if (number_start + 2 == _cp) {
            _err->invalid_number_literal_unexpected_char(_cp, c);
            return false;
        }

        add_token(token_type::t_hex_literal, number_start, _cp);
        return true;
    }
    if (c == '0' && (ch(1) == 'b' && ch(1) == 'B')) {
        next();
        while ((c == next()) && (c == '0' || c == '1')) { }
        if (number_start + 2 == _cp) {
            _err->invalid_number_literal_unexpected_char(_cp, c);
            return false;
        }

        add_token(token_type::t_binary_literal, number_start, _cp);
        return true;
    }

    if (is_number(c)) {
        token_type number_type = token_type::t_integer_literal;
        bool hasExponentPart = false;
        while ((c = next()) && is_number(c)) { }
        if (c == '.' && is_number(ch(1))) {
            number_type = token_type::t_float_literal;
            next();
        }
        else if (c == 'e' || c == 'E') {
            hasExponentPart = true;
            number_type = token_type::t_float_literal;
        }

        if (number_type == token_type::t_float_literal) {
            if (!hasExponentPart) {
                while ((c = next()) && is_number(c)) { }
            }

            if (c == 'e' || c == 'E') {
                code_point exponent_start = _cp;
                code_point exp_number_start = _cp + 1;
                if (ch(1) == '+' || ch(1) == '-') {
                    exp_number_start++;
                    next();
                }
                while ((c = next()) && is_number(c)) { }

                if (_cp == exp_number_start) {
                    //Doesn't actually have proper exponent part
                    _cp = exponent_start;
                }
            }
        }
        code_point number_end = _cp;

        add_token(number_type, number_start, number_end);
        return true;
    }
    return false;
}


char lexer::ch(code_point offset)
{
    return _c.c(_cp + offset);
}

char lexer::next_rc()
{
    char c = _c.c(_cp);
    _cp = _c.next(_cp);
    return c;
}

char lexer::next()
{
    _cp = _c.next(_cp);
    return _c.c(_cp);
}



