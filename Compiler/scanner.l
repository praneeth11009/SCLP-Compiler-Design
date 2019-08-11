%{
#include <string>
#include "parser.tab.h"
#include <stdlib.h>
%}

digit [0-9]
operator [-+*/]
namestart [a-zA-Z_]
metachar [,;(){}"'?:]
spaces [ \t]
sign [+-]

%option yylineno

%%
\n ;
{spaces}+ ;
"//".* ;
int {	
	store_token_name("INTEGER",yytext,yylineno);
	return INTEGER;
	//printf("\tToken Name: %s \tToken: %s \t Lineno: %d\n","INTEGER",yytext,yylineno);
	}
float {	
	store_token_name("FLOAT",yytext,yylineno);
	return FLOAT;
	//printf("\tToken Name: %s \tToken: %s \t Lineno: %d\n","FLOAT",yytext,yylineno);
	}
void {	
	store_token_name("VOID",yytext,yylineno);
	return VOID;
	//printf("\tToken Name: %s \tToken: %s \t Lineno: %d\n","VOID",yytext,yylineno);
	}
return {
	store_token_name("RETURN",yytext,yylineno);
	return RETURN;	
}

while {
	store_token_name("WHILE",yytext,yylineno);
	return WHILE;
	}

do {
	store_token_name("DO",yytext,yylineno);
	return DO;	
	}

if {
	store_token_name("IF",yytext,yylineno);
	return IF;
	}

else {
	store_token_name("ELSE",yytext,yylineno);
	return ELSE;
	}

print {
	store_token_name("PRINT",yytext,yylineno);
	return PRINT;
	}

{digit}+ {
	store_token_name("NUM",yytext,yylineno);
	yylval.integer_value = atoi(yytext);
	return INTEGER_NUMBER;
	//printf("\tToken Name: %s \tToken: %s \t Lineno: %d\n","NUM",yytext,yylineno);
	}
{digit}+"."{digit}+ {
	store_token_name("FNUM",yytext,yylineno);
	yylval.double_value = atof(yytext);
	return DOUBLE_NUMBER;
	//printf("\tToken Name: %s \tToken: %s \t Lineno: %d\n","FNUM",yytext,yylineno);
	}
{namestart}({namestart}|{digit})* {

	int len=strlen(yytext);
	char *name= (char*) malloc(len + 1 + 1 ); 
	strcpy(name, yytext);
	name[len]='_';
	name[len+1]='\0';
	store_token_name("NAME",yytext,yylineno);//yytext
	yylval.string_value = new string(yytext);
	return NAME;
	//printf("\tToken Name: %s \tToken: %s \t Lineno: %d\n","NAME",yytext,yylineno);
	}  
{metachar} {
	store_token_name("META CHAR",yytext,yylineno);
	//yylval.integer_value = atoi(yytext);
	return (int)yytext[0] ;
	//printf("\tToken Name: %s \tToken: %s \t Lineno: %d\n","META CHAR",yytext,yylineno);
	}
{operator} {
	store_token_name("ARITHOP",yytext,yylineno);
	//yylval.integer_value = atoi(yytext);
	return (int)yytext[0];
	//printf("\tToken Name: %s \tToken: %s \t Lineno: %d\n","ARITHOP",yytext,yylineno);
	}
"<=" {
	store_token_name("LESS_THAN_EQUAL",yytext,yylineno);
	return LESS_THAN_EQUAL;
	}

">=" {
	store_token_name("GREATER_THAN_EQUAL",yytext,yylineno);
	return GREATER_THAN_EQUAL;	
	}

== {
	store_token_name("EQUAL",yytext,yylineno);
	return EQUAL;	
	}

"=" {
	store_token_name("ASSIGN_OP",yytext,yylineno);
	//yylval.integer_value = atoi(yytext);
	return ASSIGN;
	//printf("\tToken Name: %s \tToken: %s \t Lineno: %d\n","ASSIGN_OP",yytext,yylineno);
	}

"<" {
	store_token_name("LESS_THAN",yytext,yylineno);
	return LESS_THAN;
	}

">" {
	store_token_name("GREATER_THAN",yytext,yylineno);
	return GREATER_THAN;
	}

"!=" {
	store_token_name("NOT_EQUAL",yytext,yylineno);
	return NOT_EQUAL;
	}

"&&" {
	store_token_name("AND",yytext,yylineno);
	return AND;
	}

"||" {
	store_token_name("OR",yytext,yylineno);
	return OR;	
	}

"!" {
	store_token_name("NOT",yytext,yylineno);
	return NOT;
	}



. {
	printf("cs316: Error Line: %d: scanner internal error--no action found\n",yylineno);
	}


%%

int yywrap(void){
	return 1;
}



