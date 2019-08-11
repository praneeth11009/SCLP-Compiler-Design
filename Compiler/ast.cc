#include<iostream>
using namespace std;
template class Number_Ast<double>;
template class Number_Ast<int>;
int  Ast::labelCounter;

//// PROC ///////
void Procedure::set_return_type(Data_Type ret_type) {
	return_type = ret_type;
}

///// AST /////////////////

Ast::Ast(){}
Ast::~Ast(){}
Data_Type Ast::get_data_type(){
	return node_data_type;
}
void Ast::set_data_type(Data_Type dt){
	node_data_type = dt;
}

bool Ast::is_value_zero(){
	return false;
}

bool Ast::check_ast(){
	return true;
}
Symbol_Table_Entry & Ast::get_symbol_entry(){
	Symbol_Table_Entry * temp = NULL;
	return *temp;
}

void Ast::print(ostream & file_buffer){
	std::cout<<"in ast print\n";
}

//////  NUMBER AST //////////////////////
template <class T>
Number_Ast<T>::Number_Ast(T number, Data_Type constant_data_type, int line){
	constant = number;
	lineno = line;
	node_data_type = constant_data_type;
	ast_num_child = zero_arity;
}

template<class T>
Number_Ast<T>::~Number_Ast(){}

template<class T>
void Number_Ast<T>::print(ostream & file_buffer){
	file_buffer<<"Num : "<<constant;
}

template<class T>
Data_Type Number_Ast<T>::get_data_type(){
	return node_data_type;
}

template<class T>
void Number_Ast<T>::set_data_type(Data_Type dt){
	node_data_type = dt;
}

template<class T>
bool Number_Ast<T>::is_value_zero(){
	if(constant == 0) return true;
	else return false;
}

////// NAME AST ///////////////////////////////

Name_Ast::Name_Ast(string & name, Symbol_Table_Entry & var_entry, int line){
	variable_symbol_entry = & var_entry;
	lineno = line;
	ast_num_child = zero_arity;
	node_data_type = variable_symbol_entry->get_data_type();
}

Name_Ast::~Name_Ast(){
	delete variable_symbol_entry;
}

Data_Type Name_Ast::get_data_type(){
	return node_data_type;
}
Symbol_Table_Entry & Name_Ast::get_symbol_entry(){
	return * variable_symbol_entry;
}
void Name_Ast::set_data_type(Data_Type dt){
	node_data_type = dt;
}

void Name_Ast::print(ostream & file_buffer){
	file_buffer<<"Name : "<<variable_symbol_entry->get_variable_name();
}

////// ASSIGNMENT AST //////////////////////////////

Assignment_Ast::Assignment_Ast(Ast * temp_lhs, Ast * temp_rhs, int line){
	lineno = line;
	lhs = temp_lhs;
	rhs = temp_rhs;
	node_data_type = temp_rhs->get_data_type();
	ast_num_child = binary_arity;
}

Assignment_Ast::~Assignment_Ast(){
	delete lhs;
	delete rhs;
}

bool Assignment_Ast::check_ast(){
	if(rhs->get_data_type() == lhs->get_data_type()) return true;
	else return false;
}

void Assignment_Ast::print(ostream & file_buffer){
	file_buffer<<"\n"<<AST_SPACE<<"Asgn:"<<endl;
	file_buffer<<AST_NODE_SPACE<<"LHS (";
	lhs->print(file_buffer);
	file_buffer<<")\n";
	file_buffer<<AST_NODE_SPACE<<"RHS (";
	rhs->print(file_buffer);
	file_buffer<<")";
}

////// ARITH EXPR AST ///////////////////////////////////

Data_Type Arithmetic_Expr_Ast::get_data_type(){
	return node_data_type;
}
void Arithmetic_Expr_Ast::set_data_type(Data_Type dt){
	node_data_type = dt;
}
bool Arithmetic_Expr_Ast::check_ast(){
	if(rhs->get_data_type() == lhs->get_data_type()) return true;
	else return false;
}

///// PLUS AST ////////////////////////////////

Plus_Ast::Plus_Ast(Ast * l, Ast * r, int line){
	lhs = l;
	rhs = r;
	lineno = line;
	ast_num_child = binary_arity;
}

void Plus_Ast::print(ostream & file_buffer){
	file_buffer<<"\n"<<AST_NODE_SPACE<<"Arith: PLUS"<<endl;
	file_buffer<<AST_SUB_NODE_SPACE<<"LHS (";
	lhs->print(file_buffer);
	file_buffer<<")\n";
	file_buffer<<AST_SUB_NODE_SPACE<<"RHS (";
	rhs->print(file_buffer);
	file_buffer<<")";
}

///// MINUS AST ////////////////////

Minus_Ast::Minus_Ast(Ast * l, Ast * r, int line){
	lhs = l;
	rhs = r;
	lineno = line;
	ast_num_child = binary_arity;
}

void Minus_Ast::print(ostream & file_buffer){
	file_buffer<<"\n"<<AST_NODE_SPACE<<"Arith: MINUS"<<endl;
	file_buffer<<AST_SUB_NODE_SPACE<<"LHS (";
	lhs->print(file_buffer);
	file_buffer<<")\n";
	file_buffer<<AST_SUB_NODE_SPACE<<"RHS (";
	rhs->print(file_buffer);
	file_buffer<<")";
}

///// MULT AST //////////////////////

Mult_Ast::Mult_Ast(Ast * l, Ast * r, int line){
	lhs = l;
	rhs = r;
	lineno = line;
	ast_num_child = binary_arity;
}

void Mult_Ast::print(ostream & file_buffer){
	file_buffer<<"\n"<<AST_NODE_SPACE<<"Arith: MULT"<<endl;
	file_buffer<<AST_SUB_NODE_SPACE<<"LHS (";
	lhs->print(file_buffer);
	file_buffer<<")\n";
	file_buffer<<AST_SUB_NODE_SPACE<<"RHS (";
	rhs->print(file_buffer);
	file_buffer<<")";
}

///// DIVIDE AST ///////////////////

Divide_Ast::Divide_Ast(Ast * l, Ast * r, int line){
	lhs = l;
	rhs = r;
	lineno = line;
	ast_num_child = binary_arity;
}

void Divide_Ast::print(ostream & file_buffer){
	file_buffer<<"\n"<<AST_NODE_SPACE<<"Arith: DIV"<<endl;
	file_buffer<<AST_SUB_NODE_SPACE<<"LHS (";
	lhs->print(file_buffer);
	file_buffer<<")\n";
	file_buffer<<AST_SUB_NODE_SPACE<<"RHS (";
	rhs->print(file_buffer);
	file_buffer<<")";
}

///// UMINUS AST /////////////////////

UMinus_Ast::UMinus_Ast(Ast * l, Ast * r, int line){
	lhs = l;
	lineno = line;
	ast_num_child = unary_arity;
}

void UMinus_Ast::print(ostream & file_buffer){
	file_buffer<<"\n"<<AST_NODE_SPACE<<"Arith: UMINUS"<<endl;
	file_buffer<<AST_SUB_NODE_SPACE<<"LHS (";
	lhs->print(file_buffer);
	file_buffer<<")";
}


///// CONDITIONAL EXPR AST  //////////
Conditional_Expression_Ast::Conditional_Expression_Ast(Ast * cond0, Ast * l, Ast * r,int line){
	cond = cond0;
	lhs = l;
	rhs = r;
	lineno = line;
	ast_num_child = ternary_arity;
}

Conditional_Expression_Ast::~Conditional_Expression_Ast(){

}

void Conditional_Expression_Ast::print(ostream & file_buffer) {
	file_buffer<<"\n"<<AST_SPACE<<"Cond:";
	file_buffer<<"\n"<<AST_NODE_SPACE<<"IF_ELSE";
	//file_buffer<<AST_SUB_NODE_SPACE;
	cond->print(file_buffer);
	file_buffer<<"\n"<<AST_NODE_SPACE<<"LHS (";
	lhs->print(file_buffer);
	file_buffer<<")";
	file_buffer<<"\n"<<AST_NODE_SPACE<<"RHS (";
	rhs->print(file_buffer);
	file_buffer<<")";
}

////// RELATIONAL_EXPR ///////////

Relational_Expr_Ast::Relational_Expr_Ast(Ast * lhs, Relational_Op rop, Ast * rhs, int line){
	lhs_condition=lhs;
	rhs_condition=rhs;
	lineno=line;
	ast_num_child=binary_arity;
	rel_op=rop;
}
Relational_Expr_Ast::~Relational_Expr_Ast(){
	delete lhs_condition;
	delete rhs_condition;
}

Data_Type Relational_Expr_Ast::get_data_type(){//add check for datatype match in parser?
	return node_data_type;
}
void Relational_Expr_Ast::set_data_type(Data_Type dt){
	node_data_type = dt;
}

bool Relational_Expr_Ast::check_ast(){
	if(rhs_condition->get_data_type() == lhs_condition->get_data_type()) return true;
	else return false;
}

void Relational_Expr_Ast::print(ostream & file_buffer){
	file_buffer<<"\n"<<AST_NODE_SPACE<<"Condition: ";
	if (rel_op==less_equalto){
		file_buffer<<"LE";
	}
	else if (rel_op==less_than){
		file_buffer<<"LT";
	}
	else if (rel_op==greater_than){
		file_buffer<<"GT";
	}
	else if (rel_op==greater_equalto){
		file_buffer<<"GE";
	}
	else if (rel_op==equalto){
		file_buffer<<"EQ";
	}
	else if (rel_op==not_equalto){
		file_buffer<<"NE";
	}
	file_buffer<<"\n"<<AST_SUB_NODE_SPACE<<"LHS (";
	lhs_condition->print(file_buffer);
	file_buffer<<")";
	file_buffer<<"\n"<<AST_SUB_NODE_SPACE<<"RHS (";
	rhs_condition->print(file_buffer);
	file_buffer<<")";
}

////Logical_Expr_Ast

Logical_Expr_Ast::Logical_Expr_Ast(Ast * lhs, Logical_Op bop, Ast * rhs, int line){
	lhs_op=lhs;
	rhs_op=rhs;
	bool_op=bop;
	lineno=line;
    if (lhs!=NULL)
	    ast_num_child=binary_arity;
    else{
        ast_num_child=unary_arity;
    }
}

Logical_Expr_Ast::~Logical_Expr_Ast(){
	delete lhs_op;
	delete rhs_op;
}

Data_Type Logical_Expr_Ast::get_data_type(){//add check for datatype match in parser?
	return node_data_type;
}

void Logical_Expr_Ast::set_data_type(Data_Type dt){
	node_data_type = dt;
}

bool Logical_Expr_Ast::check_ast(){
    //cout<<"check\n";
    if(rhs_op!=NULL){
	    if(rhs_op->get_data_type() == lhs_op->get_data_type()) return true;
	    else return false;
    }
    else return true;
}

void Logical_Expr_Ast::print(ostream & file_buffer){
	file_buffer<<"\n"<<AST_NODE_SPACE<<"Condition: ";
	if (bool_op==_logical_not){//revisit
		file_buffer<<"NOT\n";
		file_buffer<<AST_SUB_NODE_SPACE<<"RHS (";
		rhs_op->print(file_buffer);
		file_buffer<<")";
		return;
	}
	else if (bool_op==_logical_or){
		file_buffer<<"OR";
	}
	else if (bool_op==_logical_and){
		file_buffer<<"AND";
	}
	
	file_buffer<<"\n"<<AST_SUB_NODE_SPACE<<"LHS (";
	lhs_op->print(file_buffer);
	file_buffer<<")";
	file_buffer<<"\n"<<AST_SUB_NODE_SPACE<<"RHS (";
	rhs_op->print(file_buffer);
	file_buffer<<")";
}
////////

Sequence_Ast::Sequence_Ast(int line){
	lineno=line;
	//statement_list = *(new list<Ast*>());
}

Sequence_Ast::~Sequence_Ast(){
	while(!statement_list.empty()) delete statement_list.front(), statement_list.pop_front();
	//while(!sa_icode_list.empty()) delete sa_icode_list.front(), sa_icode_list.pop_front();
}

void Sequence_Ast::ast_push_back(Ast * ast){
	statement_list.push_back(ast);
}

void Sequence_Ast::print(ostream & file_buffer){
    for(auto const& x:this->statement_list){
        file_buffer<<"\n"<<AST_NODE_SPACE;
        x->print(file_buffer);
        //file_buffer<<"\n";
    }
}

void Sequence_Ast::print_icode(ostream & file_buffer){
   // file_buffer<<"\nicode";
}
void Sequence_Ast::print_assembly(ostream & file_buffer){
   // file_buffer<<"\nasmcode";
}

///////Selection_Statement_Ast
Selection_Statement_Ast::Selection_Statement_Ast(Ast * cond0,Ast* then_part0, Ast* else_part0, int line){
	cond=cond0;
	then_part=then_part0;
	else_part=else_part0;
	lineno=line;
}

Selection_Statement_Ast::~Selection_Statement_Ast(){
	delete cond;
	delete then_part;
	delete else_part;
}

Data_Type Selection_Statement_Ast::get_data_type(){
	return node_data_type;
}

void Selection_Statement_Ast::set_data_type(Data_Type dt){
	node_data_type = dt;
}

bool Selection_Statement_Ast::check_ast(){
	bool check;
    check=cond->check_ast();
    if(then_part!=NULL) check=check && then_part->check_ast();
    if(else_part!=NULL) check=check&&else_part->check_ast();
    return check;
}

void Selection_Statement_Ast::print(ostream & file_buffer){
    file_buffer<<"\n"<<AST_SPACE<<"IF : ";
    file_buffer<<"\n"<<AST_SPACE<<"CONDITION (";
    cond->print(file_buffer); 
    file_buffer<<")";
    file_buffer<<"\n"<<AST_SPACE<<"THEN (";
    then_part->print(file_buffer);
    file_buffer<<")";
    if (else_part!=NULL){
        file_buffer<<"\n"<<AST_SPACE<<"ELSE (";
        else_part->print(file_buffer);
        file_buffer<<")";
    }
}

///

Iteration_Statement_Ast::Iteration_Statement_Ast(Ast * cond0, Ast* body0, int line, bool do_form){
	cond=cond0;
	body=body0;
	is_do_form = do_form;
	lineno=line;
}

Iteration_Statement_Ast::~Iteration_Statement_Ast(){
	delete cond;
	delete body;
}

Data_Type Iteration_Statement_Ast::get_data_type(){
	return node_data_type;
}

void Iteration_Statement_Ast::set_data_type(Data_Type dt){
	node_data_type = dt;
}

bool Iteration_Statement_Ast::check_ast(){
	bool check;
    check=cond->check_ast();
    if(body!=NULL) check=check && body->check_ast();
    return check;
}

void Iteration_Statement_Ast::print(ostream & file_buffer){
	if(is_do_form){
        file_buffer<<"\n"<<AST_SPACE<<"DO (";
        body->print(file_buffer);
        file_buffer<<")";
        file_buffer<<"\n"<<AST_SPACE<<"WHILE CONDITION (";
        cond->print(file_buffer);
        file_buffer<<")";
    }
    else{
        file_buffer<<"\n"<<AST_SPACE<<"WHILE : ";
        file_buffer<<"\n"<<AST_SPACE<<"CONDITION (";
        cond->print(file_buffer);
        file_buffer<<")";
        file_buffer<<"\n"<<AST_SPACE<<"BODY (";
        body->print(file_buffer);
        file_buffer<<")";
    }
}   
/*
Code_For_Ast & Iteration_Statement_Ast::compile(){
	Code_For_Ast *ca=NULL;
	return *ca;
}*/
Print_Ast::Print_Ast(Ast *v, int line){
	var=v;
	lineno = line;

}

Print_Ast::~Print_Ast(){
	
}

void Print_Ast::print(std::ostream & file_buffer){
	file_buffer<<"\n"<<AST_SPACE<<"Print :"<<endl<<AST_NODE_SPACE<<"(";
	var->print(file_buffer);
	file_buffer<<")";
}

///// RETURN AST /////////////

Return_Ast::Return_Ast(Ast * ret_val,string name,int line){
	return_value = ret_val;
	proc_name = name;
	lineno = line;
}
Return_Ast::~Return_Ast(){}

Data_Type Return_Ast::get_data_type(){
	return return_value->get_data_type();
}

void Return_Ast::print(ostream & file_buffer){
	file_buffer<<"\n"<<AST_SPACE<<"RETURN ";
	return_value->print(file_buffer);
	file_buffer<<"\n";
}

///// CALL AST /////////////

Call_Ast::Call_Ast(string name, int line){
	procedure_name = name;
}
Call_Ast::~Call_Ast(){}

Data_Type Call_Ast::get_data_type(){
	return node_data_type;
}

void Call_Ast::set_register(Register_Descriptor * reg){
	return_value_reg = reg;
}

void Call_Ast::check_actual_formal_param(Symbol_Table & formal_param_list){
	int i=0;
	for(list<Ast*>::iterator it = actual_param_list.begin(); it != actual_param_list.end(); ++it){
		if(formal_param_list.get_symbol_table_entry_by_index(i).get_data_type() != (*it)->get_data_type()){
			cout<<"cs316 Error : Actual parameters do not match with formal parameters"<<endl;
			exit(1);
		}
		i++;
	}
}

void Call_Ast::set_actual_param_list(list<Ast *> & param_list){
	actual_param_list = param_list;
}

void Call_Ast::print(ostream & file_buffer){
	file_buffer<<"\n"<<AST_SPACE<<"FN CALL: "<<procedure_name<<"(";
	for (list<Ast*>::iterator it = actual_param_list.begin(); it != actual_param_list.end() ; ++it){
		file_buffer<<endl<<AST_NODE_SPACE;
		(*it)->print(file_buffer);
	}
	file_buffer<<")";
}