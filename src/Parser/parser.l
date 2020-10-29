%option noyywrap

%{
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

{NUMBER} { yylval.number = atof(yytext); return NUMBER; }
{ID} { yylval.id = yytext; return ID; }
{STRING} {yylval.string = yytext; return STRING; }
{BOOL} {yylval.boolean = (yytext == "true") ? true:false; return BOOL; }
{NULL} {yylval.isNull = true; return NULL; }

if {return IF; };
else {return ELSE; };
return {return RETURN; };
loop {return LOOP; };
let {return LET; };
const {return CONST; };
continue {return CONTINUE; };
typeof {return TYPEOF; };

"+" return PLUS;
"-" return MINUS;
"*" return TIMES;
"/" return DIVIDE;
"^" return POWER;
"(" return LEFT;
")" return RIGHT;
"," return PARAM_SEPARATOR;

"\n" return END;
