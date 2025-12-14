#pragma once

#include "token.h"
#include "ast.h"

#include <vector>
#include <memory>
#include <stdexcept>
#include <cstdlib>

class Parser
{
private:
    const std::vector<Token>& tokens;
    size_t current;

public:
    Parser(const std::vector<Token>& t)
        : tokens(t), current(0)
    {
    }

    // ================= PROGRAM =================

    std::vector<std::unique_ptr<Stmt>> ParseProgram()
    {
        std::vector<std::unique_ptr<Stmt>> statements;

        while (true)
        {
            // Skip blank lines
            while (Match(TokenType::NEWLINE))
            {
            }

            // Stop cleanly at EOF
            if (IsAtEnd())
                break;

            // Top-level '}' is illegal
            if (Check(TokenType::RBRACE))
                throw std::runtime_error("Unexpected '}'");

            statements.push_back(ParseStatement());
            Consume(TokenType::NEWLINE, "Expected newline after statement");
        }

        return statements;
    }

private:
    // ================= STATEMENTS =================

    std::unique_ptr<Stmt> ParseStatement()
    {
        if (Match(TokenType::PRINT))
            return ParsePrint();

        if(Match(TokenType::VAR))
          return ParseVarDecl();

        if (Match(TokenType::LBRACE))
            return ParseBlock();

        if (Check(TokenType::IDENTIFIER))
            return ParseAssignment();

        throw std::runtime_error("Expected statement");
    }

    std::unique_ptr<Stmt> ParsePrint()
    {
        auto expr = ParseExpression();
        return std::make_unique<PrintStmt>(std::move(expr));
    }

    std::unique_ptr<Stmt> ParseAssignment()
    {
        Token name = Consume(TokenType::IDENTIFIER, "Expected variable name");
        Consume(TokenType::ASSIGN, "Expected '='");

        auto expr = ParseExpression();
        return std::make_unique<AssignStmt>(name.lexeme, std::move(expr));
    }

    std::unique_ptr<Stmt> ParseBlock()
    {
        // Require newline after '{'
        Consume(TokenType::NEWLINE, "Expected newline after '{'");

        std::vector<std::unique_ptr<Stmt>> statements;

        while (true)
        {
            // Skip blank lines inside block
            while (Match(TokenType::NEWLINE))
            {
            }

            // End of block
            if (Check(TokenType::RBRACE))
                break;

            if (IsAtEnd())
                throw std::runtime_error("Unterminated block");

            statements.push_back(ParseStatement());
            Consume(TokenType::NEWLINE, "Expected newline after statement");
        }

        Consume(TokenType::RBRACE, "Expected '}'");

        return std::make_unique<BlockStmt>(std::move(statements));
    }

    // ================= EXPRESSIONS =================

    std::unique_ptr<Expr> ParseExpression()
    {
        return ParseTerm();
    }

    std::unique_ptr<Expr> ParseTerm()
    {
        auto expr = ParseFactor();

        while (Match(TokenType::PLUS) || Match(TokenType::MINUS))
        {
            char op = Previous().lexeme[0];
            auto right = ParseFactor();
            expr = std::make_unique<BinaryExpr>(op, std::move(expr), std::move(right));
        }

        return expr;
    }

    std::unique_ptr<Expr> ParseFactor()
    {
        auto expr = ParsePrimary();

        while (Match(TokenType::STAR) || Match(TokenType::SLASH))
        {
            char op = Previous().lexeme[0];
            auto right = ParsePrimary();
            expr = std::make_unique<BinaryExpr>(op, std::move(expr), std::move(right));
        }

        return expr;
    }

    std::unique_ptr<Expr> ParsePrimary()
    {
        if (Match(TokenType::NUMBER))
            return std::make_unique<NumberExpr>(std::stod(Previous().lexeme));

        if (Match(TokenType::IDENTIFIER))
            return std::make_unique<VariableExpr>(Previous().lexeme);

        if (Match(TokenType::LPAREN))
        {
            auto expr = ParseExpression();
            Consume(TokenType::RPAREN, "Expected ')'");
            return expr;
        }

        throw std::runtime_error("Expected expression");
    }

    // ================= HELPERS =================

    bool Match(TokenType type)
    {
        if (Check(type))
        {
            Advance();
            return true;
        }
        return false;
    }

    Token Consume(TokenType type, const char* msg)
    {
        if (Check(type))
            return Advance();
        throw std::runtime_error(msg);
    }

    bool Check(TokenType type) const
    {
        if (IsAtEnd())
            return false;
        return Peek().type == type;
    }

    Token Advance()
    {
        if (!IsAtEnd())
            current++;
        return Previous();
    }

    bool IsAtEnd() const
    {
        return Peek().type == TokenType::END_OF_FILE;
    }

    Token Peek() const
    {
        return tokens[current];
    }

    Token Previous() const
    {
        return tokens[current - 1];
    }

    std::unique_ptr<Stmt> ParseVarDecl()
    {
        Token name = Consume(TokenType::IDENTIFIER, "Expected variable name after 'var'");
        Consume(TokenType::ASSIGN, "Expected '=' after variable name");
    
        auto init = ParseExpression();
        return std::make_unique<VarDeclStmt>(name.lexeme, std::move(init));
    }


};

