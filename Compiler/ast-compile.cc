#include <iostream>
using namespace std;
//
template class Number_Ast<double>;
template class Number_Ast<int>;
//Number_Ast
template <class T>
Code_For_Ast & Number_Ast<T>::compile(){
	list<Icode_Stmt *> *ic_l=new list<Icode_Stmt *>();
	Icode_Stmt *stmt;
	Register_Descriptor * rd;
	Ics_Opd *opd1;
	Ics_Opd *r;
	if (this->get_data_type()==int_data_type){
		opd1=new Const_Opd<int>(constant);
		rd=machine_desc_object.get_new_register<int_reg>();//spim_register_table[v0];//
		r=new Register_Addr_Opd(rd);
		stmt=new Move_IC_Stmt(imm_load, opd1, r);
		ic_l->push_back(stmt);

	}
	else{
		opd1=new Const_Opd<double>(constant);
		rd=machine_desc_object.get_new_register<float_reg>();//spim_register_table[f2];//
		r=new Register_Addr_Opd(rd);
		stmt=new Move_IC_Stmt(imm_load_d, opd1, r);
		ic_l->push_back(stmt);
	}
	Code_For_Ast *ca=new Code_For_Ast(*ic_l, rd);
	return *ca;
}

template <class T>
Code_For_Ast & Number_Ast<T>::compile_and_optimize_ast(Lra_Outcome & lra){
	Code_For_Ast *ca=NULL;
	return *ca;
}

//Name_Ast

Code_For_Ast & Name_Ast::create_store_stmt(Register_Descriptor * store_register){

	list<Icode_Stmt *> *ic_l=new list<Icode_Stmt *>();
	Icode_Stmt *stmt;
	//Register_Descriptor * rd;
	Ics_Opd *r;
	Ics_Opd *opd1=new Mem_Addr_Opd(*variable_symbol_entry);
	if (variable_symbol_entry->get_data_type()==int_data_type){
		
		r=new Register_Addr_Opd(store_register);
		stmt=new Move_IC_Stmt(store, r,opd1);//opd1<-r
		ic_l->push_back(stmt);
		store_register->reset_register_occupied(); ///revisit
	}
	else{
		r=new Register_Addr_Opd(store_register);
		stmt=new Move_IC_Stmt(store_d, r, opd1);
		ic_l->push_back(stmt);
		store_register->reset_register_occupied(); ///revisit
	}
	Code_For_Ast *ca=new Code_For_Ast(*ic_l, store_register);
	return *ca;
}

Code_For_Ast & Name_Ast::compile(){

	list<Icode_Stmt *> *ic_l=new list<Icode_Stmt *>();
	Icode_Stmt *stmt;
	Register_Descriptor * rd;
	Ics_Opd *r;
	Ics_Opd *opd1=new Mem_Addr_Opd(*variable_symbol_entry);
	if (variable_symbol_entry->get_data_type()==int_data_type){
		rd=machine_desc_object.get_new_register<int_reg>();
		r=new Register_Addr_Opd(rd);
		stmt=new Move_IC_Stmt(load, opd1, r);
		ic_l->push_back(stmt);
	}
	else{
		rd=machine_desc_object.get_new_register<float_reg>();
		r=new Register_Addr_Opd(rd);
		stmt=new Move_IC_Stmt(load_d, opd1, r);
		ic_l->push_back(stmt);
	}
	Code_For_Ast *ca=new Code_For_Ast(*ic_l, rd);
	return *ca;
}

Code_For_Ast & Name_Ast::compile_and_optimize_ast(Lra_Outcome & lra){
	Code_For_Ast *ca=NULL;
	return *ca;
}

//

Code_For_Ast & Ast::compile(){
	Code_For_Ast *ca=NULL;
	return *ca;
}

Code_For_Ast & Ast::compile_and_optimize_ast(Lra_Outcome & lra){
	Code_For_Ast *ca=NULL;
	return *ca;
}

Code_For_Ast & Ast::create_store_stmt(Register_Descriptor * store_register){
	Code_For_Ast *ca=NULL;
	return *ca;
}
//Arithmetic_Expr_Ast
/*
Code_For_Ast & Arithmetic_Expr_Ast::compile(){//variable_symbol_entry
	Code_For_Ast *ca=NULL;
	return *ca;
}
*/

//
Code_For_Ast & Plus_Ast::compile(){
	///
	Register_Descriptor * rd;
	Ics_Opd *opd1, *opd2, *r;
	Icode_Stmt *stmt;
	////
	Code_For_Ast *lca=&(lhs->compile());	
	Code_For_Ast *rca=&(rhs->compile());
	///append rhs to lhs
	list<Icode_Stmt *> *rhs_icl=&(rca->get_icode_list());
	for(auto const& stmt : *rhs_icl){
		lca->append_ics(*stmt);
	}
	opd1=new Register_Addr_Opd(lca->get_reg()); 
	opd2=new Register_Addr_Opd(rca->get_reg());
	////
	if (rhs->get_data_type()==int_data_type){
		rd=machine_desc_object.get_new_register<int_reg>();
		r=new Register_Addr_Opd(rd);
		stmt=new Compute_IC_Stmt(add, opd1, opd2, r);

	}
	else{
		rd=machine_desc_object.get_new_register<float_reg>();
		r=new Register_Addr_Opd(rd);
		stmt=new Compute_IC_Stmt(add_d, opd1, opd2, r);
	}
	////freeing regs
	lca->get_reg()->reset_register_occupied();
	rca->get_reg()->reset_register_occupied();
	///
	lca->append_ics(*stmt);
	lca->set_reg(rd);

	return *lca;
}

Code_For_Ast & Plus_Ast::compile_and_optimize_ast(Lra_Outcome & lra){
	Code_For_Ast *ca=NULL;
	return *ca;
}

//
Code_For_Ast & Minus_Ast::compile(){
	///
	Register_Descriptor * rd;
	Ics_Opd *opd1, *opd2, *r;
	Icode_Stmt *stmt;
	////
	Code_For_Ast *lca=&(lhs->compile());	
	Code_For_Ast *rca=&(rhs->compile());
	///append rhs to lhs
	list<Icode_Stmt *> *rhs_icl=&(rca->get_icode_list());
	for(auto const& stmt : *rhs_icl){
		lca->append_ics(*stmt);
	}
	opd1=new Register_Addr_Opd(lca->get_reg()); 
	opd2=new Register_Addr_Opd(rca->get_reg());
	////
	if (rhs->get_data_type()==int_data_type){
		rd=machine_desc_object.get_new_register<int_reg>();
		r=new Register_Addr_Opd(rd);
		stmt=new Compute_IC_Stmt(sub, opd1, opd2, r);

	}
	else{
		rd=machine_desc_object.get_new_register<float_reg>();
		r=new Register_Addr_Opd(rd);
		stmt=new Compute_IC_Stmt(sub_d, opd1, opd2, r);
	}
	////freeing regs
	lca->get_reg()->reset_register_occupied();
	rca->get_reg()->reset_register_occupied();
	///
	lca->append_ics(*stmt);
	lca->set_reg(rd);

	return *lca;
}

Code_For_Ast & Minus_Ast::compile_and_optimize_ast(Lra_Outcome & lra){
	Code_For_Ast *ca=NULL;
	return *ca;
}
//
Code_For_Ast & Divide_Ast::compile(){
	///
	Register_Descriptor * rd;
	Ics_Opd *opd1, *opd2, *r;
	Icode_Stmt *stmt;
	////
	Code_For_Ast *lca=&(lhs->compile());	
	Code_For_Ast *rca=&(rhs->compile());
	///append rhs to lhs
	list<Icode_Stmt *> *rhs_icl=&(rca->get_icode_list());
	for(auto const& stmt : *rhs_icl){
		lca->append_ics(*stmt);
	}
	opd1=new Register_Addr_Opd(lca->get_reg()); 
	opd2=new Register_Addr_Opd(rca->get_reg());
	////
	if (rhs->get_data_type()==int_data_type){
		rd=machine_desc_object.get_new_register<int_reg>();
		r=new Register_Addr_Opd(rd);
		stmt=new Compute_IC_Stmt(divd, opd1, opd2, r);

	}
	else{
		rd=machine_desc_object.get_new_register<float_reg>();
		r=new Register_Addr_Opd(rd);
		stmt=new Compute_IC_Stmt(div_d, opd1, opd2, r);
	}
	////freeing regs
	lca->get_reg()->reset_register_occupied();
	rca->get_reg()->reset_register_occupied();
	///
	lca->append_ics(*stmt);
	lca->set_reg(rd);

	return *lca;
}

Code_For_Ast & Divide_Ast::compile_and_optimize_ast(Lra_Outcome & lra){
	Code_For_Ast *ca=NULL;
	return *ca;
}
//
Code_For_Ast & Mult_Ast::compile(){
	///
	Register_Descriptor * rd;
	Ics_Opd *opd1, *opd2, *r;
	Icode_Stmt *stmt;
	////
	Code_For_Ast *lca=&(lhs->compile());	
	Code_For_Ast *rca=&(rhs->compile());
	///append rhs to lhs
	list<Icode_Stmt *> *rhs_icl=&(rca->get_icode_list());
	for(auto const& stmt : *rhs_icl){
		lca->append_ics(*stmt);
	}
	opd1=new Register_Addr_Opd(lca->get_reg()); 
	opd2=new Register_Addr_Opd(rca->get_reg());
	////
	if (rhs->get_data_type()==int_data_type){
		rd=machine_desc_object.get_new_register<int_reg>();
		r=new Register_Addr_Opd(rd);
		stmt=new Compute_IC_Stmt(mult, opd1, opd2, r);
	}
	else{
		rd=machine_desc_object.get_new_register<float_reg>();
		r=new Register_Addr_Opd(rd);
		stmt=new Compute_IC_Stmt(mult_d, opd1, opd2, r);
	}
	////freeing regs
	lca->get_reg()->reset_register_occupied();
	rca->get_reg()->reset_register_occupied();
	///
	lca->append_ics(*stmt);
	lca->set_reg(rd);

	return *lca;
}

Code_For_Ast & Mult_Ast::compile_and_optimize_ast(Lra_Outcome & lra){
	Code_For_Ast *ca=NULL;
	return *ca;
}
//
Code_For_Ast & UMinus_Ast::compile(){

	Register_Descriptor * rd;
	Ics_Opd *opd1, *r;
	Icode_Stmt *stmt;
	////
	Code_For_Ast *lca=&(lhs->compile());	
	opd1=new Register_Addr_Opd(lca->get_reg()); 
	if (lhs->get_data_type()==int_data_type){
		rd=machine_desc_object.get_new_register<int_reg>();
		r=new Register_Addr_Opd(rd);
		stmt=new Compute_IC_Stmt(uminus, opd1, NULL, r);
	}
	else{
		rd=machine_desc_object.get_new_register<float_reg>();
		r=new Register_Addr_Opd(rd);
		stmt=new Compute_IC_Stmt(uminus_d, opd1, NULL, r);
	}	
	////freeing regs
	lca->get_reg()->reset_register_occupied();
	///
	lca->append_ics(*stmt);
	lca->set_reg(rd);
	return *lca;
}

Code_For_Ast & UMinus_Ast::compile_and_optimize_ast(Lra_Outcome & lra){
	Code_For_Ast *ca=NULL;
	return *ca;
}
//
Code_For_Ast & Assignment_Ast::compile(){
	Code_For_Ast *ca=&(rhs->compile());
	Code_For_Ast *lca=&(lhs->create_store_stmt(ca->get_reg()));
	ca->set_reg(lca->get_reg());
	list<Icode_Stmt *> *lhs_icl=&(lca->get_icode_list());
	for(auto const& stmt : *lhs_icl){
		ca->append_ics(*stmt);
	}
	return *ca;
}

Code_For_Ast & Assignment_Ast::compile_and_optimize_ast(Lra_Outcome & lra){
	//Code_For_Ast *ca=NULL;
	//return *ca;
	return compile();
}

// Code_For_Ast & Assignment_Ast::create_store_stmt(Register_Descriptor * store_register){
// 	Code_For_Ast *ca=NULL;
// 	return *ca;
// }
//


//Conditional_Expression_Ast
Code_For_Ast & Conditional_Expression_Ast::compile(){
	//////////////
	Register_Descriptor * rd, *zr, *crd;
	zr=new Register_Descriptor(zero, "zero", int_num, fixed_reg);
	Ics_Opd *opd1, *opd2, *r, *copd;
	Icode_Stmt *stmt, *print_lab;
	Code_For_Ast *cca=&(cond->compile());
	copd=new Register_Addr_Opd(cca->get_reg()); 
	crd=cca->get_reg();
	string lab1=get_new_label();
	stmt=new Control_Flow_IC_Stmt(beq, copd,NULL,lab1);
	cca->append_ics(*stmt);
	//cca->get_reg()->reset_register_occupied();
	//
	Code_For_Ast *tca=&(lhs->compile()) ;
	list<Icode_Stmt *> *then_icl=&(tca->get_icode_list());
	for(auto const& stmt : *then_icl){
		cca->append_ics(*stmt);
	}
	if (lhs->get_data_type()==int_data_type){
		opd2=new Register_Addr_Opd(zr);
		opd1=new Register_Addr_Opd(tca->get_reg());  
		rd=machine_desc_object.get_new_register<int_reg>();
		r=new Register_Addr_Opd(rd); /////resue this in else part ?
		stmt=new Compute_IC_Stmt(or_t, opd1, opd2, r);
		cca->append_ics(*stmt);
		cca->set_reg(rd);
	}
	else{
		//opd2=new Register_Addr_Opd(zr);
		opd1=new Register_Addr_Opd(tca->get_reg());  
		rd=machine_desc_object.get_new_register<float_reg>();
		r=new Register_Addr_Opd(rd);
		stmt=new Move_IC_Stmt(move_d, opd1, r);
		cca->append_ics(*stmt);
		cca->set_reg(rd);
	}
	tca->get_reg()->reset_register_occupied();
	string lab2=get_new_label();// rest label
	Icode_Stmt *skip_else=new Label_IC_Stmt(j, lab2);
	cca->append_ics(*skip_else);
	print_lab=new Label_IC_Stmt(label, lab1);//label of elses
	cca->append_ics(*print_lab);

	Code_For_Ast *eca=&(rhs->compile()) ;
	list<Icode_Stmt *> *else_icl=&(eca->get_icode_list());
	for(auto const& stmt : *else_icl){
		cca->append_ics(*stmt);
	}	
	if (lhs->get_data_type()==int_data_type){
		opd2=new Register_Addr_Opd(zr);
		opd1=new Register_Addr_Opd(eca->get_reg());  
		rd=cca->get_reg();
		r=new Register_Addr_Opd(rd);
		stmt=new Compute_IC_Stmt(or_t, opd1, opd2, r);
		cca->append_ics(*stmt);
		cca->set_reg(rd);
	}
	else{
		//opd2=new Register_Addr_Opd(zr);
		opd1=new Register_Addr_Opd(eca->get_reg());  
		rd=cca->get_reg();
		r=new Register_Addr_Opd(rd);
		stmt=new Move_IC_Stmt(move_d, opd1, r);
		cca->append_ics(*stmt);
		cca->set_reg(rd);
	}
	eca->get_reg()->reset_register_occupied();
	print_lab=new Label_IC_Stmt(label, lab2);//label of rest
	cca->append_ics(*print_lab);
	crd->reset_register_occupied();
	return *cca;
}

// Code_For_Ast & Conditional_Expression_Ast::compile_and_optimize_ast(Lra_Outcome & lra){
// 	Code_For_Ast *ca=NULL;
// 	return *ca;
// }

//Relational_Expr_Ast
Code_For_Ast & Relational_Expr_Ast::compile(){
	///
	Register_Descriptor * rd;
	Ics_Opd *opd1, *opd2, *r, *copd;
	Icode_Stmt *stmt, *print_lab;
	////
	Code_For_Ast *lca=&(lhs_condition->compile());	
	Code_For_Ast *rca=&(rhs_condition->compile());
	///append rhs to lhs
	list<Icode_Stmt *> *rhs_icl=&(rca->get_icode_list());
	for(auto const& stmt : *rhs_icl){
		lca->append_ics(*stmt);
	}
	opd1=new Register_Addr_Opd(lca->get_reg()); 
	opd2=new Register_Addr_Opd(rca->get_reg());
	rd=machine_desc_object.get_new_register<int_reg>();
	r=new Register_Addr_Opd(rd);
	////
	if (rhs_condition->get_data_type()==int_data_type){
		if (rel_op==less_equalto){
			stmt=new Compute_IC_Stmt(sle, opd1, opd2, r);
		}
		else if (rel_op==less_than){
			stmt=new Compute_IC_Stmt(slt, opd1, opd2, r);
		}
		else if (rel_op==greater_than){
			stmt=new Compute_IC_Stmt(sgt, opd1, opd2, r);	
		}
		else if (rel_op==greater_equalto){
			stmt=new Compute_IC_Stmt(sge, opd1, opd2, r);	
		}
		else if (rel_op==equalto){
			stmt=new Compute_IC_Stmt(seq, opd1, opd2, r);
		}
		else if (rel_op==not_equalto){
			stmt=new Compute_IC_Stmt(sne, opd1, opd2, r);	
		}
		////freeing regs
		lca->get_reg()->reset_register_occupied();
		rca->get_reg()->reset_register_occupied();
		///
		lca->append_ics(*stmt);
		lca->set_reg(rd);

		return *lca;
	}
	else{
		if (rel_op==less_equalto){
			stmt=new Compute_IC_Stmt(sle_d, opd1, opd2, NULL);
		}
		else if (rel_op==less_than){
			stmt=new Compute_IC_Stmt(slt_d, opd1, opd2, NULL);
		}
		else if (rel_op==greater_than){
			stmt=new Compute_IC_Stmt(sgt_d, opd1, opd2, NULL);	
		}
		else if (rel_op==greater_equalto){
			stmt=new Compute_IC_Stmt(sge_d, opd1, opd2, NULL);	
		}
		else if (rel_op==equalto){
			stmt=new Compute_IC_Stmt(seq_d, opd1, opd2, NULL);
		}
		else if (rel_op==not_equalto){
			stmt=new Compute_IC_Stmt(sne_d, opd1, opd2, NULL);	
		}
		////////
		lca->get_reg()->reset_register_occupied();
		rca->get_reg()->reset_register_occupied();
		lca->append_ics(*stmt);
		//setting comp from flag to res to a reg
		string lab1=get_new_label(), lab2=get_new_label();
		if (rel_op==greater_than || rel_op==greater_equalto || rel_op==not_equalto)
			stmt=new Control_Flow_IC_Stmt(bc1f, NULL, NULL, lab1);
		else 
			stmt=new Control_Flow_IC_Stmt(bc1t, NULL, NULL, lab1);
		lca->append_ics(*stmt);
		//lab2 for rest
		//lab1 fo setting flag=1
		//SET TO ZERO
		copd=new Const_Opd<int>(0);
		stmt=new Move_IC_Stmt(imm_load, copd, r);
		lca->append_ics(*stmt);
		//J LAB2
		stmt=new Label_IC_Stmt(j, lab2);
		lca->append_ics(*stmt);
		//print lab1
		print_lab=new Label_IC_Stmt(label, lab1);//lab of  set1
		lca->append_ics(*print_lab);
		//SET TO  1
		copd=new Const_Opd<int>(1);
		stmt=new Move_IC_Stmt(imm_load, copd, r);
		lca->append_ics(*stmt);
		//set reg, print rest labe; ie lab2
		lca->set_reg(rd);
		print_lab=new Label_IC_Stmt(label, lab2);//lab of  rest of code
		lca->append_ics(*print_lab);
		////////
		return *lca;
	}
}

// Code_For_Ast & Relational_Expr_Ast::compile_and_optimize_ast(Lra_Outcome & lra){
// 	Code_For_Ast *ca=NULL;
// 	return *ca;
// }

//Logical_Expr_Ast
Code_For_Ast & Logical_Expr_Ast::compile(){
	Register_Descriptor * rd;
	Ics_Opd *opd1, *opd2, *r;
	Icode_Stmt *stmt;
	////
	Code_For_Ast *lca, *rca;
	if (ast_num_child==binary_arity){
		lca=&(lhs_op->compile());
	}
	else{
		Register_Descriptor * rd0;
		Ics_Opd *opd0, *r0;
		list<Icode_Stmt *> *ic_l0=new list<Icode_Stmt *>();
		opd0=new Const_Opd<int>(1);
		rd0=machine_desc_object.get_new_register<int_reg>();//spim_register_table[v0];//
		r0=new Register_Addr_Opd(rd0);
		stmt=new Move_IC_Stmt(imm_load, opd0, r0);
		ic_l0->push_back(stmt);
		lca=new Code_For_Ast(*ic_l0, rd0);
	}
	rca=&(rhs_op->compile());
	//if (ast_num_child==binary_arity){
		list<Icode_Stmt *> *rhs_icl=&(rca->get_icode_list()); ///append rhs to lhs	
		for(auto const& stmt : *rhs_icl){
			lca->append_ics(*stmt);
		}
		opd1=new Register_Addr_Opd(lca->get_reg()); 
	//} 
	opd2=new Register_Addr_Opd(rca->get_reg());
	rd=machine_desc_object.get_new_register<int_reg>();
	r=new Register_Addr_Opd(rd);
	//
	if (bool_op==_logical_not){
		stmt=new Compute_IC_Stmt(not_t, opd2, opd1, r); ////note order of opds reverse !!!!!!!!!!!!!!!!
		// rca->get_reg()->reset_register_occupied();
		// ///
		// rca->append_ics(*stmt);
		// rca->set_reg(rd);
		// return *rca;
	}
	else if (bool_op==_logical_or){
        stmt=new Compute_IC_Stmt(or_t, opd1, opd2, r);
	}
	else if (bool_op==_logical_and){
		stmt=new Compute_IC_Stmt(and_t, opd1, opd2, r);
	}
	////freeing regs
	lca->get_reg()->reset_register_occupied();
	rca->get_reg()->reset_register_occupied();
	///
	lca->append_ics(*stmt);
	lca->set_reg(rd);

	return *lca;
}

// Code_For_Ast & Logical_Expr_Ast::compile_and_optimize_ast(Lra_Outcome & lra){
// 	Code_For_Ast *ca=NULL;
// 	return *ca;
// }

//Selection_Statement_Ast
Code_For_Ast & Selection_Statement_Ast::compile(){
	
	//////////////
	Register_Descriptor * rd;
	Ics_Opd *opd1, *opd2, *r, *copd;
	Icode_Stmt *stmt, *print_lab;
	Code_For_Ast *cca=&(cond->compile());
	copd=new Register_Addr_Opd(cca->get_reg()); 
	string lab1=get_new_label();
	stmt=new Control_Flow_IC_Stmt(beq, copd,NULL,lab1);
	cca->append_ics(*stmt);
	//
	Code_For_Ast *tca=&(then_part->compile()) ;
	list<Icode_Stmt *> *then_icl=&(tca->get_icode_list());
	for(auto const& stmt : *then_icl){
		cca->append_ics(*stmt);
	}
	cca->get_reg()->reset_register_occupied();
	if (else_part!=NULL){
		string lab2=get_new_label();// rest label
		Icode_Stmt *skip_else=new Label_IC_Stmt(j, lab2);
		cca->append_ics(*skip_else);
		print_lab=new Label_IC_Stmt(label, lab1);//label of elses
		cca->append_ics(*print_lab);
		///append a print stmt for label
		///print \nlabel of else:
		Code_For_Ast *eca=&(else_part->compile()) ;
		list<Icode_Stmt *> *else_icl=&(eca->get_icode_list());
		for(auto const& stmt : *else_icl){
			cca->append_ics(*stmt);
		}
		print_lab=new Label_IC_Stmt(label, lab2);//label of elses
		cca->append_ics(*print_lab);
	}
	else{
		print_lab=new Label_IC_Stmt(label, lab1);//label of elses
		cca->append_ics(*print_lab);
	}
	//append stmt to print label of rest

	return *cca;
}
//free cond reg after then part before else part

// Code_For_Ast & Selection_Statement_Ast::compile_and_optimize_ast(Lra_Outcome & lra){
// 	Code_For_Ast *ca=NULL;
// 	return *ca;
// }

//Iteration_Statement_Ast
Code_For_Ast & Iteration_Statement_Ast::compile(){
	Code_For_Ast *ca=new Code_For_Ast(), *cca, *bca;
	Ics_Opd *copd;
	Icode_Stmt *stmt, *print_lab;
	list<Icode_Stmt *> *icl;
	///////
	cca=&(cond->compile());
	copd=new Register_Addr_Opd(cca->get_reg()); 
	//
	string lab1=get_new_label(), lab2=get_new_label();
	stmt=new Control_Flow_IC_Stmt(bne, copd, NULL, lab1);
	cca->append_ics(*stmt);
	//
	if (! is_do_form){
		Icode_Stmt *jump_to_cond=new Label_IC_Stmt(j, lab2);
		ca->append_ics(*jump_to_cond);
	}
	//
	bca=&(body->compile());
	print_lab=new Label_IC_Stmt(label, lab1);
	ca->append_ics(*print_lab);
	icl=&(bca->get_icode_list());
	for(auto const& stmt : *icl){
		ca->append_ics(*stmt);
	}
	print_lab=new Label_IC_Stmt(label, lab2);
	ca->append_ics(*print_lab);
	icl=&(cca->get_icode_list());
	for(auto const& stmt : *icl){
		ca->append_ics(*stmt);
	}
	cca->get_reg()->reset_register_occupied();
	return *ca;
}
//do not free cond reg, but first compile cond stmt
//lab1 for body , 2 for cond
//compile cond
//get labels
//compile body
// Code_For_Ast & Iteration_Statement_Ast::compile_and_optimize_ast(Lra_Outcome & lra){
// 	Code_For_Ast *ca=NULL;
// 	return *ca;
// }

//Sequence_Ast
Code_For_Ast & Sequence_Ast::compile(){
	Code_For_Ast *ca=new Code_For_Ast(), *tca;
	list<Icode_Stmt *> *icl;
	for(auto const& astmt : statement_list){
		tca=&(astmt->compile());
		icl=&(tca->get_icode_list());
		for(auto const& stmt : *icl){
			ca->append_ics(*stmt);
		}
	}
	return *ca;
}
// Code_For_Ast & Sequence_Ast::compile_and_optimize_ast(Lra_Outcome & lra){
// 	Code_For_Ast *ca=NULL;
// 	return *ca;
// }


//


Code_For_Ast & Print_Ast::compile(){
	Code_For_Ast *ca;
	Register_Descriptor * rd;
	Icode_Stmt *stmt, *print_lab;
	list<Icode_Stmt *> *icl=new list<Icode_Stmt*>();
	Ics_Opd *opd, *opd1, *r;
	Ics_Opd *mopd=new Mem_Addr_Opd(var->get_symbol_entry());
	if (var->get_data_type()==int_data_type){//print
		//load 1 into v0
		rd=new Register_Descriptor(v0, "v0", int_num, int_reg);//spim_register_table[v0];//
		r=new Register_Addr_Opd(rd);
		opd1=new Const_Opd<int>(1);
		stmt=new Move_IC_Stmt(imm_load, opd1, r);
		icl->push_back(stmt);
		//load var into a0
		rd=new Register_Descriptor(a0, "a0", int_num, argument);
		r=new Register_Addr_Opd(rd);
		stmt=new Move_IC_Stmt(load, mopd, r);
		icl->push_back(stmt);
		//
		//string *lab=new string("syscall");
		stmt=new Label_IC_Stmt(label, "syscall");	
		icl->push_back(stmt);
	}
	else{
		//load 3 into v0
		rd=new Register_Descriptor(v0, "v0", int_num, int_reg);//spim_register_table[v0];//
		r=new Register_Addr_Opd(rd);
		opd1=new Const_Opd<int>(3);
		stmt=new Move_IC_Stmt(imm_load, opd1, r);
		icl->push_back(stmt);
		//load var into f12
		rd=new Register_Descriptor(f12, "f12", float_num, float_reg);
		r=new Register_Addr_Opd(rd);
		stmt=new Move_IC_Stmt(load_d, mopd, r);
		icl->push_back(stmt);
		//
		//string *lab=new string("syscall");
		stmt=new Label_IC_Stmt(label, "syscall");
		icl->push_back(stmt);
	}
	ca=new Code_For_Ast(*icl, rd);
}


//Return_Ast
Code_For_Ast & Return_Ast::compile(){
	Code_For_Ast *ca = new Code_For_Ast();
	Icode_Stmt *move_stmt, *return_stmt;

	return_stmt = new Label_IC_Stmt(ret_inst,proc_name);

	if(return_value == NULL){
		ca->append_ics(*return_stmt);
	}
	else {
		Ics_Opd *mopd = new Mem_Addr_Opd(return_value->get_symbol_entry());
		Ics_Opd* r1,* r2;
		Register_Descriptor * rd1,* rd2;
		
		Code_For_Ast *return_code = &(return_value->compile());
		list<Icode_Stmt*> * icl = &(return_code->get_icode_list());
		for(auto const& stmt : *icl){
			ca->append_ics(*stmt);
		}
		rd1 = return_code->get_reg();
		r1 = new Register_Addr_Opd(rd1);

		if(return_value->get_data_type() == int_data_type) {

			rd2 = new Register_Descriptor(v1, "v1", int_num, fn_result);
			r2=new Register_Addr_Opd(rd2);
			move_stmt = new Move_IC_Stmt(mov, r1, r2);

			ca->append_ics(*move_stmt);
			ca->append_ics(*return_stmt);
		}
		else if (return_value->get_data_type() == double_data_type){

			rd2 = new Register_Descriptor(f0, "f0", float_num, fn_result);
			r2=new Register_Addr_Opd(rd2);
			move_stmt = new Move_IC_Stmt(move_d, r1, r2);

			ca->append_ics(*move_stmt);
			ca->append_ics(*return_stmt);
		}
		rd1->reset_register_occupied();
	}
	return *ca;
}

Code_For_Ast & Return_Ast::compile_and_optimize_ast(Lra_Outcome & lra){
	Code_For_Ast *ca=NULL;
	return *ca;
}


/// CALL AST 
Code_For_Ast & Call_Ast::compile(){
	Code_For_Ast *ca=new Code_For_Ast();
	Icode_Stmt* load_stmt, *store_stmt, *call_stmt;

	Ics_Opd *r,*actual_mem, *formal_mem, *const_mem;
	Register_Descriptor * rd;


	Procedure * procedure_prototype = program_object.get_procedure_prototype(procedure_name);
	Symbol_Table formal_table = procedure_prototype->get_formal_param_list();
	

	if(procedure_prototype->get_return_type() == int_data_type){
		return_value_reg = new Register_Descriptor(v1, "v1", int_num, fn_result);
	}
	else if ((procedure_prototype->get_return_type() == double_data_type)){
		return_value_reg = new Register_Descriptor(f0, "f0", float_num, fn_result);
	}

	list<Symbol_Table_Entry*> formal_list = formal_table.get_table();
	list<Symbol_Table_Entry*>::reverse_iterator it0 = formal_list.rbegin();

	int param_offset = 0;

	for(list<Ast*>::reverse_iterator it = actual_param_list.rbegin(); it != actual_param_list.rend(); ++it){
		Ast * current_param = *it;

		string entry_name = (*it0)->get_variable_name();
		Data_Type entry_dt = (*it0)->get_data_type();
		Symbol_Table_Entry * current_formal = new Symbol_Table_Entry(entry_name,entry_dt,(*it0)->get_lineno(),sp_ref);
		current_formal->set_symbol_scope(formal);
		current_formal->set_start_offset(-param_offset);
		if(entry_dt == double_data_type){
			param_offset += 8;
			current_formal->set_end_offset(-param_offset);
		}
		else if (entry_dt == int_data_type){
			param_offset += 4;
			current_formal->set_end_offset(-param_offset);
		}

		++it0;
		formal_mem = new Mem_Addr_Opd(*current_formal);

		
		Code_For_Ast *param_code = &(current_param->compile());
		list<Icode_Stmt*> * icl = &(param_code->get_icode_list());
		for(auto const& stmt : *icl){
			ca->append_ics(*stmt);
		}
		rd = param_code->get_reg();
		r = new Register_Addr_Opd(rd);
 
		if(current_formal->get_data_type() != current_param->get_data_type()){
			cout<<"cs316 Error : Actual paramters and formal parameters should match"<<endl;
			exit(0);
		}	

		if(current_formal->get_data_type() == int_data_type) {
			store_stmt = new Move_IC_Stmt(store, r, formal_mem);
		}
		else if (current_formal->get_data_type() == double_data_type){
			store_stmt = new Move_IC_Stmt(store_d, r, formal_mem);
		}
		ca->append_ics(*store_stmt);
		rd->reset_register_occupied();
	}

	call_stmt = new Label_IC_Stmt(jal,procedure_name);
	ca->append_ics(*call_stmt);

	if(procedure_prototype->get_return_type() != void_data_type){
		ca->set_reg(return_value_reg);
	}
	return *ca;
}

Code_For_Ast & Call_Ast::compile_and_optimize_ast(Lra_Outcome & lra){
	Code_For_Ast *ca=NULL;
	return *ca;
}