
%{
#include <math.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include "AST/statements.h"

int yyerror (char const *s);
extern int yylex (void);
extern void setCode(std::string &code);
extern FILE *yyin;
extern int lineNum;

AST_Block* prog;
%}

%union{
    AST_NODE* node;
    AST_Block* block; 
}

%token STRING NUMBER ID BOOL _NULL
%token IF ELSE RETURN LOOP LET CONST CONTINUE TYPEOF
%token PLUS MINUS TIMES DIVIDE POWER COMPARE NOT ASSIGN
%token PARAN_LEFT PARAN_RIGHT PARAM_SEPARATOR BRACKET_LEFT BRACKET_RIGHT
%token END_OF_LINE

%left PLUS MINUS
%left TIMES DIVIDE
%left NEG
%right POWER

%type <node> NUMBER STRING ID BOOL _NULL
%type <node> Expression
%type <node> Statement
%type <node> Line
%type <node> Block
%type <block> Input

%define parse.error verbose
%start Input
%%

Input: %empty { $$ = prog; }; 
| Input Line { $$->push($2); };

Line:  Expression END_OF_LINE { $$ = $1; };
| Statement END_OF_LINE { $$ = $1; };

Block: %empty { $$ = new AST_NODE { new AST_Block }; };
| Line { AST_Block* block = new AST_Block; block->push($1); $$ = new AST_NODE {block}; }
| Block Line { downcast<AST_Block*>($$)->push($2); };

Expression: NUMBER
| STRING
| ID
| BOOL
| _NULL
| Expression PLUS Expression { std::string op = "+"; $$ = new AST_NODE { new AST_Binary($1, $3, op) };};
| Expression MINUS Expression {std::string op = "-"; $$ = new AST_NODE { new AST_Binary($1, $3, op) };};
| Expression TIMES Expression {std::string op = "*"; $$ = new AST_NODE { new AST_Binary($1, $3, op) };};
| Expression DIVIDE Expression {std::string op = "/"; $$ = new AST_NODE { new AST_Binary($1, $3, op) };};
| Expression POWER Expression {std::string op = "^"; $$ = new AST_NODE { new AST_Binary($1, $3, op) };};
| PARAN_LEFT Expression PARAN_RIGHT {$$ = $2; };
| Expression COMPARE Expression {std::string op = "=="; $$ = new AST_NODE { new AST_Binary($1, $3, op) }; };
| Expression NOT COMPARE Expression {std::string op = "!="; $$ = new AST_NODE { new AST_Binary($1, $4, op) }; };

Statement: IF PARAN_LEFT Expression PARAN_RIGHT BRACKET_LEFT Block BRACKET_RIGHT { $$ = new AST_NODE { new AST_If($3, $6, NULL) }; };
| IF PARAN_LEFT Expression PARAN_RIGHT Expression { $$ = new AST_NODE { new AST_If($3, new AST_NODE { new AST_Block($5) }, NULL) }; };
| RETURN Expression { $$ = new AST_NODE { new AST_Return($2) }; };

%%

int yyerror(char const *s) {
  std::cout<<"Error at line " << lineNum << ": " << s <<std::endl;
  return 1;
}

AST_Block* parse(std::string &code) {
    prog = new AST_Block();
    setCode(code);
    int ret = yyparse();
    if (ret){
	fprintf(stderr, "%d error found.\n",ret);
    }
    return prog;
}

