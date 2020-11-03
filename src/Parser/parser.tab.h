/* A Bison parser, made by GNU Bison 3.7.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_SRC_PARSER_PARSER_TAB_H_INCLUDED
# define YY_YY_SRC_PARSER_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    STRING = 258,                  /* STRING  */
    NUMBER = 259,                  /* NUMBER  */
    ID = 260,                      /* ID  */
    BOOL = 261,                    /* BOOL  */
    _NULL = 262,                   /* _NULL  */
    IF = 263,                      /* IF  */
    ELSE = 264,                    /* ELSE  */
    RETURN = 265,                  /* RETURN  */
    LOOP = 266,                    /* LOOP  */
    LET = 267,                     /* LET  */
    CONST = 268,                   /* CONST  */
    CONTINUE = 269,                /* CONTINUE  */
    TYPEOF = 270,                  /* TYPEOF  */
    PLUS = 271,                    /* PLUS  */
    MINUS = 272,                   /* MINUS  */
    TIMES = 273,                   /* TIMES  */
    DIVIDE = 274,                  /* DIVIDE  */
    POWER = 275,                   /* POWER  */
    COMPARE = 276,                 /* COMPARE  */
    NOT = 277,                     /* NOT  */
    ASSIGN = 278,                  /* ASSIGN  */
    PARAN_LEFT = 279,              /* PARAN_LEFT  */
    PARAN_RIGHT = 280,             /* PARAN_RIGHT  */
    COMMA = 281,                   /* COMMA  */
    BRACKET_LEFT = 282,            /* BRACKET_LEFT  */
    BRACKET_RIGHT = 283,           /* BRACKET_RIGHT  */
    QUESTION_MARK = 284,           /* QUESTION_MARK  */
    DOUBLE_DOT = 285,              /* DOUBLE_DOT  */
    S_BRACKET_LEFT = 286,          /* S_BRACKET_LEFT  */
    S_BRACKET_RIGHT = 287,         /* S_BRACKET_RIGHT  */
    END_OF_LINE = 288,             /* END_OF_LINE  */
    NEG = 289                      /* NEG  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 18 "./src/Parser/parser.y"

    AST_NODE* node;
    AST_Block* block; 
    AST_List* list;
    AST_PairList* pairList;

#line 105 "src/Parser/parser.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_SRC_PARSER_PARSER_TAB_H_INCLUDED  */
