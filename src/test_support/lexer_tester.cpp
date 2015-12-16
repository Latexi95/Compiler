#include "lexer_tester.h"
#include <fstream>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include "../compiler_lib/lexer/lexer.h"
#include "../common/error_producer.h"
#include "../common/error_handler.h"
#include "../test_support/token_list_matcher.h"

lexer_tester::lexer_tester()
{
    for (unsigned i = 0; i < (unsigned)token_type::last_token_type; ++i) {
        token_name_map[token::token_type_name((token_type)i)] = (token_type)i;
    }
    token_name_map["id"] = token_type::t_identifier;
    token_name_map["{"] = token_type::t_brace_left;
    token_name_map["}"] = token_type::t_brace_right;
    token_name_map["("] = token_type::t_paren_left;
    token_name_map[")"] = token_type::t_paren_right;
    token_name_map["["] = token_type::t_bracket_left;
    token_name_map["]"] = token_type::t_bracket_right;

}

lexer_tester::~lexer_tester()
{

}

bool lexer_tester::test(const std::string &source_file, const std::string &reference_file)
{
    std::cout << "lexer_tester: " << source_file << " : " << reference_file << std::endl;

    std::ifstream source(source_file);
    std::string token_name;
    std::string source_code;
    std::string line;
    while (std::getline(source, line)) {
        source_code += line + '\n';
    }

    if (source_code.empty()) {
        std::cerr << "Source file \"" << source_file << "\" empty or doesn't exist." <<std::endl;
        return false;
    }

    std::ifstream reference(reference_file);
    std::vector<token_type> token_types;
    while (reference >> token_name) {
        boost::to_lower(token_name);
        auto it = token_name_map.find(token_name);
        if (it == token_name_map.end()) {
            std::cerr << "Invalid reference file " << reference_file << ": Unknown token " << token_name << std::endl;
            return false;
        }
        token_types.push_back(it->second);
    }

    std::shared_ptr<console_print_error_handler> err_handler = std::make_shared<console_print_error_handler>();
    code c(source_file);
    c.set_code(source_code);
    lexer l(c);
    l.err_producer().add_handler(err_handler);

    if (!l.tokenize()) { return false; }

    token_list_matcher matcher;
    matcher.set_match_list(token_types);
    return matcher.does_match(c.tokens());
}
