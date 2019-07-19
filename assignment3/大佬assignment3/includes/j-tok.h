#ifdef INCLUDE_TOKENKINDS
#ifndef J_TOK_INCLUDE_TOKENKINDS
#define J_TOK_INCLUDE_TOKENKINDS

// The kinds of token that are recognised
// enum TokenKind
// {
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

    jk_first = 4000,                // Jack Language token kinds all > 4000
    jk_identifier,
    jk_integerConstant,
    jk_stringConstant,


    jk_keyword,                     // keyword ::= 'var' | 'while' | 'if' | 'else' | 'let'
    jk_class,
    jk_constructor,
    jk_function,
    jk_method,
    jk_field,
    jk_static,
    jk_var,
    jk_int,
    jk_char,
    jk_boolean,
    jk_void,
    jk_true,
    jk_false,
    jk_null,
    jk_this,
    jk_let,
    jk_do,
    jk_if,
    jk_else,
    jk_while,
    jk_return,

    jk_symbol,                      // symbol ::= '{' | '}' | '(' | ')' | '[' | ']' |
                                    //            '.' | ',' | ';' | '+' | '-' | '*' | '/' |
                                    //            '&' | '|' | '<' | '>' | '=' | '~' 
    jk_lcb,
    jk_rcb,
    jk_lrb,
    jk_rrb,
    jk_lsb,
    jk_rsb,
    jk_stop,
    jk_comma,
    jk_semi,
    jk_add,
    jk_sub,
    jk_times,
    jk_divide,
    jk_and,
    jk_or,
    jk_lt,
    jk_gt,
    jk_eq,
    jk_not,

    jk_eoi,                         // end of input reached or a legal token cannot be formed
    jk_oops,                        // a stand in null value for TokenKind
    jk_last,
//} ;
#endif //J_TOK_INCLUDE_TOKENKINDS
#else
#ifndef J_TOK_H
#define J_TOK_H

                                    // Jack
extern Token j_next_token() ;
extern std::string j_tokeniser_context() ;
extern std::string j_token_to_string(TokenKind k) ;
extern TokenKind j_token_to_token(TokenKind k) ;
extern TokenKind j_string_to_token(std::string s) ;

extern tokeniser j_tokeniser() ;    // return a tokeniser object - and register the tokeniser

#endif //J_TOK_H
#endif //INCLUDE_TOKENKINDS
