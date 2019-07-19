#ifndef TOKENISER_H
#define TOKENISER_H

// Interface for all tokenisers
// To add your own follow the structure in the workshop tokenisers

#include <string>

// The kinds of token that are recognised?
// In order to avoid problems if a program needs to use multiple tokenisers
// - each tokeniser must use a unique range of TokenKind values
// - each tokeniser must use a unique prefix to name its TokenKind values
enum TokenKind
{
    ak_first_token = 10000,                     // Hack Assembly Language (ak), Cinstr ::= 'dest=comp;jump'
    bk_first_token = 11000,                     // Alternate Assembler syntax (bk), Cinstr ::= 'comp->dest;jump'
    vk_first_token = 20000,                     // Hack Virtual Machine Code (vk)
    wk_first_token = 30000,                     // Workshop example Language (wk)
    jk_first_token = 40000,                     // Jack (jk)

                                                // include TokenKind definitions for each tokeniser
#define INCLUDE_TOKENKINDS
#include "a-tok.h"
#include "j-tok.h"
#include "v-tok.h"
#include "w-tok.h"
#undef INCLUDE_TOKENKINDS
                                                // this is a project specific file that includes the required TokenKinds
    zk_very_last                                // very last item so individual lists can all end with a comma
} ;

// A Token object
struct Token
{
    TokenKind kind ;                            // the kind of token
    TokenKind token ;                           // if kind is a group, the actual token
    std::string spelling ;                      // the actual characters, if not a real token the value is ""
    int ivalue ;                                // if an integer then its value otherwise 0
    int line ;                                  // line number the token started on - first line is number 1
    int column ;                                // line column the token started on - first column is number 1
};
                                                // to record token string mappings
extern void record_global_mapping(TokenKind k,std::string spelling) ;

                                                // operator<< for Tokens
extern std::ostream & operator<<(std::ostream &,Token const &) ;


// A tokeniser object
typedef class tokeniser_ *tokeniser ;           // each tokeniser must have a function that returns a pointer to its tokeniser
class tokeniser_
{
public:
    TokenKind first ;                                              // first and last token values used
    TokenKind last ;
    TokenKind eoi ;                                                // End of input token value
    Token (*next_token)() ;                                        // returns next token, initialises tokeniser on its first call
    std::string (*context)() ;                                     // returns error message context indicating last token read
    std::string (*token_to_string)(TokenKind k) ;                  // string representation of a token kind
    TokenKind (*token_to_token)(TokenKind k,TokenKind oops) ;      // the token group a token belongs to, itself if not in a group, oops if not found
    TokenKind (*string_to_token)(std::string s,TokenKind oops) ;   // which token is uniquely represented by the string, oops if not found
} ;

                                                // include function definitions for each tokeniser
#include "a-tok.h"
#include "j-tok.h"
#include "v-tok.h"
#include "w-tok.h"


//////////////////////////////////////////////////////
//
//    EXAMPLE OF USING A TOKENISER
//
//
//    // print all tokens read by a tokeniser
//    void print_tokens(tokensier t)
//    {
//        token = t->next_token() ;         // first read initialises the tokeniser
//        while ( token.kind != t->eoi )    // keep going until end of input token returned
//        {
//            cout << token << endl ;       // print the last token read
//            token = t->next_token() ;     // read the next one
//            count++ ;                     // keep count just because we can
//        }
//                                          // print out the token count
//        cout << "read " << count << " tokens\n" ;
//    }
//
//    ...
// 
//        // pass the Jack tokeniser to print_tokens
//        print_tokens( j_tokeniser() ) ;
//
//
//

#endif //TOKENISER_H
