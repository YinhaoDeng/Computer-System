#ifndef J_AST_NODES
#define J_AST_NODES

#include "ast.h"

// Abstract Syntax Tree
// All AST nodes are subclasses of ast_node
// All AST nodes listed here are abstract - you cannot create them yourself
// - every node you are allowed to create objects for has a create function
// - you must use the create functions to get a new AST node
// All AST nodes have a function to cast a pointer to its own type
// - it throws an exception if the node pointed to is the wrong kind
// All fields of AST nodes are constants except for the Sequence nodes
// - these can have additional child nodes appended after their creation

// forward declaration of AST node classes and pointer types for each one

typedef class jn_class_node        *jn_class ;
typedef class jn_var_decs_node     *jn_var_decs ;
typedef class jn_var_dec_node      *jn_var_dec ;
typedef class jn_subr_decs_node    *jn_subr_decs ;
typedef class jn_subr_node         *jn_subr ;
typedef class jn_constructor_node  *jn_constructor ;
typedef class jn_function_node     *jn_function ;
typedef class jn_method_node       *jn_method ;
typedef class jn_param_list_node   *jn_param_list ;
typedef class jn_subr_body_node    *jn_subr_body ;
typedef class jn_statements_node   *jn_statements ;
typedef class jn_statement_node    *jn_statement ;
typedef class jn_let_node          *jn_let ;
typedef class jn_let_array_node    *jn_let_array ;
typedef class jn_if_node           *jn_if ;
typedef class jn_if_else_node      *jn_if_else ;
typedef class jn_while_node        *jn_while ;
typedef class jn_do_node           *jn_do ;
typedef class jn_return_node       *jn_return ;
typedef class jn_return_void_node  *jn_return_void ;
typedef class jn_return_expr_node  *jn_return_expr ;
typedef class jn_expr_list_node    *jn_expr_list ;
typedef class jn_return_expr_node  *jn_return_expr ;
typedef class jn_expr_node         *jn_expr ;
typedef class jn_int_node          *jn_int ;
typedef class jn_string_node       *jn_string ;
typedef class jn_bool_node         *jn_bool ;
typedef class jn_null_node         *jn_null ;
typedef class jn_this_node         *jn_this ;
typedef class jn_var_node          *jn_var ;
typedef class jn_array_index_node  *jn_array_index ;
typedef class jn_unary_op_node     *jn_unary_op ;
typedef class jn_infix_op_node     *jn_infix_op ;
typedef class jn_call_node         *jn_call ;
typedef class jn_subr_call_node    *jn_subr_call ;
typedef class jn_method_call_node  *jn_method_call ;

//***** Utility Functions *****//
                                                            // all ASTs have their own version of these three
extern ast         jn_deep_copy(ast t) ;                    // make a deep copy of abstract syntax tree t, nullptr and unexpected nodes are fatal errors
extern void        jn_print_as_xml(ast t,int indent_by) ;   // print jn_class_node as XML, indent_by > 0 causes pretty printing, errors become XML nodes
extern jn_class    jn_parse_xml() ;                         // parse jn_class_node in XML from standard input, all parsing errors are fatal errors

//***** Syntax Specific AST nodes *****//



// class ::= 
class jn_class_node : public ast_node
{
public:
    const string class_name ;
    const jn_var_decs decs ;
    const jn_subr_decs subrs ;
    jn_class_node(string,jn_var_decs,jn_subr_decs) ;
} ;
extern jn_class jn_class_create(string class_name,jn_var_decs decs,jn_subr_decs subrs) ;
extern jn_class to_jn_class(ast) ;

// var_decs ::= 
class jn_var_decs_node : public ast_node
{
public:
    virtual int size() =0;
    virtual jn_var_dec get(int) =0;
    virtual void append(jn_var_dec) =0;
    jn_var_decs_node() ;
} ;
extern jn_var_decs jn_var_decs_create() ;
extern jn_var_decs to_jn_var_decs(ast) ;

// var_dec ::= 
class jn_var_dec_node : public ast_node
{
public:
    const string segment ;
    const string name ;
    const int offset ;
    const string type ;
    jn_var_dec_node(string,string,int,string) ;
} ;
extern jn_var_dec jn_var_dec_create(string segment,string name,int offset,string type) ;
extern jn_var_dec to_jn_var_dec(ast) ;

// subr_decs ::= 
class jn_subr_decs_node : public ast_node
{
public:
    virtual int size() =0;
    virtual jn_subr get(int) =0;
    virtual void append(jn_subr) =0;
    jn_subr_decs_node() ;
} ;
extern jn_subr_decs jn_subr_decs_create() ;
extern jn_subr_decs to_jn_subr_decs(ast) ;

// subr ::= 
// no objects required
class jn_subr_node : public ast_node
{
public:
    const string vtype ;
    const string name ;
    const jn_param_list params ;
    const jn_subr_body body ;
    jn_subr_node(ast_kind,string,string,jn_param_list,jn_subr_body) ;
} ;
extern jn_subr to_jn_subr(ast) ;

// constructor ::= 
class jn_constructor_node : public jn_subr_node
{
public:
    jn_constructor_node(string,string,jn_param_list,jn_subr_body) ;
} ;
extern jn_constructor jn_constructor_create(string vtype,string name,jn_param_list params,jn_subr_body body) ;
extern jn_constructor to_jn_constructor(ast) ;

// function ::= 
class jn_function_node : public jn_subr_node
{
public:
    jn_function_node(string,string,jn_param_list,jn_subr_body) ;
} ;
extern jn_function jn_function_create(string vtype,string name,jn_param_list params,jn_subr_body body) ;
extern jn_function to_jn_function(ast) ;

// method ::= 
class jn_method_node : public jn_subr_node
{
public:
    jn_method_node(string,string,jn_param_list,jn_subr_body) ;
} ;
extern jn_method jn_method_create(string vtype,string name,jn_param_list params,jn_subr_body body) ;
extern jn_method to_jn_method(ast) ;

// param_list ::= 
class jn_param_list_node : public ast_node
{
public:
    virtual int size() =0;
    virtual jn_var_dec get(int) =0;
    virtual void append(jn_var_dec) =0;
    jn_param_list_node() ;
} ;
extern jn_param_list jn_param_list_create() ;
extern jn_param_list to_jn_param_list(ast) ;

// subr_body ::= 
class jn_subr_body_node : public ast_node
{
public:
    const jn_var_decs decs ;
    const jn_statements body ;
    jn_subr_body_node(jn_var_decs,jn_statements) ;
} ;
extern jn_subr_body jn_subr_body_create(jn_var_decs decs,jn_statements stats) ;
extern jn_subr_body to_jn_subr_body(ast) ;

// statements ::= 
class jn_statements_node : public ast_node
{
public:
    virtual int size() =0;
    virtual jn_statement get(int) =0;
    virtual void append(jn_statement) =0;
    jn_statements_node() ;
} ;
extern jn_statements jn_statements_create() ;
extern jn_statements to_jn_statements(ast) ;

// statement ::= 
// no objects required
class jn_statement_node : public ast_node {} ;
extern jn_statement to_jn_statement(ast) ;

// let ::= 
class jn_let_node : public ast_node
{
public:
    const jn_var var ;
    const jn_expr expr ;
    jn_let_node(ast_kind,jn_var,jn_expr) ;
} ;
extern jn_let jn_let_create(jn_var var,jn_expr expr) ;
extern jn_let to_jn_let(ast) ;

// let_array ::= 
class jn_let_array_node : public jn_let_node
{
public:
    const jn_expr index ;
    jn_let_array_node(jn_var,jn_expr,jn_expr) ;
} ;
extern jn_let_array jn_let_array_create(jn_var var,jn_expr index,jn_expr expr) ;
extern jn_let_array to_jn_let_array(ast) ;

// if ::= 
class jn_if_node : public ast_node
{
public:
    const jn_expr cond ;
    const jn_statements if_true ;
    jn_if_node(ast_kind,jn_expr,jn_statements) ;
} ;
extern jn_if jn_if_create(jn_expr cond,jn_statements if_true) ;
extern jn_if to_jn_if(ast) ;

// if_else ::= 
class jn_if_else_node : public jn_if_node
{
public:
    const jn_statements if_false ;
    jn_if_else_node(jn_expr,jn_statements,jn_statements) ;
} ;
extern jn_if_else jn_if_else_create(jn_expr cond,jn_statements if_true,jn_statements if_false) ;
extern jn_if_else to_jn_if_else(ast) ;

// while ::= 
class jn_while_node : public ast_node
{
public:
    const jn_expr cond ;
    const jn_statements body ;
    jn_while_node(jn_expr,jn_statements) ;
} ;
extern jn_while jn_while_create(jn_expr cond,jn_statements body) ;
extern jn_while to_jn_while(ast) ;

// do ::= 
class jn_do_node : public ast_node
{
public:
    const jn_call call ;
    jn_do_node(jn_call) ;
} ;
extern jn_do jn_do_create(jn_call call) ;
extern jn_do to_jn_do(ast) ;

// return ::= 
class jn_return_node : public ast_node
{
public:
    jn_return_node(ast_kind) ;
} ;
extern jn_return jn_return_create() ;
extern jn_return to_jn_return(ast) ;

// return_expr ::= 
class jn_return_expr_node : public jn_return_node
{
public:
    const jn_expr expr ;
    jn_return_expr_node(jn_expr) ;
} ;
extern jn_return_expr jn_return_expr_create(jn_expr) ;
extern jn_return_expr to_jn_return_expr(ast) ;

// expr_list ::= 
class jn_expr_list_node : public ast_node
{
public:
    virtual int size() =0;
    virtual jn_expr get(int) =0;
    virtual void append(jn_expr) =0;
    virtual void prefix(jn_expr) =0;
    jn_expr_list_node() ;
} ;
extern jn_expr_list jn_expr_list_create() ;
extern jn_expr_list to_jn_expr_list(ast) ;

// expr ::= 
// no objects required
class jn_expr_node : public ast_node {} ;
extern jn_expr to_jn_expr(ast) ;

// int ::= '0' to '32767'
class jn_int_node : public ast_node
{
public:
    const int ic ;
    jn_int_node(int) ;
} ;
extern jn_int jn_int_create(int ic) ;
extern jn_int to_jn_int(ast) ;

// string ::= 
class jn_string_node : public ast_node
{
public:
    const string sc ;
    jn_string_node(string) ;
} ;
extern jn_string jn_string_create(string sc) ;
extern jn_string to_jn_string(ast) ;

// bool ::= 'true' | 'false'
class jn_bool_node : public ast_node
{
public:
    const bool tf ;
    jn_bool_node(bool) ;
} ;
extern jn_bool jn_bool_create(bool tf) ;
extern jn_bool to_jn_bool(ast) ;

// null ::= 'null'
class jn_null_node : public ast_node
{
public:
    jn_null_node() ;
} ;
extern jn_null jn_null_create() ;
extern jn_null to_jn_null(ast) ;

// this ::= 'this'
class jn_this_node : public ast_node
{
public:
    jn_this_node() ;
} ;
extern jn_this jn_this_create() ;
extern jn_this to_jn_this(ast) ;

// var ::= 
class jn_var_node : public ast_node
{
public:
    const string segment ;
    const string name ;
    const int offset ;
    const string type ;
    jn_var_node(string,string,int,string) ;
} ;
extern jn_var jn_var_create(string segment,string name,int offset,string type) ;
extern jn_var to_jn_var(ast) ;

// array_index ::= 
class jn_array_index_node : public ast_node
{
public:
    const jn_var var ;
    const jn_expr index ;
    jn_array_index_node(jn_var,jn_expr) ;
} ;
extern jn_array_index jn_array_index_create(jn_var var,jn_expr index) ;
extern jn_array_index to_jn_array_index(ast) ;

// unary_op ::= 
class jn_unary_op_node : public ast_node
{
public:
    const string op ;
    const jn_expr expr ;
    jn_unary_op_node(string,jn_expr) ;
} ;
extern jn_unary_op jn_unary_op_create(string op,jn_expr expr) ;
extern jn_unary_op to_jn_unary_op(ast) ;

// infix_op ::= 
class jn_infix_op_node : public ast_node
{
public:
    const jn_expr lhs ;
    const string op ;
    const jn_expr rhs ;
    jn_infix_op_node(jn_expr,string,jn_expr) ;
} ;
extern jn_infix_op jn_infix_op_create(jn_expr lhs,string op,jn_expr rhs) ;
extern jn_infix_op to_jn_infix_op(ast) ;

// call ::= ((class_name | var_name) '.') subr_name '(' expr_list ')'
class jn_call_node : public ast_node
{
public:
    const bool method_call ;
    const string class_name ;
    const string subr_name ;
    const jn_expr_list expr_list ;
    jn_call_node(bool,string,string,jn_expr_list) ;
} ;
extern jn_call jn_call_create(bool method_call,string class_name,string subr_name,jn_expr_list expr_list) ;
extern jn_call to_jn_call(ast) ;

#endif // J_AST_NODES
