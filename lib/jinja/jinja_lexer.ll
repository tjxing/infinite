%option noyywrap
%option nounput
%option noinput
%option nodefault
%option batch
%option debug
%option prefix="infinite_jinja"

%{
    #include <cerrno>
    #include <climits>
    #include <cstdlib>
    #include <cstring>
    #include <string>
    #include "jinja.h"
    #include "jinja_parser.tab.hpp"

    #define YY_USER_ACTION loc.columns(yyleng);
    #define YY_DECL infinite::jinja::parser::symbol_type infinite_jinjalex(infinite::jinja::Context& ctx)

    infinite::jinja::parser::symbol_type make_NUMBER(const std::string &s, const infinite::jinja::location& loc);
%}

id    [a-zA-Z][a-zA-Z0-9]*
int   [0-9]+
blank [ \t\r]

%%
%{
    infinite::jinja::location& loc = ctx.loc;
    loc.step();
%}

{blank}+ loc.step();
\n+      loc.lines(yyleng); loc.step();

"-"      return infinite::jinja::parser::make_MINUS(loc);
"+"      return infinite::jinja::parser::make_PLUS(loc);
"*"      return infinite::jinja::parser::make_STAR(loc);
"/"      return infinite::jinja::parser::make_SLASH(loc);
"("      return infinite::jinja::parser::make_LPAREN(loc);
")"      return infinite::jinja::parser::make_RPAREN(loc);
":="     return infinite::jinja::parser::make_ASSIGN(loc);

{int}    return make_NUMBER(yytext, loc);
{id}     return infinite::jinja::parser::make_IDENTIFIER(yytext, loc);

.        { throw infinite::jinja::parser::syntax_error(loc, "invalid character: " + std::string(yytext)); }
<<EOF>>  return infinite::jinja::parser::make_YYEOF(loc);
%%

infinite::jinja::parser::symbol_type make_NUMBER(const std::string &s, const infinite::jinja::location &loc)
{
    errno = 0;
    long n = strtol(s.c_str(), NULL, 10);
    if (!(INT_MIN <= n && n <= INT_MAX && errno != ERANGE)) {
        throw infinite::jinja::parser::syntax_error(loc, "integer is out of range: " + s);
    }
    return infinite::jinja::parser::make_NUMBER((int) n, loc);
}

void infinite::parse_jinja(const std::string_view s)
{
    YY_BUFFER_STATE buffer = yy_scan_bytes(s.data(), s.size());
    yy_switch_to_buffer(buffer);
    
    infinite::jinja::Context ctx;
    ctx.loc.initialize();
    ctx.variables["one"] = 1;
    ctx.variables["two"] = 2;
    infinite::jinja::parser parse(ctx);
    parse();

    yy_delete_buffer(buffer);
    yylex_destroy();
}
