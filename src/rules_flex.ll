%{
#include "rules_bison.h"

#define YY_DECL                                        \
       yy::parser::token_type                         \
       yylex (yy::parser::semantic_type* pyylval,      \
              yy::parser::location_type* pyylloc)

YY_DECL;
#define TOKEN yy::parser::token
#define yyterminate() return TOKEN::END
#define D(x) printf(x " ")
int cur_line = 1;

extern "C" int yywrap(void)
{
   printf("end of file");
   return 1;
}

%}

whitespaces    [\t\r ]+

%%
"or"                             { D("OR"); return TOKEN::OR; }
"and"                             { D("AND"); return TOKEN::AND; }
"not"                             { D("NOT"); return TOKEN::NOT; }

[a-zA-Z_][a-zA-Z0-9_]*            { D("LITERAL"); pyylval->u_string = new std::string (yytext); return TOKEN::LITERAL; }

"("                             { D("LRBRAKET"); return TOKEN::LRBRAKET; }
")"                             { D("RRBRAKET"); return TOKEN::RRBRAKET; }
"["                             { D("LSBRAKET"); return TOKEN::LSBRAKET; }
"]"                             { D("RSBRAKET"); return TOKEN::RSBRAKET; }
"="                             { D("EQUAL"); return TOKEN::EQUAL; }
"'"                             { D("QUOTE"); return TOKEN::QUOTE; }
"\n"                            { D("EOL"); return TOKEN::EOL; }

{whitespaces}  /* eat spaces */

%%

#ifdef FF_LEX_ONLY
main()
{
yy::parser::semantic_type yylval;
yy::parser::location_type yylloc;
  while(yylex(&yylval, &yylloc));
}
#endif
