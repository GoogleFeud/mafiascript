
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
    AST_PairList* pairList;
}

%token STRING NUMBER ID BOOL _NULL 
%token IF ELSE RETURN LOOP LET CONST CONTINUE TYPEOF
%token PLUS MINUS TIMES DIVIDE POWER COMPARE NOT ASSIGN NOT_EQUAL
%token PARAN_LEFT PARAN_RIGHT COMMA BRACKET_LEFT BRACKET_RIGHT QUESTION_MARK DOUBLE_DOT S_BRACKET_LEFT S_BRACKET_RIGHT NEXT
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
%type <node> If Define Binary_Operation Assign Literal Ternery Array Object Function Return Loop LoopBeforeInit
%type <block> Input 
%type <list> ExpressionList VarList
%type <pairList> PairList

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

PairList: %empty {$$ = new AST_PairList; };
| STRING DOUBLE_DOT Expression {$$ = new AST_PairList($1, $3); };
| PairList COMMA STRING DOUBLE_DOT Expression { $$->push($3, $5); };

VarList: %empty { $$ = new AST_List; };
| ID {$$ = new AST_List($1); };
| VarList COMMA ID { $$->push($3); };
 

Expression: Literal
| Binary_Operation
| Assign
| Ternery
| Array
| Object
| Function


Statement: 
If
| Define
| Return
| Loop

Array:
S_BRACKET_LEFT ExpressionList S_BRACKET_RIGHT {$$ = new AST_NODE { new AST_Array($2) }; };

Object:
BRACKET_LEFT PairList BRACKET_RIGHT {$$ = new AST_NODE { new AST_Object($2) }; };

Function:
 PARAN_LEFT VarList PARAN_RIGHT NEXT BRACKET_LEFT Block BRACKET_RIGHT { $$ = new AST_NODE { new AST_Function($2, $6) }; };
| S_BRACKET_LEFT VarList S_BRACKET_RIGHT PARAN_LEFT VarList PARAN_RIGHT NEXT BRACKET_LEFT Block BRACKET_RIGHT { $$ = new AST_NODE { new AST_Function($5, $9, $2) }; };

Literal:
NUMBER
| STRING
| ID
| BOOL
| _NULL

If: IF PARAN_LEFT Expression PARAN_RIGHT Any { $$ = new AST_NODE { new AST_If($3, $5, NULL) }; };
| IF PARAN_LEFT Expression PARAN_RIGHT Any ELSE Any { $$ = new AST_NODE { new AST_If($3, $5, $7) }; };

Define:
LET ID { $$ = new AST_NODE { new AST_Declare($2, new AST_NODE { new AST_Null }) }; };
| LET ID ASSIGN Expression {$$ = new AST_NODE { new AST_Declare($2, $4) }; };

Return:
RETURN Expression { $$ = new AST_NODE { new AST_Return($2) }; };


Binary_Operation:
Expression PLUS Expression { std::string op = "+"; $$ = new AST_NODE { new AST_Binary($1, $3, op) };};
| Expression MINUS Expression {std::string op = "-"; $$ = new AST_NODE { new AST_Binary($1, $3, op) };};
| Expression TIMES Expression {std::string op = "*"; $$ = new AST_NODE { new AST_Binary($1, $3, op) };};
| Expression DIVIDE Expression {std::string op = "/"; $$ = new AST_NODE { new AST_Binary($1, $3, op) };};
| Expression POWER Expression {std::string op = "^"; $$ = new AST_NODE { new AST_Binary($1, $3, op) };};
| PARAN_LEFT Expression PARAN_RIGHT {$$ = $2; };
| Expression COMPARE Expression {std::string op = "=="; $$ = new AST_NODE { new AST_Binary($1, $3, op) }; };
| Expression NOT_EQUAL Expression {std::string op = "!="; $$ = new AST_NODE { new AST_Binary($1, $3, op) }; };

Assign:
ID ASSIGN Expression {$$ = new AST_NODE { new AST_Assign($1, $3) }; };

Ternery:
Expression QUESTION_MARK Expression DOUBLE_DOT Expression { $$ = new AST_NODE { new AST_Ternery($1, $3, $5) }; };

LoopBeforeInit:
Define { $$ = $1; };
| Expression { $$ = $1; };

Loop:
LOOP PARAN_LEFT LoopBeforeInit COMMA Expression COMMA Expression PARAN_RIGHT Any { $$ = new AST_NODE { new AST_Loop($3, $5, $7, $9) }; };
| LOOP PARAN_LEFT LoopBeforeInit COMMA Expression PARAN_RIGHT Any { $$ = new AST_NODE { new AST_Loop($3, $5, $7) }; };
| LOOP PARAN_LEFT LoopBeforeInit PARAN_RIGHT Any { $$ = new AST_NODE { new AST_Loop($3, $5) }; };

%%

int yyerror(char const *s) {
  std::cout<<"Error at line " << lineNum << ": " << s <<std::endl;
  return 1;
}

AST_Block* parseAST(std::string &code) {
    prog = new AST_Block();
    setCode(code);
    int ret = yyparse();
    if (ret){
	fprintf(stderr, "%d error found.\n",ret);
    }
    return prog;
}

