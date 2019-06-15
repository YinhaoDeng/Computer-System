// convert Text into Tokens 
#include "tokeniser.h"
#include <iostream>

using namespace std ;
using namespace Assignment_Tokeniser ;

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
        cout << token_to_string(token) << endl ;
        token = next_token() ;
        count++ ;
    }
    cout << "read " << count << " tokens" << endl ;

    return 0 ;
}
