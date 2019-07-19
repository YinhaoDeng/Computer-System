#include "ast.h"

void jack_codegen(jn_class t)
{
}

int main(int argc,char **argv)
{
    // parse an AST in XML and print VM code
    jack_codegen(jn_parse_xml()) ;

    // flush the output and any errors
    print_output() ;
    print_errors() ;
}

