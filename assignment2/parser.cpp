// convert Hack Assembly Language into an abstract syntax tree
#include "iobuffer.h"
#include "tokeniser.h"
#include "abstract-syntax-tree.h"
#include <algorithm>
#include <vector>

// to make out programs a bit neater
using namespace std ;

using namespace Hack_Assembler ;

// grammer to be parsed:
// a_program ::= instruction* eoi
// instruction ::= a_label | a_instr_name | a_instruction | c_instr_dest | c_instr_reg | c_instr_alu_op
// a_label ::= label
// a_instr_name ::= name
// a_instruction ::= number
// c_instr_reg ::= register (equals alu_op)? (semi jump)?
// c_instr_dest ::= dest equals alu_op (semi jump)?
// c_instr_alu_op ::= alu_op (semi jump)?
// Tokens: label, name, number, null, dest, register, alu_op, jump

// forward declare parsing functions - one per rule
ast parse_program() ;
ast parse_instruction() ;
ast parse_a_label() ;
ast parse_a_instr_name() ;
ast parse_a_instruction() ;
ast parse_c_instr_dest() ;
ast parse_c_instr_reg() ;
ast parse_c_instr_alu_op() ;

// Note: always read one token after the one recognised

// a_program ::= instruction* eoi
ast parse_program()
{
    vector<ast> instructions ;

    // we stop when we see the EOI token
    // this may be end of input or an error
    // we cannot tell so treat both as end of input
    // every new assembler instruction we find is appended to the_program
    while ( have(tk_instruction) )
    {
        instructions.push_back(parse_instruction()) ;
    }
    mustbe(tk_eoi) ;

    return create_program(instructions) ;
}

/*****************   REPLACE THE FOLLOWING CODE  ******************/

// a_program ::= instruction* eoi
ast parse_instruction()
{
   if(have(tk_label))                      //(LOOP)  (END)   ()
   {
        return parse_a_label();
   }
   else if(have(tk_address))       //
   {
        if(have(tk_name)) 
            {return parse_a_instr_name();}
        else if(have(tk_number))
            {return parse_a_instruction();}
   }else
        return parse_c_instr_reg();
    // if ( have(tk_label) ) return parse_a_label() ;
                                                                                                // if ( have(tk_name) ) return parse_a_instr_name() ;
                                                                                             // if ( have(tk_number) ) return parse_a_instruction() ;
    // if ( have(tk_register) ) return parse_c_instr_reg() ;
    // if ( have(tk_dest) ) return parse_c_instr_dest() ;
    // if ( have(tk_alu_op) ) return parse_c_instr_alu_op() ;
    mustbe(tk_instruction) ;

    return -1 ;
}

// tk_label is the token for '(' label ')'
ast parse_a_label()
{
    string label = "" ;
    label= token_spelling();////////////////////////////////////////////////////////////
    next_token() ;              // replace this with code to parse a label
    return create_label(label) ;
}

// tk_address is the token for '@' name
ast parse_a_instr_name()
{
    string name = "" ;
    name = token_spelling(); /////////////////////////
    next_token() ;              // replace this with code to parse '@'name
    return create_a_name(name) ;
}

// tk_number is the token for '@' number
ast parse_a_instruction()
{
    int ivalue = 0 ;
    ivalue = token_ivalue(); //////////////////////////////////
    next_token() ;              // replace this with code to parse '@'number
    return create_a_instruction(ivalue) ;
}


/*
    c-instruction, there are 4 different formats:
    case 1: A
    case 2: D=A
    case 3: D=A;JGT
    case 4: A;JGT
*/

// c_instruction ::= tk_register (tk_assign tk_alu_op)? (tk_semi tk_jump)?
ast parse_c_instr_reg()   //C instruction that starts with a register           ? (= A)   or    ? (; JGT)
{   //initialize 4 string variables
    string dest = "NULL" ;
    string alu_op = "NULL" ;  //comp
    string jump = "NULL" ;

    vector <string> cInstruction;
    cInstruction.push_back(token_spelling());   //AM
    for(int i=0; i<2;i++)  
    {
        next_token();     //
        cInstruction.push_back(token_spelling());  //AM=
    }
    
    if(cInstruction.at(1)=="=")
    {
        alu_op = cInstruction.at(2);
        dest = cInstruction.at(0);
    }

    if(cInstruction.at(1)==";")    //A;JGT
    {
        alu_op = cInstruction.at(0);
        jump = cInstruction.at(2);
    }

    next_token();
    return create_c_instruction(dest,alu_op,jump);
}


// c_instruction ::= tk_dest tk_assign tk_alu_op (tk_semi tk_jump)?     D=A  (;JGT)
ast parse_c_instr_dest()            //C instruction that starts with a destination
{    
    string dest = "" ;
    string alu_op = "" ;  //comp
    string jump = "" ;
    //already known part
    dest = token_spelling(mustbe(tk_assign));   //D
    alu_op = token_spelling(mustbe(tk_alu_op)); //D = A

    if(have(tk_semi)==true){   
        next_token();                //D = A ;
        jump = token_spelling(mustbe(tk_jump)); // D = A; JGT
    }else{ //if(have(tk_semi)==false)           //D = A
        jump = "NULL";
    }
    return create_c_instruction(dest,alu_op,jump) ;
}

// c_instruction ::= tk_alu_op (tk_semi tk_jump)?     A (; JGT)
ast parse_c_instr_alu_op() //C instruction that starts with an alu_op
{
    string dest = "" ;
    string alu_op = "" ;
    string jump = "" ;

    dest = "NULL";   //It has to be NULL in here
    alu_op = token_spelling(mustbe(tk_alu_op)); //already known

    if(have(tk_semi)==true){
        next_token();
        jump =  token_spelling(mustbe(tk_jump));
    }else{
        jump = "NULL";            
    }
    return create_c_instruction(dest,alu_op,jump) ;
}






/*****************        DOWN TO HERE         ******************/

// main program
int main(int argc,char **argv)
{
    // hide error messages
    config_errors(iob_buffer) ;

    // initialise the tokeniser by reading the first token
    next_token() ;

    // parse a class and print the abstract syntax tree as XML
    ast_print_as_xml(parse_program(),4) ;

    // flush the output and any errors
    print_output() ;
    print_errors() ;
}

