#include "parser.h"

int KalidoscopeParser::get_token() {
  while (isspace(last_char)) {
    last_char = getchar();
  }
  if (isalpha(last_char)) {
    identifier_str = last_char;
    while (isalnum((last_char = getchar()))) {
      identifier_str += last_char;
    }
    if (identifier_str == "def")
      return TOKEN_DEF;
    else if (identifier_str == "extern") {
      return TOKEN_EXTERN;
    }
    return TOKEN_IDENTIFIER;
  }
  if (isdigit(last_char) || last_char == '.') {
    std::string num_str;
    do {
      last_char = getchar();
    } while (isdigit(last_char) || last_char == '.');

    num_val = strtod(num_str.c_str(), 0);
    return TOKEN_NUM;
  }
  if (last_char == '#') {
    do {
      last_char = getchar();
    } while (last_char != EOF && last_char != '\n' && last_char != '\r');
    if (last_char != EOF) {
      return get_token();
    }
  }
  if (last_char == EOF) {
    return TOKEN_EOF;
  }
  int this_char = last_char;
  last_char = getchar();
  return this_char;
}