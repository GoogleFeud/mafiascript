
%{
#include <math.h>
#include <stdio.h>
#include <string>

int yyerror (char const *s);
extern int yylex (void);
%}

%union{
    Base* ast; 
    double number;
    char* string;
    char* id;
    bool boolean;
    bool isNull;
}

%token STRING NUMBER ID BOOL NULL
%token IF ELSE RETURN LOOP LET CONST CONTINUE TYPEOF
%token PLUS MINUS TIMES DIVIDE POWER
%token LEFT RIGHT PARAM_SEPARATOR
%token END

%left PLUS MINUS
%left TIMES DIVIDE
%left NEG
%right POWER

%type <number> NUMBER
%type <string> STRING
%type <id> ID
%type <boolean> BOOL
%type <ast> ID
%type <ast> FunctionDec DeclarationList

%define parse.error verbose
%start Input
%%

Input: %empty;
Input: Input Line;

Expression: 

%%

int yyerror(, char const *s) {
  printf("%s\n", s);
}

void parse() {
    int ret = yyparse();
    if (ret){
	fprintf(stderr, "%d error found.\n",ret);
    }
}

