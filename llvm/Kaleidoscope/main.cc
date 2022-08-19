#include "expr.h"
#include "parser.h"

std::unique_ptr<ExprAST> LogError(const char *Str) {
  fprintf(stderr, "LogError: %s\n", Str);
  return nullptr;
}
std::unique_ptr<PrototypeAST> LogErrorP(const char *Str) {
  LogError(Str);
  return nullptr;
}

int main(int argc, char const *argv[]) {
  KalidoscopeParser parser;

  return 0;
}
