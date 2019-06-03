#ifndef V_AST_NODES
#define V_AST_NODES

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

typedef class vn_class_node   *vn_class ;
typedef class vn_command_node *vn_command ;
typedef class vn_op_node      *vn_op ;
typedef class vn_jump_node    *vn_jump ;
typedef class vn_func_node    *vn_func ;
typedef class vn_stack_node   *vn_stack ;

//***** Utility Functions *****//
                                                            // all ASTs have their own version of these three
extern ast         vn_deep_copy(ast t) ;                    // make a deep copy of abstract syntax tree t, nullptr and unexpected nodes are fatal errors
extern void        vn_print_as_xml(ast t,int indent_by) ;   // print vn_class_node as XML, indent_by > 0 causes pretty printing, errors become XML nodes
extern vn_class    vn_parse_xml() ;                         // parse vn_class_node in XML from standard input, all parsing errors are fatal errors

//***** Syntax Specific AST nodes *****//

// vn_class ::= command*
class vn_class_node : public ast_node
{
public:
    virtual int size() =0;
    virtual vn_command get(int) =0;
    virtual void append(vn_command) =0;
    vn_class_node() ;
} ;
extern vn_class vn_class_create() ;
extern vn_class to_vn_class(ast) ;

// command ::= an_op | a_jump | a_func | a_stack
// no objects required
class vn_command_node : public ast_node {} ;

// typesafe pointer assignments
extern vn_command to_vn_command(ast) ;

// an_op ::= 'add' | 'and' | 'eq' | 'gt' | 'lt' | 'neg' | 'not' | 'or' | 'sub' | 'return'
class vn_op_node : public ast_node
{
public:
    string op ;
    vn_op_node(string) ;
} ;
extern vn_op vn_op_create(string op) ;
extern vn_op to_vn_op(ast) ;

// a_jump ::= jump label
class vn_jump_node : public ast_node
{
public:
    const string jump ;
    const string label ;
    vn_jump_node(string,string) ;
} ;
extern vn_jump vn_jump_create(string jump,string label) ;
extern vn_jump to_vn_jump(ast) ;

// a_func ::= func label number
class vn_func_node : public ast_node
{
public:
    const string func ;
    const string label ;
    const int number ;
    vn_func_node(string,string,int) ;
} ;
extern vn_func vn_func_create(string func,string label,int number) ;
extern vn_func to_vn_func(ast) ;

// a_stack ::= stack segment number
class vn_stack_node : public ast_node
{
public:
    const string stack ;
    const string segment ;
    const int number ;
    vn_stack_node(string,string,int) ;
} ;
extern vn_stack vn_stack_create(string stack,string segment,int number) ;
extern vn_stack to_vn_stack(ast) ;

#endif // V_AST_NODES
