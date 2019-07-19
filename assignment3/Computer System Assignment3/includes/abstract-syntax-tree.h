#ifndef JACK_AST_H
#define JACK_AST_H

#include <string>
#include <vector>
#include "tokeniser.h"

// Abstract Syntax Tree
// All AST nodes have a private implementation
// - the AST values passed around are indexes into a private data structure
// - vector like nodes, AST nodes are immutable
// - every AST node has a 'create' function, eg 'create_while(_condition,_statements)'
// - every field of an AST node has a 'get' function, eg, 'get_while_statements(_while_node)'
// - vector like AST nodes have a 'size_of' function, eg 'size_of_statements(_statements_node);'
// - vector like AST nodes have a 'get' function, eg 'get_statements(_statements_node,2);'

// all errors encountered by AST functions will result in calls to fatal_error() defined in iobuffer.h
// an additional error message will be printed to standard error
// the running program will terminate with an exit status of -1
// errors include
// - passing an ast value that does not identify an AST node
// - passing an ast value that refers to an inappropriate kind of AST node
// - tree traversal errors when print a tree as XML
// - parsing errors when constructing an XML tree

// An Abstract Sytax Tree is language specific and has its own private namespace, eg Jack_Compiler

// Jack Compiler
namespace Jack_Compiler
{
    // we use integers to identify AST nodes in the public interface
    typedef int ast ;

    // shorthand to avoid std::
    using namespace std ;

    // enumeration for AST node classes
    // ast_alpha & ast_omega describe the maximum range of ast_kind values
    // ast_oops is for error reporting
    enum ast_kind
    {
        ast_alpha = 4000,       // keep enum ranges unique
        ast_tokens,             // vector of ast_token nodes
        ast_token,              // tokeniser Token

        ast_class,              // class
        ast_class_var_decs,     // vector of class variable declarations
        ast_var_dec,            // single variable declaration
        ast_subr_decs,          // vector of subroutine declarations

        ast_subr,               // subroutine declaration
        ast_constructor,        // constructor
        ast_function,           // function
        ast_method,             // method
        ast_param_list,         // vector of subroutine parameter declarations
        ast_subr_body,          // body of a subroutine
        ast_var_decs,           // vector of subroutine local var declarations

        ast_statements,         // statements
        ast_statement,          // statement
        ast_let,                // let statement - variable assignment
        ast_let_array,          // let statement - array element assignment
        ast_if,                 // if then statement
        ast_if_else,            // if then else statement
        ast_while,              // while statement
        ast_do,                 // do statement
        ast_return,             // return statement - nothing returned
        ast_return_expr,        // return statement - expression value returned

        ast_expr_list,          // expression list - arguments to a subrountine call
        ast_expr,               // expression - alternating term op term op term ... ending with a term
        ast_term,               // expression term
        ast_int,                // integer constant
        ast_string,             // string constant
        ast_bool,               // boolean constant - false or true
        ast_null,               // null
        ast_this,               // this
        ast_var,                // variable
        ast_array_index,        // array index
        ast_op,                 // an operator
        ast_unary_op,           // unary op and operand
        ast_call,               // constructor, function or method call


        ast_oops,               // for error reporting
        ast_omega               // largest ast_kind value
    };

    /***** AST Functions *****
      * IMPORTANT:
      * all errors encountered by these functions will result in calls to fatal_error()
      * an additional error message will be printed to standard error
      * the running program will terminate with an exit status of -1
    */

    //***** AST node kind functions *****//

    extern string   ast_kind_to_string(ast_kind k) ;        // returns string representation of ast kind k
    extern ast_kind ast_node_kind(ast t) ;                  // what kind of ast node is t?
    extern bool     ast_have_kind(ast t,ast_kind k) ;       // can node t be safely used as a node of kind k?
    extern void     ast_mustbe_kind(ast t,ast_kind k) ;     // call fatal_error() if node t cannot be safely used as a node of kind k

    //***** XML printing / parsing functions for Jack AST nodes *****//

    // XML token kinds
    enum xml_token_kind
    {
        xml_opentag,    // an open tag, tag name is in spelling
        xml_closetag,   // a close tag, tag name is in spelling
        xml_text,       // a #text node, text is in spelling
        xml_eoi         // end of xml input or error?
    };

    // an XML token
    struct xml_token
    {
        xml_token_kind kind ;
        string spelling ;
    } ;

    // print tree t as XML, an indent_by > 0 causes pretty printing
    extern void ast_print_as_xml(ast t,int indent_by) ;

    // construct a Jack_Compiler abstract syntax tree by parsing XML from standard input,
    // any pretty printed indents are ignored,
    // the result will be either an ast_tokens node or an ast_class node
    extern ast ast_parse_xml() ;



    //***** AST nodes to represent tokens *****//

    // create a tokens AST node, initialised by a vector of token nodes
    // ast_tokens node - a vector of ast_token nodes
    // tokens must be a vector of ast_token nodes
    extern ast create_tokens(vector<ast> tokens) ;
    extern ast size_of_tokens(ast t) ;
    extern ast get_tokens(ast t,int i) ;

    // ast_token node - represents a single Token value
    extern ast create_token(TokenKind kind,string spelling,int ivalue,int line,int column) ;
    extern TokenKind get_token_kind(ast token) ;
    extern string get_token_spelling(ast token) ;
    extern int get_token_ivalue(ast token) ;
    extern int get_token_line(ast token) ;
    extern int get_token_column(ast token) ;


    //***** AST nodes to represent Jack classes *****//
    // class ::= 
    // ast_class - a Jack class
    // it has a class name, static and field declarations (decs) and subroutine declarations (subrs)
    // decs must be an ast_class_var_decs node
    // subrs must be an ast_subr_decs node
    extern ast create_class(string class_name,ast decs,ast subrs) ;
    extern string get_class_class_name(ast _class) ;
    extern ast get_class_var_decs(ast _class) ;
    extern ast get_class_subr_decs(ast _class) ;


    // class_var_decs ::= 
    // ast_class_var_decs - a vector of ast_var nodes
    // vars must be a vector of ast_var nodes
    extern ast create_class_var_decs(vector<ast> vars) ;
    extern ast size_of_class_var_decs(ast t) ;
    extern ast get_class_var_decs(ast t,int i) ;


    // var_decs ::= 
    // ast_var_decs - a vector of ast_var_dec nodes
    // vars must be a vector of ast_var_dec nodes
    extern ast create_var_decs(vector<ast> vars) ;
    extern ast size_of_var_decs(ast t) ;
    extern ast get_var_decs(ast t,int i) ;


    // var_dec ::= 
    // ast_var_dec node
    // it records the variable's name, it's segment name, it's segment offset and it's type
    extern ast create_var_dec(string name,string segment,int offset,string type) ;
    extern string get_var_dec_segment(ast var_dec) ;
    extern string get_var_dec_name(ast var_dec) ;
    extern int get_var_dec_offset(ast var_dec) ;
    extern string get_var_dec_type(ast var_dec) ;


    // var ::= 
    // ast_var node
    // it records the variable's name, it's segment name, it's segment offset and it's type
    extern ast create_var(string name,string segment,int offset,string type) ;
    extern string get_var_segment(ast var_dec) ;
    extern string get_var_name(ast var_dec) ;
    extern int get_var_offset(ast var_dec) ;
    extern string get_var_type(ast var_dec) ;


    // ast_subr_decs node - a vector of ast_subr nodes
    // subrs must be a vector of subroutine declaration nodes
    extern ast create_subr_decs(vector<ast> subrs) ;
    extern ast size_of_subr_decs(ast t) ;
    extern ast get_subr_decs(ast t,int i) ;


    // subr ::= 
    // ast_subr node - a subroutine declaration
    // subr must be one of ast_constructor, ast_function or ast_method
    extern ast create_subr(ast subr) ;
    extern ast get_subr_subr(ast subr) ;


    // ast_constructor_node - a Jack constructor
    // it records the return type, constructor's name, its parameters and its body
    // params must be an ast_param_list node
    // body must be an ast_subr_body node
    extern ast create_constructor(string vtype,string name,ast params,ast body) ;
    extern string get_constructor_vtype(ast constructor) ;
    extern string get_constructor_name(ast constructor) ;
    extern ast get_constructor_param_list(ast constructor) ;
    extern ast get_constructor_subr_body(ast constructor) ;

    // ast_function_node - a Jack function
    // it records the return type, function's name, its parameters and its body
    // params must be an ast_param_list node
    // body must be an ast_subr_body node
    extern ast create_function(string vtype,string name,ast params,ast body) ;
    extern string get_function_vtype(ast function) ;
    extern string get_function_name(ast function) ;
    extern ast get_function_param_list(ast function) ;
    extern ast get_function_subr_body(ast function) ;


    // ast_method_node - a Jack method
    // it records the return type, method's name, its parameters and its body
    // params must be an ast_param_list node
    // body must be an ast_subr_body node
    extern ast create_method(string vtype,string name,ast params,ast body) ;
    extern string get_method_vtype(ast method) ;
    extern string get_method_name(ast method) ;
    extern ast get_method_param_list(ast method) ;
    extern ast get_method_subr_body(ast method) ;


    // param_list ::= 
    // ast_param_list node - subroutine parameter declarations
    // params must be a vector of ast_var_dec nodes
    extern ast create_param_list(vector<ast> params) ;
    extern int size_of_param_list(ast param_list) ;
    extern ast get_param_list(ast param_list,int index) ;


    // subr_body ::= 
    // ast_subr_body node - body of a Jack subroutine, ie a constructor, function or method
    // decs must be an ast_var_decs node
    // body must be an ast_statements node
    extern ast create_subr_body(ast decs,ast body) ;
    extern ast get_subr_body_decs(ast subr_body) ;
    extern ast get_subr_body_body(ast subr_body) ;


    // statements ::= 
    // ast_statements node - a Jack statement sequence
    // statements must be a vector of ast_statement node
    extern ast create_statements(vector<ast> statements) ;
    extern int size_of_statements(ast statements) ;
    extern ast get_statements(ast statements,int index) ;


    // statement ::= 
    // ast_statement node - a single statement
    // statement must be one of ast_let, ast_let_array, ast_if, ast_if_else, ast_while, ast_do, ast_return or ast_return_expr nodes
    extern ast create_statement(ast statement) ;
    extern ast get_statement_statement(ast statement) ;


    // let ::= 
    // ast_let node - a Jack variable assignment statement
    // var must be an ast_var node
    // index must be an ast_expr node
    // expr must be an ast_expr node
    extern ast create_let(ast var,ast expr) ;
    extern ast get_let_var(ast _let) ;                      // also accepts an ast_let_array node
    extern ast get_let_expr(ast _let) ;                     // also accepts an ast_let_array node


    // let_array ::= 
    // ast_let_array node - a Jack array assignment statement
    // var must be an ast_var node
    // index must be an ast_expr node
    // expr must be an ast_expr node
    extern ast create_let_array(ast var,ast index,ast expr) ;
    extern ast get_let_array_var(ast _let) ;                // also accepts an ast_let node
    extern ast get_let_array_index(ast _let) ;
    extern ast get_let_array_expr(ast _let) ;               // also accepts an ast_let node


    // if ::= 
    // ast_if node - a Jack if then statement
    // condition must be an ast_expr node
    // if_true must be a statements node
    extern ast create_if(ast condition,ast if_true) ;
    extern ast get_if_condition(ast _if) ;                  // also accepts an ast_if_else node
    extern ast get_if_if_true(ast _if) ;                    // also accepts an ast_if_else node


    // if_else ::= 
    // ast_if_else node - a Jack if then else statement
    // condition must be an ast_expr node
    // if_true must be a statements node
    // if_false must be a statements node
    extern ast create_if_else(ast condition,ast if_true,ast if_false) ;
    extern ast get_if_else_condition(ast _if) ;             // also accepts an ast_if node
    extern ast get_if_else_if_true(ast _if) ;               // also accepts an ast_if node
    extern ast get_if_else_if_false(ast _if) ;


    // while ::= 
    // ast_while node - a Jack while loop
    // condition must be an ast_expr node
    // body must be a statements node
    extern ast create_while(ast condition,ast body) ;
    extern ast get_while_condition(ast _while) ;
    extern ast get_while_body(ast _while) ;


    // do ::= 
    // ast_do node - a Jack void function call statement
    // call must be an ast_call node
    extern ast create_do(ast call) ;
    extern ast get_do_call(ast _do) ;


    // return ::= 
    // ast_return node - a Jack return statement with no result
    extern ast create_return() ;


    // return_expr ::= 
    // ast_return_expr node - a Jack return statement with a result
    // expr must be an ast_expr node
    extern ast create_return_expr(ast expr) ;
    extern ast get_return_expr(ast _return) ;


    // expr_list ::= 
    // ast_expr_list node - arguments to a Jack subroutine call
    // exprs must be a vector of ast_expr nodes
    extern ast create_expr_list(vector<ast> exprs) ;
    extern int size_of_expr_list(ast expr_list) ;
    extern ast get_expr_list(ast expr_list,int index) ;


    // expression ::= 
    // ast_expr node - a Jack expression
    // expr must be  a vector alternating between ast_term and ast_op nodes, ending with an ast_term
    extern ast create_expr(vector<ast> expr) ;
    extern int size_of_expr(ast expr) ;
    extern ast get_expr(ast expr,int index) ;


    // term ::= 
    // ast_term node - a single expression term
    // term must be one of ast_int, ast_string, ast_bool, ast_null, ast_this, ast_expr, ast_var, ast_array_index, ast_unary_op or ast_call
    extern ast create_term(ast term) ;
    extern ast get_term_term(ast term) ;


    // ast_op node - a single infix operator
    // op must be a legal infix operator
    extern ast create_op(string op) ;
    extern string get_op_op(ast op) ;


    // int ::= '0' to '32767'
    // ast_int node - a Jack integer constant
    // _constant must be in the range 0 up to 32,767
    extern ast create_int(int _constant) ;
    extern int get_int_constant(ast _int) ;


    // string ::= 
    // ast_string node - a Jack string constant
    extern ast create_string(string _constant) ;
    extern string get_string_constant(ast _string) ;


    // bool ::= 'true' | 'false'
    // ast_bool node - a Jack boolean constant, ie true or false
    extern ast create_bool(bool t_or_f) ;
    extern bool get_bool_t_or_f(ast _bool) ;


    // null ::= 'null'
    // ast_null node - the Jack null value
    extern ast create_null() ;


    // this ::= 'this'
    // ast_this node - the object operated on by the current Jack method
    extern ast create_this() ;


    // array_index ::= 
    // ast_array_index node - a Jack array indexing expression
    // var must be an ast_var node
    // index must be an ast_expr node
    extern ast create_array_index(ast var,ast index) ;
    extern ast get_array_index_var(ast _array_index) ;
    extern ast get_array_index_index(ast _array_index) ;


    // unary_op ::= 
    // ast_unary_op node - a Jack unary op expression
    // op must be a unary op, ie '-' or '~'
    // index must be an ast_term node
    extern ast create_unary_op(string op,ast term) ;
    extern string get_unary_op_op(ast unary_op) ;
    extern ast get_unary_op_term(ast unary_op) ;


    // call ::= ((class_name | var_name) '.') subr_name '(' expr_list ')'
    // ast_call node - a Jack subroutine call
    // if a method call, the first ast_expr node in expr_list is the object being operated on
    // expr_list must be an ast_expr_list node
    extern ast create_call(bool method_call,string class_name,string subr_name,ast expr_list) ;
    extern bool get_call_method_call(ast call) ;
    extern string get_call_class_name(ast call) ;
    extern string get_call_subr_name(ast call) ;
    extern ast get_call_expr_list(ast call) ;
}

#endif //JACK_AST_H
