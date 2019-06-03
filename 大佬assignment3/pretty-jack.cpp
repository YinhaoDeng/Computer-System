#include "ast.h"

void jack_pretty(jn_class t)
{
}

int main(int argc,char **argv)
{
    // parse an AST in XML and pretty print as Jack
    jack_pretty(jn_parse_xml()) ;

    // flush the output and any errors
    print_output() ;
    print_errors() ;
}

