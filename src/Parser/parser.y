
%{
#include <math.h>
#include <stdio.h>
#include <string>
#include "AST/statements.h"

int yyerror (char const *s);
extern int yylex (void);


%}

%union{
    AST_ANY* any; 
    AST_EXPRESSION* exp;
}

%token STRING NUMBER ID BOOL _NULL
%token IF ELSE RETURN LOOP LET CONST CONTINUE TYPEOF
%token PLUS MINUS TIMES DIVIDE POWER
%token PARAN_LEFT PARAN_RIGHT PARAM_SEPARATOR
%token END_OF_LINE

%left PLUS MINUS
%left TIMES DIVIDE
%left NEG
%right POWER

%type <exp> NUMBER STRING ID BOOL _NULL
%type <exp> Expression

%define parse.error verbose
%start Input
%%

Input: %empty 
| Input Line { printf("Input code block!\n"); };

Line: %empty;
| Expression END_OF_LINE { printf("expression\n"); };
/* | Statement END_OF_LINE { printf("statement\n"); }; */
| Line Expression END_OF_LINE { printf("line expression\n"); };
/* | Line Statement END_OF_LINE { printf("line statement\n"); }; */


Expression: NUMBER
| STRING
| ID
| BOOL
| _NULL
| Expression PLUS Expression { std::string op = "+"; $$ = new AST_EXPRESSION { new AST_Binary($1, $3, op) }; printf("Plusss!"); };
| Expression MINUS Expression {std::string op = "-"; $$ = new AST_EXPRESSION { new AST_Binary($1, $3, op) };};
| Expression TIMES Expression {std::string op = "*"; $$ = new AST_EXPRESSION { new AST_Binary($1, $3, op) };};
| Expression DIVIDE Expression {std::string op = "/"; $$ = new AST_EXPRESSION { new AST_Binary($1, $3, op) };};
| Expression POWER Expression {std::string op = "^"; $$ = new AST_EXPRESSION { new AST_Binary($1, $3, op) };};
| PARAN_LEFT Expression PARAN_RIGHT {$$ = $2; };

%%

int yyerror(char const *s) {
  printf("%s\n", s);
  return 1;
}

void parse() {
    int ret = yyparse();
    if (ret){
	fprintf(stderr, "%d error found.\n",ret);
    }
}

