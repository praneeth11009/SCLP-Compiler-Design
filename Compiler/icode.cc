#include <string>

template class Const_Opd<double>;
template class Const_Opd<int>;

// Code_For_Ast

Code_For_Ast::Code_For_Ast() {
	list<Icode_Stmt *> *temp=new list<Icode_Stmt *>();
	list<Icode_Stmt *> ics_list=*temp;
}

Code_For_Ast::Code_For_Ast(list<Icode_Stmt *> & ic_l, Register_Descriptor * reg) {
	ics_list = ic_l;
	result_register = reg;
}

void Code_For_Ast::append_ics(Icode_Stmt & ics){
	ics_list.push_back(&ics);
}

list<Icode_Stmt *> & Code_For_Ast::get_icode_list() {
	return ics_list;
}

Register_Descriptor * Code_For_Ast::get_reg(){
	return result_register;
}
void Code_For_Ast::set_reg(Register_Descriptor * reg){
	result_register = reg;
}

Code_For_Ast & Code_For_Ast::operator=(const Code_For_Ast & rhs){
	Code_For_Ast *temp=new Code_For_Ast(rhs);
	ics_list = temp->get_icode_list();
	result_register = temp->get_reg() ;
	return *this;
}

/// Instruction_Descriptor

Instruction_Descriptor::Instruction_Descriptor (Tgt_Op op, string name0, string mnn, string ics, Icode_Format icf, Assembly_Format af){
	inst_op = op;
	mnemonic = mnn;
	name = name0;
	ic_symbol = ics;
	ic_format = icf;
	assem_format = af;
}
Instruction_Descriptor::Instruction_Descriptor(){}

Tgt_Op Instruction_Descriptor::get_op(){
	return inst_op;
}
string Instruction_Descriptor::get_name(){
	return name;
}
string Instruction_Descriptor::get_mnemonic(){
	return mnemonic;
}
string Instruction_Descriptor::get_ic_symbol(){
	return ic_symbol;
}
Icode_Format Instruction_Descriptor::get_ic_format(){
	return ic_format;
}
Assembly_Format Instruction_Descriptor::get_assembly_format(){
	return assem_format;
}

void Instruction_Descriptor::print_instruction_descriptor(ostream & file_buffer){
	file_buffer<<inst_op;
}


/// Icode statement operands

//Ics_opd 
Register_Descriptor * Ics_Opd::get_reg(){}

//Mem_Addr_Opd
Mem_Addr_Opd::Mem_Addr_Opd(Symbol_Table_Entry & se){
	symbol_entry = &se;
}

void Mem_Addr_Opd::print_ics_opd(ostream & file_buffer){
	file_buffer<<symbol_entry->get_variable_name();
}
void Mem_Addr_Opd::print_asm_opd(ostream & file_buffer){
	if (symbol_entry->get_symbol_scope() == global ){
		file_buffer<<symbol_entry->get_variable_name();
	}
	else {
		if (symbol_entry->get_ref_offset()==fp_ref)
			file_buffer<<symbol_entry->get_start_offset()<<"($fp)";
		else
			file_buffer<<symbol_entry->get_start_offset()<<"($sp)";
	}
}

Mem_Addr_Opd & Mem_Addr_Opd::operator= (const Mem_Addr_Opd & rhs){
	//symbol_entry = rhs.symbol_entry; ////////// symbol_entry private ????????????
	//Mem_Addr_Opd *temp=new Mem_Addr_Opd(rhs);
	//*this
	return *this;
}

// Register_Opd

Register_Addr_Opd::Register_Addr_Opd(Register_Descriptor * rd){
	register_description = rd;
}

Register_Descriptor * Register_Addr_Opd::get_reg(){
	return register_description;
}
void Register_Addr_Opd::print_ics_opd(ostream & file_buffer){
	file_buffer<<register_description->get_name();
}
void Register_Addr_Opd::print_asm_opd(ostream & file_buffer){
	file_buffer<<"$"<<register_description->get_name();
}

Register_Addr_Opd & Register_Addr_Opd::operator=(const Register_Addr_Opd & rhs){
	//register_description = rhs.get_reg();
	std::cout<<"regadopd =op invoked\n";
	//memcpy (this, &rhs, sizeof (rhs));
	return *this;
}

// Const_Opd

template <class T>
Const_Opd<T>::Const_Opd (T num0){
	num = num0;
}

template <class T>
void Const_Opd<T>::Const_Opd::print_ics_opd(ostream & file_buffer){
	file_buffer<<num;
}

template <class T>
void Const_Opd<T>::Const_Opd::print_asm_opd(ostream & file_buffer){
	file_buffer<<num;
}

template <class T>
Const_Opd<T> & Const_Opd<T>::Const_Opd::operator=(const Const_Opd & rhs){
	//num = rhs.num; /////////////////// NUM private ?????????????????????????///
	//std::cout<<"const op opd =op invoked\n";
	//memcpy (this, &rhs, sizeof (rhs));
	return *this;
}


/// Icode statements
/*
//Icode_stmt
Instruction_Descriptor & Instruction_Descriptor::get_op(){
	return op_desc;
}
Ics_Opd * Instruction_Descriptor::get_opd1(){}
Ics_Opd * Instruction_Descriptor::get_opd2(){}
Ics_Opd * Instruction_Descriptor::get_result(){}

void Instruction_Descriptor::set_opd1(Ics_Opd * io){}
void Instruction_Descriptor::set_opd2(Ics_Opd * io){}
void Instruction_Descriptor::set_result(Ics_Opd * io){}
*/
//Icode_Stmt
Instruction_Descriptor & Icode_Stmt::get_op(){
	return op_desc;
}

Ics_Opd * Icode_Stmt::get_opd1(){
	Ics_Opd * opd;
	return opd;
}

Ics_Opd * Icode_Stmt::get_opd2(){
	Ics_Opd * opd;
	return opd;
}

Ics_Opd * Icode_Stmt::get_result(){
	Ics_Opd * opd;
	return opd;
}

void Icode_Stmt::set_opd1(Ics_Opd * io){

}
void Icode_Stmt::set_opd2(Ics_Opd * io){

}
void Icode_Stmt::set_result(Ics_Opd * io){

}


//Move_IC_Stmt
Move_IC_Stmt::Move_IC_Stmt(Tgt_Op inst_op, Ics_Opd * opd1, Ics_Opd * result){
	// op_desc = Instruction_Descriptor(inst_op, "move", "MOV", "mov", i_op_r_o1, a_op_r_o1); // CHECK
	op_desc = *machine_desc_object.spim_instruction_table[inst_op];
	this->opd1 = opd1;
	this->result = result;
	//std::cout<<"in move ic stmt constructor\n";
} 
Move_IC_Stmt & Move_IC_Stmt::operator=(const Move_IC_Stmt & rhs){
	Move_IC_Stmt *temp=new Move_IC_Stmt(rhs);
	op_desc = temp->get_inst_op_of_ics();
	opd1 = temp->get_opd1();
	result = temp->get_result();
	return *this;
}

Instruction_Descriptor & Move_IC_Stmt::get_inst_op_of_ics(){
	return op_desc;
}

Ics_Opd * Move_IC_Stmt::get_opd1(){
	return opd1;
}
void Move_IC_Stmt::set_opd1(Ics_Opd * io){
	opd1 = io;
}

Ics_Opd * Move_IC_Stmt::get_result(){
	return result;
}
void Move_IC_Stmt::set_result(Ics_Opd * io){
	result = io;
}

void Move_IC_Stmt::print_icode(ostream & file_buffer){
	//char *s=printf(file_buffer, "\t%s:\t", op_desc.get_name());
	file_buffer<<"\t"<<op_desc.get_name()<<":\t\t";
	result->print_ics_opd(file_buffer);
	file_buffer<<" <- ";
	opd1->print_ics_opd(file_buffer);
	file_buffer<<endl;

}
void Move_IC_Stmt::print_assembly(ostream & file_buffer){
	file_buffer<<"\t"<<op_desc.get_mnemonic()<<" ";
	if ((op_desc.get_name()=="store") || (op_desc.get_name()=="store.d")){
		opd1->print_asm_opd(file_buffer);
		file_buffer<<", ";
		result->print_asm_opd(file_buffer);
		file_buffer<<endl;
	}
	else{
		result->print_asm_opd(file_buffer);
		file_buffer<<", ";
		opd1->print_asm_opd(file_buffer);
		file_buffer<<endl;
	}
}

//Compute_IC_Stmt

Compute_IC_Stmt::Compute_IC_Stmt(Tgt_Op inst_op, Ics_Opd * opd1, Ics_Opd * opd2, Ics_Opd * result){
	// op_desc = Instruction_Descriptor(inst_op, "compute", sprintf("%s",inst_op), sprintf("%s",inst_op), i_op_r_o1_o2, a_op_r_o1_o2); // CHECK
	op_desc = *machine_desc_object.spim_instruction_table[inst_op];
	this->opd1 = opd1;
	this->opd2 = opd2;
	this->result = result;
}

Compute_IC_Stmt & Compute_IC_Stmt::operator=(const Compute_IC_Stmt & rhs){
	Compute_IC_Stmt *temp=new Compute_IC_Stmt(rhs);
	op_desc=temp->get_inst_op_of_ics();
	opd1=temp->get_opd1();
	opd2=temp->get_opd2();
	result=temp->get_result();
	return *this;
}

Instruction_Descriptor & Compute_IC_Stmt::get_inst_op_of_ics(){
	return op_desc;
}

Ics_Opd * Compute_IC_Stmt::get_opd1(){
	return opd1;
}
void Compute_IC_Stmt::set_opd1(Ics_Opd * io){
	opd1 = io;
}

Ics_Opd * Compute_IC_Stmt::get_opd2(){
	return opd2;
}
void Compute_IC_Stmt::set_opd2(Ics_Opd * io){
	opd2 = io;
}

Ics_Opd * Compute_IC_Stmt::get_result(){
	return result;
}
void Compute_IC_Stmt::set_result(Ics_Opd * io){
	result = io;
}

void Compute_IC_Stmt::print_icode(ostream & file_buffer){
	file_buffer<<"\t"<<op_desc.get_name()<<":\t\t";
	result->print_ics_opd(file_buffer);
	file_buffer<<" <- ";
	opd1->print_ics_opd(file_buffer);
	if(opd2!=NULL){
		file_buffer<<" , ";
		opd2->print_ics_opd(file_buffer);
	}
	file_buffer<<endl;
}
void Compute_IC_Stmt::print_assembly(ostream & file_buffer){
	if (result==NULL){
		file_buffer<<"\t"<<op_desc.get_mnemonic()<<" ";
		opd1->print_asm_opd(file_buffer);
		file_buffer<<", ";
		opd2->print_asm_opd(file_buffer);
	}
	else{
		file_buffer<<"\t"<<op_desc.get_mnemonic()<<" ";
		result->print_asm_opd(file_buffer);
		file_buffer<<", ";
		opd1->print_asm_opd(file_buffer);
		if(opd2!=NULL){//could make it cleaner but this will do 
			file_buffer<<", ";
			opd2->print_asm_opd(file_buffer);
		}
	}
	file_buffer<<endl;
}

//Control Flow

Control_Flow_IC_Stmt::Control_Flow_IC_Stmt(Tgt_Op op, Ics_Opd * o1, Ics_Opd * o2, string label,int size){
	// op_desc = Instruction_Descriptor(inst_op, "controlflow", sprintf("%s",inst_op), sprintf("%s",inst_op), i_op_r_o1_o2, a_op_r_o1_o2); // CHECK
	op_desc = *machine_desc_object.spim_instruction_table[op];
	opd1 = o1;
	opd2 = o2;
	offset = label;
	actual_param_size = size;
}
Control_Flow_IC_Stmt & Control_Flow_IC_Stmt::operator=(const Control_Flow_IC_Stmt & rhs){
	Control_Flow_IC_Stmt *temp=new Control_Flow_IC_Stmt(rhs);
	op_desc = temp->get_inst_op_of_ics();
	opd1 = temp->get_opd1();
	opd2 = temp->get_opd2();
	offset = temp->get_Offset();
	
	return *this;
}

Instruction_Descriptor & Control_Flow_IC_Stmt::get_inst_op_of_ics(){
	return op_desc;
}

Ics_Opd * Control_Flow_IC_Stmt::get_opd1(){
	return opd1;
}
void Control_Flow_IC_Stmt::set_opd1(Ics_Opd * io){
	opd1 = io;
}

Ics_Opd * Control_Flow_IC_Stmt::get_opd2(){
	return opd2;
}
void Control_Flow_IC_Stmt::set_opd2(Ics_Opd * io){
	opd2 = io;
}

string Control_Flow_IC_Stmt::get_Offset(){
	return offset;
}
void Control_Flow_IC_Stmt::set_Offset(string label){
	offset = label;
}

void Control_Flow_IC_Stmt::print_icode(ostream & file_buffer){
	if (op_desc.get_name()=="bc1t" || op_desc.get_name()=="bc1f"){
		file_buffer<<"\t"<<op_desc.get_name()<<":\t\t";
		file_buffer<<"goto "<<offset;
		file_buffer<<endl;
	}
	else{
		file_buffer<<"\t"<<op_desc.get_name()<<":\t\t";
		opd1->print_ics_opd(file_buffer);
		file_buffer<<" , zero : goto "<<offset;
		file_buffer<<endl;
	}
}
void Control_Flow_IC_Stmt::print_assembly(ostream & file_buffer){
	if (op_desc.get_name()=="bc1t" || op_desc.get_name()=="bc1f"){
		file_buffer<<"\t"<<op_desc.get_mnemonic()<<" "<<offset;
		file_buffer<<endl;
	}
	else{
		file_buffer<<"\t"<<op_desc.get_mnemonic()<<" ";
		opd1->print_asm_opd(file_buffer);
		file_buffer<<", $zero, "<<offset<<" ";
		file_buffer<<endl;
	}
}

// Label IC Stmt
Label_IC_Stmt::Label_IC_Stmt(Tgt_Op inst_op, string label){
	// op_desc = Instruction_Descriptor(inst_op, "label", sprintf("%s",inst_op), sprintf("%s",inst_op), i_op_st, a_op_st); // CHECK
	op_desc = *machine_desc_object.spim_instruction_table[inst_op];
	this->label = label;
}

Label_IC_Stmt & Label_IC_Stmt::operator=(const Label_IC_Stmt & rhs){
	Label_IC_Stmt *temp=new Label_IC_Stmt(rhs);
	op_desc = temp->get_inst_op_of_ics();
	label = temp->get_label();

	return *this;
}

Instruction_Descriptor & Label_IC_Stmt::get_inst_op_of_ics(){
	return op_desc;
}

string Label_IC_Stmt::get_label(){
	return label;
}
void Label_IC_Stmt::set_label(string label){
	this->label = label;
}

void Label_IC_Stmt::print_icode(ostream & file_buffer){
	if (op_desc.get_name()=="jump"){
		file_buffer<<"\tgoto "<<label;
		file_buffer<<endl;
	}
	else if (op_desc.get_op() == ret_inst){
		file_buffer<<"\treturn"<<endl;
	}
	else if (op_desc.get_op() == jal){
		file_buffer<<"\tcall "<<label<<endl;
	}
	else if (label == "syscall"){
		file_buffer<<"\t"<<"print"<<endl;
	}
	else{
		file_buffer<<"\n"<<label<<":"<<endl;
	}
}

void Label_IC_Stmt::print_assembly(ostream & file_buffer){	
	if (op_desc.get_name()=="jump"){
		file_buffer<<"\tj "<<label;
		file_buffer<<endl;
	}
	else if (op_desc.get_op()==ret_inst){
		file_buffer<<"\t"<<"j epilogue_"<<label<<endl;
	}
	else if (op_desc.get_op() == jal){
		Procedure * procedure_prototype = program_object.get_procedure_prototype(label);
		Symbol_Table formal_table = procedure_prototype->get_formal_param_list();
		int size_of_table = formal_table.get_size();

		file_buffer<<"\t"<<"sub $sp, $sp, "<<size_of_table-8<<endl;
		file_buffer<<"\t"<<"jal "<<label<<endl;
		file_buffer<<"\t"<<"add $sp, $sp, "<<size_of_table-8<<endl;
	}
	else if (label=="syscall"){
		file_buffer<<"\t"<<label<<endl;
	}
	else {
		file_buffer<<"\n"<<label<<":"<<endl;
	}
	
}