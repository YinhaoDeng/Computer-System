#ifdef INCLUDE_TOKENKINDS
#ifndef V_TOK_INCLUDE_TOKENKINDS
#define V_TOK_INCLUDE_TOKENKINDS

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

    // Hack Virtual Machine Tokeniser Tokens
    // *** TOKENS
    // * digit  ::= '0'-'9'
    // * digit19  ::= '1'-'9'
    // * letter ::= 'a'-'z'|'A'-'Z'|'_'|':'|'.'
    //
    // * an_op ::= 'add' | 'and' | 'eq' | 'gt' | 'lt' | 'neg' | 'not' | 'or' | 'sub' | 'return'
    // * a_jump ::= 'goto' | 'if-goto' | 'label'
    // * a_stack ::= 'call' | 'function'
    // * a_label ::= letter (letter | digit)*
    // * a_segment ::= 'argument' | 'constant' | 'local' | 'pointer' | 'static' | 'temp' | 'that' | 'this'
    // * a_number ::= '0' | (digit19 digit*)
    // * identifier ::= (letter|'_') (letter|digit|'_')*
    //
    // *** OTHER RULES
    // * between tokens, space ' ', tab '\t', carriage return '\r' and newline '\n' are ignored
    // * between tokens any other character that cannot start a token results in an EOI token
    // * if an EOI token has been returned all future tokens returned are EOI
    //
    vk_first = 2000,                // VM token kinds all > 2000
    vk_an_op,                       // an operator
    vk_a_jump,                      // a jump
    vk_a_func,                      // a func
    vk_a_stack,                     // a stack
    vk_a_label,                     // a label
    vk_a_segment,                   // a segment
    vk_a_number,                    // a number 0 to 32767

                                    // the operator commands - vk_an_op
    vk_add,                         // add
    vk_and,                         // and
    vk_eq,                          // eq
    vk_gt,                          // gt
    vk_lt,                          // lt
    vk_neg,                         // neg
    vk_not,                         // not
    vk_or,                          // or
    vk_sub,                         // sub
    vk_return,                      // return

                                    // jump commands - vk_a_jump
    vk_goto,                        // goto
    vk_if_goto,                     // if-goto
    vk_label,                       // label

                                    // the function commands - vk_a_func
    vk_call,                        // call
    vk_function,                    // function

                                    // the stack commands - vk_a_stack
    vk_pop,                         // pop
    vk_push,                        // push

                                    // the segment names - vk_a_segment
    vk_argument,                    // argument
    vk_constant,                    // constant
    vk_local,                       // argument
    vk_pointer,                     // argument
    vk_static,                      // argument
    vk_temp,                        // argument
    vk_that,                        // argument
    vk_this,                        // argument

    vk_eoi,                         // end of input reached or a legal token cannot be formed
    vk_oops,                        // a stand in null value for TokenKind
    vk_last,
//} ;
#endif //V_TOK_INCLUDE_TOKENKINDS
#else
#ifndef V_TOK_H
#define V_TOK_H

                                    // Hack Virtual Machine Language
extern Token v_next_token() ;
extern std::string v_tokeniser_context() ;
extern std::string v_token_to_string(TokenKind k) ;
extern TokenKind v_token_to_token(TokenKind k) ;
extern TokenKind v_string_to_token(std::string s) ;

extern tokeniser v_tokeniser() ;    // return a tokeniser object - and register the tokeniser

#endif //V_TOK_H
#endif //INCLUDE_TOKENKINDS
