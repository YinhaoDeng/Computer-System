#ifndef ASSIGNMENT_TOKENISER_H
#define ASSIGNMENT_TOKENISER_H

#include <string>

// Interface to the assignment tokeniser
namespace Assignment_Tokeniser
{
    // a shorthand for string
    typedef std::string string ;

    // the type of a token returned by the tokeniser
    typedef int Token ;

    // ***** BNF *****
    //
    // *** SYNTAX
    // * literals inside ' '
    // * grouping ( )
    // * ASCII ranges -
    // * alternatives |
    // * 0 or 1 ?
    // * 0 or more *
    // * 1 or more +
    //
    // *** TOKENS
    // * digit  ::= '0'-'9'
    // * digit19  ::= '1'-'9'
    // * int ::= '0' | (digit19 digit*)
    // * double ::= int ('.' digit+)? (('e'|'E') ('+'|'-')? digit+)?
    // * printable ::= ' '|'!'|'#'-'~'
    // * string ::= '"' printable* '"'
    // * letter ::= 'a'-'z'|'A'-'Z'
    // * identifier ::= (letter|'_'|'$') (letter|digit|'_'|'$')*
    //
    // * keyword ::= 'if', 'while', 'else', etc.
    //
    // * symbol ::= ';', ':', '@', etc.
    //
    // * eol_comment_char ::= ' '-'~'|'\t'|'\r'
    // * eol_comment ::= '/' '/' eol_comment_char* '\n'
    //
    // * adhoc_comment_char ::= eol_comment_char|'\n'
    // * adhoc_comment ::= '/*' adhoc_comment_char* '*/'
    //
    // *** OTHER RULES
    // * the spelling for a double with an exponent always replaces the 'E' with 'e' and adds '+' if there is no sign
    // * between tokens, space, tab, carriage return and newline are ignored
    // * between tokens any other character that cannot start a token results in an EOI token
    // * only printables, tabs, carriage returns and newline are permitted, all other characters are bad
    // * inside a string, oneline or multiline comment, EOI or a bad character results in an EOI token
    // * if an EOI token has been returned all future tokens returned are EOI
    // * once the end of input is reached all future tokens returned are EOI
    //


    // The kinds of token that are recognised
    enum TokenKind
    {
        tk_eol_comment,     // '//' eol_comment_char* '\n', // and newline are removed from the spelling
        tk_adhoc_comment,   // '/*' adhoc_comment_char* '*/', /* and */ are removed from the spelling

        tk_identifier,      // any identifier

        tk_number,          // one of tk_integer or tk_double
        tk_integer,         // any integer 有了
        tk_double,          // any floating point number
        tk_string,          // '"' printable* '"', "s are removed from the spelling, no "s allowed in a string

        tk_keyword,         // one of tk_if, tk_while, tk_else, tk_class or tk_int
        tk_if,              // 'if' 有了
        tk_while,           // 'while' 有了
        tk_else,            // 'else' 有了
        tk_class,           // 'class' 有了 
        tk_int,             // 'int' 有了


        tk_symbol,          // one of tk_at, tk_semi, tk_colon, tk_not, tk_comma, tk_stop, tk_eq,
                            //        tk_spaceship, tk_rcb, tk_lcb, tk_rrb, tk_lrb, tk_rsb, tk_lsb or tk_div
        tk_at,              // '@' 有了
        tk_semi,            // ';' 有了
        tk_colon,           // ':' 有了
        tk_not,             // '!' 有了
        tk_comma,           // ',' 有了
        tk_stop,            // '.' 有了
        tk_eq,              // '=='有了
        tk_spaceship,       // '<=>'有了
        tk_rcb,             // '}' 有了
        tk_lcb,             // '{' 有了
        tk_rrb,             // ')' 有了
        tk_lrb,             // '(' 有了
        tk_rsb,             // ']' 有了
        tk_lsb,             // '[' 有了
        tk_div,             // '/' 有了

        tk_eoi,             // end of input reached

        tk_oops             // for error handling
    } ;

    // ***** the following are implemented in the pre-compiled lib/tokeniser-extras.o object file *****

    // return a string representation of the given token kind
    extern string token_kind_to_string(TokenKind kind) ;

    // returns the TokenKind that would cause token_kind_to_string to return s, or tk_oops
    extern TokenKind string_to_token_kind(std::string s) ;

    // create a new Token object and return its ID
    extern Token new_token(TokenKind kind,string spelling,int start_line,int start_column) ;

    // return the TokenKind for the given token
    extern TokenKind token_kind(Token token) ;

    // return the characters that make up the given token
    extern string token_spelling(Token token) ;

    // return the line number where the given token was found, the first line is line number 1
    extern int token_line(Token token) ;

    // return the column position where the given token starts on its line, the first character is in column number 1
    // the column count assumes that tabs are replaced by spaces so that the next character is on an 8 character boundary
    extern int token_column(Token token) ;

    // return a string representation of the given token
    extern string token_to_string(Token token) ;

    // ***** the assignment involves implementing the following functions by completing tokeniser.cpp *****

    // is the token of the given kind or does it belong to the given grouping?
    extern bool token_is_in(Token token,TokenKind kind_or_grouping) ;

    // read the next token from standard input
    extern Token next_token() ;

    // generate a context string for the given token
    // it shows the line before the token,
    // the line containing the token, and
    // a line with a ^ marking the token's position
    // tab stops are every 8 characters
    // in the context string, tabs are replaced by spaces (1 to 8)
    // so that the next character is on an 8 character boundary
    extern string token_context(Token token) ;
}
#endif //ASSIGNMENT_TOKENISER_H
