#include "iobuffer.h"
#include "symbols.h"
#include "abstract-syntax-tree.h"

// to shorten our code:
using namespace std ;
using namespace Jack_Compiler ;

// ***** WHAT TO DO *****
//
// MODIFY the skeleton code below to make an optimised copy an abstract syntax tree, ast, of a Jack class.
//
// NOTE: abstract syntax trees are immutable - they cannot change - and cannot contain cycles so sub-trees
//       can be safely shared across multiple trees. As a result copying a node only requires the creation
//       of a new tree node if one of its fields changes or one of its sub-trees changes.
//
// The skeleton code has one function per node in the abstract tree
//  - they each extract all fields into local variables
//  - they each call the appropriate walk_* function to copy any sub-trees
//  - they each return the original tree if none of the fields changed or none of the sub-trees were copied
//
// The structure of the skeleton code is just a suggestion
//  - you may want to change the parameters / results to suit your own logic
//  - you can change it as much as you like

// forward declarations of one function per node in the abstract syntax tree
ast walk_class(ast t) ;
ast walk_class_var_decs(ast t) ;
ast walk_var_dec(ast t) ;
ast walk_subr_decs(ast t) ;
ast walk_subr(ast t) ;
ast walk_constructor(ast t) ;
ast walk_function(ast t) ;
ast walk_method(ast t) ;
ast walk_param_list(ast t) ;
ast walk_subr_body(ast t) ;
ast walk_var_decs(ast t) ;
ast walk_statements(ast t) ;
ast walk_statement(ast t) ;
ast walk_let(ast t) ;
ast walk_let_array(ast t) ;
ast walk_if(ast t) ;
ast walk_if_else(ast t) ;
ast walk_while(ast t) ;
ast walk_do(ast t) ;
ast walk_call(ast t) ;
ast walk_return(ast t) ;
ast walk_return_expr(ast t) ;
ast walk_expr_list(ast t) ;
ast walk_expr(ast t) ;
ast walk_term(ast t) ;
ast walk_op(ast t) ;
ast walk_unary_op(ast t) ;
ast walk_int(ast t) ;
ast walk_string(ast t) ;
ast walk_bool(ast t) ;
ast walk_null(ast t) ;
ast walk_this(ast t) ;
ast walk_var(ast t) ;
ast walk_array_index(ast t) ;
ast walk_unary_op(ast t) ;

ast walk_class(ast t)
{
    string myclassname = get_class_class_name(t) ;
    ast var_decs = get_class_var_decs(t) ;
    ast subr_decs = get_class_subr_decs(t) ;

    ast var_decs_copy = walk_class_var_decs(var_decs) ;
    ast subr_decs_copy = walk_subr_decs(subr_decs) ;

    if ( var_decs_copy == var_decs && subr_decs_copy == subr_decs ) return t ;

    return create_class(myclassname,var_decs_copy,subr_decs_copy) ;
}

ast walk_class_var_decs(ast t)
{
    vector<ast> decs ;

    bool copied = false ;
    int ndecs = size_of_class_var_decs(t) ;
    for ( int i = 0 ; i < ndecs ; i++ )
    {
        ast deci = get_class_var_decs(t,i) ;
        ast copy = walk_var_dec(deci) ;
        if ( deci != copy ) copied = true ;

        decs.push_back(copy) ;
    }

    if ( !copied ) return t ;

    return create_class_var_decs(decs) ;
}

ast walk_var_dec(ast t)
{
    string name = get_var_dec_name(t) ;
    string type = get_var_dec_type(t) ;
    string segment = get_var_dec_segment(t) ;
    int offset = get_var_dec_offset(t) ;

    return t ;
}

ast walk_subr_decs(ast t)
{
    vector<ast> decs ;

    bool copied = false ;
    int size = size_of_subr_decs(t) ;
    for ( int i = 0 ; i < size ; i++ )
    {
        ast deci = get_subr_decs(t,i) ;
        ast copy = walk_subr(deci) ;
        if ( deci != copy ) copied = true ;

        decs.push_back(copy) ;
    }

    if ( !copied ) return t ;

    return create_subr_decs(decs) ;
}

ast walk_subr(ast t)
{
    ast subr = get_subr_subr(t) ;
    ast copy ;

    switch(ast_node_kind(subr))
    {
    case ast_constructor:
        copy = walk_constructor(subr) ;
        break ;
    case ast_function:
        copy = walk_function(subr) ;
        break ;
    case ast_method:
        copy = walk_method(subr) ;
        break ;
    default:
        fatal_error(0,"bad subroutine dec found") ;
        break ;
    }

    if ( subr == copy ) return t ;

    return create_subr(copy) ;
}

ast walk_constructor(ast t)
{
    string vtype = get_constructor_vtype(t) ;
    string name = get_constructor_name(t) ;
    ast param_list = get_constructor_param_list(t) ;
    ast subr_body = get_constructor_subr_body(t) ;

    walk_param_list(param_list) ;
    walk_subr_body(subr_body) ;

    ast param_list_copy = walk_param_list(param_list) ;
    ast subr_body_copy = walk_subr_body(subr_body) ;

    if ( param_list_copy == param_list && subr_body_copy == subr_body ) return t ;

    return create_constructor(vtype,name,param_list_copy,subr_body_copy) ;
}

ast walk_function(ast t)
{
    string vtype = get_function_vtype(t) ;
    string name = get_function_name(t) ;
    ast param_list = get_function_param_list(t) ;
    ast subr_body = get_function_subr_body(t) ;

    ast param_list_copy = walk_param_list(param_list) ;
    ast subr_body_copy = walk_subr_body(subr_body) ;

    if ( param_list_copy == param_list && subr_body_copy == subr_body ) return t ;

    return create_function(vtype,name,param_list_copy,subr_body_copy) ;
}

ast walk_method(ast t)
{
    string vtype = get_method_vtype(t) ;
    string name = get_method_name(t) ;
    ast param_list = get_method_param_list(t) ;
    ast subr_body = get_method_subr_body(t) ;

    ast param_list_copy = walk_param_list(param_list) ;
    ast subr_body_copy = walk_subr_body(subr_body) ;

    if ( param_list_copy == param_list && subr_body_copy == subr_body ) return t ;

    return create_method(vtype,name,param_list_copy,subr_body_copy) ;
}

ast walk_param_list(ast t)
{
    vector<ast> decs ;

    bool copied = false ;
    int size = size_of_param_list(t) ;
    for ( int i = 0 ; i < size ; i++ )
    {
        ast deci = get_param_list(t,i) ;
        ast copy = walk_var_dec(deci) ;
        if ( deci != copy ) copied = true ;

        decs.push_back(copy) ;
    }

    if ( !copied ) return t ;

    return create_param_list(decs) ;
}

ast walk_subr_body(ast t)
{
    ast decs = get_subr_body_decs(t) ;
    ast body = get_subr_body_body(t) ;

    ast decs_copy = walk_var_decs(decs) ;
    ast body_copy = walk_statements(body) ;

    if ( decs_copy == decs && body_copy == body ) return t ;

    return create_subr_body(decs_copy,body_copy) ;
}

ast walk_var_decs(ast t)
{
    vector<ast> decs ;

    bool copied = false ;
    int size = size_of_var_decs(t) ;
    for ( int i = 0 ; i < size ; i++ )
    {
        ast deci = get_var_decs(t,i) ;
        ast copy = walk_var_dec(deci) ;
        if ( deci != copy ) copied = true ;

        decs.push_back(copy) ;
    }

    if ( !copied ) return t ;

    return create_var_decs(decs) ;
}

ast walk_statements(ast t)
{
    vector<ast> decs ;

    bool copied = false ;
    int size = size_of_statements(t) ;
    for ( int i = 0 ; i < size ; i++ )
    {
        ast deci = get_statements(t,i) ;
        ast copy = walk_statement(deci) ;
        if ( deci != copy ) copied = true ;

        decs.push_back(copy) ;
    }

    if ( !copied ) return t ;

    return create_statements(decs) ;
}

ast walk_statement(ast t)
{
    ast statement = get_statement_statement(t) ;
    ast copy ;

    switch(ast_node_kind(statement))
    {
    case ast_let:
        copy = walk_let(statement) ;
        break ;
    case ast_let_array:
        copy = walk_let_array(statement) ;
        break ;
    case ast_if:
        copy = walk_if(statement) ;
        break ;
    case ast_if_else:
        copy = walk_if_else(statement) ;
        break ;
    case ast_while:
        copy = walk_while(statement) ;
        break ;
    case ast_do:
        copy = walk_do(statement) ;
        break ;
    case ast_return:
        copy = walk_return(statement) ;
        break ;
    case ast_return_expr:
        copy = walk_return_expr(statement) ;
        break ;
    case ast_statements:
        copy = walk_statements(statement) ;
        break ;
    default:
        fatal_error(0,"Unexpected statement kind") ;
        break ;
    }

    if ( copy == statement ) return t ;

    return create_statement(copy) ;
}

ast walk_let(ast t)
{
    ast var = get_let_var(t) ;
    ast expr = get_let_expr(t) ;

    ast var_copy = walk_var(var) ;
    ast expr_copy = walk_expr(expr) ;

    if ( var_copy == var && expr_copy == expr ) return t ;

    return create_let(var_copy,expr_copy) ;
}

ast walk_let_array(ast t)
{
    ast var = get_let_array_var(t) ;
    ast index = get_let_array_index(t) ;
    ast expr = get_let_array_expr(t) ;

    ast var_copy = walk_var(var) ;
    ast index_copy = walk_expr(index) ;
    ast expr_copy = walk_expr(expr) ;

    if ( var_copy == var && index_copy == index && expr_copy == expr ) return t ;

    return create_let_array(var_copy,index_copy,expr_copy) ;
}

ast walk_if(ast t)
{
    ast condition = get_if_condition(t) ;
    ast if_true = get_if_if_true(t) ;

    ast condition_copy = walk_expr(condition) ;
    ast if_true_copy = walk_statements(if_true) ;

    if ( condition_copy == condition && if_true_copy == if_true ) return t ;

    return create_if(condition_copy,if_true_copy) ;
}

ast walk_if_else(ast t)
{
    ast condition = get_if_else_condition(t) ;
    ast if_true = get_if_else_if_true(t) ;
    ast if_false = get_if_else_if_false(t) ;

    ast condition_copy = walk_expr(condition) ;
    ast if_true_copy = walk_statements(if_true) ;
    ast if_false_copy = walk_statements(if_false) ;

    if ( condition_copy == condition && if_true_copy == if_true && if_false_copy == if_false ) return t ;

    return create_if_else(condition_copy,if_true_copy,if_false_copy) ;
}

ast walk_while(ast t)
{
    ast condition = get_while_condition(t) ;
    ast body = get_while_body(t) ;

    ast condition_copy = walk_expr(condition) ;
    ast body_copy = walk_statements(body) ;

    if ( condition_copy == condition && body_copy == body ) return t ;

    return create_while(condition_copy,body_copy) ;
}

ast walk_do(ast t)
{
    ast call = get_do_call(t) ;

    ast copy = walk_call(call) ;

    if ( copy == call ) return t ;

    return create_do(copy) ;
}

ast walk_call(ast t)
{
    bool method_call = get_call_method_call(t) ;
    string class_name = get_call_class_name(t) ;
    string subr_name = get_call_subr_name(t) ;
    ast expr_list = get_call_expr_list(t) ;

    ast copy = walk_expr_list(expr_list) ;

    if ( copy == expr_list ) return t ;

    return create_call(method_call,class_name,subr_name,copy) ;
}

ast walk_return(ast t)
{
    return t ;
}

ast walk_return_expr(ast t)
{
    ast expr = get_return_expr(t) ;

    ast copy = walk_expr(expr) ;

    if ( copy == expr ) return t ;

    return create_return_expr(copy) ;
}

ast walk_expr_list(ast t)
{
    vector<ast> exprs ;

    bool copied = false ;
    int size = size_of_expr_list(t) ;
    for ( int i = 0 ; i < size ; i++ )
    {
        ast expri = get_expr_list(t,i) ;
        ast copy = walk_expr(expri) ;
        if ( expri != copy ) copied = true ;

        exprs.push_back(copy) ;
    }

    if ( !copied ) return t ;

    return create_expr_list(exprs) ;
}

ast walk_expr(ast t)
{
    vector<ast> terms ;

    bool copied = false ;
    int size = size_of_expr(t) ;
    for ( int i = 0 ; i < size ; i++ )
    {
        ast termop = get_expr(t,i) ;
        ast copy = i % 2 == 0 ? walk_term(termop) : walk_op(termop) ;
        if ( termop != copy ) copied = true ;

        terms.push_back(copy) ;
    }

    if ( !copied ) return t ;

    return create_class_var_decs(terms) ;
}

ast walk_op(ast t)
{
    string op = get_op_op(t) ;

    return t ;
}

ast walk_term(ast t)
{
    ast term = get_term_term(t) ;
    ast copy ;

    switch(ast_node_kind(term))
    {
    case ast_int:
        copy = walk_int(term) ;
        break ;
    case ast_string:
        copy = walk_string(term) ;
        break ;
    case ast_bool:
        copy = walk_bool(term) ;
        break ;
    case ast_null:
        copy = walk_null(term) ;
        break ;
    case ast_this:
        copy = walk_this(term) ;
        break ;
    case ast_expr:
        copy = walk_expr(term) ;
        break ;
    case ast_var:
        copy = walk_var(term) ;
        break ;
    case ast_array_index:
        copy = walk_array_index(term) ;
        break ;
    case ast_unary_op:
        copy = walk_unary_op(term) ;
        break ;
    case ast_call:
        copy = walk_call(term) ;
        break ;
    default:
        fatal_error(0,"Unexpected term kind\n") ;
        break ;
    }
    
    if ( copy == term ) return t ;

    return create_term(copy) ;
}

ast walk_int(ast t)
{
    int _constant = get_int_constant(t) ;

    return t ;
}

ast walk_string(ast t)
{
    string _constant = get_string_constant(t) ;

    return t ;
}

ast walk_bool(ast t)
{
    bool _constant = get_bool_t_or_f(t) ;

    return t ;
}

ast walk_null(ast t)
{
    return t ;
}

ast walk_this(ast t)
{
    return t ;
}

ast walk_var(ast t)
{
    string name = get_var_name(t) ;
    string type = get_var_type(t) ;
    string segment = get_var_segment(t) ;
    int offset = get_var_offset(t) ;

    return t ;
}

ast walk_array_index(ast t)
{

    ast var = get_array_index_var(t) ;
    ast index = get_array_index_index(t) ;

    ast var_copy = walk_var(var) ;
    ast index_copy = walk_expr(index) ;

    if ( var_copy == var && index_copy == index ) return t ;

    return create_array_index(var_copy,index_copy) ;
}

ast walk_unary_op(ast t)
{
    string uop = get_unary_op_op(t);
    ast term = get_unary_op_term(t) ;

    ast copy = walk_term(term) ;

    if ( copy == term ) return t ;

    return create_unary_op(uop,copy) ;
}

// main program
int main(int argc,char **argv)
{
    // walk an AST in XML and print VM code
    ast_print_as_xml(walk_class(ast_parse_xml()),4) ;

    // flush the output and any errors
    print_output() ;
    print_errors() ;
}

