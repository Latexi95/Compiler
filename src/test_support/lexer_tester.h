#include "../compiler_lib/common/token.h"
#include "../compiler_lib/common/code.h"
#include <unordered_map>

class lexer_tester
{
public:
    lexer_tester();
    ~lexer_tester();

    bool test(const std::string &source_file, const std::string &reference_file);
private:
    std::unordered_map<std::string, token_type> token_name_map;
};

