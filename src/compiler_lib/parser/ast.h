#pragma once

#include <string>
#include <cstdint>
#include "../common/code_point.h"
#include <memory>

namespace ast
{

struct expr;
struct primary_expr;

struct expr;

struct node {
	virtual ~node() {}
	code_point point;
};




struct unary_expr : node
{
	unary_operator oper;
	std::unique_ptr<primary_expr> operand;

};

struct expression;

struct operation : node
{
	char operator_;
	std::unique_ptr<expression> expr;
};

struct expression : node
{

};
//struct string_literal : ;




}

