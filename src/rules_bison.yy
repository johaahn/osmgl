%skeleton "lalr1.cc"
%require "2.3"
%defines

%{
#include "rules_bison.h"

#include <iostream>
#include <stdio.h>
#include <string>
using namespace std;
std::string * pstr_reduce = NULL;

 # define YY_DECL                                        \
       yy::parser::token_type                         \
       yylex (yy::parser::semantic_type* yylval,      \
              yy::parser::location_type* yylloc)
%}



%debug
%error-verbose

%locations

%union {
    std::string  * u_string;
    bool u_int;
}

%{
YY_DECL;
%}
%token <u_string> LITERAL
%token LRBRAKET
%token RRBRAKET
%token LSBRAKET
%token RSBRAKET
%token EQUAL
%token QUOTE
%token OR
%token NOT
%token AND
%token END	0	"end of file"
%token EOL

%type <u_int> exp
%type <u_int> tag
%type <u_int> value

%start value

/* precedence et association */
%right EQUAL
%left QUOTE
%%

tag: LSBRAKET LITERAL RSBRAKET EQUAL QUOTE LITERAL QUOTE
    { 
        string str_key = *$2;
        string str_value = *$6;
        printf("key:%s value:%s\n", str_key.c_str(), str_value.c_str());
        $$ = true;
    }

exp: LRBRAKET exp RRBRAKET
	{ $$ = $2; }
	| exp OR exp
    { $$ = $1 || $3; }
    | exp AND exp
    { $$ = $1 && $3; }
    | NOT exp
    { $$ = !$2; }
	| tag
	{ $$ = $1; }

value: exp EOL
    { printf("value:%d\n", $1); }
%%

void yy::parser::error (const yy::location &loc, const std::string &s) {
	fprintf (stderr, "begin line:%d column:%d\n", loc.begin.line, loc.begin.column);
	fprintf (stderr, "end line:%d column:%d\n", loc.end.line, loc.end.column);
	if(pstr_reduce) {
	   fprintf (stderr, "near %s: %s\n",pstr_reduce->c_str(), s.c_str());
	}
	exit(1);
}

int main () {
	yy::parser * parser = new yy::parser();
	int i = parser->parse();
	if(i) {
		cout << "Wrong syntax" << endl;
	}
	return i;
}


