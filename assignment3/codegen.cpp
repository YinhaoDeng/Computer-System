#include "iobuffer.h"
#include "symbols.h"
#include "abstract-syntax-tree.h"

// to shorten our code:
using namespace std ;
using namespace Jack_Compiler ;

// ***** WHAT TO DO *****
//
// MODIFY the skeleton code below to walk an abstract syntax tree, ast, of a Jack class
//        and generate the equivalent Hack Virtual Machine Code.
//
// NOTE: the correct segment and offset is recorded with every variable in the ast
//       so the code generator does not need to use any symbol tables
//
// The skeleton code has one function per node in the abstract tree
//  - they each extract all fields into local variables
//  - they each call the appropriate walk_* function to walk any sub-trees
//
// The structure of the skeleton code is just a suggestion
//  - you may want to change the parameters / results to suit your own logic
//  - you can change it as much as you like

// forward declarations of one function per node in the abstract syntax tree
void walk_class(ast t) ;
void walk_class_var_decs(ast t) ;
void walk_var_dec(ast t) ;
void walk_subr_decs(ast t) ;
void walk_subr(ast t) ;
void walk_constructor(ast t) ;
void walk_function(ast t) ;
void walk_method(ast t) ;
void walk_param_list(ast t) ;
void walk_subr_body(ast t) ;
void walk_var_decs(ast t) ;
void walk_statements(ast t) ;
void walk_statement(ast t) ;
void walk_let(ast t) ;
void walk_let_array(ast t) ;
void walk_if(ast t) ;
void walk_if_else(ast t) ;
void walk_while(ast t) ;
void walk_do(ast t) ;
void walk_call(ast t) ;
void walk_return(ast t) ;
void walk_return_expr(ast t) ;
void walk_expr_list(ast t) ;
void walk_expr(ast t) ;
void walk_term(ast t) ;
void walk_op(ast t) ;
void walk_int(ast t) ;
void walk_string(ast t) ;
void walk_bool(ast t) ;
void walk_null(ast t) ;
void walk_this(ast t) ;
void walk_var(ast t) ;
void walk_array_index(ast t) ;
void walk_unary_op(ast t) ;
///////////////////////////////////////////////
static int ifCounter;
static int whileCounter;
static int FieldNumber = 0;
static string className;
static string returnType;
/////////////////////////////////////////////////////////
void walk_class(ast t) //given implementation
{
    className = get_class_class_name(t) ;
    ast var_decs = get_class_var_decs(t) ;
    ast subr_decs = get_class_subr_decs(t) ;
    walk_class_var_decs(var_decs) ;
    walk_subr_decs(subr_decs) ;
}

void walk_class_var_decs(ast t)  //given implementation
{
    int ndecs = size_of_class_var_decs(t) ;
    for ( int i = 0 ; i < ndecs ; i++ )
    {
        walk_var_dec(get_class_var_decs(t,i)) ;
    }
}


void walk_subr(ast t) //given implementation
{
    ast subr = get_subr_subr(t) ;

    switch(ast_node_kind(subr))
    {
    case ast_constructor:
        walk_constructor(subr) ;
        break ;
    case ast_function:
        walk_function(subr) ;
        break ;
    case ast_method:
        walk_method(subr) ;
        break ;
    default:
        fatal_error(0,"Unexpected subroutine kind") ;
        break ;
    }
}

void walk_constructor(ast t)
{
    /*
    constructor
    function class_name.subr_name nLocal
    push constant FieldNumber
    call Memory.alloc 1
    pop pointer 0
    statements
    */
    returnType = get_constructor_vtype(t);
    string name = get_constructor_name(t);
    ast param_list = get_constructor_param_list(t) ;
    ast subr_body = get_constructor_subr_body(t) ;

    ast var_decs = get_subr_body_decs(subr_body);

    cout<<"function "<<className<< "."<<name<< " "<<size_of_var_decs(var_decs)<<endl; //size_of_var_decs(var_decs) returns how many localvariables there are
    cout<<"push constant "<<FieldNumber<<endl;
    cout<<"call Memory.alloc 1"<<endl;
    cout<<"pop pointer 0"<<endl;

    walk_param_list(param_list) ;
    walk_subr_body(subr_body) ;
}



void walk_statements(ast t)   //given implementation
{
    int nstatements = size_of_statements(t) ;
    for ( int i = 0 ; i < nstatements ; i++ )
    {
        walk_statement(get_statements(t,i)) ;
    }
}

void walk_method(ast t)             
{
    /*
        function class_name.subr_name nVar
        push argument 0
        pop pointer 0
        statements
    */
    returnType = get_method_vtype(t) ;
    string name = get_method_name(t) ;
    ast param_list = get_method_param_list(t) ;
    ast subr_body = get_method_subr_body(t) ;

    ast  var_decs = get_subr_body_decs(subr_body);

    cout<<"function "<<className<<"."<<name<<" "<<size_of_var_decs(var_decs)<<endl;
    cout<<"push argument 0"<<endl;
    cout<<"pop pointer 0"<<endl;

    walk_param_list(param_list) ;
    walk_subr_body(subr_body) ;
}

void walk_param_list(ast t)  //given implementation             
{
    int ndecs = size_of_param_list(t) ;
    for ( int i = 0 ; i < ndecs ; i++ )
    {
        walk_var_dec(get_param_list(t,i)) ;
    }
}

void walk_array_index(ast t)
{
    /*
        expression
        push varsegment varoffset
        add
        pop pointer 1
        push that 0
    */
    ast var = get_array_index_var(t) ;
    ast index = get_array_index_index(t) ;

    walk_var(var) ;
    walk_expr(index) ;

    cout<<"add"<<endl;
    cout<<"pop pointer 1"<<endl;
    cout<<"push that 0"<<endl;
}

void walk_var_dec(ast t)   //I made it!
{
    if(get_var_dec_segment(t)== "this")
    {
        FieldNumber++;
    }
}

void walk_var_decs(ast t)  //given implementation
{
    int ndecs = size_of_var_decs(t) ;
    for ( int i = 0 ; i < ndecs ; i++ )
    {
        walk_var_dec(get_var_decs(t,i)) ;
    }
}


void walk_statement(ast t)   //given implementation
{
    ast statement = get_statement_statement(t) ;

    switch(ast_node_kind(statement))
    {
    case ast_let:
        walk_let(statement) ;
        break ;
    case ast_let_array:
        walk_let_array(statement) ;
        break ;
    case ast_if:
        walk_if(statement) ;
        break ;
    case ast_if_else:
        walk_if_else(statement) ;
        break ;
    case ast_while:
        walk_while(statement) ;
        break ;
    case ast_do:
        walk_do(statement) ;
        break ;
    case ast_return:
        walk_return(statement) ;
        break ;
    case ast_return_expr:
        walk_return_expr(statement) ;
        break ;
    case ast_statements:
        walk_statements(statement) ;
        break ;
    default:
        fatal_error(0,"Unexpected statement kind") ;
        break ;
    }
}

void walk_subr_body(ast t)
{
    ast decs = get_subr_body_decs(t) ;
    ast body = get_subr_body_body(t) ;

    ifCounter = 0;
    whileCounter = 0;

    walk_var_decs(decs) ;
    walk_statements(body) ;
}

void walk_let(ast t)  
{
    /*
        expressions
        pop segement offset
    */
    ast var = get_let_var(t) ;
    ast expr = get_let_expr(t) ;

    walk_expr(expr) ;
    cout<<"pop "<<get_var_segment(var)<<" "<<get_var_offset(var)<<endl;
}

void walk_let_array(ast t)
{
    /*
        expression    e.g push constant 3
        push varSegment varOffset
        add
        (expression2)
        pop temp 0
        pop pointer 1
        push temp 0
        pop that 0
    */
    ast var = get_let_array_var(t);
    ast index = get_let_array_index(t);
    ast expr = get_let_array_expr(t);

    walk_expr(index);
    walk_var(var);
    cout<<"add"<<endl;

    walk_expr(expr);

    cout<<"pop temp 0"<<endl;
    cout<<"pop pointer 1"<<endl;
    cout<<"push temp 0"<<endl;
    cout<<"pop that 0"<<endl;
}

void walk_if(ast t)
{
    /*
        expression
        if-goto IF_TRUEn    //n=if counter
        goto IF_FALSEn
        label IF_TRUEn
        statements
        label IF_FALSEn
    */
    ast condition = get_if_condition(t) ;
    ast if_true = get_if_if_true(t) ;
    int n = ifCounter;

    walk_expr(condition) ;

    cout<<"if-goto IF_TRUE"<<n<<endl;
    cout<<"goto IF_FALSE"<<n<<endl;
    cout<<"label IF_TRUE"<<n<<endl;

    walk_statements(if_true) ;

    cout<<"label IF_FALSE"<<n<<endl;

    ifCounter++;
}

void walk_if_else(ast t)
{
    /*
        expression
        if-goto IF_TRUEn //n = ifCounter
        goto IF_FALSEn
        label IF_TRUEn
        statements
        goto IF_ENDn  //if there is an else part
        label IF_FALSEm
        statements  //if there is an else part
        label IF_END //if there is an else part
    */    
    ast condition = get_if_else_condition(t) ;
    ast if_true = get_if_else_if_true(t) ;
    ast if_false = get_if_else_if_false(t) ;
    int n = ifCounter;

    walk_expr(condition) ;

    cout<<"if-goto IF_TRUE"<<n<<endl;
    cout<<"goto IF_FALSE"<<n<<endl;
    cout<<"label IF_TRUE"<<n<<endl;

    walk_statements(if_true) ;

    cout<<"goto IF_END"<<n<<endl;
    cout<<"label IF_FALSE"<<n<<endl;

    walk_statements(if_false) ;
    cout<<"label IF_END"<<n<<endl;

    ifCounter++;
}

void walk_while(ast t)
{
    /*
        label WHILE_EXPn //n=whileCounter
        expressions
        not
        if-goto WHILE_ENDn
        statements
        goto WHILE_EXPn
        label WHILE_ENDn
    */
    ast condition = get_while_condition(t) ;
    ast body = get_while_body(t) ;
    int n =whileCounter;

    cout<<"label WHILE_EXP"<<n<<endl;

    walk_expr(condition) ;

    cout<<"not"<<endl;
    cout<<"if-goto WHILE_END"<<n<<endl;
    walk_statements(body) ;
    cout<<"goto WHILE_EXP"<<n<<endl;
    cout<<"label WHILE_END"<<n<<endl;

    whileCounter++;
}

void walk_do(ast t)
{
    ast call = get_do_call(t) ;
    walk_call(call) ;
    cout<<"pop temp 0"<<endl;
}

void walk_call(ast t)
{
    /*
        call clss_name.subr_name  nArgu
        pop temp 0
    */
    bool method_call = get_call_method_call(t) ;
    string class_name = get_call_class_name(t) ;
    string subr_name = get_call_subr_name(t) ;
    ast expr_list = get_call_expr_list(t) ;
    int nArg = size_of_expr_list(expr_list);

    walk_expr_list(expr_list) ;
    cout<<"call "<<class_name<<"."<<subr_name<<" "<<nArg<<endl;
}

void walk_return(ast t) 
{
    /*
        push constant 0
        return
    */
   cout<<"push constant 0"<<endl;
   cout<<"return"<<endl;
}

void walk_return_expr(ast t)
{
    /*
        expression
        return
    */
    ast expr = get_return_expr(t) ;

    walk_expr(expr) ;
    cout<<"return"<<endl;
}

void walk_expr_list(ast t)   //given implementation
{
    int nexpressions = size_of_expr_list(t) ;
    for ( int i = 0 ; i < nexpressions ; i++ )
    {
        walk_expr(get_expr_list(t,i));
    }
}

void walk_expr(ast t)
{
    int term_ops = size_of_expr(t);
    walk_term(get_expr(t,0));
    for(int i=1; i<term_ops; i+=2)
    {
        ast op =get_expr(t,i);
        ast term = get_expr(t, i+1);
        walk_term(term);
        walk_op(op);
    }
}

void walk_term(ast t)  //given implemenation
{
    ast term = get_term_term(t) ;

    switch(ast_node_kind(term))
    {
    case ast_int:
        walk_int(term) ;
        break ;
    case ast_string:
        walk_string(term) ;
        break ;
    case ast_bool:
        walk_bool(term) ;
        break ;
    case ast_null:
        walk_null(term) ;
        break ;
    case ast_this:
        walk_this(term) ;
        break ;
    case ast_expr:
        walk_expr(term) ;
        break ;
    case ast_var:
        walk_var(term) ;
        break ;
    case ast_array_index:
        walk_array_index(term) ;
        break ;
    case ast_unary_op:
        walk_unary_op(term) ;
        break ;
    case ast_call:
        walk_call(term) ;
        break ;
    default:
        fatal_error(0,"Unexpected term kind") ;
        break ;
    }
}

void walk_op(ast t) 
{
    /*
        +add  -sub  *call Math.multiply 2
        /call Math.divide 2
        <lt   >gt  =eq  &and
        |or
    */
    string op = get_op_op(t) ;
 
    if(op[0]=='+')  cout<<"add"<<endl;   //traslate each operation signs
    else if(op[0]=='-')  cout<<"sub"<<endl;
    else if(op[0]=='*')  cout<<"call Math.mutiply 2"<<endl;
    else if(op[0]=='/')  cout<<"call Math.divide 2"<<endl;
    else if(op[0]=='<')  cout<<"lt"<<endl;
    else if(op[0]=='>')  cout<<"gt"<<endl;
    else if(op[0]=='=')  cout<<"eq"<<endl;
    else if(op[0]=='&')  cout<<"and"<<endl;
    else if(op[0]=='|')  cout<<"or"<<endl;

}

void walk_int(ast t)
{
    /*
        push constant 9 (number)
    */
    int _constant = get_int_constant(t);
    cout<<"push constant "<<_constant<<endl;
}

void walk_string(ast t) //////////////////////////////////don't know how to do it
{
    /*
        push string is weird to achieve
        
        push constant stringlength
        call String.new 1
        
    */
}

void walk_function(ast t)
{
    /* 
    function class_name.subr_name nVar
    statements
    */
    returnType = get_function_vtype(t) ;
    string name = get_function_name(t) ;
    ast param_list = get_function_param_list(t) ;
    ast subr_body = get_function_subr_body(t) ;

    ast  var_decs = get_subr_body_decs(subr_body);
    
    cout<<"function "<<className<<"."<<name<<" "<<size_of_var_decs(var_decs)<<endl;

    walk_param_list(param_list) ;
    walk_subr_body(subr_body) ;
}

void walk_bool(ast t)
{
    /*
        true -------
        push constant 0
        not
        false-----
        push constant 0
    */
    bool _constant = get_bool_t_or_f(t) ;
    cout<<"push constant 0"<<endl;
    if(_constant)
    {
        cout<<"not"<<endl;
    }
}

void walk_null(ast t)
{
    /*
        push constant 0
    */
   cout<<"push constant 0"<<endl;
}

void walk_this(ast t)
{
    /*
        push pointer 0
    */
   cout<<"push pointer 0"<<endl;
}

void walk_subr_decs(ast t)   //given implementation
{
    int size = size_of_subr_decs(t) ;
    for ( int i = 0 ; i < size ; i++ )
    {
        walk_subr(get_subr_decs(t,i)) ;
    }
}

void walk_var(ast t)
{
    /*
        push varsegement varoffset
    */
    string name = get_var_name(t) ;
    string type = get_var_type(t) ;
    string segment = get_var_segment(t) ;


    cout<<"push "<<segment<<" "<<get_var_offset(t)<<endl;
}


void walk_unary_op(ast t)
{
    /*
        (term)
        neg   //if -
        not   //if ~
    */
    string uop = get_unary_op_op(t);
    ast term = get_unary_op_term(t) ;

    walk_term(term) ;

    if(uop == "-") cout<<"neg"<<endl;
    else if(uop == "~")  cout<<"not"<<endl; 
}

// main program
int main(int argc,char **argv)
{
    // walk an AST parsed from XML and print VM code
    walk_class(ast_parse_xml()) ;

    // flush the output and any errors
    print_output() ;
    print_errors() ;
}

