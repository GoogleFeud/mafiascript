%option noyywrap

%{
#include <iostream>
#include "AST/statements.h"
#include "parser.tab.h"
#include <string>

int lineNum = 1;

void setCode(std::string &code) {
    yy_scan_string(const_cast<char*>(code.c_str()));
};
%}

STRING \"([^\\\"]|\\.)*\"
NUMBER [+-]?([0-9]+([.][0-9]*)?|[.][0-9]+)
ID [_a-zA-Z][_a-zA-Z0-9]*
KEYWORD let|const|if|else|loop|return|continue
BOOL true|false
NULL null
THEN "=>"


%%
"if" return IF;
"else" return ELSE;
"return" return RETURN;
"loop" return LOOP;
"let" return LET;
"const" return CONST;
"continue" return CONTINUE;
"typeof" return TYPEOF;
"and" return AND;
"or" return OR;


{NUMBER} { yylval.node = new AST_NODE { new AST_Number(atof(yytext)) }; return NUMBER; }
"true" {yylval.node = new AST_NODE { new AST_Bool(true) }; return BOOL; }
"false" {yylval.node = new AST_NODE { new AST_Bool(false) }; return BOOL; }
{NULL} {yylval.node = new AST_NODE { new AST_Null }; return _NULL; }
{ID} { std::string res = std::string { yytext }; yylval.node = new AST_NODE { new AST_Var(res) }; return ID; }
{STRING} {
    char* str = (char*)malloc(yyleng - 1);
    memcpy(str, yytext + 1, yyleng - 2);
    str[yyleng - 2] = 0;
    yylval.node = new AST_NODE { new AST_String(std::string(str)) };
    return STRING; 
    }

"=" return ASSIGN;
"==" return COMPARE;
"!=" return NOT_EQUAL;
"!" return NOT;
"+" return PLUS;
"-" return MINUS;
"*" return TIMES;
"/" return DIVIDE;
"^" return POWER;
"(" return PARAN_LEFT;
")" return PARAN_RIGHT;
"{" return BRACKET_LEFT;
"}" return BRACKET_RIGHT;
"[" return S_BRACKET_LEFT;
"]" return S_BRACKET_RIGHT;
"," return COMMA;
"?" return QUESTION_MARK;
":" return DOUBLE_DOT;
"=>" return NEXT;
"&&" return AND;
"||" return OR;
">" return GREATER_THAN;
">=" return GREATER_OR_EQUAL;
"<" return LESS_THAN;
"<=" return LESS_OR_EQUAL;
"%" return MODULO;
 
";" return END_OF_LINE;
"\n" { ++lineNum; };
%%