#include <string>
#include <vector>
#include <cctype>
#include <vector>
#include "token.h"

using namespace std;


class Lexer
{
private:
    std::string source;
    size_t current;

public:
    Lexer(const std::string& src)
        : source(src), current(0)
    {
    }

    std::vector<Token> Tokenize()
    {
        std::vector<Token> tokens;
        bool lastWasNewline = false;

        while (!IsAtEnd())
        {
            char c = Advance();

            // Whitespace handling
            if (std::isspace(c))
            {
                if (c == '\n')
                {
                    if (!lastWasNewline)
                    {
                        tokens.push_back({TokenType::NEWLINE, "\\n"});
                        lastWasNewline = true;
                    }
                }
                continue;
            }

            lastWasNewline = false;

            // String literal
            if (c == '"')
            {
                tokens.push_back(StringLiteral());
            }
            // Identifier or keyword
            else if (std::isalpha(c) || c == '_')
            {
                tokens.push_back(Identifier(c));
            }
            // Number
            else if (std::isdigit(c))
            {
                tokens.push_back(Number(c));
            }
            // Operators / symbols
            else
            {
                tokens.push_back(Symbol(c));
            }
        }

        tokens.push_back({TokenType::END_OF_FILE, ""});
        return tokens;
    }

private:
    bool IsAtEnd() const
    {
        return current >= source.size();
    }

    char Advance()
    {
        return source[current++];
    }

    char Peek() const
    {
        if (IsAtEnd())
            return '\0';
        return source[current];
    }

    // ---------- Token scanners ----------

    Token Identifier(char first)
    {
        std::string value(1, first);

        while (std::isalnum(Peek()) || Peek() == '_')
            value += Advance();

        if (value == "print")
            return {TokenType::PRINT, value};
        
        if (value == "var")
            return {TokenType::VAR, value};

        return {TokenType::IDENTIFIER, value};
    }

    Token Number(char first)
    {
        std::string value(1, first);

        while (std::isdigit(Peek()))
            value += Advance();

        if (Peek() == '.')
        {
            value += Advance();
            while (std::isdigit(Peek()))
                value += Advance();
        }

        return {TokenType::NUMBER, value};
    }

    Token StringLiteral()
    {
        std::string value;

        while (!IsAtEnd() && Peek() != '"')
        {
            // strings cannot span lines in this language
            if (Peek() == '\n')
                return {TokenType::INVALID, "Unterminated string"};

            value += Advance();
        }

        if (IsAtEnd())
            return {TokenType::INVALID, "Unterminated string"};

        Advance(); // consume closing "

        return {TokenType::STRING, value};
    }

    Token Symbol(char c)
    {
      switch (c)
      {
        case '{':
    return{TokenType::LBRACE, "{"};
    break;

    case '}':
    return {TokenType::RBRACE, "}"};
    break;

          case '(': return {TokenType::LPAREN, "("};
          case ')': return {TokenType::RPAREN, ")"};
          case '+': return {TokenType::PLUS, "+"};
          case '-': return {TokenType::MINUS, "-"};
          case '*': return {TokenType::STAR, "*"};
          case '/': return {TokenType::SLASH, "/"};

          case '=':
              if (Peek() == '=')
              {
                  Advance();
                  return {TokenType::EQUAL_EQUAL, "=="};
              }
              return {TokenType::ASSIGN, "="};

          case '!':
              if (Peek() == '=')
              {
                  Advance();
                  return {TokenType::NOT_EQUAL, "!="};
              }
              return {TokenType::INVALID, "!"};

          case '<':
              if (Peek() == '=')
              {
                  Advance();
                  return {TokenType::LESS_EQUAL, "<="};
              }
              return {TokenType::LESS, "<"};

          case '>':
              if (Peek() == '=')
              {
                  Advance();
                  return {TokenType::GREATER_EQUAL, ">="};
              }
              return {TokenType::GREATER, ">"};

          default:
              return {TokenType::INVALID, std::string(1, c)};
        }
    }
};

