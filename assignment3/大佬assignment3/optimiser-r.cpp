#include "ast.h"

jn_class jack_optimiser_r(jn_class t)
{
    return t ;
}

int main(int argc,char **argv)
{
    // parse an AST in XML, optimise it, print the new AST as XML
    jn_print_as_xml(jack_optimiser_r(jn_parse_xml()),4) ;

    // flush the output and any errors
    print_output() ;
    print_errors() ;
}

