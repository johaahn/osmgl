/*
 * OsmGL rules parser.
 *
 * Copyright (C) 2010 <Johann Baudy> johann.baudy@gnu-log.net
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3.0 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */
%{


#include <iostream>
#include <stdio.h>
#include <string>
using namespace std;

#include "rules_bison.h"

int yyerror(char* errstr);
int yylex (void);

%}



%debug
%error-verbose

%locations

%union {
    std::string  * u_string;
    bool u_int;
}

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

int yyerror(char* errstr) {
	printf("Error: %s\n", errstr);
	return EXIT_FAILURE;
}

