// convert an abstract syntax tree for Hack Assembly language into machine code
#include "iobuffer.h"
#include "symbols.h"
#include "tokeniser.h"
#include "abstract-syntax-tree.h"

// to simplify the code
using namespace std ;
using namespace CS_Symbol_Tables ;
using namespace Hack_Assembler ;
/////////////////////////////////////////////////////////////////
string r="";/////////////////////////////////////////////////////
string toBinary(int n)//////////////////////////////////////////
{///////////////////////////////////////////////////////////////
    r=(n%2==0 ?"0":"1")+r;//////////////////////////////////////
    if (n / 2 != 0) //////////////////////////////////////////
        toBinary(n / 2);//////////////////////////////////////
    return r;///////////////////////////////////////////////////
}///////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
string upto16bit(string s)//////////////////////////////////////  decimal to 16-bit binary
{////////////////////////////////////////////////////////////////
    while(s.length()<16)////////////////////////////////////////////
	{////////////////////////////////////////////////////////////
		s = "0" + s;//////////////////////////////////////////////
	}///////////////////////////////////////////////////////////
	return s;///////////////////////////////////////////////////
}/////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
string dec_bi(int a)/////////////////////////////////////////////
{////////////////////////////////////////////////////////////////
    r="";///////////////////////////////////////////////////////
	return upto16bit(toBinary(a));///////////////////////////////
}////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

static bool firstPass = true;
//static bool second
static symbols symbolTable;

// this function provides an example of how to walk an abstract syntax tree constructed by ast_parse_xml()
void walk_program(ast the_program)
{
    // the_program contains a vector of instructions
    int ninstructions = size_of_program(the_program);
    ///////////////////////I strat from here/////////////////////////////////////

    TokenKind dest;
    TokenKind alu_op;
    TokenKind jump;

    int varCounter = 16;// define a variable counter, initialize it as 16
    int lineCounter = 0; //difine a line counter
    

    if(firstPass)
    {
        symbolTable = create_strings() ;// Create a new empty table to store predifined symbols    check the symbol.h file  (line 116)
        //Insert all the predefined symbols in to the table we just created. using insert_strings(), if fail, throw error
         insert_strings(symbolTable,"SP","0000000000000000");
         insert_strings(symbolTable,"LCL","0000000000000001");
         insert_strings(symbolTable,"ARG","0000000000000010");
         insert_strings(symbolTable,"THIS","0000000000000011")     ;
         insert_strings(symbolTable,"THAT","0000000000000100")     ;
         insert_strings(symbolTable,"R0","0000000000000000")       ;
         insert_strings(symbolTable,"R1","0000000000000001")       ;
         insert_strings(symbolTable,"R2","0000000000000010")       ;
         insert_strings(symbolTable,"R3","0000000000000011")       ;
         insert_strings(symbolTable,"R4","0000000000000100")       ;
         insert_strings(symbolTable,"R5","0000000000000101")       ;
         insert_strings(symbolTable,"R6","0000000000000110")      ;
         insert_strings(symbolTable,"R7","0000000000000111")       ;
         insert_strings(symbolTable,"R8","0000000000001000")      ;
         insert_strings(symbolTable,"R9","0000000000001001")      ;
         insert_strings(symbolTable,"R10","0000000000001010")      ;
         insert_strings(symbolTable,"R11","0000000000001011")      ;
         insert_strings(symbolTable,"R12","0000000000001100")      ;
         insert_strings(symbolTable,"R13","0000000000001101")     ;
         insert_strings(symbolTable,"R14","0000000000001110")     ;
         insert_strings(symbolTable,"R15","0000000000001111")      ;
         insert_strings(symbolTable,"SCREEN","0100000000000000") ;
         insert_strings(symbolTable,"KEYBOARD","0110000000000000") ;
    }

    for ( int i = 0 ; i < ninstructions ; i++ )
    {
        lineCounter++;   //update lineCounter
        ast instruction = get_program(the_program,i) ;


        switch(ast_node_kind(instruction))
        { //switch start
            case ast_label:
                if(firstPass)
                {
                    insert_strings(symbolTable, get_label_name(instruction), dec_bi(--lineCounter));
                }
                break ;

            case ast_a_name:  
                if(!firstPass)   //in second pass, look for user
                {
                    if(insert_strings(symbolTable, get_a_name_unresolved(instruction), dec_bi(varCounter)))
                    {
                        varCounter++;
                    }
                    cout << lookup_strings(symbolTable, get_a_name_unresolved(instruction)) << endl;
                }       
                break ;

            case ast_a_instruction:
                if(!firstPass) //second pass
                {
                    cout << dec_bi(get_a_instruction_value(instruction)) <<endl;
                }
                break ;

            case ast_c_instruction:
                if(!firstPass)
                {
                    dest = string_to_token_kind(get_c_instruction_dest(instruction));
                    alu_op = string_to_token_kind(get_c_instruction_alu(instruction));
                    jump = string_to_token_kind(get_c_instruction_jump(instruction));

                    cout<<"111"; //c-instruction start
                    //c-instruction comp part: alu_op        //print out 7 bits
                    if(alu_op == tk_register_A)           {cout<<"0110000";}
                    else if (alu_op == tk_register_M)     {cout<<"1110000";}
                    else if (alu_op == tk_register_D)     {cout<<"0001100";}
                    else if (alu_op == tk_alu_0)          {cout<<"0101010";}
                    else if (alu_op == tk_alu_1)          {cout<<"0111111";}
                    else if (alu_op == tk_alu_sub_1)      {cout<<"0111010";}
                    else if (alu_op == tk_alu_not_D)      {cout<<"0001101";}
                    else if (alu_op == tk_alu_not_A)      {cout<<"0110001";}
                    else if (alu_op == tk_alu_sub_D)      {cout<<"0001111";}
                    else if (alu_op == tk_alu_sub_A)      {cout<<"0110011";}
                    else if (alu_op == tk_alu_D_add_1)      {cout<<"0011111";}
                    else if (alu_op == tk_alu_A_add_1)      {cout<<"0110111";}
                    else if (alu_op == tk_alu_D_sub_1)      {cout<<"0001110";}
                    else if (alu_op == tk_alu_A_sub_1)      {cout<<"0110010";}
                    else if (alu_op == tk_alu_D_add_A)      {cout<<"0000010";}
                    else if (alu_op == tk_alu_D_sub_A)      {cout<<"0010011";}
                    else if (alu_op == tk_alu_A_sub_D)      {cout<<"0000111";}
                    else if (alu_op == tk_alu_D_and_A)      {cout<<"0000000";}
                    else if (alu_op == tk_alu_D_or_A)      {cout<<"0010101";}
                    else if (alu_op == tk_alu_not_M)      {cout<<"1110001";}
                    else if (alu_op == tk_alu_sub_M)      {cout<<"1110011";}
                    else if (alu_op == tk_alu_M_add_1)      {cout<<"1110111";}
                    else if (alu_op == tk_alu_M_sub_1)      {cout<<"1110010";}
                    else if (alu_op == tk_alu_D_add_M)      {cout<<"1000010";}
                    else if (alu_op == tk_alu_D_sub_M)      {cout<<"1010011";}
                    else if (alu_op == tk_alu_M_sub_D)      {cout<<"1000111";}
                    else if (alu_op == tk_alu_D_and_M)      {cout<<"1000000";}
                    else if (alu_op == tk_alu_D_or_M)      {cout<<"1010101";}
                    //c-instruction destination
                    if(dest == tk_register_D)           {cout<<"010";}
                    else if (dest == tk_register_A)     {cout<<"100";}
                    else if (dest == tk_register_M)     {cout<<"001";}
                    else if (dest == tk_dest_MD)        {cout<<"011";}
                    else if (dest == tk_dest_AM)        {cout<<"101";}
                    else if (dest == tk_dest_AD)        {cout<<"110";}
                    else if (dest == tk_dest_AMD)       {cout<<"111";}
                    else if (dest == tk_null)           {cout<<"000";}
                    //c-instruction jump
                    if(jump == tk_jmp)           {cout<<"111\n";}
                    else if (jump == tk_jlt)     {cout<<"100\n";}
                    else if (jump == tk_jle)     {cout<<"110\n";}
                    else if (jump == tk_jgt)     {cout<<"001\n";}
                    else if (jump == tk_jge)     {cout<<"011\n";}
                    else if (jump == tk_jeq)     {cout<<"010\n";}
                    else if (jump == tk_jne)     {cout<<"101\n";}
                    else if (jump == tk_null)    {cout<<"000\n";}
                }
                break ;

            default:
                fatal_error(0,"// bad node - expected ast_label,ast_a_name,ast_a_instruction or ast_c_instruction\n") ;
                break ;
        }  //switch end
       
    }
     firstPass = false;
}

// translate an abstract syntax tree representation of Hack Assembly language into Hack machine code
static void asm_translator(ast the_program)
{  //go through the whole program twice
    walk_program(the_program); //the first pass
    walk_program(the_program); //the second pass
}

// main progra
int main(int argc,char **argv)
{
    // error messages
    config_errors(iob_buffer) ;

    // parse abstract syntax tree and pass to the translator
    asm_translator(ast_parse_xml()) ;

    // flush output and errors
    print_output() ;
    print_errors() ;
}
