#ifndef HACKASM_AST_H
#define HACKASM_AST_H

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
// these calls are supplied error messages that cannot be suppressed
// errors include
// - passing an ast value that does not identify an AST node
// - passing an ast value that refers to an inappropriate kind of AST node
// - parsing errors when constructing an XML tree

// An Abstract Sytax Tree is language specific and has its own private namespace, eg Hack_Assembler, Jack

// Hack Assembler
namespace Hack_Assembler
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
        ast_alpha = 1000,           // smallest ast_kind value, kept distinct from other trees
        ast_tokens,                 // vector of ast_token nodes
        ast_token,                  // tokeniser Token

                                    // Groupings only - there are no nodes of these kinds
                                    // These are used to describe a group of nodes that are used instead
        ast_label_or_instruction,   // label or instruction - groups label, a_name, a_instruction and c_instruction

        ast_program,                // a vector of label or instruction nodes
        ast_label,                  // a label 
        ast_a_name,                 // an A instruction using an unresolved name
        ast_a_instruction,          // an A instruction as a number
        ast_c_instruction,          // a C instruction

        ast_oops,                   // for error reporting
        ast_omega                   // largest ast_kind value
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
    extern bool     ast_is_kind(ast t,ast_kind k) ;         // is node t of kind k?

                                                            // the next two functions recognise grouped kinds
    extern bool     ast_have_kind(ast t,ast_kind k) ;       // can node t be safely used as a node of kind k?
    extern void     ast_mustbe_kind(ast t,ast_kind k) ;     // call fatal_error() if node t cannot be safely used as a node of kind k

    //***** XML printing / parsing functions for Workshop AST nodes *****//

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

    // construct a Workshop_Compiler abstract syntax tree by parsing XML from standard input,
    // any pretty printed indents are ignored,
    // the result will be either an ast_tokens node or an ast_program node
    extern ast ast_parse_xml() ;



    //***** AST nodes to represent tokens *****//

    // create a tokens AST node, initialised by a vector of token nodes
    // ast_tokens node - a vector of ast_token nodes
    // tokens must be a vector of ast_token nodes
    extern ast create_tokens(vector<ast> nodes) ;
    extern ast size_of_tokens(ast t) ;
    extern ast get_tokens(ast t,int i) ;

    // ast_token node - represents a single Token value
    extern ast create_token(TokenKind kind,string spelling,int ivalue,int line,int column) ;
    extern TokenKind get_token_kind(ast token) ;
    extern string get_token_spelling(ast token) ;
    extern int get_token_ivalue(ast token) ;
    extern int get_token_line(ast token) ;
    extern int get_token_column(ast token) ;

    //***** AST nodes to represent Hack Assembly Language programs *****//

    // program ::= (label | a_name | a_instruction | c_instruction)*
    // ast_program node - a vector of Hack Assembly language instructions
    // instructions must be a vector of label, a_name, a_instruction and c_instruction nodes
    extern ast create_program(vector<ast> instructions) ;
    extern ast size_of_program(ast instructions) ;
    extern ast get_program(ast instructions,int i) ;

    // label ::= '(' name ')'
    // ast_label node - a label name
    extern ast create_label(string name) ;
    extern string get_label_name(ast t) ;

    // a_name ::= '@' name
    // ast_a_name node - an a instruction using an unresolved name
    extern ast create_a_name(string unresolved) ;
    extern string get_a_name_unresolved(ast a_instruction) ;

    // a_instruction ::= '@' number
    // ast_a_instruction node - an a instruction containing a final value
    extern ast create_a_instruction(int value) ;
    extern int get_a_instruction_value(ast a_instruction) ;

    // c_instruction ::= (dest '=')? alu (';' jump)?
    // C instruction node - containing the dest, alu and jump fields
    extern ast create_c_instruction(string _dest,string _alu,string _jump) ;
    extern string get_c_instruction_dest(ast c_instruction) ;
    extern string get_c_instruction_alu(ast c_instruction) ;
    extern string get_c_instruction_jump(ast c_instruction) ;

}

#endif //HACKASM_AST_H
