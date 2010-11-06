/*
 * OsmGL rules scanner.
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
#include <string>
#include "rules_bison.h"

//#define yylval calc_lval
#define D(x)
%}
%option noyywrap

whitespaces    [\t\r ]+

%%
"or"                             { D("OR"); return OR; }
"and"                             { D("AND"); return AND; }
"not"                             { D("NOT"); return NOT; }

[a-zA-Z_][a-zA-Z0-9_]*            { D("LITERAL"); yylval.u_string = new std::string (yytext); return LITERAL; }

"("                             { D("LRBRAKET"); return LRBRAKET; }
")"                             { D("RRBRAKET"); return RRBRAKET; }
"["                             { D("LSBRAKET"); return LSBRAKET; }
"]"                             { D("RSBRAKET"); return RSBRAKET; }
"="                             { D("EQUAL"); return EQUAL; }
"'"                             { D("QUOTE"); return QUOTE; }
"\n"                            { D("EOL"); return EOL; }

{whitespaces}  /* eat spaces */

%%

#ifdef FF_LEX_ONLY
main()
{

}
#endif
