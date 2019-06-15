#include "iobuffer.h"
#include "symbols.h"
#include "abstract-syntax-tree.h"

// to shorten our code:
using namespace std ;
using namespace Jack_Compiler ;

// ***** WHAT TO DO *****
//
// MODIFY the skeleton code below to walk an abstract syntax tree, ast, of a Jack class
//        and pretty print the equivalent Jack source code.
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

bool callingMethod;
int indentation = 0;  //indentation indentation  4 spaces
bool behindspace = false;

void makeIndentation()
{
    for(int i=0; i<indentation; i++)
    {
        cout<<"    ";
    }
}

void walk_class(ast t)
{
    ast var_decs = get_class_var_decs(t) ;
    ast subr_decs = get_class_subr_decs(t) ;

    cout<<"class "<<get_class_class_name(t)<<endl;
    cout<<"{"<<endl;
    indentation++; //add indentation 1 indentation, which is 4 spaces now

    walk_class_var_decs(var_decs) ;
    walk_subr_decs(subr_decs) ;
    indentation--;
    cout<<"}"<<endl; //no behindspace ahead
}

void walk_class_var_decs(ast t)
{
    int ndecs = size_of_class_var_decs(t) ;
    for ( int i = 0 ; i < ndecs ; i++ )
    {
        walk_var_dec(get_class_var_decs(t,i)) ;
    }
    if(ndecs > 0) //there is variable decleration
    {
        cout<<endl;
    }
}


void walk_subr_decs(ast t)
{
    int size = size_of_subr_decs(t) ;
    for ( int i = 0 ; i < size ; i++ )
    {
        if(i != 0)
        {
            cout<<endl;
        }
        walk_subr(get_subr_decs(t,i)) ;
    }
}

void walk_subr(ast t)
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
    string vtype = get_constructor_vtype(t) ;
    string name = get_constructor_name(t) ;
    ast param_list = get_constructor_param_list(t) ;
    ast subr_body = get_constructor_subr_body(t) ;

    makeIndentation();
    cout<<"constructor"<<vtype<<" "<<name<<"(";
    walk_param_list(param_list) ;
    cout<<")"<<endl;
    walk_subr_body(subr_body) ;
}


void walk_method(ast t)
{
    ast param_list = get_method_param_list(t) ;
    ast subr_body = get_method_subr_body(t) ;

    makeIndentation();
    cout<<"method"<<get_method_vtype(t)<<" "<<get_method_name(t)<<"(";
    walk_param_list(param_list) ;
    cout<<")"<<endl;
    walk_subr_body(subr_body) ;
}

void walk_var_dec(ast t)
{
    string name = get_var_dec_name(t) ;
    string type = get_var_dec_type(t) ;
    string segment = get_var_dec_segment(t) ;
    int offset = get_var_dec_offset(t) ;

    if(segment == "local") //convert segment
    {
        segment = "var";
    }
    if(segment == "this")//convert segment
    {
        segment = "field";
    }
    if(segment == "argument") //convert segment
    {
        cout<<type<<" "<<name;
    }else
    {
        makeIndentation();
        cout<<segment<<" "<<type<<" "<<name<<" ;"<<endl;
    }
}

void walk_param_list(ast t)
{
    int ndecs = size_of_param_list(t) ;
    for ( int i = 0 ; i < ndecs ; i++ )
    {
        if(i != 0)
        {
            cout<<",";
        }
        walk_var_dec(get_param_list(t,i)) ;
    }
}

void walk_subr_body(ast t)
{
    ast decs = get_subr_body_decs(t) ;
    ast body = get_subr_body_body(t) ;

    makeIndentation();
    cout<<"{"<<endl;
    indentation++;
    walk_var_decs(decs) ;
    walk_statements(body) ;
    indentation--;
    makeIndentation();
    cout<<"}"<<endl;
}

void walk_var_decs(ast t)
{
    int ndecs = size_of_var_decs(t) ;
    for ( int i = 0 ; i < ndecs ; i++ )
    {
        walk_var_dec(get_var_decs(t,i)) ;
    }
    if(ndecs>0)   //if there is local variable declaration
    {
        cout<<endl;
    }
}

void walk_statements(ast t)
{
    int nstatements = size_of_statements(t) ;
    for ( int i = 0 ; i < nstatements ; i++ )
    {
        if(i != 0 && behindspace==true)   //let do return, no behindspace behind them;         if while, behindspace
        {
            cout<<endl;
        }
        walk_statement(get_statements(t,i)) ;
    }
}

void walk_function(ast t)
{
    string vtype = get_function_vtype(t) ;
    string name = get_function_name(t) ;
    ast param_list = get_function_param_list(t) ;
    ast subr_body = get_function_subr_body(t) ;

    makeIndentation();
    cout<<"function "<<vtype<<" "<<name<<"(";
    walk_param_list(param_list) ;
    cout<<")"<<endl;
    walk_subr_body(subr_body) ;
}



void walk_let(ast t)
{
    ast var = get_let_var(t) ;
    ast expr = get_let_expr(t) ;

    makeIndentation();
    cout<<"let ";
    walk_var(var) ;
    cout<<" = ";
    walk_expr(expr) ;
    cout<<" ;"<<endl;
}

void walk_return_expr(ast t)
{
    ast expr = get_return_expr(t) ;

    makeIndentation();
    cout<<"return ";
    walk_expr(expr) ;
    cout<<" ;"<<endl;
}

void walk_let_array(ast t)
{
    ast var = get_let_array_var(t) ;
    ast index = get_let_array_index(t) ;
    ast expr = get_let_array_expr(t) ;

    makeIndentation();
    cout<<"let ";
    walk_var(var) ;
    cout<<"[";
    walk_expr(index) ;
    cout<<"] =";
    walk_expr(expr) ;
    cout<<" ;"<<endl; //there is behindspace before semi
}

void walk_if(ast t)
{
    ast condition = get_if_condition(t) ;
    ast if_true = get_if_if_true(t) ;


    makeIndentation();
    cout<<"if {";
    walk_expr(condition) ;
    cout<<"}"<<endl;
    makeIndentation();
    cout<<"{"<<endl;
    indentation++;
    walk_statements(if_true) ;
    indentation--;

    makeIndentation();
    cout<<"}"<<endl;
}

void walk_if_else(ast t)
{
    ast condition = get_if_else_condition(t) ;
    ast if_true = get_if_else_if_true(t) ;
    ast if_false = get_if_else_if_false(t) ;

    makeIndentation();
    cout<<"if (";
    walk_expr(condition) ;  //condition part
    cout<<")"<<endl;
    makeIndentation();
    cout<<"{"<<endl;  //left lcb

    indentation++; //add 4 spaces
    walk_statements(if_true) ;
    indentation--;

    makeIndentation();
    cout<<"}"<<endl;


    makeIndentation();
    cout<<"else"<<endl;

    makeIndentation();
    cout<<"{"<<endl;  //left lcb

    indentation++;
    walk_statements(if_false) ;
    indentation--; //bring indentation back

    makeIndentation();
    cout<<"}"<<endl;
}

void walk_statement(ast t)
{
    ast statement = get_statement_statement(t) ;

    switch(ast_node_kind(statement))
    {
    case ast_let:
        behindspace = false;
        walk_let(statement) ;
        break ;
    case ast_let_array:
        behindspace = false;
        walk_let_array(statement) ;
        break ;
    case ast_if:
        behindspace = true;
        walk_if(statement) ;
        break ;
    case ast_if_else:
        behindspace = true;
        walk_if_else(statement) ;
        break ;
    case ast_while:
        behindspace = true;
        walk_while(statement) ;
        break ;
    case ast_do:
        behindspace = false;
        walk_do(statement) ;
        break ;
    case ast_return:
        behindspace = false;
        walk_return(statement) ;
        break ;
    case ast_return_expr:
        behindspace = false;
        walk_return_expr(statement) ;
        break ;
    case ast_statements:
        behindspace = false;
        walk_statements(statement) ;
        break ;
    default:
        fatal_error(0,"Unexpected statement kind") ;
        break ;
    }
}

void walk_while(ast t)
{
    ast condition = get_while_condition(t) ;
    ast body = get_while_body(t) ;

    makeIndentation();
    cout<<"while (";
    walk_expr(condition) ;  //condition part
    cout<<")"<<endl;

    makeIndentation();
    cout<<"{"<<endl;
    indentation++;
    walk_statements(body) ;
    indentation--;
    makeIndentation();
    cout<<"}"<<endl;
}

void walk_do(ast t)
{
    ast call = get_do_call(t) ;

    makeIndentation();
    cout<<"do ";
    walk_call(call) ;
    cout<<" ;"<<endl;
}

void walk_call(ast t)
{
    bool method_call = get_call_method_call(t) ;
    ast expr_list = get_call_expr_list(t) ;

    if(method_call==true)
    {
        callingMethod = true;
        walk_expr(get_expr_list(expr_list,0)); //get classname from syntax tree  //classname.subrname
        cout<<"."<<get_call_subr_name(t)<<"(";
        walk_expr_list(expr_list);
    }
    else if(method_call==false)
    {
        callingMethod = false;
        cout<<get_call_class_name(t)<<"."<<"(";
        walk_expr_list(expr_list);
    }
    cout<<")";
}

void walk_return(ast t)
{
    makeIndentation();
    cout<<"return ;"<<endl;
}


void walk_expr_list(ast t)
{
    int start;
    int nexpressions = size_of_expr_list(t) ;
    if(callingMethod ==true)
    {
        start = 1;
    }else if(callingMethod==false)
    {
        start = 0;
    }

    for ( int i = 0 ; i < nexpressions ; i++ )
    {
        if(i!=start)
        {
            cout<<",";
        }
        walk_expr(get_expr_list(t,i)) ;
    }
}

void walk_expr(ast t)  //given implementation do not change
{
    int term_ops = size_of_expr(t) ;
    for ( int i = 0 ; i < term_ops ; i++ )
    {
        ast term_op = get_expr(t,i) ;
        if ( i % 2 == 0 )
        {
            walk_term(term_op) ;
        }
        else
        {
            walk_op(term_op) ;
        }
    }
}

void walk_term(ast t)
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
        cout<<"(";
        walk_expr(term) ;
        cout<<")";
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
    string op = get_op_op(t) ;
    cout<<" "<<op<<" ";
}

void walk_int(ast t)
{
    int _constant = get_int_constant(t) ;
    cout<<_constant;
}

void walk_string(ast t)
{
    string _constant = get_string_constant(t) ;
    cout<<_constant;
}

void walk_bool(ast t)
{
    bool _constant = get_bool_t_or_f(t) ;
    if(_constant==true)
    {
        cout<<"true";
    }else
    {
        cout<<"false";
    }
    
}

void walk_null(ast t)
{
    cout<<"null";
}

void walk_this(ast t)
{
    cout<<"this";
}

void walk_var(ast t)
{
    string name = get_var_name(t) ;
    cout<<name;
}

void walk_array_index(ast t)
{
    ast var = get_array_index_var(t) ;
    ast index = get_array_index_index(t) ;

    walk_var(var) ;
    cout<<"[";
    walk_expr(index) ;
    cout<<"]";
}

void walk_unary_op(ast t)
{
    ast term = get_unary_op_term(t) ;
    cout<<get_unary_op_op(t);
    walk_term(term) ;
}

// main program
int main(int argc,char **argv)
{
    // walk an AST parsed from XML and pretty print equivalent Jack code
    walk_class(ast_parse_xml()) ;

    // flush the output and any errors
    print_output() ;
    print_errors() ;
}

