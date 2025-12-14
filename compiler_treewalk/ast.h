#pragma once
#include <string>
#include <memory>
#include <vector>

// ---------- Expressions ----------

struct Expr
{
    virtual ~Expr() = default;
};

struct NumberExpr : Expr
{
    double value;
    explicit NumberExpr(double v) : value(v) {}
};

struct VariableExpr : Expr
{
    std::string name;
    explicit VariableExpr(const std::string& n) : name(n) {}
};

struct BinaryExpr : Expr
{
    char op;
    std::unique_ptr<Expr> left;
    std::unique_ptr<Expr> right;

    BinaryExpr(char o,
               std::unique_ptr<Expr> l,
               std::unique_ptr<Expr> r)
        : op(o), left(std::move(l)), right(std::move(r)) {}
};

// ---------- Statements ----------

struct Stmt
{
    virtual ~Stmt() = default;
};

struct AssignStmt : Stmt
{
    std::string name;
    std::unique_ptr<Expr> value;

    AssignStmt(const std::string& n, std::unique_ptr<Expr> v)
        : name(n), value(std::move(v)) {}
};

struct PrintStmt : Stmt
{
    std::unique_ptr<Expr> value;

    explicit PrintStmt(std::unique_ptr<Expr> v)
        : value(std::move(v)) {}
};

struct BlockStmt : Stmt
{
  std::vector<std::unique_ptr<Stmt>> statements;
  BlockStmt(std::vector<std::unique_ptr<Stmt>> stmts)
        : statements(std::move(stmts))
    {
    }
};

