#pragma once

enum class unary_operator {
	op_log_not,
	op_binary_not,
	op_negation,
	op_suffix_increment,
	op_postfix_increment,
	op_suffix_decrement,
	op_postfix_increment,
	op_indirection,
	op_address_of,
	op_move
};

enum class oper {
	op_assign,
	op_add,
	op_add_assign,
	op_sub,
	op_sub_assign,
	op_mul,
	op_mul_assign,
	op_div,
	op_div_assign,
	op_mod,
	op_mod_assign,
	op_left_shift,
	op_left_shift_assign,
	op_right_shift_assign,

	op_greater,
	op_greater_equal,
	op_less,
	op_less_equal,
	op_equal,
	op_not_equal,
	op_log_and,
	op_log_or,
	op_bin_and,
	op_bin_or,
	op_bin_xor,
};

