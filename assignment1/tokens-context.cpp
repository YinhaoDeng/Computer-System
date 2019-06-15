// convert Text into Tokens 
#include "tokeniser.h"
#include <iostream>

using namespace std ;
using namespace Assignment_Tokeniser ;

// token_is_in test
void token_is_in_test(Token token,TokenKind kind)
{
    cout << "token_is_in(token," << token_kind_to_string(kind) << ") returns " ;
    cout << (token_is_in(token,kind) ? "true" : "false") ;
    cout << endl;
}

// this main program tokenises standard input and
// prints the tokens as they are found
int main(int argc,char **argv)
{
    Token token ;
    int count = 0 ;

    // remember and display each token as it is read
    token = next_token() ;
    while ( token_kind(token) != tk_eoi )
    {
        // output context every 4 tokens
        if ( ++count % 4 == 0 )
        {
            cout << token_context(token) << endl ;
            token_is_in_test(token,tk_identifier) ;
            token_is_in_test(token,tk_integer) ;
            token_is_in_test(token,tk_double) ;
            token_is_in_test(token,tk_keyword) ;
            token_is_in_test(token,tk_symbol) ;
        }

        cout << token_to_string(token) << endl ;
        token = next_token() ;
    }
    cout << "read " << count << " tokens" << endl ;

    return 0 ;
}
