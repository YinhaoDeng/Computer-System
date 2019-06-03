#include "ast.h"
#include "symbols.h"

tokeniser t;
Token current;
void syntax_error() {
    Token peek();
    exit(0);
}


class variable : public symbol_value {
 public:
    string name, type, segment;
    int offset;
    jn_var_dec create() const {
        return jn_var_dec_create(segment, name, offset, type);
    }
    jn_var getvar() const {
        if (name == "this") {
            return to_jn_var(jn_this_create());
        }
        return jn_var_create(segment, name, offset, type);
    }
    jn_this get_this() const {
          if (name == "this") {
            return jn_this_create();
        }
    }
};

//       symbol_table_values mytable = create_symbol_table_values() ;
symbol_table_values static_or_field = create_symbol_table_values();
symbol_table_values args   = create_symbol_table_values();
symbol_table_values locals = create_symbol_table_values();
int local_offset, static_offset, field_offset;
string className;

variable* find_variable(string name) {
    if (static_or_field->lookup(name)) {
        return (variable *)static_or_field->lookup(name);
    }
    if (locals->lookup(name)) {
        return (variable *)locals->lookup(name);
    }
    syntax_error();
}
Token &peek() {
    return current;
}

Token poll() {
    Token ret = current;
    current = t->next_token();
    return ret;
}



bool keyword(std::string key) {
    if (peek().kind >= jk_keyword &&
        peek().kind  < jk_symbol &&
        peek().spelling == key) {
            return true;
        }
}

bool keyword() {
    if (peek().kind >= jk_keyword &&
        peek().kind  < jk_symbol) {
            return true;
        }
}

bool symbol(std::string key) {
    if (peek().kind >= jk_symbol &&
        peek().kind  < jk_eoi    &&
        peek().spelling == key) {
            return true;
        }
}

bool symbol() {
    if (peek().kind >= jk_symbol &&
        peek().kind  < jk_eoi) {
            return true;
        }
}

bool ll_kind(TokenKind tk) {
    return peek().kind == tk;
}

bool intConst() {
    return ll_kind(jk_integerConstant);
}

bool strConst() {
    return ll_kind(jk_stringConstant);
}

bool ident() {
    return ll_kind(jk_identifier);
}


Token expect(bool val) {
    if (val) {
        return poll();
    } else {
        syntax_error();
    }
}


bool a_type() {
    return (keyword("int")
    || keyword("char")
    || keyword("boolean")
    || ident());
}

string parse_type() {
    if (a_type()) {
        return poll().spelling;
    }
    syntax_error();
}

string parse_vtype() {
    if (keyword("void") || a_type()) {
        return poll().spelling;
    }
    syntax_error();
}

string get_name() {
    return expect(ident()).spelling;
}

jn_var_dec parse_var_dec() {
    string t = parse_type();
    string n = get_name();
    variable *x = new variable;
    x->name = n;
    x->segment = "argument";
    x->offset = local_offset++;
    x->type = t;
    if (!locals->insert(x->name, x)) {
        syntax_error();
    }
    return x->create();
}

jn_param_list parse_param_list() {
    jn_param_list r = jn_param_list_create();
    if (a_type()) {
        r->append(parse_var_dec());
        while (symbol(",")) {
            poll();
            r->append(parse_var_dec());
        }
    }
    return r;
}

jn_var_decs parse_var_decs() {
    jn_var_decs r = jn_var_decs_create();
    int offset = 0;
    while (true) {
        if (keyword("var")) {
            string segment = poll().spelling;
            string type = parse_type();
            string varname = get_name();
            variable *x = new variable;
            x->name = varname;
            x->segment = "local";
            x->offset = local_offset++;
            x->type = type;
            if (!locals->insert(x->name, x)) {
                syntax_error();
            }
            r->append(x->create());
            while (symbol(",")) {
                poll();
                string varname = get_name();
                variable *x = new variable;
                x->name = varname;
                x->segment = "local";
                x->offset = local_offset++;
                x->type = type;
                if (!locals->insert(x->name, x)) {
                    syntax_error();
                }
                r->append(x->create());
            }
            expect(symbol(";"));
        } else {
            break;
        }
    }
    return r;
}

bool an_infix() {
    static const string op = "+-*/&|<>=";
    return symbol() && op.find(peek().spelling) != string::npos;
}

jn_expr parse_expression();
jn_expr_list parse_expr_list() {
    jn_expr_list r = jn_expr_list_create();
    if (symbol(")")) {
        return r;
    }
    jn_expr e = parse_expression();
    r->append(e);
    while (symbol(",")) {
        poll();
        jn_expr e = parse_expression();
        r->append(e);
    }
    return r;
}

jn_call parse_call(string name = get_name()) {
    if (symbol(".")) {
        poll();
        string name2 = get_name();
        expect(symbol("("));
        jn_expr_list list = parse_expr_list();
        expect(symbol(")"));
        if (isupper(name[0])) {
            return jn_call_create(false, name, name2, list);
        } else {
            jn_expr_list r1 = jn_expr_list_create();
            variable *var = find_variable(name);
            r1->append(to_jn_expr(var->getvar()));
            for (int i = 0; i < list->size(); ++i) {
                r1->append(list->get(i));
            }
            return jn_call_create(true, var->type, name2, r1);
        }
    }
    if (symbol("(")) {
        poll();
        jn_expr_list list = parse_expr_list();
        expect(symbol(")"));
        jn_expr_list r1 = jn_expr_list_create();
        r1->append(to_jn_expr(jn_this_create()));
        for (int i = 0; i < list->size(); ++i) {
            r1->append(list->get(i));
        }
        return jn_call_create(true, className, name, r1);
    }
}

jn_expr parse_term() {
    if (intConst()) {
        return to_jn_expr(jn_int_create(poll().ivalue));
    }

    if (strConst()) {
        return to_jn_expr(jn_string_create(poll().spelling));
    }

    if (symbol("(")) {
        poll();
        jn_expr e = parse_expression();
        expect(symbol(")"));
        return to_jn_expr(e);
    }

    if (symbol("~") || symbol("-")) {
        string op = poll().spelling;
        ast term = parse_term();
        return to_jn_expr(jn_unary_op_create(op, to_jn_expr(term)));
    }
    if (keyword("true")) {
        poll();
        return to_jn_expr(jn_bool_create(true));
    }
    if (keyword("false")) {
        poll();
        return to_jn_expr(jn_bool_create(false));
    }
    if (keyword("null")) {
        poll();
        return to_jn_expr(jn_null_create());
    }

    if (keyword("this")) {
        poll();
        return to_jn_expr(jn_this_create());
    }
    if (ident()) {
        string name = get_name();
        if (symbol("[")) {
            poll();
            jn_expr index = parse_expression();
            expect(symbol("]"));
            return to_jn_expr(jn_array_index_create(
                    find_variable(name)->getvar(), index));
        }
        if (symbol(".") || symbol("(")) {
            return to_jn_expr(parse_call(name));
        }
        return to_jn_expr(find_variable(name)->getvar());
    }
    syntax_error();
}

jn_expr parse_expression() {
    jn_expr term = to_jn_expr(parse_term());
        
    while (an_infix()) {
        string op = poll().spelling;
        jn_expr term2 = to_jn_expr(parse_term());
        term = to_jn_expr(jn_infix_op_create(term, op, term2));
    }
    return term;
}

jn_statements parse_statements();

jn_statement parse_let() {
    expect(keyword("let"));
    string name = get_name();
    if (symbol("[")) {
        poll();
        jn_expr index = parse_expression();
        expect(symbol("]"));
        expect(symbol("="));
        jn_expr expr = parse_expression();
        expect(symbol(";"));
        return to_jn_statement(jn_let_array_create(
            find_variable(name)->getvar(), index, expr));
    } else {
        expect(symbol("="));
        jn_expr expr = parse_expression();
        expect(symbol(";"));
        return to_jn_statement(jn_let_create(
            find_variable(name)->getvar(), expr));
    }
}

jn_statement parse_if() {
    expect(keyword("if"));
    expect(symbol("("));
    jn_expr expr = parse_expression();
    expect(symbol(")"));
    expect(symbol("{"));
    jn_statements st = parse_statements();
    expect(symbol("}"));
    if (keyword("else")) {
        poll();
        expect(symbol("{"));
        jn_statements st2 = parse_statements();
        expect(symbol("}")); 
        return to_jn_statement(jn_if_else_create(expr, st, st2));
    } else {
        return to_jn_statement(jn_if_create(expr, st));
    }
}

jn_statement parse_while() {
    expect(keyword("while"));
    expect(symbol("("));
    jn_expr expr = parse_expression();
    expect(symbol(")"));
    expect(symbol("{"));
    jn_statements st = parse_statements();
    expect(symbol("}"));
    return to_jn_statement(jn_while_create(expr, st));
}

jn_statement parse_do() {
    expect(keyword("do"));
    jn_call call = parse_call();
    expect(symbol(";"));
    
    return to_jn_statement(jn_do_create(call));
}

jn_statement parse_return() {
    expect(keyword("return"));
    if (!symbol(";")) {
        jn_expr expr = parse_expression();
        expect(symbol(";"));
        return to_jn_statement(jn_return_expr_create(expr));
    } else {
        poll();
        return to_jn_statement(jn_return_create());
    }
}
jn_statements parse_statements() {
    jn_statements st = jn_statements_create();
    while (keyword("let")
       || keyword("if")
       || keyword("while")
       || keyword("do")
       || keyword("return")) {
           if (keyword("let")) {
               st->append(parse_let());
           } else if (keyword("if")) {
               st->append(parse_if());
           } else if (keyword("while")) {
               st->append(parse_while());
           } else if (keyword("do")) {
               st->append(parse_do());
           } else if (keyword("return")) {
               st->append(parse_return());
           }
       }
    return st;
}


jn_subr_body parse_subr_body() {
    expect(symbol("{"));
    jn_var_decs   d = parse_var_decs();
    jn_statements s = parse_statements();
    expect(symbol("}"));
    return jn_subr_body_create(d, s);
}

jn_constructor parse_ctor() {
    expect(keyword("constructor"));
    string name = get_name();
    string fname = get_name();
    expect(symbol("("));
    delete locals;
    locals = create_symbol_table_values();
    local_offset = 0;
    jn_param_list params = parse_param_list();
    local_offset = 0;
    expect(symbol(")"));
    jn_subr_body body = parse_subr_body();
    return jn_constructor_create(name, fname, params, body);
}

jn_function parse_function() {
    expect(keyword("function"));
    string name = parse_vtype();
    string fname = get_name();
    expect(symbol("("));
    delete locals;
    locals = create_symbol_table_values();
    local_offset = 0;
    jn_param_list params = parse_param_list();
    local_offset = 0;
    expect(symbol(")"));
    jn_subr_body body = parse_subr_body();
    return jn_function_create(name, fname, params, body);
}

jn_method parse_method() {
    expect(keyword("method"));
    string name = parse_vtype();
    string fname = get_name();
    expect(symbol("("));
    delete locals;
    locals = create_symbol_table_values();
    local_offset = 1;
    jn_param_list params = parse_param_list();
    variable *x = new variable;
    x->name = "this";
    x->type = className;
    x->offset = 0;
    x->segment = "argument";
    locals->insert("this", x);
    expect(symbol(")"));
    
    local_offset = 0;
    jn_subr_body body = parse_subr_body();
    return jn_method_create(name, fname, params, body);
}
jn_subr_decs parse_subroutineDecs() {
    jn_subr_decs r = jn_subr_decs_create();
    while (true) {
        if (keyword("constructor")) {
            r->append(parse_ctor());
        } else if (keyword("function")) {
            r->append(parse_function());
        } else if (keyword("method")) {
            r->append(parse_method());
        } else {
            break;
        }     
    }
        
    return r;
}

jn_var_decs parse_classVarDecs() {
    jn_var_decs r = jn_var_decs_create();
    
    while (true) {
        
        if (keyword("static") || keyword("field")) {
            int *offset = keyword("static") ? &static_offset : &field_offset;
            string segment = poll().spelling;
            string type = parse_type();
            string varname = get_name();
            segment = segment == "static" ? "static" : "this";
            variable *x = new variable;
            x->name = varname;
            x->offset = (*offset)++;
            x->segment = segment;
            x->type = type;
            if (!static_or_field->insert(x->name, x)) {
                syntax_error();
            }
            r->append(x->create());
            while (symbol(",")) {
                poll();
                string varname = get_name();
                variable *x = new variable;
                x->name = varname;
                x->offset = (*offset)++;
                x->segment = segment;
                x->type = type;
                if (!static_or_field->insert(x->name, x)) {
                   syntax_error();
                }
                r->append(x->create());
            }
            expect(symbol(";"));
        } else {
            break;
        }
    }
    return r;
}

jn_class parse_class() {
    expect(keyword("class"));
    string name = expect(ident()).spelling;
    className = name;
    expect(symbol("{"));
    jn_var_decs vars = parse_classVarDecs();
    jn_subr_decs subs = parse_subroutineDecs();
    expect(symbol("}"));
    return jn_class_create(name, vars, subs);
}

jn_class jack_parser() {
    t = j_tokeniser();
    current = t->next_token();
    jn_class j = parse_class();
    expect(peek().kind == jk_eoi);
    return j;
}

int main(int argc,char **argv)
{
    // parse a class and print the abstract syntax tree as XML
    jn_print_as_xml(jack_parser(),4) ;

    // flush the output and any errors
    print_output() ;
    print_errors() ;
}

