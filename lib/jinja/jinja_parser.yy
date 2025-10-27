%skeleton "lalr1.cc"
%require "3.8.1"
%header
%define api.token.raw
%define api.token.constructor
%define api.value.type variant
%define api.namespace {infinite::jinja}
%define parse.assert
%define parse.trace
%define parse.error detailed
%define parse.lac full
%locations

%code requires {
    #include <string>
    #include <map>
    #include "location.hh"

    namespace infinite {
        namespace jinja {
            class Context
            {
            public:
                std::map<std::string, int> variables;
                location loc;
            };
        }
    }
}

%param { infinite::jinja::Context& ctx }

%code {
    infinite::jinja::parser::symbol_type infinite_jinjalex(infinite::jinja::Context& ctx);
    #define yylex infinite_jinjalex
}

%define api.token.prefix {JINJA_TOK_}
%token
    ASSIGN ":="
    MINUS   "-"
    PLUS    "+"
    STAR    "*"
    SLASH   "/"
    LPAREN  "("
    RPAREN  ")"
;
%token <std::string> IDENTIFIER "identifier"
%token <int> NUMBER "number"
%nterm <int> exp

%printer { yyo << $$; } <*>;

%%
%start unit;
unit: assignments exp {  };

assignments:
    %empty                   {}
    | assignments assignment {}
;

assignment:
    "identifier" ":=" exp { ctx.variables[$1] = $3; };

%left "+" "-";
%left "*" "/";
exp:
    "number"
    | "identifier" { $$ = ctx.variables[$1]; }
    | exp "+" exp  { $$ = $1 + $3; }
    | exp "-" exp  { $$ = $1 - $3; }
    | exp "*" exp  { $$ = $1 * $3; }
    | exp "/" exp  { $$ = $1 / $3; }
    | "(" exp ")"  { $$ = $2; }
%%

void infinite::jinja::parser::error(const infinite::jinja::location& l, const std::string& m)
{
    std::cerr << l << ": " << m << std::endl;
}
