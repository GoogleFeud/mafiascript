
%{
#include <math.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include "AST/statements.h"

int yyerror (char const *s);
extern int yylex (void);
extern FILE *yyin;
extern int lineNum;
%}

%union{
    AST_ANY* any; 
    AST_EXPRESSION* exp;
}

%token STRING NUMBER ID BOOL _NULL
%token IF ELSE RETURN LOOP LET CONST CONTINUE TYPEOF
%token PLUS MINUS TIMES DIVIDE POWER
%token PARAN_LEFT PARAN_RIGHT PARAM_SEPARATOR BRACKET_LEFT BRACKET_RIGHT
%token END_OF_LINE

%left PLUS MINUS
%left TIMES DIVIDE
%left NEG
%right POWER

%type <exp> NUMBER STRING ID BOOL _NULL
%type <exp> Expression
%type <any> Statement
%type <any> Line
%type <any> Block

%define parse.error verbose
%start Input
%%

Input: %empty 
| Input Line { printf("Input code block!\n"); };

Line: %empty;
| Expression END_OF_LINE { $$ = expressionToAny($1); };
| Statement END_OF_LINE { $$ = $1; };

Block: %empty { $$ = new AST_ANY { new AST_Block() }; };
| Line { AST_Block* block = new AST_Block(); block->push($1); $$ = new AST_ANY {block}; }
| Block Line { downcast<AST_Block*>($$)->push($2); };

Expression: NUMBER
| STRING
| ID
| BOOL
| _NULL
| Expression PLUS Expression { std::string op = "+"; $$ = new AST_EXPRESSION { new AST_Binary($1, $3, op) };};
| Expression MINUS Expression {std::string op = "-"; $$ = new AST_EXPRESSION { new AST_Binary($1, $3, op) };};
| Expression TIMES Expression {std::string op = "*"; $$ = new AST_EXPRESSION { new AST_Binary($1, $3, op) };};
| Expression DIVIDE Expression {std::string op = "/"; $$ = new AST_EXPRESSION { new AST_Binary($1, $3, op) };};
| Expression POWER Expression {std::string op = "^"; $$ = new AST_EXPRESSION { new AST_Binary($1, $3, op) };};
| PARAN_LEFT Expression PARAN_RIGHT {$$ = $2; };

Statement: IF PARAN_LEFT Expression PARAN_RIGHT Block {
    printf("If block!");
}

%%

int yyerror(char const *s) {
  std::cout<<"Error at line " << lineNum << ": " << s <<std::endl;
  return 1;
}

void parse(FILE* content) {
    yyin = content;
    int ret = yyparse();
    if (ret){
	fprintf(stderr, "%d error found.\n",ret);
    }
}

