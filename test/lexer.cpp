
#define BOOST_TEST_MODULE lexer
#include <boost/test/unit_test.hpp>
#include "../src/compiler_lib/lexer/lexer.h"
#include "../src/common/error_producer.h"
#include "../src/common/error_handler.h"
#include "../src/test_support/token_list_matcher.h"
#include "../src/test_support/lexer_tester.h"

BOOST_AUTO_TEST_CASE( lexer_simple_test ) {
    ;

    std::shared_ptr<console_print_error_handler> err_handler = std::make_shared<console_print_error_handler>();
    code c{"test.l"};
    c.set_code(
                "module test_module;\n"
                "///Targetted comment\n"
                "namespace hello_world {\n"
                "public struct s {\n"
                "\ti32 f1;\n"
                "\ti8[32 / 4] f2;\n"
                "}\n"
                "trait t { }\n"
                "}"
                );

    lexer l(c);
    l.err_producer().add_handler(err_handler);

    BOOST_REQUIRE_EQUAL(l.tokenize(), true);

    token_list_matcher matcher;
    matcher.set_match_list({
                               token_type::k_module,
                               token_type::t_identifier,
                               token_type::t_semicolon,
                               token_type::t_target_comment,
                               token_type::k_namespace,
                               token_type::t_identifier,
                               token_type::t_brace_left,
                               token_type::k_public,
                               token_type::k_struct,
                               token_type::t_identifier,
                               token_type::t_brace_left,
                               token_type::t_identifier,
                               token_type::t_identifier,
                               token_type::t_semicolon,
                               token_type::t_identifier,
                               token_type::t_bracket_left,
                               token_type::t_integer_literal,
                               token_type::t_divide,
                               token_type::t_integer_literal,
                               token_type::t_bracket_right,
                               token_type::t_identifier,
                               token_type::t_semicolon,
                               token_type::t_brace_right,
                               token_type::k_trait,
                               token_type::t_identifier,
                               token_type::t_brace_left,
                               token_type::t_brace_right,
                               token_type::t_brace_right
                           });

    BOOST_CHECK_EQUAL(matcher.does_match(c.tokens()), true);
}


BOOST_AUTO_TEST_CASE ( automated_lexer_tests ) {

    lexer_tester tester;
    bool success = true;

#define AUTOMATED_TEST(_file_) success &= tester.test(TEST_DIRECTORY _file_, TEST_DIRECTORY _file_ ".ref")

    AUTOMATED_TEST("/lexer_test_data/basic.ly");
    AUTOMATED_TEST("/lexer_test_data/string_inline_expression.ly");


#undef AUTOMATED_TEST

    BOOST_CHECK_MESSAGE(success, "All automated lexer tests didn't complete successfully");
}
