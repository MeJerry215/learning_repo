#ifndef PARSER_HEADER
#define PARSER_HEADER
#include "expr.h"
#include "token.h"
#include <ctype.h>
#include <stdio.h>
#include <string>

class KalidoscopeParser {
private:
  std::string identifier_str;
  double num_val;
  char last_char;
  int cur_tok;

public:
  KalidoscopeParser()
      : identifier_str(""), num_val(0.0), last_char(' '), cur_tok(0) {}

  int get_token();

  int get_next_token() { return (cur_tok = get_token()); }

  std::unique_ptr<ExprAST> ParseNumberExpr();

  std::unique_ptr<ExprAST> ParseParenExpr();

  std::unique_ptr<ExprAST> ParseIdentifierExpr();

  std::unique_ptr<ExprAST> ParsePrimary();
};

#endif