
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
%token PLUS MINUS TIMES DIVIDE POWER COMPARE NOT ASSIGN NOT_EQUAL OR AND GREATER_THAN GREATER_OR_EQUAL LESS_THAN LESS_OR_EQUAL MODULO
%token PARAN_LEFT PARAN_RIGHT COMMA BRACKET_LEFT BRACKET_RIGHT QUESTION_MARK DOUBLE_DOT S_BRACKET_LEFT S_BRACKET_RIGHT NEXT DOT
%token END_OF_LINE

%left IF ELSE RETURN LOOP LET CONST CONTINUE TYPEOF
%left AND OR 
%left PLUS MINUS
%left TIMES DIVIDE
%left COMAPRE NOT_EQUAL 
%left ASSIGN
%right POWER 

%type <node> NUMBER STRING ID BOOL _NULL 
%type <node> Expression
%type <node> Statement
%type <node> Line Any
%type <node> Block
%type <node> If Define Binary_Operation Assign Literal Ternery Array Object Function Return Loop LoopBeforeInit And_Or Typeof Call Accessor
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
 

Expression: 
Binary_Operation;
| Accessor;
| And_Or;
| Literal;
| Ternery;
| Array;
| Object;
| Function;
| Assign;
| Call;
| Typeof;

Statement: 
If
| Define;
| Return;
| Loop;

Array:
S_BRACKET_LEFT ExpressionList S_BRACKET_RIGHT {$$ = new AST_NODE { new AST_Array($2) }; };

Object:
BRACKET_LEFT PairList BRACKET_RIGHT {$$ = new AST_NODE { new AST_Object($2) }; };

Function:
 PARAN_LEFT VarList PARAN_RIGHT NEXT BRACKET_LEFT Block BRACKET_RIGHT { $$ = new AST_NODE { new AST_Function($2, $6) }; };
| PARAN_LEFT VarList PARAN_RIGHT NEXT Expression { $$ = new AST_NODE { new AST_Function($2, $5) }; };
| S_BRACKET_LEFT VarList S_BRACKET_RIGHT PARAN_LEFT VarList PARAN_RIGHT NEXT BRACKET_LEFT Block BRACKET_RIGHT { $$ = new AST_NODE { new AST_Function($5, $9, $2) }; };
| S_BRACKET_LEFT VarList S_BRACKET_RIGHT PARAN_LEFT VarList PARAN_RIGHT NEXT Expression { $$ = new AST_NODE { new AST_Function($5, $8, $2) }; };

Literal:
NUMBER
| ID
| STRING
| BOOL
| _NULL

If: IF PARAN_LEFT Expression PARAN_RIGHT Any { $$ = new AST_NODE { new AST_If($3, $5, NULL) }; };
| IF PARAN_LEFT Expression PARAN_RIGHT Any ELSE Any { $$ = new AST_NODE { new AST_If($3, $5, $7) }; };

Define:
LET ID { $$ = new AST_NODE { new AST_Declare($2, new AST_NODE { new AST_Null }) }; };
| LET ID ASSIGN Expression {$$ = new AST_NODE { new AST_Declare($2, $4) }; };

Return:
RETURN { $$ = new AST_NODE { new AST_Return() }; };
| RETURN Expression { $$ = new AST_NODE { new AST_Return($2) }; };


Binary_Operation:
Expression PLUS Expression { $$ = new AST_NODE { new AST_Binary($1, $3, BINARY_Ops::OP_ADDITION) };};
| Expression MINUS Expression { $$ = new AST_NODE { new AST_Binary($1, $3, BINARY_Ops::OP_SUBTRACTION) };};
| Expression TIMES Expression { $$ = new AST_NODE { new AST_Binary($1, $3, BINARY_Ops::OP_MULTIPLICATION) };};
| Expression DIVIDE Expression { $$ = new AST_NODE { new AST_Binary($1, $3, BINARY_Ops::OP_DIVISION) };};
| Expression POWER Expression { $$ = new AST_NODE { new AST_Binary($1, $3, BINARY_Ops::OP_POWER) };};
| PARAN_LEFT Expression PARAN_RIGHT {$$ = $2; };
| Expression COMPARE Expression { $$ = new AST_NODE { new AST_Binary($1, $3, BINARY_Ops::OP_EQUAL) }; };
| Expression NOT_EQUAL Expression { $$ = new AST_NODE { new AST_Binary($1, $3, BINARY_Ops::OP_NOT_EQUAL) }; };
| Expression GREATER_THAN Expression { $$ = new AST_NODE { new AST_Binary($1, $3, BINARY_Ops::OP_GREATER_THAN) }; };
| Expression GREATER_OR_EQUAL Expression { $$ = new AST_NODE { new AST_Binary($1, $3, BINARY_Ops::OP_GREATER_OR_EQUAL) }; };
| Expression LESS_THAN Expression { $$ = new AST_NODE { new AST_Binary($1, $3, BINARY_Ops::OP_LESS_THAN) }; };
| Expression LESS_OR_EQUAL Expression { $$ = new AST_NODE { new AST_Binary($1, $3, BINARY_Ops::OP_LESS_OR_EQUAL) }; };
| Expression MODULO Expression { $$ = new AST_NODE { new AST_Binary($1, $3, BINARY_Ops::OP_MODULO) }; };

And_Or:
Expression AND Expression {$$ = new AST_NODE { new AST_And($1, $3) }; };
| Expression OR Expression {$$ = new AST_NODE { new AST_Or($1, $3) }; };

Assign:
ID ASSIGN Expression {$$ = new AST_NODE { new AST_Assign($1, $3) }; };

Ternery:
Expression QUESTION_MARK Expression DOUBLE_DOT Expression { $$ = new AST_NODE { new AST_Ternery($1, $3, $5) }; };

Typeof:
TYPEOF Expression { $$ = new AST_NODE { new AST_Typeof($2) }; };

LoopBeforeInit:
Define { $$ = $1; };
| Expression { $$ = $1; };

Loop:
LOOP PARAN_LEFT LoopBeforeInit COMMA Expression COMMA Expression PARAN_RIGHT Any { $$ = new AST_NODE { new AST_Loop($3, $5, $7, $9) }; };
| LOOP PARAN_LEFT LoopBeforeInit COMMA Expression PARAN_RIGHT Any { $$ = new AST_NODE { new AST_Loop($3, $5, $7) }; };
| LOOP PARAN_LEFT LoopBeforeInit PARAN_RIGHT Any { $$ = new AST_NODE { new AST_Loop($3, $5) }; };

Call:
ID PARAN_LEFT ExpressionList PARAN_RIGHT { $$ = new AST_NODE { new AST_Call($3, $1) }; };
| Call PARAN_LEFT ExpressionList PARAN_RIGHT { $$ = new AST_NODE { new AST_Call($3, $1) }; };

Accessor:
ID DOT ID {$$ = new AST_NODE { new AST_Accessor($1, $3) }; };
| ID S_BRACKET_LEFT Expression S_BRACKET_RIGHT {$$ = new AST_NODE { new AST_Accessor($1, $3) }; };
| ID DOT Call {$$ = new AST_NODE { new AST_Accessor($1, $3) }; };
| Accessor DOT ID { downcast<AST_Accessor*>($$)->push($3); };
| Accessor S_BRACKET_LEFT Expression S_BRACKET_RIGHT { downcast<AST_Accessor*>($$)->push($3); };
| Accessor DOT Call { downcast<AST_Accessor*>($$)->push($3); };

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

