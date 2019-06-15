#include "tokeniser.h"
#include "includes/tokeniser.h"
#include <ctype.h>

// to shorten the code
using namespace std;

namespace Workshop_Tokeniser {
// parse a single character symbol
// we know ch is the one character symbol
static Token parse_single_char_symbol(TokenKind kind, string spelling)
{
    // always read one character past the end of the token
    nextch();

    // return a new Token object
    return new_token(kind, spelling);
}

// parse a number - always read one extra character
// we know ch is the first digit of the number
static Token parse_integer()
{
    string spelling = "";

    // append digits to spelling until we read past the end of the integer
    do {
        spelling += ch;
        nextch();
    } while (isdigit(ch));

    // return a new Token object
    return new_token(tk_integer, spelling);
}

static Token parse_identifier()
{
    string spelling = "";

    // append digits to spelling until we read past the end of the integer
    do {
        spelling += ch;
        nextch();
    } while (isalpha(ch) || isdigit(ch));

    TokenKind tk = string_to_token_kind(spelling);

    if (tk == tk_oops) {
        tk = tk_identifier;
    }

    // return a new Token object
    return new_token(tk, spelling);
}

static Token parse_start_with_lt()
{
    nextch();
    if (ch == '=') {
        nextch();
        return new_token(tk_le, "<=");
    } else {
        return new_token(tk_lt, "<");
    }
}

static Token parse_start_with_gt()
{
    nextch();
    if (ch == '=') {
        nextch();
        return new_token(tk_ge, ">=");
    } else {
        return new_token(tk_gt, ">");
    }
}

static Token parse_start_with_eq()
{
    nextch();
    if (ch == '=') {
        nextch();
        return new_token(tk_eq, "==");
    } else {
        return new_token(tk_assign, "=");
    }
}

static Token parse_start_with_not()
{
    nextch();
    if (ch == '=') {
        nextch();
        return new_token(tk_ne, "!=");
    } else {
        // return new_token(tk_not, "!"); 
        return new_token(tk_eoi, "!"); 
    }
}

static Token parse_start_with_slash()
{
    nextch();
    if (ch == '/') {
        return new_token(tk_divide, "/"); 
    } else if (ch == '*') {
       
        return new_token(tk_divide, "/"); 
    } else {
        return new_token(tk_divide, "/");
    }
}

// return the next Token object by reading more of the input
Token next_token()
{

    while (ch != EOF) // loop until EOF or a token is read
    {
        switch (ch) // ch is always the next char to read
        {
        case ' ': // ignore whitespace
        case '\t':
        case '\r':
        case '\n':
            nextch(); // read next character and go around again
            break;

            // when we find the first character of a token
            // call a function to read all of its characters

            // single character symbols

        case '+': return parse_single_char_symbol(tk_add, "+");
        case '-':return parse_single_char_symbol(tk_sub, "-");
        case '*':return parse_single_char_symbol(tk_times, "*");
        case '@':return parse_single_char_symbol(tk_at, "@");
        case '{':return parse_single_char_symbol(tk_lcb, "{");
        case '}':return parse_single_char_symbol(tk_rcb, "}");
        case '(':return parse_single_char_symbol(tk_lrb, "(");
        case ')':return parse_single_char_symbol(tk_rrb, ")");
        case ':': return parse_single_char_symbol(tk_colon, ":");
        case ';': return parse_single_char_symbol(tk_semi, ";");
        case '.':return parse_single_char_symbol(tk_dot, ".");
        case ',': return parse_single_char_symbol(tk_comma, ",");
        case '"': return parse_single_char_symbol(tk_dquote, "\"");
        case '0' ... '9': return parse_integer();
        case 'a' ... 'z':
        case 'A' ... 'Z':  return parse_identifier();
        case '<':    return parse_start_with_lt();
        case '>': return parse_start_with_gt();
        case '!': return parse_start_with_not();
        case '/': return parse_start_with_slash();
        case '=': return parse_start_with_eq();

        default: // throw error message
            ch = EOF;
            return new_token(tk_eoi, "");
        }
    }
    ch = EOF;
    return new_token(tk_eoi, "");
}
}
