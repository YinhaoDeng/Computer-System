#ifdef INCLUDE_TOKENKINDS
#ifndef A_TOK_INCLUDE_TOKENKINDS
#define A_TOK_INCLUDE_TOKENKINDS

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

    // Hack Assembly Tokeniser Tokens
    // *** TOKENS
    // * digit    ::= '0'-'9'
    // * digit19  ::= '1'-'9'
    // * letter   ::= 'a'-'z'|'A'-'Z'|'_'|':'|'.'
    // * name     ::= letter ( letter | digit )*
    // * number   ::= '0' | (digit19 digit*)
    //
    // TOKEN: DEFINITION
    // address    ::= ( '@' name ) | ( '@' number )
    // label      ::= '(' name ')'
    // dest       ::= 'MD' | 'AM' | 'AD' | 'AMD'
    // register   ::= 'A' | 'M' | 'D' 
    // aluop      ::= '0' | '1' | '-1' | '!D' | '!A' | '-D' | '-A' | 'D+1' | 'A+1' |
    //                'D-1' | 'A-1' | 'D+A' | 'D-A' | 'A-D' | 'D&A' | 'D|A' | '!M' |
    //                '-M' | 'M+1' | 'M-1' | 'D+M' | 'D-M' | 'M-D' | 'D&M' | 'D|M'
    // jump       ::= 'JMP' | 'JLT' | 'JLE' | 'JGT' | 'JGE' | 'JEQ' | 'JNE'
    // equals     ::= '='
    // semi       ::= ';'
    // null       ::= 'NULL'

    ak_first = 1000,                // ASM token kinds all > 1000

    ak_dest,                        // dest ::= 'MD' | 'AM' | 'AD' | 'AMD'
    ak_dest_MD,
    ak_dest_AM,
    ak_dest_AD,
    ak_dest_AMD,

    ak_register,                    // register ::= 'A' | 'M' | 'D'
    ak_register_A,
    ak_register_M,
    ak_register_D,

    ak_alu_op,                      // alu-op ::= '0' | '1' | '-1' | '!D' | '!A' | '-D' |
                                    // '-A' | 'D+1' | 'A+1' | 'D-1' | 'A-1' | 'D+A' | 'D-A' |
                                    //'A-D' | 'D&A' | 'D|A' | '!M' | '-M' | 'M+1' | 'M-1' |
                                    // 'D+M' | 'D-M' | 'M-D' | 'D&M' | 'D|M'
    ak_alu_0,
    ak_alu_1,
    ak_alu_sub_1,
    ak_alu_not_D,
    ak_alu_not_A,
    ak_alu_sub_D,
    ak_alu_sub_A,
    ak_alu_D_add_1,
    ak_alu_A_add_1,
    ak_alu_D_sub_1,
    ak_alu_A_sub_1,
    ak_alu_D_add_A,
    ak_alu_D_sub_A,
    ak_alu_A_sub_D,
    ak_alu_D_and_A,
    ak_alu_D_or_A,
    ak_alu_not_M,
    ak_alu_sub_M,
    ak_alu_M_add_1,
    ak_alu_M_sub_1,
    ak_alu_D_add_M,
    ak_alu_D_sub_M,
    ak_alu_M_sub_D,
    ak_alu_D_and_M,
    ak_alu_D_or_M,

    ak_jump,                        // jump ::= 'JMP' | 'JLT' | 'JLE' | 'JGT' | 'JGE' | 'JEQ' | 'JNE'
    ak_jmp,
    ak_jlt,
    ak_jle,
    ak_jgt,
    ak_jge,
    ak_jeq,
    ak_jne,

                                    // '@', '(', ')' are removed from the address, number and label values
    ak_address,                     // address ::= ( '@' name ) | ( '@' number )
    ak_name,
    ak_number,
    ak_label,                       // label   ::= '(' name ')'
    ak_assign,                      // equals  ::= '='
    ak_semi,                        // semi    ::= ';'
    ak_null,                        // null    ::= 'NULL'

    ak_eoi,                         // end of input reached or a legal token cannot be formed
    ak_oops,
    ak_last,
//} ;
#endif //A_TOK_INCLUDE_TOKENKINDS
#else
#ifndef A_TOK_H
#define A_TOK_H

                                    // Hack Assembly Language
extern Token a_next_token() ;
extern std::string a_tokeniser_context() ;
extern std::string a_token_to_string(TokenKind k) ;
extern TokenKind a_token_to_token(TokenKind k) ;
extern TokenKind a_string_to_token(std::string s) ;

extern tokeniser a_tokeniser() ;    // return a tokeniser object - and register the tokeniser

#endif //A_TOK_H
#endif //INCLUDE_TOKENKINDS
