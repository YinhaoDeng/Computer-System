#ifndef J_AST_ASTKINDS
#define J_AST_ASTKINDS

// AST for Jack

//enum ast_kind
//{
    k_jn_first = 4000,      // keep enum ranges unique
    k_jn_class,
    k_jn_var_decs,
    k_jn_var_dec,
    k_jn_subr_decs,
    k_jn_subr,        // no node but groups constructor, function and method
    k_jn_constructor,
    k_jn_function,
    k_jn_method,
    k_jn_param_list,
    k_jn_subr_body,
    k_jn_statements,
    k_jn_statement,         // no node but groups following statements
    k_jn_let,
    k_jn_let_array,
    k_jn_if,
    k_jn_if_else,
    k_jn_while,
    k_jn_do,
    k_jn_return,
    k_jn_return_expr,
    k_jn_expr_list,
    k_jn_expr,              // no node but groups remaining expression components
    k_jn_term,
    k_jn_int,
    k_jn_string,
    k_jn_bool,
    k_jn_null,
    k_jn_this,
    k_jn_var,         
    k_jn_array_index,
    k_jn_unary_op,
    k_jn_infix_op,
    k_jn_call,
    k_jn_last,
//};

#endif // J_AST_ASTKINDS

