#ifndef AST_H
#define AST_H

#include <string>
#include "tokeniser.h"
#include "iobuffer.h"

// Abstract Syntax Tree
// All AST nodes are subclasses of ast_node
// All AST nodes listed here are abstract - you cannot create them yourself
// - every node you are allowed to create objects for has a create function
// - you must use the create functions to get a new AST node
// All AST nodes have a function to cast a pointer to its own type
// - it throws an exception if the node pointed to is the wrong kind
// All fields of AST nodes are constants except for the Sequence nodes
// - these can have additional child nodes appended after their creation

// enumeration for AST node classes
// k_alpha & k_omega describe the maximum range of ast_kind values
// k_oops is for error reporting
enum ast_kind
{
    k_alpha,            // smallest ast_kind value
    k_ast_tokens,       // vector of ast_token nodes
    k_ast_token,        // tokeniser Token
    k_ast_error,        // error messages

#include "a-ast-kinds.h"
#include "v-ast-kinds.h"
#include "w-ast-kinds.h"
#include "j-ast-kinds.h"

    k_oops,             // these are for error reporting
    k_nullptr,          // kind of a nullptr
    k_omega             // largest ast_kind value
};

// forward declaration of AST node classes and pointer types for each one
typedef std::string string ;
typedef class ast_private_node *ast_private ;
typedef class ast_node *ast ;
typedef class ast_tokens_node *ast_tokens ;
typedef class ast_token_node *ast_token ;
typedef class ast_error_node *ast_error ;

//***** Class Definitions *****//

// ast_node is the parent class for all kinds of AST
// is_it() is pure virtual to stop you creating AST nodes without using their create function
class ast_node
{
public:
    const ast_kind kind ;                                   // the kind of node
    const ast_private internals ;                           // house keeping such as visited / copied markers
    virtual bool is_it(ast_kind another_kind) =0;           // can this node be safely cast to another kind?
    ast_node(ast_kind) ;
    virtual ~ast_node() ;                                   // this is virtual so all subclass destructors always get called
} ;


//***** Utility AST Functions *****//

extern ast         ast_parent(ast t) ;                      // returns parent of t, or nullptr
extern ast_kind    ast_kind_of(ast t) ;                     // what kind of ast node t is - k_nullptr if nullptr
extern bool        ast_kind_is_it(ast t,ast_kind k) ;       // is ast node t kind k? check includes kinds it can be safely cast to
extern void        ast_kind_mustbe(ast t,ast_kind k) ;      // check node is of the required type - if not throw an exception

                                                            // all ASTs have their own version of these three
extern ast         ast_deep_copy(ast t) ;                   // make a deep copy of abstract syntax tree t, nullptr and unexpected nodes are fatal errors
extern void        ast_print_as_xml(ast t,int indent_by) ;  // print ast_tokens_node as XML, indent_by > 0 causes pretty printing, errors become XML nodes
extern ast_tokens  ast_parse_xml() ;                        // parse ast_tokens_node in XML from standard input, all parsing errors are fatal errors

//***** Utility AST nodes *****//

// ast_tokens ::= ast_token*
class ast_tokens_node : public ast_node
{
public:
    virtual int size() =0;
    virtual ast_token get(int) =0;
    virtual void append(ast_token) =0;
    ast_tokens_node() ;
} ;
extern ast_tokens ast_tokens_create() ;
extern ast_tokens to_ast_tokens(ast) ;

// ast_token
class ast_token_node : public ast_node
{
public:
    Token tok ;
    ast_token_node(Token tok) ;
} ;
extern ast_token ast_token_create(Token tok) ;
extern ast_token to_ast_token(ast) ;

// ast_error
class ast_error_node : public ast_node
{
public:
    string message ;
    ast_error_node(string message) ;
} ;
extern ast_error ast_error_create(string message) ;
extern ast_error to_ast_error(ast) ;

#include "a-ast-nodes.h"
#include "j-ast-nodes.h"
#include "v-ast-nodes.h"
#include "w-ast-nodes.h"

#endif // AST_H
