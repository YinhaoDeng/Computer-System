#ifndef W_AST_NODES
#define W_AST_NODES

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
typedef class wn_program_node           *wn_program ;
typedef class wn_declarations_node      *wn_declarations ;
typedef class wn_statement_node         *wn_statement ;
typedef class wn_ifStatement_node       *wn_ifStatement ;
typedef class wn_ifelseStatement_node   *wn_ifelseStatement ;
typedef class wn_whileStatement_node    *wn_whileStatement ;
typedef class wn_letStatement_node      *wn_letStatement ;
typedef class wn_statementSequence_node *wn_statementSequence ;
typedef class wn_expression_node        *wn_expression ;
typedef class wn_infix_expression_node  *wn_infix_expression ;
typedef class wn_condition_node         *wn_condition ;
typedef class wn_term_node              *wn_term ;
typedef class wn_integerConstant_node   *wn_integerConstant ;
typedef class wn_identifier_node        *wn_identifier ;
typedef class wn_infix_node             *wn_infix ;
typedef class wn_relop_node             *wn_relop ;

//***** Utility Functions *****//
                                                            // the next functions treat all errors as fatal
extern ast         wn_deep_copy(ast t) ;                    // make a deep copy of abstract syntax tree t, nullptr and unexpected nodes are fatal errors
extern void        wn_print_as_xml(ast t,int indent_by) ;   // print wn_program_node as XML, indent_by > 0 causes pretty printing, errors become XML nodes
extern wn_program  wn_parse_xml() ;                         // parse wn_program_node in XML from standard input, all parsing errors are fatal errors

//***** Syntax Specific AST nodes *****//

// Syntax Specific AST nodes

//program ::= declarations statement
class wn_program_node : public ast_node
{
public:
    const wn_declarations decls ;
    const wn_statement stat ;
    wn_program_node(wn_declarations,wn_statement) ;
} ;
extern wn_program wn_program_create(wn_declarations decls,wn_statement stat) ;
extern wn_program to_wn_program(ast) ;

// declarations ::= ('var' identifier ';')*
class wn_declarations_node : public ast_node
{
public:
    virtual int size() =0;
    virtual wn_identifier get(int) =0;
    virtual void append(wn_identifier) =0;
    wn_declarations_node() ;
} ;
extern wn_declarations wn_declarations_create() ;
extern wn_declarations to_wn_declarations(ast) ;


// statement ::= whileStatement | ifStatement | letStatement | '{' statementSequence '}'
// no objects required
class wn_statement_node : public ast_node {} ;
extern wn_statement to_wn_statement(ast) ;

// whileStatement ::= 'while' '(' condition ')' statement
class wn_whileStatement_node : public ast_node
{
public:
    const wn_condition cond ;
    const wn_statement stat ;
    wn_whileStatement_node(wn_condition,wn_statement) ;
} ;
extern wn_whileStatement wn_whileStatement_create(wn_condition cond,wn_statement stat) ;
extern wn_whileStatement to_wn_whileStatement(ast) ;

// ifStatement ::= 'if' '(' condition ')' statement ('else' statement)?
class wn_ifStatement_node : public ast_node
{
public:
    const wn_condition cond ;
    const wn_statement then_stat ;
    wn_ifStatement_node(wn_condition,wn_statement) ;
} ;
extern wn_ifStatement wn_ifStatement_create(wn_condition cond,wn_statement then_stat) ;
extern wn_ifStatement to_wn_ifStatement(ast) ;

class wn_ifelseStatement_node : public ast_node
{
public:
    const wn_condition cond ;
    const wn_statement then_stat ;
    const wn_statement else_stat ;
    wn_ifelseStatement_node(wn_condition,wn_statement,wn_statement) ;
} ;
extern wn_ifelseStatement wn_ifelseStatement_create(wn_condition cond,wn_statement then_stat,wn_statement else_stat) ;
extern wn_ifelseStatement to_wn_ifelseStatement(ast) ;

// letStatement ::= 'let' identifier '=' expression ';'
class wn_letStatement_node : public ast_node
{
public:
    const wn_identifier id ;
    const wn_expression expr ;
    wn_letStatement_node(wn_identifier,wn_expression) ;
} ;
extern wn_letStatement wn_letStatement_create(wn_identifier id,wn_expression expr) ;
extern wn_letStatement to_wn_letStatement(ast) ;

// statementSequence ::= statement*
class wn_statementSequence_node : public ast_node
{
public:
    virtual int size() =0;
    virtual wn_statement get(int) =0;
    virtual void append(wn_statement) =0;
    wn_statementSequence_node() ;
} ;
extern wn_statementSequence wn_statementSequence_create() ;
extern wn_statementSequence to_wn_statementSequence(ast) ;

// expression ::= term | infix_expression
// no objects required
class wn_expression_node : public ast_node {} ;
extern wn_expression to_wn_expression(ast) ;

// infix_expression ::= term infix term
class wn_infix_expression_node : public ast_node
{
public:
    const wn_term lhs ;
    const wn_infix op ;
    const wn_term rhs ;
    wn_infix_expression_node(wn_term,wn_infix,wn_term) ;
} ;
extern wn_infix_expression wn_infix_expression_create(wn_term lhs,wn_infix op,wn_term rhs) ;
extern wn_infix_expression to_wn_infix_expression(ast) ;

// condition ::= term relop term
class wn_condition_node : public ast_node
{
public:
    const wn_term lhs ;
    const wn_relop op ;
    const wn_term rhs ;
    wn_condition_node(wn_term,wn_relop,wn_term) ;
} ;
extern wn_condition wn_condition_create(wn_term lhs,wn_relop op,wn_term rhs) ;
extern wn_condition to_wn_condition(ast) ;

// term ::= identifier | integerConstant
// no objects required
class wn_term_node : public ast_node {} ;
extern wn_term to_wn_term(ast) ;

// relop ::= '<' | '<=' | '==' | '!=' | '>' | >='
class wn_relop_node : public ast_node
{
public:
    const string op ;
    wn_relop_node(string) ;
} ;
extern wn_relop wn_relop_create(string) ;
extern wn_relop to_wn_relop(ast) ;

// identifier ::= ('a'-'z'|'A'-'Z')('a'-'z'|'A'-'Z'|'0'-'9')*
class wn_identifier_node : public ast_node
{
public:
    const string id ;
    wn_identifier_node(string) ;
} ;
extern wn_identifier wn_identifier_create(string) ;
extern wn_identifier to_wn_identifier(ast) ;

// integerConstant ::= ('0'-'9')('0'-'9')*
class wn_integerConstant_node : public ast_node
{
public:
    const int num ;
    wn_integerConstant_node(int) ;
} ;
extern wn_integerConstant wn_integerConstant_create(int) ;
extern wn_integerConstant to_wn_integerConstant(ast) ;

// infix ::= '+' | '-' | '*' | '/'
class wn_infix_node : public ast_node
{
public:
    const string op ;
    wn_infix_node(string) ;
} ;
extern wn_infix wn_infix_create(string) ;
extern wn_infix to_wn_infix(ast) ;

#endif // W_AST_NODES
