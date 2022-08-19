#ifndef EXPR_HEADER
#define EXPR_HEADER
#include <memory>
#include <vector>

class ExprAST {
public:
  virtual ~ExprAST(){};
};

class NumberExpr : public ExprAST {
  double val;

public:
  NumberExpr(double v) : val(v){};
};

class VariableExprAST : public ExprAST {
  std::string name;

public:
  VariableExprAST(std::string &n) : name(n){};
};

class BinaryExprAST : public ExprAST {
  char op;
  std::unique_ptr<ExprAST> lhs, rhs;

public:
  BinaryExprAST(char o, std::unique_ptr<ExprAST> l, std::unique_ptr<ExprAST> r)
      : op(o), lhs(std::move(l)), rhs(std::move(r)) {}
};

class CallExprAST : public ExprAST {
  std::string callee;
  std::vector<std::unique_ptr<ExprAST>> args;

public:
  CallExprAST(const std::string &call,
              std::vector<std::unique_ptr<ExprAST>> args)
      : callee(call), args(std::move(args)) {}
};

class PrototypeAST {
  std::string name;
  std::vector<std::string> args;

public:
  PrototypeAST(const std::string &name, std::vector<std::string> args)
      : name(name), args(std::move(args)) {}

  const std::string &get_name() const { return name; }
};

class FunctionAST {
  std::unique_ptr<PrototypeAST> proto;
  std::unique_ptr<ExprAST> body;

public:
  FunctionAST(std::unique_ptr<PrototypeAST> proto,
              std::unique_ptr<ExprAST> body)
      : proto(std::move(proto)), body(std::move(body)) {}
};

#endif