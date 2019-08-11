//debug
// #include <iostream>
// using namespace std;
///Eval_Result

int Eval_Result::get_int_value(){
	return 0;
}

void Eval_Result::set_value(int value){
	
}
double Eval_Result::get_double_value(){
	return 0;
}

void Eval_Result::set_value(double value){
	
}

bool Eval_Result::is_variable_defined(){
	return true;
}

void Eval_Result::set_variable_status(bool def){

}

///Eval_Result_Value

int Eval_Result_Value::get_int_value(){
	return 0;
}

void Eval_Result_Value::set_value(int value){
	
}
double Eval_Result_Value::get_double_value(){
	return 0;
}

void Eval_Result_Value::set_value(double value){
	
}

///Eval_Result_Value_Int

Eval_Result_Value_Int::Eval_Result_Value_Int(){
	defined=false;
	//value=0;
	result_type=int_result;
}

Eval_Result_Value_Int::~Eval_Result_Value_Int(){

}

void Eval_Result_Value_Int::set_value(int number){
	value=number;
	defined=true;
	
}

void Eval_Result_Value_Int::set_value(double number){
	//doesn't reach here?
}

int Eval_Result_Value_Int::get_int_value(){
	return value;
}

void Eval_Result_Value_Int::set_variable_status(bool def){
	defined=def;
}

bool Eval_Result_Value_Int::is_variable_defined(){
	return defined;
}

void Eval_Result_Value_Int::set_result_enum(Result_Enum res){
	result_type=res;
}

Result_Enum Eval_Result_Value_Int::get_result_enum(){
	return result_type;
}

///Eval_Result_Value_Double

Eval_Result_Value_Double::Eval_Result_Value_Double(){
	defined=false;
	//value=0;
	result_type=double_result;
}

Eval_Result_Value_Double::~Eval_Result_Value_Double(){

}

void Eval_Result_Value_Double::set_value(int number){
	//doesn't reach here?	
	value=number;
    defined = true;
}

void Eval_Result_Value_Double::set_value(double number){
	value=number;
	defined=true;
}

double Eval_Result_Value_Double::get_double_value(){
	return value;
}

void Eval_Result_Value_Double::set_variable_status(bool def){
	defined=def;
}

bool Eval_Result_Value_Double::is_variable_defined(){
	return defined;
}

void Eval_Result_Value_Double::set_result_enum(Result_Enum res){
	result_type=res;
}

Result_Enum Eval_Result_Value_Double::get_result_enum(){
	return result_type;
}

///Local_Environment
Local_Environment::Local_Environment(){

}

Local_Environment::~Local_Environment(){

}

void Local_Environment::print(ostream & file_buffer){
	//int num_var=0;//debug
	for (auto const& x : variable_table){
		file_buffer<<AST_SPACE<<x.first<<" : ";
		if (x.second->is_variable_defined()){
			if (x.second->get_result_enum()==int_result){
				file_buffer<<x.second->get_int_value()<<"\n";
			}
			else if (x.second->get_result_enum()==double_result){
				file_buffer<<x.second->get_double_value()<<"\n";
			}
		}
		else{
			file_buffer<<"undefined\n";
		}
		//num_var++;
	}
	//file_buffer<<num_var<<"\n";
}

bool Local_Environment::is_variable_defined(string name){
	if (does_variable_exist(name)){
		return variable_table[name]->is_variable_defined();
	}
	return false;
}

Eval_Result * Local_Environment::get_variable_value(string name){
	Eval_Result* er=NULL;
	if (does_variable_exist(name)){
		return variable_table[name];
	}
	return er;
}

void Local_Environment::put_variable_value(Eval_Result & value, string name){
	//variable_table.insert({name, value});
	variable_table[name]=&value;
}

bool Local_Environment::does_variable_exist(string name){
	return (variable_table.find(name) != variable_table.end());
}
