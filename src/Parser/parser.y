
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
    AST_List* list;
}

%token STRING NUMBER ID BOOL _NULL 
%token IF ELSE RETURN LOOP LET CONST CONTINUE TYPEOF
%token PLUS MINUS TIMES DIVIDE POWER COMPARE NOT ASSIGN 
%token PARAN_LEFT PARAN_RIGHT COMMA BRACKET_LEFT BRACKET_RIGHT QUESTION_MARK DOUBLE_DOT S_BRACKET_LEFT S_BRACKET_RIGHT
%token END_OF_LINE

%left PLUS MINUS
%left TIMES DIVIDE
%left NEG
%right POWER

%type <node> NUMBER STRING ID BOOL _NULL 
%type <node> Expression
%type <node> Statement
%type <node> Line Any
%type <node> Block
%type <node> If
%type <block> Input
%type <list> ExpressionList

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

Any: Expression { $$ = $1; };
| Statement { $$ = $1; };
| BRACKET_LEFT Block BRACKET_RIGHT {$$ = $2; };

ExpressionList: %empty { $$ = new AST_List; };
| Expression { $$ = new AST_List($1); };
| ExpressionList COMMA Expression { $$->push($3); };

Expression: NUMBER
| STRING
| ID
| BOOL
| _NULL
// Binary 
| Expression PLUS Expression { std::string op = "+"; $$ = new AST_NODE { new AST_Binary($1, $3, op) };};
| Expression MINUS Expression {std::string op = "-"; $$ = new AST_NODE { new AST_Binary($1, $3, op) };};
| Expression TIMES Expression {std::string op = "*"; $$ = new AST_NODE { new AST_Binary($1, $3, op) };};
| Expression DIVIDE Expression {std::string op = "/"; $$ = new AST_NODE { new AST_Binary($1, $3, op) };};
| Expression POWER Expression {std::string op = "^"; $$ = new AST_NODE { new AST_Binary($1, $3, op) };};
| PARAN_LEFT Expression PARAN_RIGHT {$$ = $2; };
| Expression COMPARE Expression {std::string op = "=="; $$ = new AST_NODE { new AST_Binary($1, $3, op) }; };
| Expression NOT COMPARE Expression {std::string op = "!="; $$ = new AST_NODE { new AST_Binary($1, $4, op) }; };
// Assign
| ID ASSIGN Expression {$$ = new AST_NODE { new AST_Assign($1, $3) }; };
//Ternery
| Expression QUESTION_MARK Expression DOUBLE_DOT Expression { $$ = new AST_NODE { new AST_Ternery($1, $3, $5) }; };
//Array
| S_BRACKET_LEFT ExpressionList S_BRACKET_RIGHT {$$ = new AST_NODE { new AST_Array($2) }; };
 
Statement: If;
| LET ID ASSIGN Expression {$$ = new AST_NODE { new AST_Declare($2, $4) }; };
| RETURN Expression { $$ = new AST_NODE { new AST_Return($2) }; };

If: IF PARAN_LEFT Expression PARAN_RIGHT Any { $$ = new AST_NODE { new AST_If($3, $5, NULL) }; };
| IF PARAN_LEFT Expression PARAN_RIGHT Any ELSE Any { $$ = new AST_NODE { new AST_If($3, $5, $7) }; };
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

