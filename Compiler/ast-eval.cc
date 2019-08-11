template class Number_Ast<double>;
template class Number_Ast<int>;
//#include <iostream>
//using namespace std;
/////////
void Symbol_Table::create(Local_Environment & local_global_variables_table){
	
	for (auto const& ste : variable_table) {
		Eval_Result *temp=NULL;
		if(ste->get_data_type()==int_data_type){
			temp=new Eval_Result_Value_Int();
			if (ste->get_symbol_scope()==global){
				temp->set_value(0);
               // std::cout<<"setting to 0 : "<<ste->get_variable_name()<<endl;
			}
		}
		else if (ste->get_data_type()==double_data_type){
			temp=new Eval_Result_Value_Double();
			if (ste->get_symbol_scope()==global){
				temp->set_value(0.0);
                //std::cout<<"setting to 0 : "<<ste->get_variable_name()<<endl;
			}
		}
		else{
			//putting function ptr in table. is it required now?
		}
		//std::cout<<ste->get_variable_name()<<" ";
    	local_global_variables_table.put_variable_value(*temp ,ste->get_variable_name());
	}
	//std::cout<<endl;
}




/////////
Eval_Result & Ast::get_value_of_evaluation(Local_Environment & eval_env){
	Eval_Result* er= NULL;
	return *er;
}
 
void Ast::set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result){
	return;
}



void Ast::print_value(Local_Environment & eval_env, ostream & file_buffer){
	print(file_buffer);
}
//


///////Number_Ast

template <class T>
Eval_Result & Number_Ast<T>::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	Eval_Result *er;
	if (this->get_data_type()==int_data_type){
		er=new Eval_Result_Value_Int();
		er->set_value(constant);
	}
	else //if (this->get_data_type()==double_data_type)
	{
		er=new Eval_Result_Value_Double();
		er->set_value(constant);
	}
	return *er;
}

//////Plus_Ast
Eval_Result & Plus_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	Eval_Result *er;
	Eval_Result *l,*r;
	l=&(lhs->evaluate(eval_env, file_buffer));
	r=&(rhs->evaluate(eval_env, file_buffer));
	//assuming data types of l, r are same ie. already checked?
	if (l->get_result_enum()==int_result){
		er=new Eval_Result_Value_Int();
		er->set_value(l->get_int_value()+r->get_int_value());
	}
	else {
		er=new Eval_Result_Value_Double();
		er->set_value(l->get_double_value()+r->get_double_value());
	}
	return *er;
}


//////Minus_Ast
Eval_Result & Minus_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	Eval_Result *er;
	Eval_Result *l,*r;
	l=&(lhs->evaluate(eval_env, file_buffer));
	r=&(rhs->evaluate(eval_env, file_buffer));
	//assuming data types of l, r are same ie. already checked?
	if (l->get_result_enum()==int_result){
		er=new Eval_Result_Value_Int();
		er->set_value(l->get_int_value()-r->get_int_value());
	}
	else {
		er=new Eval_Result_Value_Double();
		er->set_value(l->get_double_value()-r->get_double_value());
	}
	return *er;
}

//////Divide_Ast
Eval_Result & Divide_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	Eval_Result *er;
	Eval_Result *l,*r;
	l=&(lhs->evaluate(eval_env, file_buffer));
	r=&(rhs->evaluate(eval_env, file_buffer));
	//assuming data types of l, r are same ie. already checked?
	if (l->get_result_enum()==int_result){
		er=new Eval_Result_Value_Int();
        if (r->get_int_value()==0){
            printf("cs316: Error : error Divide by 0\n");
            exit(1);
        }
		er->set_value(l->get_int_value()/r->get_int_value());
	}
	else {
		er=new Eval_Result_Value_Double();
        if (r->get_double_value()==0){
              printf("cs316: Error : error Divide by 0\n");
              exit(1);
         }
		er->set_value(l->get_double_value()/r->get_double_value());
	}
	return *er;
}

//////Mult_Ast
Eval_Result & Mult_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	Eval_Result *er;
	Eval_Result *l,*r;
	l=&(lhs->evaluate(eval_env, file_buffer));
	r=&(rhs->evaluate(eval_env, file_buffer));
	//assuming data types of l, r are same ie. already checked?
	if (l->get_result_enum()==int_result){
		er=new Eval_Result_Value_Int();
		er->set_value(l->get_int_value()*r->get_int_value());
	}
	else {
		er=new Eval_Result_Value_Double();
		er->set_value(l->get_double_value()*r->get_double_value());
	}
	return *er;
}

///////UMinus_Ast

Eval_Result & UMinus_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	Eval_Result *er;
	Eval_Result *l;
	l=&(lhs->evaluate(eval_env, file_buffer));
	//assuming data types of l, r are same ie. already checked?
	if (l->get_result_enum()==int_result){
		er=new Eval_Result_Value_Int();
		er->set_value(-l->get_int_value());
	}
	else {
		er=new Eval_Result_Value_Double();
		er->set_value(-l->get_double_value());
	}
	return *er;
}

///////Name_Ast

Eval_Result & Name_Ast::get_value_of_evaluation(Local_Environment & eval_env){
	Eval_Result *er;
	
	if (! eval_env.does_variable_exist(variable_symbol_entry->get_variable_name())){
		//not local variable msut be global else error would have been caught already
		er=interpreter_global_table.get_variable_value(variable_symbol_entry->get_variable_name());
	}
	else
		er=eval_env.get_variable_value(variable_symbol_entry->get_variable_name());
	return *er;
}

void Name_Ast::print_value(Local_Environment & eval_env, ostream & file_buffer){
	Eval_Result *er=&(get_value_of_evaluation(eval_env));
	//eval_env.get_variable_value(variable_symbol_entry->get_variable_name());
	file_buffer<<"\n"<<AST_SPACE<<variable_symbol_entry->get_variable_name()<<" : ";
	if (er->get_result_enum()==int_result){
		file_buffer<<er->get_int_value()<<"\n\n";
	}
	else{
		file_buffer<<er->get_double_value()<<"\n\n";
	}
}

void Name_Ast::set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result){
	if (eval_env.does_variable_exist(variable_symbol_entry->get_variable_name())){
		eval_env.put_variable_value(result, variable_symbol_entry->get_variable_name());
	}
	else if (interpreter_global_table.does_variable_exist(variable_symbol_entry->get_variable_name())){
		interpreter_global_table.put_variable_value(result, variable_symbol_entry->get_variable_name());
	}
	else{
		//shouldn't get here. variable if not in local scope must be in global scope
	}
}

Eval_Result & Name_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){//check this
	return get_value_of_evaluation(eval_env);
}


/////Assignment_Ast

Eval_Result & Assignment_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	Eval_Result *r;
	r=&(rhs->evaluate(eval_env, file_buffer));
	lhs->set_value_of_evaluation(eval_env,*r);
	print(file_buffer);
	lhs->print_value(eval_env, file_buffer);
	return *r;
}


////Relational_Expr_Ast

Eval_Result & Relational_Expr_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	Eval_Result *er=new Eval_Result_Value_Int();
	Eval_Result *l,*r;
	l=&(lhs_condition->evaluate(eval_env, file_buffer));
	r=&(rhs_condition->evaluate(eval_env, file_buffer));
	if (rel_op==less_equalto){
		if (l->get_result_enum()==int_result){
			er->set_value(l->get_int_value() <= r->get_int_value());
		}
		else{
			er->set_value(l->get_double_value() <= r->get_double_value());
		}	
	}
	else if (rel_op==less_than){
		if (l->get_result_enum()==int_result){
			er->set_value(l->get_int_value() < r->get_int_value());
		}
		else{
			er->set_value(l->get_double_value() < r->get_double_value());
		}	
	}
	else if (rel_op==greater_than){
		if (l->get_result_enum()==int_result){
			er->set_value(l->get_int_value() > r->get_int_value());
		}
		else{
			er->set_value(l->get_double_value() > r->get_double_value());
		}	
	}
	else if (rel_op==greater_equalto){
		if (l->get_result_enum()==int_result){
			er->set_value(l->get_int_value() >= r->get_int_value());
		}
		else{
			er->set_value(l->get_double_value() >= r->get_double_value());
		}	
	}
	else if (rel_op==equalto){
		if (l->get_result_enum()==int_result){
			er->set_value(l->get_int_value() == r->get_int_value());
		}
		else{
			er->set_value(l->get_double_value() == r->get_double_value());
		}	
	}
	else if (rel_op==not_equalto){
		if (l->get_result_enum()==int_result){
			er->set_value(l->get_int_value() != r->get_int_value());
		}
		else{
			er->set_value(l->get_double_value() != r->get_double_value());
		}	
	}

	return *er;
}

///Logical_Expr_Ast

Eval_Result & Logical_Expr_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	Eval_Result *er=new Eval_Result_Value_Int();
	Eval_Result *l,*r;
	if (ast_num_child==binary_arity) l=&(lhs_op->evaluate(eval_env, file_buffer));
	r=&(rhs_op->evaluate(eval_env, file_buffer));
   // cout<<"eval logical";
	if (bool_op==_logical_not){
		er->set_value(! r->get_int_value());
	}
	else if (bool_op==_logical_or){
      //   if(l->get_int_value()){
      //       er->set_value(l->get_int_value());
      //   }
      //   else{
		    // er->set_value(r->get_int_value());
      //   }
		er->set_value(l->get_int_value() || r->get_int_value());
	}
	else if (bool_op==_logical_and){
		er->set_value(l->get_int_value() && r->get_int_value());
	}
	return *er;
}

///Conditional_Expression_Ast

Eval_Result & Conditional_Expression_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	Eval_Result *er;
	Eval_Result *l,*r, *b;
	l=&(lhs->evaluate(eval_env, file_buffer));
	r=&(rhs->evaluate(eval_env, file_buffer));
	b=&(cond->evaluate(eval_env, file_buffer));
	//print(file_buffer);
	if (b->get_int_value()){
		return *l;
	}
	else{
		return *r;
	}
}


////
Eval_Result & Iteration_Statement_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	Eval_Result *er=NULL;
	if (is_do_form){
		do {
			body->evaluate(eval_env, file_buffer);
		}
		while(cond->evaluate(eval_env, file_buffer).get_int_value());
	}
	else{
		while(cond->evaluate(eval_env, file_buffer).get_int_value()){
			body->evaluate(eval_env, file_buffer);
		}
	}
	return *er;
}

Eval_Result & Selection_Statement_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	Eval_Result *er=NULL;
    //std::cout<<"in if-else eval";
	if (cond->evaluate(eval_env, file_buffer).get_int_value()){
		then_part->evaluate(eval_env, file_buffer);
	}
	else if (else_part!=NULL){
		else_part->evaluate(eval_env, file_buffer);
	}
    //std::cout<<"here";
	return *er;
}

///
Eval_Result & Sequence_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	Eval_Result *er=NULL;
	for(auto const& stmt : statement_list){
		//file_buffer<<"evaluating.....\n";
		stmt->evaluate(eval_env, file_buffer);
	}
	return *er;
}


////Return_Ast

Eval_Result & Return_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	Eval_Result *er=NULL;
	return_value->evaluate(eval_env,file_buffer);
	return *er;
}

//// Call_Ast

Eval_Result & Call_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	Eval_Result *er=NULL;
	for(list<Ast* >::iterator it = actual_param_list.begin(); it != actual_param_list.end(); ++it){
		(*it)->evaluate(eval_env, file_buffer);
	}
	return *er;
}