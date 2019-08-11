%{
#include <stdio.h>
#include <string>
#include <iostream>
extern "C" {	
	void yyerror (char *);
}
extern int yylex(void);
extern int yylineno;

Table_Scope current = global;
ostream & os = cout;
list<Symbol_Table_Entry *>* global_table;
list<Symbol_Table_Entry *>* local_table;
list<Symbol_Table_Entry *>* formal_table;
string current_proc = "";
list<Procedure *> * procedures = new list<Procedure*>();
%}

%union YYSTYPE
{
	int integer_value;
	double double_value;
	std::string * string_value;
	list<Ast *> * ast_list;
	Ast * ast;
	Symbol_Table * symbol_table;
	Symbol_Table_Entry * symbol_entry;
	Basic_Block * basic_block;
	Procedure * procedure;
	map<string, Procedure *>* procedure_map;
	Program * program;
	Data_Type * data_type;
	vector<string> * vec_string;
	list<Symbol_Table_Entry*> * list_sym;
	Sequence_Ast * seq_ast;
	Selection_Statement_Ast * sel_ast;
	Return_Ast * ret_ast;
	Call_Ast * call_ast;
}

%token INTEGER_NUMBER BBNUM INTEGER DOUBLE_NUMBER NAME RETURN FLOAT ASSIGN VOID UMINUS WHILE IF ELSE DO GREATER_THAN GREATER_THAN_EQUAL LESS_THAN LESS_THAN_EQUAL EQUAL NOT_EQUAL AND OR NOT PRINT

%right ASSIGN
%right '?' ':'
%left GREATER_THAN GREATER_THAN_EQUAL LESS_THAN LESS_THAN_EQUAL EQUAL NOT_EQUAL
%left '+' '-' 
%left '*' '/'
%left AND OR
%right UMINUS
%nonassoc IFST
%nonassoc ELSE
%nonassoc NOT

%type <integer_value> INTEGER_NUMBER
%type <double_value> DOUBLE_NUMBER
%type <string_value> NAME
%type <procedure> Proc Proc_start func_decl 
%type <procedure_map> Proclist
%type <ast_list> ProcBody actual_params actual_params_h
%type <ast> AssignExp Variable Constant E iteration_stmt conditional_expr rel_expr logical_expr sequence print_stmt func_call return_stmt
%type <sel_ast> selection_stmt
%type <symbol_table> Global
%type <vec_string> L
%type <list_sym > D func_ops func_ops_h
%type <data_type> T
%type <program> Prog
%type <string_value> NAME1
%type <symbol_entry> single_decl;

%%
Prog : Global Proclist {
		$$ = new Program();
		$1->set_table_scope(global);
		$$->set_global_table(*$1);
		$$->set_procedure_map(*$2);
		program_object = *$$;
	};
Global : {$$ = new Symbol_Table();
		if(current == global){
			global_table = new list<Symbol_Table_Entry *>();
			$$->set_table_scope(global);
		}
		else {
			local_table = new list<Symbol_Table_Entry *>();	
			$$->set_table_scope(local);
		}
       } 
	| Global D {
		$$ = $1;
		for(list<Symbol_Table_Entry*>::iterator it = $2->begin(); it != $2->end() ; ++it){
			(*it)->set_symbol_scope($1->get_table_scope());
			$$->push_symbol(*it);
			if (current == global){
				global_table->push_back(*it);
			}
			else {
				local_table->push_back(*it);
			}
		}
		//cout<<"D"<<endl;
	}
	| Global func_decl {
		if(current == local){
			char numstr[100];
			sprintf(numstr,"cs316: Error : Error in line %d Procedures cannot be declared inside other procedures ",yylineno);
			yyerror(numstr);
			exit(0);			
		}
		$$ = $1;
		//cout<<"func_decl"<<endl;
	}
	;

Proclist : Proc {
		string name = $1->get_proc_name();
		$$ = new map<string, Procedure* >();
		(*$$)[name] = $1;
	}
	| Proclist Proc {
		$$ = $1;
		string name = $2->get_proc_name();
		(*$$)[name] = $2;
	}
	;

Proc_start :  T NAME1 '(' func_ops ')' {
		//cout<<"proc "<<*$2<<endl;
		int found = 0;
		for(list<Procedure*>::iterator it = procedures->begin(); it != procedures->end() ; ++it){
			string var_name = (*it)->get_proc_name();
			if(var_name.compare(*$2) == 0) { found = 1; $$ = *it;}
		}
		//cout<<"found "<<found<<endl;
		if(found == 0){
			$$ = new Procedure(*$1,*$2,yylineno);
			
			//cout<<"not declared before "<<(*$2)<<endl;
			formal_table = new list<Symbol_Table_Entry*>();
			Symbol_Table * sym_tab = new Symbol_Table();
			for(list<Symbol_Table_Entry*>::iterator it = $4->begin(); it != $4->end() ; ++it){
				(*it)->set_symbol_scope(formal);
				sym_tab->push_symbol(*it);
				formal_table->push_back(*it);
			}
			$$->set_formal_param_list(*sym_tab);
			$$->set_return_type(*$1);

			procedures->push_back($$);
		}
		if(found == 1){
			if($$->is_proc_defined()){
				char numstr[100];
				sprintf(numstr,"cs316: Error : Error in line : %d Procedure with this name is already defined ",yylineno);
				yyerror(numstr);
				exit(0);
			}
			if($$->get_return_type() != *$1){
				char numstr[100];
				sprintf(numstr,"cs316: Error : Error in line : %d return type in declaration and definition not matching",yylineno);
				yyerror(numstr);
				exit(0);
			}
			//cout<<"declared before"<<endl;
			formal_table = new list<Symbol_Table_Entry*>();
			Symbol_Table * sym_tab = new Symbol_Table();
			for(list<Symbol_Table_Entry*>::iterator it = $4->begin(); it != $4->end() ; ++it){
				//cout<<"sym "<<(*it)->get_variable_name()<<endl;
				(*it)->set_symbol_scope(formal);
				sym_tab->push_symbol(*it);
				formal_table->push_back(*it);
			}
			$$->set_formal_param_list(*sym_tab);
			$$->set_return_type(*$1);

		}
		current = local;
		current_proc = *$2;
		//cout<<"proc_end"<<endl;
	}
	| T NAME1 '(' T type_list ')' {
		$$ = new Procedure(*$1,*$2,yylineno);
		procedures->push_back($$);
	}
	;

func_decl : Proc_start ';' {
		$$ = $1;
		current = global;
		current_proc = "";
	}
	;
type_list : {}
	| type_list ',' T {} 
	;
func_ops : {
		$$ = new list<Symbol_Table_Entry*>();
	}
	| single_decl func_ops_h {
		$$ = $2;
		$$->push_front($1);	
	}
	;
func_ops_h : {$$ = new list<Symbol_Table_Entry*>();}
	| func_ops_h ',' single_decl {
		$$ = $1;
		$$->push_back($3);
	}
	;

Proc : Proc_start '{' Global ProcBody '}' {
		string proc_name = $1->get_proc_name();

		$$ = $1;
		$$->set_ast_list(*$4);
		$3->set_table_scope(local);
		$$->set_local_list(*$3);
		$$->set_proc_is_defined();
	}
	;
ProcBody : {$$ = new list<Ast*>();} 
	| ProcBody sequence {
		$$ = $1;
		$$->push_back($2);
		//cout<<"procbody"<<endl;
	}
	;

print_stmt: PRINT Variable ';'{
		$$=new Print_Ast($2, yylineno);
	}
	;
return_stmt: RETURN E ';'{
		$$ = new Return_Ast($2, current_proc , yylineno);
	}
	| RETURN ';' {
		$$ = new Return_Ast(NULL, current_proc, yylineno);	
	}
	;
func_call : NAME1 '(' actual_params ')'  {
		Call_Ast* temp = new Call_Ast(*$1,yylineno);
		// Check if func is defined and get return type
		Data_Type ret_type; 
		int found = 0;

		for(list<Procedure*>::iterator it = procedures->begin(); it != procedures->end() ; ++it){
			string var_name = (*it)->get_proc_name();
			if(var_name.compare(*$1) == 0) { found = 1; ret_type = (*it)->get_return_type();}
		}
		if(found == 0){
			char numstr[100];
			sprintf(numstr,"cs316: Error : semantic error in line %d : Procedure is not defined ", yylineno);
			yyerror(numstr);
			exit(0);
		}

		temp->set_actual_param_list(*$3);
		temp->set_data_type(ret_type);
		$$ = temp;
	}
	;
sequence : //{$$ = new Sequence_Ast(yylineno);} |
	 '{' ProcBody '}' {
		if ($2->empty()){
			char numstr[100];
			sprintf(numstr,"cs316: Error : syntax error in line %d : Block of statements cannot be empty ", yylineno);
			yyerror(numstr);
			exit(0);
		}
		else {
			Sequence_Ast* temp = new Sequence_Ast(yylineno);//check this!!!!!!
			for (list<Ast*>::iterator it = $2->begin(); it != $2->end() ; ++it){
				temp->ast_push_back(*it);
			}
			$$ = temp;
		}
	}
	| AssignExp {
		$$ = $1;
	}
	| selection_stmt {
		$$ = $1;
	}
	| iteration_stmt {
		$$ = $1;
	}
	| print_stmt {
		$$ = $1;
	}
	| func_call ';' {
		$$ = $1;
	}
	| return_stmt {
		$$ = $1;	
	}
	;

selection_stmt : IF '(' logical_expr ')' sequence ELSE sequence {
		$$ = new Selection_Statement_Ast($3,$5,$7,yylineno);
	}
	| IF '(' logical_expr ')' sequence %prec IFST {
		$$ = new Selection_Statement_Ast($3,$5,NULL,yylineno);
	}
	;

iteration_stmt : WHILE '(' logical_expr ')' sequence {
		$$ = new Iteration_Statement_Ast($3, $5, yylineno, false);
	}
	| DO sequence WHILE '(' logical_expr ')' ';'{
		$$ = new Iteration_Statement_Ast($5, $2, yylineno, true);
	}
	;

conditional_expr : logical_expr '?' E ':' E {
		if ($3->get_data_type() != $5->get_data_type()){
			char numstr[100];
			sprintf(numstr,"cs316: Error : syntax error in line %d :  Conditional statement data type not compatible", yylineno);
			yyerror(numstr);
			exit(0);
		}
		$$ = new Conditional_Expression_Ast($1,$3,$5,yylineno);	
		$$->set_data_type($3->get_data_type());
	}
	;

rel_expr : E GREATER_THAN E {
		if($1->get_data_type() != $3->get_data_type()){
			char numstr[100];
			sprintf(numstr,"cs316: Error : syntax error in line %d : Relational statement data type not compatible", yylineno);
			yyerror(numstr);
			exit(0);
		}
		else {
			$$ = new Relational_Expr_Ast($1,greater_than,$3,yylineno);
		}
	}
	| E GREATER_THAN_EQUAL E {
		if($1->get_data_type() != $3->get_data_type()){
			char numstr[100];
			sprintf(numstr,"cs316: Error : syntax error in line %d : Relational statement data type not compatible", yylineno);
			yyerror(numstr);
			exit(0);
		}
		else {
			$$ = new Relational_Expr_Ast($1,greater_equalto,$3,yylineno);
		}
		
	}
	| E LESS_THAN E {
		if($1->get_data_type() != $3->get_data_type()){
			char numstr[100];
			sprintf(numstr,"cs316: Error : syntax error in line %d : Relational statement data type not compatible", yylineno);
			yyerror(numstr);
			exit(0);
		}
		else {
			$$ = new Relational_Expr_Ast($1,less_than,$3,yylineno);
		}
		
	}
	| E LESS_THAN_EQUAL E {
		if($1->get_data_type() != $3->get_data_type()){
			char numstr[100];
			sprintf(numstr,"cs316: Error : syntax error in line %d : Relational statement data type not compatible", yylineno);
			yyerror(numstr);
			exit(0);
		}
		else {
			$$ = new Relational_Expr_Ast($1,less_equalto,$3,yylineno);
		}
		
	}
	| E EQUAL E {
		if($1->get_data_type() != $3->get_data_type()){
			char numstr[100];
			sprintf(numstr,"cs316: Error : syntax error in line %d : Relational statement data type not compatible", yylineno);
			yyerror(numstr);
			exit(0);
		}
		else {
			$$ = new Relational_Expr_Ast($1,equalto,$3,yylineno);
		}
		
	}
	| E NOT_EQUAL E {
		if($1->get_data_type() != $3->get_data_type()){
			char numstr[100];
			sprintf(numstr,"cs316: Error : syntax error in line %d : Relational statement data type not compatible", yylineno);
			yyerror(numstr);
			exit(0);
		}
		else {
			$$ = new Relational_Expr_Ast($1,not_equalto,$3,yylineno);
		}
		
	}

	;
logical_expr : rel_expr {
		$$ = $1;
	}
	| '(' logical_expr ')' {
		$$ = $2;
	} 
	| logical_expr AND logical_expr {
		$$ = new Logical_Expr_Ast($1,_logical_and,$3,yylineno);
	}
	| logical_expr OR logical_expr {
		$$ = new Logical_Expr_Ast($1,_logical_or,$3,yylineno);
	}
	| NOT logical_expr {
		$$ = new Logical_Expr_Ast(NULL,_logical_not,$2,yylineno);
	}
	;


D : T L ';'{
		$$ = new list<Symbol_Table_Entry*>();
		for(int i = 0; i < $2->size(); i++){
			Symbol_Table_Entry * entry = new Symbol_Table_Entry((*$2)[i],*$1,yylineno);
			entry->set_data_type(*$1);
			$$->push_back(entry);
		}
	}
	| single_decl ';' {
		$$ = new list<Symbol_Table_Entry*>();
		$$->push_back($1);
	}
	;

actual_params : {
		$$ = new list<Ast*>();
	}
	| actual_params_h E {
		$$ = $1;
		$$->push_back($2);	
	}
	;
actual_params_h : {$$ = new list<Ast*>();}
	| actual_params_h E ',' {
		$$ = $1;
		$$->push_back($2);
	}
	;
single_decl : T NAME1 {
		$$ = new Symbol_Table_Entry(*$2,*$1,yylineno);
		$$->set_data_type(*$1);
	}
	;

T : INTEGER {
		$$ = new Data_Type;
		*$$ = int_data_type;
	} 
	| FLOAT {
		$$ = new Data_Type;
		*$$ = double_data_type;
	} 
	| VOID {
		$$ = new Data_Type;		
		*$$ = void_data_type;
	} ;

NAME1 : NAME {
		string * name_str = $1;
		if ((*name_str).compare("main") != 0) (*name_str).append("_");
		$$ = name_str;
	};

L : NAME1 ',' NAME1 {	
		$$ = new vector<string>();
		$$->push_back(*$1);
		$$->push_back(*$3);
	} 
	| L ',' NAME1 {
		$$ = $1;
		$$->push_back(*$3); 
	} ;

Variable : NAME1 {		
		
		Symbol_Table_Entry * entry;
		int found = 0; 
		for(list<Symbol_Table_Entry*>::iterator it = local_table->begin(); it != local_table->end() ; ++it){
			string var_name = (*it)->get_variable_name();
			if(var_name.compare(*$1) == 0) { found = 1; entry = *it;}	
		}
		if (found == 0){
			for(list<Symbol_Table_Entry*>::iterator it = formal_table->begin(); it != formal_table->end() ; ++it){
				string var_name = (*it)->get_variable_name();
				if(var_name.compare(*$1) == 0) { found = 1; entry = *it;}	
			}
		}
		if (found == 0){
			for(list<Symbol_Table_Entry*>::iterator it = global_table->begin(); it != global_table->end() ; ++it){
				string var_name = (*it)->get_variable_name();
				if(var_name.compare(*$1) == 0) { found = 1; entry = *it;}	
			}
		}
		
		if(found == 1) {
			$$ = new Name_Ast(*$1,*entry,yylineno);
			$$->set_data_type(entry->get_data_type());
		}
		else {
			char numstr[100];
			sprintf(numstr,"cs316: Error : syntax error in line %d : Variable has not been declared", yylineno);
			yyerror(numstr);
			exit(0);
		}
	};
Constant : INTEGER_NUMBER {
		$$ = new Number_Ast<int>($1,int_data_type,yylineno);
	} 
	| DOUBLE_NUMBER {
		$$ = new Number_Ast<double>($1,double_data_type,yylineno);	
	} ;
AssignExp : Variable ASSIGN E ';' {
		if($1->get_data_type() != $3->get_data_type()){
			char numstr[100];
			sprintf(numstr,"cs316: Error : syntax error in line %d : Assignment type mismatch", yylineno);
			yyerror(numstr);
			exit(0);
		}
		else {
			$$ = new Assignment_Ast($1,$3,yylineno);
		}
	} 
	| Variable ASSIGN func_call ';'{
		if($1->get_data_type() != $3->get_data_type()){
			char numstr[100];
			sprintf(numstr,"cs316: Error : syntax error in line %d : Assignment type mismatch", yylineno);
			yyerror(numstr);
			exit(0);
		}
		else {
			$$ = new Assignment_Ast($1,$3,yylineno);
		}
	}
	;
E : Constant {
		$$ = $1;
	}
	| Variable {
		$$ = $1;
	}
    	| conditional_expr{
		$$=$1;
    	}
	| '(' E ')' {
		$$ = $2;
	}
	| E '+' E {
		$$ = new Plus_Ast($1,$3,yylineno);
		Data_Type dt1 = $1->get_data_type();
		Data_Type dt2 = $3->get_data_type();
		if(dt1 == dt2){
			$$->set_data_type(dt1);	
		}
		else {
			char numstr[100];
			sprintf(numstr,"cs316: Error : syntax error in line %d : Arithmetic type mismatch", yylineno);
			yyerror(numstr);
			exit(0);		
		}
	}
	| E '-' E {
		$$ = new Minus_Ast($1,$3,yylineno);
		Data_Type dt1 = $1->get_data_type();
		Data_Type dt2 = $3->get_data_type();
		if(dt1 == dt2){
			$$->set_data_type(dt1);	
		}
		else {
			char numstr[100];
			sprintf(numstr,"cs316: Error : syntax error in line %d : Arithmetic type mismatch", yylineno);
			yyerror(numstr);
			exit(0);		
		}
	}
	| E '/' E {
		$$ = new Divide_Ast($1,$3,yylineno);
		Data_Type dt1 = $1->get_data_type();
		Data_Type dt2 = $3->get_data_type();
		if(dt1 == dt2){
			$$->set_data_type(dt1);	
		}	
		else {
			char numstr[100];
			sprintf(numstr,"cs316: Error: syntax error in line %d : Arithmetic type mismatch", yylineno);
			yyerror(numstr);
			exit(0);	
		}	
	}
	| E '*' E {
		$$ = new Mult_Ast($1,$3,yylineno);
		Data_Type dt1 = $1->get_data_type();
		Data_Type dt2 = $3->get_data_type();
		if(dt1 == dt2){
			$$->set_data_type(dt1);	
		}
		else {
			char numstr[100];
			sprintf(numstr,"cs316: Error : syntax error in line %d : Arithmetic type mismatch", yylineno);
			yyerror(numstr);
			exit(0);
		}
	}
	| '-' E %prec UMINUS {
		Data_Type dt = $2->get_data_type();
		$$ = new UMinus_Ast($2,NULL,yylineno);
		$$->set_data_type(dt);
	} 
	;

 
