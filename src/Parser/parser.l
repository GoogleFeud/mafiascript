%option noyywrap

%{
#include "AST/statements.h"
#include "parser.tab.h"
#include <string>
%}

STRING \"([^\\\"]|\\.)*\"
NUMBER [+-]?([0-9]+([.][0-9]*)?|[.][0-9]+)
ID [_a-zA-Z][_a-zA-Z0-9]*
KEYWORD let|const|if|else|loop|return|continue
BOOL true|false
NULL null
THEN "=>"


%%
"if" {return IF; };
"else" {return ELSE; };
"return" {return RETURN; };
"loop" {return LOOP; };
"let" {return LET; };
"const" {return CONST; };
"continue" {return CONTINUE; };
"typeof" {return TYPEOF; };


{NUMBER} { yylval.exp = new AST_EXPRESSION { new AST_Number(atof(yytext)) }; return NUMBER; }
{BOOL} {yylval.exp = new AST_EXPRESSION { (yytext == "true") ? new AST_Bool(true):new AST_Bool(false) }; return BOOL; }
{NULL} {yylval.exp = new AST_EXPRESSION { new AST_Null }; return _NULL; }
{ID} { std::string res = std::string { yytext }; yylval.exp = new AST_EXPRESSION {new AST_Var(res) }; return ID; }
{STRING} {std::string res = std::string { yytext }; yylval.exp = new AST_EXPRESSION { new AST_String(res) }; return STRING; }

"+" return PLUS;
"-" return MINUS;
"*" return TIMES;
"/" return DIVIDE;
"^" return POWER;
"(" return PARAN_LEFT;
")" return PARAN_RIGHT;
"," return PARAM_SEPARATOR;

";" return END_OF_LINE;
