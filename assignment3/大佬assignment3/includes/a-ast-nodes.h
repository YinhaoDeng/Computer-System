#ifndef A_AST_NODES
#define A_AST_NODES

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

typedef class an_program_node       *an_program ;
typedef class an_instruction_node   *an_instruction ;
typedef class an_label_node         *an_label ;
typedef class an_a_instr_name_node  *an_a_instr_name ;
typedef class an_a_instruction_node *an_a_instruction ;
typedef class an_c_instruction_node *an_c_instruction ;

//***** Utility Functions *****//
                                                            // all ASTs have their own version of these three
extern ast         an_deep_copy(ast t) ;                    // make a deep copy of abstract syntax tree t, nullptr and unexpected nodes are errors
extern void        an_print_as_xml(ast t,int indent_by) ;   // print an_program_node as XML, indent_by > 0 causes pretty printing, errors become XML nodes
extern an_program  an_parse_xml() ;                         // parse an_program_node in XML from standard input, all parsing errors are fatal errors

//***** Syntax Specific AST nodes *****//

// program ::= instruction*
class an_program_node : public ast_node
{
public:
    virtual int size() =0;
    virtual an_instruction get(int) =0;
    virtual void append(an_instruction) =0;
    an_program_node() ;
} ;
extern an_program an_program_create() ;
extern an_program to_an_program(ast) ;

// instruction ::= label | a_instr_name | a_instruction | c_instruction
// no objects required
class an_instruction_node : public ast_node {} ;

// typesafe pointer assignments
extern an_instruction to_an_instruction(ast) ;

// label ::= '(' labelname ')'
class an_label_node : public ast_node
{
public:
    const string label ;
    an_label_node(string) ;
} ;
extern an_label an_label_create(string label) ;
extern an_label to_an_label(ast) ;

// a_instr_name ::= '@' name
class an_a_instr_name_node : public ast_node
{
public:
    const string name ;
    an_a_instr_name_node(string) ;
} ;
extern an_a_instr_name an_a_instr_name_create(string name) ;
extern an_a_instr_name to_an_a_instr_name(ast) ;

// a_instruction ::= '@' number
class an_a_instruction_node : public ast_node
{
public:
    const string number ;
    an_a_instruction_node(string) ;
} ;
extern an_a_instruction an_a_instruction_create(string number) ;
extern an_a_instruction to_an_a_instruction(ast) ;

// c_instruction ::= dest '=' alu_op ';' jump
class an_c_instruction_node : public ast_node
{
public:
    const string alu_op ;
    const string dest ;
    const string jump ;
    an_c_instruction_node(string,string,string) ;
} ;
extern an_c_instruction an_c_instruction_create(string alu_op,string dest,string jump) ;
extern an_c_instruction to_an_c_instruction(ast) ;

#endif // A_AST_NODES
