#ifdef INCLUDE_TOKENKINDS
#ifndef W_TOK_INCLUDE_TOKENKINDS
#define W_TOK_INCLUDE_TOKENKINDS

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

    // Workshop Example Language Tokeniser Tokens
    // TOKEN: DEFINITION
    // identifier:      ('a'-'z'|'A'-'Z')('a'-'z'|'A'-'Z'|'0'-'9')*
    // integerConstant: ('0'-'9')('0'-'9')*
    // relop:           '<' | '<=' | '==' | '!=' | '>' | >='
    // op:              '+' | '-' | '*' | '/'
    // keyword:         'var' | 'while' | 'if' | 'else' | 'let'
    // symbol:          '{' | '}' | '(' | ')' | ';' | '='

    wk_first = 3000,                // Example Language (EL) token kinds all > 3000
    wk_identifier,
    wk_integerConstant,

    wk_keyword,                     // wk_keyword 'var' | 'while' | 'if' | 'else' | 'let'
    wk_var,
    wk_while,
    wk_if,
    wk_else,
    wk_let,

    wk_op,                          // wk_op '+' | '-' | '*' | '/'
    wk_add,
    wk_sub,
    wk_times,
    wk_divide, 

    wk_relop,                       // wk_relop '<' | '<=' | '==' | '!=' | '>' | >='
    wk_lt,
    wk_le,
    wk_eq,
    wk_ne,
    wk_ge,
    wk_gt,

    wk_symbol,                      // wk_symbol '{' | '}' | '(' | ')' | ';' | '=' | ','
    wk_lcb,
    wk_rcb,
    wk_lrb,
    wk_rrb,
    wk_semi,
    wk_assign,
    wk_comma,

    wk_eoi,                         // end of input reached or a legal token cannot be formed
    wk_oops,                        // a stand in null value for TokenKind
    wk_last,
// } ;

#endif //W_TOK_INCLUDE_TOKENKINDS
#else
#ifndef W_TOK_H
#define W_TOK_H

                                    // Workshop Example Language
extern Token w_next_token() ;
extern std::string w_tokeniser_context() ;
extern std::string w_token_to_string(TokenKind k) ;
extern TokenKind w_token_to_token(TokenKind k) ;
extern TokenKind w_string_to_token(std::string s) ;

extern tokeniser w_tokeniser() ;    // return a tokeniser object - and register the tokeniser

#endif //W_TOK_H
#endif //INCLUDE_TOKENKINDS
