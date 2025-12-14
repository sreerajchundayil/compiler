#pragma once

#include "ast.h"
#include <unordered_map>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <memory>

class Interpreter
{
private:
    // Symbol table: variable name -> value
  std::vector<std::unordered_map<std::string, double>> scopes;

public:
    // Entry point: execute the whole program
    void Execute(const std::vector<std::unique_ptr<Stmt>>& statements)
    {
      scopes.push_back({}); //global scope
        for (const auto& stmt : statements)
        {
            ExecuteStmt(stmt.get());
        }
    }

private:
    // ---------------- STATEMENTS ----------------
    void EnterScope()
    {
        scopes.push_back({});
    }
    
    void ExitScope()
    {
        scopes.pop_back();
    }

    double GetVariable(const std::string& name)
    {
      for (auto it = scopes.rbegin(); it != scopes.rend(); ++it)
      {
          auto found = it->find(name);
          if (found != it->end())
              return found->second;
      }
      throw std::runtime_error("Undefined variable: " + name);
    }

    void SetVariable(const std::string& name, double value)
    {
        for (auto it = scopes.rbegin(); it != scopes.rend(); ++it)
        {
            auto found = it->find(name);
            if (found != it->end())
            {
                found->second = value;
                return;
            }
        }

    throw std::runtime_error("Undefined variable: " + name);
    }



    void ExecuteStmt(const Stmt* stmt)
    {
        // Assignment: x = expression
        if (auto assign = dynamic_cast<const AssignStmt*>(stmt))
        {
            double value = EvaluateExpr(assign->value.get());
            SetVariable(assign->name, value);
            return;
        }

        if (auto varDecl = dynamic_cast<const VarDeclStmt*>(stmt))
        {
            double value = EvaluateExpr(varDecl->initializer.get());
            DeclareVariable(varDecl->name, value);
            return;
        }


        // Print: print expression
        if (auto print = dynamic_cast<const PrintStmt*>(stmt))
        {
            double value = EvaluateExpr(print->value.get());
            std::cout << value << std::endl;
            return;
        }

        if(auto block = dynamic_cast<const BlockStmt*>(stmt))
        {
          EnterScope();
            for(const auto& s: block->statements)
              ExecuteStmt(s.get());
          ExitScope();
          return;
        }

        throw std::runtime_error("Unknown statement type");
    }

    // ---------------- EXPRESSIONS ----------------

    double EvaluateExpr(const Expr* expr)
    {
        // Number literal
        if (auto num = dynamic_cast<const NumberExpr*>(expr))
        {
            return num->value;
        }

        // Variable reference
        if (auto var = dynamic_cast<const VariableExpr*>(expr))
        {
          return GetVariable(var->name);
        }

        // Binary operation
        if (auto bin = dynamic_cast<const BinaryExpr*>(expr))
        {
            double left  = EvaluateExpr(bin->left.get());
            double right = EvaluateExpr(bin->right.get());

            switch (bin->op)
            {
            case '+': return left + right;
            case '-': return left - right;
            case '*': return left * right;
            case '/': return left / right;
            default:
                throw std::runtime_error("Unknown binary operator");
            }
        }

        throw std::runtime_error("Unknown expression type");
    }

    void DeclareVariable(const std::string& name, double value)
    {
        auto& scope = scopes.back();
    
        if (scope.count(name))
            throw std::runtime_error("Variable already declared in this scope: " + name);
    
        scope[name] = value;
    }


};

