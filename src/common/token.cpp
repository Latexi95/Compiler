#include "token.h"

token::token(token_type t, string_view txt, code_point cp) :
    _type(t),
    _name(txt),
    _cp(cp)
{

}

