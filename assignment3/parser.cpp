#include <string>
#include <vector>
#include "iobuffer.h"
#include "symbols.h"
#include "abstract-syntax-tree.h"

// to shorten our code:
using namespace std ;
using namespace CS_Symbol_Tables ;
using namespace Jack_Compiler ;

// ***** WHAT TO DO *****
//
// MODIFY the skeleton code below to parse a Jack class and construct the equivalent abstract syntax tree, ast.
//        The main function will print the ast formatted as XML
//
// NOTE: the correct segment and offset is recorded with every variable in the ast
//       so the code generator does not need to use any symbol tables
//
// NOTE: use the fatal function below to print fatal error messages with the current
//       input file position marked.
//
// The skeleton code has one function per non-terminal in the Jack grammer
//  - comments describing how to create relevant ast nodes prefix most functions
//  - ast nodes are immutable - you must create all the sub-trees before creating a new node
//
// The structure of the skeleton code is just a suggestion
//  - it matches the grammar but does not quite match the ast structure
//  - you will need to change the parameters / results / functions to suit your own logic
//  - you can change it as much as you like
//
// NOTE: the best place to start is to write code to parse the input without attempting to create
//       any ast nodes. If correct this parser will report errors when it finds syntax errors but
//       correct programs will fail because the tree printer will not be provided with a valid ast.
//       Once the parsing works modify your code to create the ast nodes.
//
// NOTE: the return -1 ; statements are only present so that this skeleton code will compile.
//       these statements need to be replaced by code that creates ast nodes.
//

// fatal error handling
static void fatal(string message)
{
    fatal_error(0,"***** Error:\n" + token_context() + message) ;
}

// The Jack grammar to be recognised:
// program          ::= One or more classes, each class in a separate file named <class_name>'.Jack'
// class            ::= 'class' identifier '{' class_var_decs subr_decs '}'
// class_var_decs   ::= (static_var_dec | field_var_dec)*
// static_var_dec   ::= 'static' type identifier (',' identifier)* ';'
// field_var_dec    ::= 'field' type identifier (',' identifier)* ';'
// type             ::= 'int' | 'char' | 'boolean' | identifier
// vtype            ::= 'void' | type
// subr_decs        ::= (constructor | function | method)*
// constructor      ::= 'constructor' identifier identifier '(' param_list ')' subr_body
// function         ::= 'function' vtype identifier '(' param_list ')' subr_body
// method           ::= 'method' vtype identifier '(' param_list ')' subr_body
// param_list       ::= ((type identifier) (',' type identifier)*)?
// subr_body        ::= '{' var_decs statements '}'
// var_decs         ::= var_dec*
// var_dec          ::= 'var' type identifier (',' identifier)* ';'

// statements       ::= statement*
// statement        ::= let | if | while | do | return
// let              ::= 'let' identifier ('[' expr ']')? '=' expr ';'
// if               ::= 'if' '(' expr ')' '{' statements '}' ('else' '{' statements '}')?
// while            ::= 'while' '(' expr ')' '{' statements '}'
// do               ::= 'do' (identifier '.')? identifier '(' expr_list ')' ';'
// return           ::= 'return' expr? ';'

// expr             ::= term (infix_op term)*
// term             ::= integer_constant | string_constant | 'true' | 'false' | 'null' | 'this' | '(' expr ')' | unary_op term | var_term
// var_term         ::= identifier | identifier '[' expr ']' | subr_call
// subr_call        ::= (identifier '.')? identifier '(' expr_list ')'
// expr_list        ::= (expr (',' expr)*)?
// infix_op         ::= '+' | '-' | '*' | '/' | '&' | '|' | '<' | '>' | '='
// unary_op         ::= '-' | '~'


// forward declarations of one function per non-terminal rule in the Jack grammar
ast parse_class() ;
ast parse_class_var_decs() ;
ast parse_static_var_dec() ;
ast parse_field_var_dec() ;
ast parse_type() ;
ast parse_vtype() ;
ast parse_subr_decs() ;
ast parse_constructor() ;
ast parse_function() ;
ast parse_method() ;
ast parse_param_list() ;
ast parse_subr_body() ;
ast parse_var_decs() ;
ast parse_var_dec() ;

ast parse_statements() ;
ast parse_statement() ;
ast parse_let() ;
ast parse_if() ;
ast parse_while() ;
ast parse_do() ;
ast parse_return() ;

ast parse_expr() ;
ast parse_term() ;
ast parse_var_term() ;
ast parse_subr_call() ;
ast parse_expr_list() ;
ast parse_infix_op() ;
ast parse_unary_op() ;
ast parse_keyword_constant() ;

static bool test;
static int staticCounter;  
static int fieldCounter;   
static int varCounter;     
static string varType;
static string term_tmp;
static bool objMethod;
static bool thisMethod;
static bool decMethod;

//create two symbol tables
static symbols classVariables;
static symbols localVariables;

static string currentClassName; 


static st_variable lookup_var(string key)
{
    st_variable var;
    var = lookup_variables(localVariables, key);

    if(var.name == "")
    {
        var = lookup_variables(classVariables, key);
    }
}

// class ::= 'class' identifier '{' class_var_decs subr_decs '}'   //declerations
// create_class(myclassname,class_var_decs,class_subrs)
ast parse_class()
{
    string className;
    ast decs;
    ast subrs;

    className = create_variables();
    

    mustbe(tk_class); 
    
    className = token_spelling();
    currentClassName = className;
    mustbe(tk_identifier);
    mustbe(tk_lcb);                 
    decs = parse_class_var_decs(); 
    subrs = parse_subr_decs();      
    mustbe(tk_rcb);                
    return create_class(currentClassName,decs,subrs);
}


// class_var_decs ::= (static_var_dec | field_var_dec)*
// create_class_var_decs(vector<ast> decs)
ast parse_class_var_decs()
{
    //create a vector to store declarations
    vector<ast> decs; 
    //set field counter to 0
    fieldCounter = 0; 

    while(have(tk_static) || (have(tk_field)))
    {
        if(have(tk_static))
        {
            while(have(tk_semi) == false)
            {
                decs.push_back(parse_static_var_dec());
            }
            mustbe(tk_semi);
        }
        else if(have(tk_field))
        {
            while(have(tk_semi) == false)
            {
                decs.push_back(parse_field_var_dec());
                mustbe(tk_semi);
            }
        }
    }
    return create_class_var_decs(decs) ;
}

// static_var_dec ::= 'static' type identifier (',' identifier)* ';'
// create_var_dec(string name,string segment,int offset,string type)
ast parse_static_var_dec() //static variable declarations:  static int x (,y,z);
{
    //declare variable
    string name;
    string segment = "static";
    int offset = staticCounter++;

    if(have(tk_static))
    {
        next_token();
        varType = token_spelling();
        mustbe(tk_type);
        name = token_spelling();
        mustbe(tk_identifier);
    }else{
        while(have(tk_static)==false)   
        {
        mustbe(tk_comma);
        name = token_spelling();
        mustbe(tk_identifier);
        }
    }
    
    insert_variables(classVariables, name, st_variable(name, varType, segment, offset));
    return create_var_dec(name, segment, offset, varType);
}

// field_var_dec ::= 'field' type identifier (',' identifier)* ';'
// create_var_dec(string name,string segment,int offset,string type)
ast parse_field_var_dec()
{
    return -1 ;
}

// type ::= 'int' | 'char' | 'boolean' | identifier
ast parse_type()
{
    return -1 ;
}

// vtype ::= 'void' | type
ast parse_vtype()
{
    return -1 ;
}

// subr_decs ::= (constructor | function | method)*
// create_subr_decs(vector<ast> subrs)
// create_subr(ast subr)
ast parse_subr_decs()
{
    vector<ast> subrs;

    while(have(tk_function)||have(tk_constructor)||have(tk_method))
    {
        if(have(tk_function))
        {
            subrs.push_back(create_subr(parse_function()));
        }else if(have(tk_constructor))
        {
            subrs.push_back(create_subr(parse_constructor()));
        }else if(have(tk_method))
        {
            subrs.push_back(create_subr(parse_method()));
        }
    }
    return create_subr_decs(subrs);
}

// constructor ::= 'constructor' identifier identifier '(' param_list ')' subr_body
// create_constructor(string vtype,string name,ast params,ast body)
ast parse_constructor()
{
    string vtype;
    string name;
    ast params;
    ast body;

    localVariables = create_variables();
    decMethod = false;

    mustbe(tk_constructor); 
    vtype = token_spelling();
    mustbe(tk_identifier);
    name = token_spelling();
    mustbe(tk_identifier);
    mustbe(tk_lrb);   
    params = parse_param_list();
    mustbe(tk_rrb);
    body = parse_subr_body();

    return create_constructor(vtype, name, params, body);
}

// function ::= 'function' vtype identifier '(' param_list ')' subr_body
// create_function(string vtype,string name,ast params,ast body)
ast parse_function()
{
    string vtype;
    string name;
    ast params;
    ast body;

    localVariables = create_variables();
    decMethod = false;

    mustbe(tk_function);
    vtype = token_spelling();
    mustbe(tk_vtype);
    name = token_spelling();
    mustbe(tk_identifier);
    mustbe(tk_lrb);
    params = parse_param_list();
    mustbe(tk_rrb);
    body = parse_subr_body();

    return create_constructor(vtype,name, params, body);
}

// method ::= 'method' vtype identifier '(' param_list ')' subr_body
// create_method(string vtype,string name,ast params,ast body)
ast parse_method()
{
    string vtype;
    string name;
    ast params;
    ast body;

    localVariables = create_variables();
    decMethod = true;

    mustbe(tk_method);
    vtype = token_spelling();
    mustbe(tk_vtype);
    name = token_spelling();
    mustbe(tk_identifier);
    mustbe(tk_lrb);
    params = parse_param_list();
    mustbe(tk_rrb);
    body = parse_subr_body();

    return create_constructor(vtype,name, params, body);
}

// param_list ::= ((type identifier) (',' type identifier)*)?  int x
// create_param_list(vector<ast> params)
// create_var_dec(string name,string segment,int offset,string type)
ast parse_param_list()
{
    vector<ast> params;
    string name;
    string segment = "argument";
    string type;
    int offset = 0;

    if(decMethod)
    {
        offset++;
    }

    if(have(tk_type))
    {
        type = token_spelling();
        mustbe(tk_type);
        name = token_spelling();
        mustbe(tk_identifier);

        insert_variables(localVariables, name, st_variable(name, type, segment,offset));

        params.push_back(create_var_dec(name,segment, offset, type));
    }

    while(have(tk_comma))
    {
        next_token();
        offset++;
        type = token_spelling();
        mustbe(tk_type);
        name = token_spelling();
        mustbe(tk_identifier);
        
        insert_variables(localVariables, name, st_variable(name, type,segment,offset));
        params.push_back(create_var_dec(name, segment, offset, type));
    }

    return create_var_dec(name, segment, offset, type);
}

// subr_body ::= '{' var_decs statements '}'  subroutine body
// create_subr_body(ast decs,ast body)
ast parse_subr_body()
{
    ast decs;
    ast body;

    mustbe(tk_lcb);  
    decs = parse_var_decs();  
    body = parse_statement();  
    mustbe(tk_rcb);  

    return create_subr_body(decs,body);
}

// var_decs ::= var_dec*
// create_var_decs(vector<ast> decs)
ast parse_var_decs()
{   
    return -1 ;
}

// var_dec ::= 'var' type identifier (',' identifier)* ';'
// create_var_dec(string name,string segment,int offset,string type)
ast parse_var_dec()
{
    return -1 ;
}

// statements ::= statement*
// create_statements(vector<ast> statements)
ast parse_statements()
{
    return -1 ;
}

// statement ::= let | if | while | do | return
// create_statement(ast statement)
ast parse_statement()
{
    return -1 ;
}

// let ::= 'let' identifier ('[' expr ']')? '=' expr ';'
// create_let(ast var,ast expr)
// create_let_array(ast var,ast index,ast expr)
ast parse_let()
{
    ast index;
    ast expr;
    st_variable temp;
    ast var;

    mustbe(tk_let);  
    temp = lookup_var(token_spelling());
    var = create_var(temp.name, temp.segment, temp.offset, temp.type);
    mustbe(tk_identifier);

    if(have(tk_eq)) 
    {
        mustbe(tk_eq);
        expr = parse_expr();
        mustbe(tk_semi);
        return create_let(var, expr);
    }
    else
    {
        mustbe(tk_lsb);  
        index = parse_expr();
        mustbe(tk_rsb); 
        mustbe(tk_eq);  
        expr = parse_expr();
        mustbe(tk_semi);
    }
    return create_let_array(var, index, expr);
}

// if ::= 'if' '(' expr ')' '{' statements '}' ('else' '{' statements '}')?
// create_if(ast condition,ast if_true)
// create_if_else(ast condition,ast if_true,ast if_false)
ast parse_if()
{
    ast condition;
    ast if_true;
    ast if_false;

    mustbe(tk_if);
    mustbe(tk_lrb);
    condition = parse_expr();
    mustbe(tk_rrb);
    mustbe(tk_lcb);

    if_true = parse_statements();
    mustbe(tk_rcb);

    if(have(tk_else))
    {
        next_token();
        mustbe(tk_lcb);
        if_false = parse_statements();
        mustbe(tk_rcb);
        return create_if_else(condition, if_true, if_false);
    }

    return create_if(condition,if_true);
}

// while ::= 'while' '(' expr ')' '{' statements '}'
// create_while(ast condition,ast body)
ast parse_while()
{
    ast condition;
    ast body;

    mustbe(tk_while);
    mustbe(tk_lrb);
    condition = parse_expr();
    mustbe(tk_rrb);
    mustbe(tk_lcb);

    body = parse_statements();
    mustbe(tk_rcb);

    return create_while(condition, body);
}

// do ::= 'do' (identifier '.')? identifier '(' expr_list ')' ';'
// create_do(ast call)
// create_call(bool method_call,string class_name,string subr_name,ast expr_list)
ast parse_do()
{
    ast call;
    bool method_call;
    string class_name;
    string subr_name;
    ast expr_list;

    objMethod = false;
    thisMethod = false;

    mustbe(tk_do);
    term_tmp = token_spelling();
    next_token;

    if(have(tk_stop))
    {
        st_variable var = lookup_var(term_tmp);

        if(var.name == "")
        {
            next_token();
            method_call = false;
            class_name = term_tmp;
            subr_name = token_spelling();
            mustbe(tk_identifier);
            mustbe(tk_lrb);
            expr_list = parse_expr_list();
            mustbe(tk_rrb);
            mustbe(tk_semi);

            call = create_call(method_call, class_name, subr_name, expr_list);
        }
        else
        {
            next_token();
            method_call = true;
            objMethod = true;
            st_variable obj = lookup_var(term_tmp); 
            class_name = obj.type;
            subr_name = token_spelling();
            mustbe(tk_identifier);
            mustbe(tk_lrb);
            expr_list = parse_expr_list();
            mustbe(tk_rrb);
            mustbe(tk_semi);

            call = create_call(method_call, class_name, subr_name, expr_list);
        }
        
    }else if(have(tk_lrb))
    {
        next_token();
        method_call = true;
        thisMethod = true;
        class_name = currentClassName;
        subr_name = term_tmp;
        expr_list = parse_expr_list();
        mustbe(tk_rrb);
        mustbe(tk_semi);

        call = create_call(method_call, class_name, subr_name, expr_list);
    }

    return create_do(call) ;
}

// return ::= 'return' expr? ';'
// create_return()
// create_return_expr(ast expr)
ast parse_return()
{
    ast expr;
    mustbe(tk_return);

    if(have(tk_semi)==false)
    {
        expr = parse_expr(); 
        mustbe(tk_semi);    
        return create_return_expr(expr);
    }
    else if(have(tk_semi)==true)  
    {
        mustbe(tk_semi);         
        return create_return();  
    }
}

// expr ::= term (infix_op term)*
// create_expr(vector<ast> expr)
ast parse_expr()
{
    vector<ast> expr;
    expr.push_back(parse_term());

    while(have(tk_infix_op))            
    {
        expr.push_back(parse_infix_op());
        next_token();
        expr.push_back(parse_term());
    }
    return create_expr(expr);
}

// term ::= integer_constant | string_constant | 'true' | 'false' | 'null' | 'this' | '(' expr ')' | unary_op term | var_term
// create_term(ast term)
// create_int(int _constant)
// create_string(string _constant)
// create_bool(bool t_or_f)
// create_null()
// create_this()
// create_unary_op(string op,ast term)
ast parse_term()
{
    ast term;

    if(have(tk_true) || have(tk_false))
    {
        term = create_bool(token_spelling()=="true"); 
        next_token();
    }
    else if (have(tk_identifier))
    {
        term_tmp = token_spelling();
        next_token();
        if(have(tk_stop))
        {
            term = parse_subr_call();
        }else if(have(tk_lrb))
        {
            term = parse_subr_call();
        }else if(have(tk_lsb))
        {
            next_token();
        }
        else
        {
            st_variable var = lookup_var(term_tmp);
            term = create_var(var.name, var.segment, var.offset, var.type);
        }
    }else if(have(tk_integerConstant))
    {
        term = create_int(token_ivalue());;

        next_token();
    }else if(have(tk_this))
    {
        term = create_this();
        next_token();
    }else if(have(tk_stringConstant))
    {
        term = create_string(token_spelling());
        next_token();
    }else if(have(tk_lrb))
    {
        next_token();
        term = parse_expr();
        mustbe(tk_rrb);
    }else if(have(tk_null))
    {
        term = create_null();
        next_token();
    }else if(have(tk_unary_op))
    {
        string opr;
        ast tmp;

        opr = token_spelling();
        next_token();
        tmp = parse_term();

        term = create_unary_op(opr,tmp);
    }
    return create_term(term);
}

// var_term ::= identifier | identifier '[' expr ']' | subr_call
// create_var(string name,string segment,int offset,string type)
// create_array_index(ast var,ast index)
ast parse_var_term()
{
    return -1 ;
}

// subr_call ::= (identifier '.')? identifier '(' expr_list ')'
// create_call(bool method_call,string class_name,string subr_name,ast expr_list)
ast parse_subr_call()
{
    bool method_call;
    string class_name;
    string subr_name;
    ast expr_list;
}

// expr_list ::= (expr (',' expr)*)?
// create_expr_list(vector<ast> exprs)
ast parse_expr_list()
{
}

// infix_op ::= '+' | '-' | '*' | '/' | '&' | '|' | '<' | '>' | '='
ast parse_infix_op()
{
    return create_op(token_spelling()) ;
}

// unary_op ::= '-' | '~'
ast parse_unary_op()
{
    return -1 ;
}

ast jack_parser()
{
    staticCounter = 0;
    // read the first token to get the tokeniser initialised
    next_token() ;

    // construct tree and return as result
    return parse_class() ;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// main program
int main(int argc,char **argv)
{
    test = false;
    config_output(iob_buffer);
    config_errors(iob_buffer);

    // parse a Jack class and print the abstract syntax tree as XML
    ast_print_as_xml(jack_parser(),4) ;

    // flush the output and any errors
    print_output() ;
    print_errors() ;
}