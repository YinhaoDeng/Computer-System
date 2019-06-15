#ifndef JACK_COMPILER_TOKENISER_H
#define JACK_COMPILER_TOKENISER_H

#include <string>
#include <iostream>

// we have one namespace per language, eg Jack_Compiler

namespace Jack_Compiler
{
    // to make our code shorter
    typedef std::string string ;

    // the type of a token returned by the tokeniser
    typedef int Token ;

    // *** BNF syntax for tokens
    // * literals inside ' '
    // * grouping ( )
    // * ASCII ranges -
    // * alternatives |
    // * 0 or 1 [ ]
    // * 0 or more *
    // * 1 or more +
    //

    // Jack Language Tokeniser Tokens
    // TOKEN: DEFINITION
    // identifier:      ('a'-'z'|'A'-'Z'|'_')('a'-'z'|'A'-'Z'|'_'|'0'-'9')*
    // integerConstant: '0' | ('1'-'9')('0'-'9')*
    // stringConstant:  '"' printable ascii characters excluding double quote(") and newline '"'
    // keyword:         'class' | 'constructor' | 'function' | 'method' | 'field' | 'static' |
    //                  'var' | 'int' | 'char' | 'boolean' | 'void' | 'true' | 'false' | 'null' |
    //                  'this' | 'let' | 'do' | 'if' | 'else' | 'while' | 'return'
    // symbol:          '{' | '}' | '(' | ')' | '[' | ']' | '.' | ',' | ';' | '+' | '-' |
    //                  '*' | '/' | '&' | '|' | '<' | '>' | '=' | '~'

    // The kinds of token that are recognised
    enum TokenKind
    {
        tk_first = 4000,        // Jack Language token kinds all > 4000

        tk_identifier,          // an identifier
        tk_integerConstant,     // integer constants from 0 to 32767
        tk_stringConstant,      // string constants, token value does not include the enclosing " "


        tk_class,               // 'class'
        tk_constructor,         // 'constructor'
        tk_function,            // 'function'
        tk_method,              // 'method'
        tk_field,               // 'field'
        tk_static,              // 'static'
        tk_var,                 // 'var'
        tk_int,                 // 'int'
        tk_char,                // 'char'
        tk_boolean,             // 'boolean'
        tk_void,                // 'void
        tk_true,                // 'true'
        tk_false,               // 'false'
        tk_null,                // 'null'
        tk_this,                // 'this'
        tk_let,                 // 'let', tk_statement
        tk_do,                  // 'do', tk_statement
        tk_if,                  // 'if', tk_statement
        tk_else,                // 'else', tk_statement
        tk_while,               // 'while', tk_statement
        tk_return,              // 'return', tk_statement

        tk_lcb,                 // '{'
        tk_rcb,                 // '}'
        tk_lrb,                 // '('
        tk_rrb,                 // ')'
        tk_lsb,                 // '['
        tk_rsb,                 // ']'
        tk_stop,                // '.'
        tk_comma,               // ','
        tk_semi,                // ';'
        tk_add,                 // '+', tk_infix_op
        tk_sub,                 // '-', tk_infix_op, tk_unary_op
        tk_times,               // '*', tk_infix_op
        tk_divide,              // '/', tk_infix_op
        tk_and,                 // '&', tk_infix_op
        tk_or,                  // '|', tk_infix_op
        tk_lt,                  // '<', tk_infix_op, tk_rel_op
        tk_gt,                  // '>', tk_infix_op, tk_rel_op
        tk_eq,                  // '=', tk_infix_op, tk_rel_op
        tk_not,                 // '~', tk_unary_op

        tk_eoi,                 // end of input reached or a legal token cannot be formed

                                // the following tokens are not produced by the tokeniser
                                // these are provided to describe groups of tokens
        tk_class_var,           // one of tk_field, tk_static
        tk_type,                // one of tk_int, tk_char, tk_boolean or tk_identifier
        tk_vtype,               // one of tk_int, tk_char, tk_boolean, tk_identifier or tk_void
        tk_subroutine,          // one of tk_constructor, tk_function or tk_method
        tk_statement,           // one of tk_let, tk_do, tk_if, tk_else, tk_whiler or tk_return
        tk_term,                // one of tk_identifier, tk_integerConstant, tk_stringConstant, tk_true, tk_false, tk_null, tk_this, tk_lrb, tk_sub or tk_not
        tk_unary_op,            // one of tk_sub or tk_not
        tk_infix_op,            // one of tk_add, tk_sub, tk_times, tk_divide, tk_and, tk_or, tk_lt, tk_gt or tk_eq,
        tk_rel_op,              // one of tk_lt, tk_gt or tk_eq,

        tk_oops,                // for error handling
        tk_last                 // the last ASM token value
    } ;

    // return a string representation of the given token kind
    extern string token_kind_to_string(TokenKind kind) ;

    // returns the TokenKind that would cause token_kind_to_string to return s, or tk_oops
    extern TokenKind string_to_token_kind(std::string s) ;

    // read the next token from standard input
    extern Token next_token() ;

    // return the current token, ie the last token returned by next_token()
    extern Token current_token() ;

    // each token has an object that internally records
    // a kind - the TokenKind that describes the token
    // a spelling - the characters that make up the specific token
    // an ivalue - the integer value if the token is a number, otherwise 0
    // a line - the line on which the token starts, the first line of the input is line 1
    // a column - the line position at which the token starts, the first character on a line is in column 1
    // a string representation of the above used to implement a to_string function

    // to assist with error reporting each token has an associated context string
    // the context string each token that includes:
    // - the line before the token,
    // - the line containing the token, and
    // - a line with a ^ marking the token's starting position

    // the following functions return the kind, spelling, ivalue, line, column, to_string or context for the current token
    extern TokenKind token_kind() ;
    extern string token_spelling() ;
    extern int token_ivalue() ;
    extern int token_line() ;
    extern int token_column() ;
    extern string token_to_string() ;
    extern string token_context() ;

    // the following functions return the kind, spelling, ivalue, line, column, to_string or context for the given token
    extern TokenKind token_kind(Token token) ;
    extern string token_spelling(Token token) ;
    extern int token_ivalue(Token token) ;
    extern int token_line(Token token) ;
    extern int token_column(Token token) ;
    extern string token_to_string(Token token) ;
    extern string token_context(Token token) ;

    // to assist with parsing, TokenKind has some extra definitions that are never returned as real tokens
    // these extra tokens can be used to check if a token is a member of a particular group
    // for example, tk_statement provides a group of all tokens that can start a statement
    

    // check if we HAVE a current token of the given kind or that belongs to the given grouping?
    extern bool have(TokenKind kind_or_grouping) ;

    // check if the given token is of the given kind or that belongs to the given grouping?
    extern bool have(Token token,TokenKind kind_or_grouping) ;

    // to further assist with parsing, functions have been provided to raise an error if an expected token is missing
    // the MUSTBE functions check a token against an expected kind or grouping
    // if the token matches, the next token is read from the input and the matching token is returned
    // if the token does not match, a fatal_error is called with a suitable message based on the token's context

    // the current token MUSTBE of the given kind or belong to the given grouping?
    extern Token mustbe(TokenKind kind_or_grouping) ;

    // the given token MUSTBE of the given kind or belong to the given grouping?
    extern Token mustbe(Token token,TokenKind kind_or_grouping) ;

}

#endif //JACK_COMPILER_TOKENISER_H
