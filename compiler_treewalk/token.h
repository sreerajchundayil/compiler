#ifndef __TOKEN__H
#define __TOKEN__H

#include <string>

enum class TokenType
{
    // Arithmetic (kept for future, parser may ignore)
    PLUS,
    MINUS,
    STAR,
    SLASH,

    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,

    // Assignment & comparison
    ASSIGN,          // =
    EQUAL_EQUAL,     // ==
    NOT_EQUAL,       // !=
    LESS,            // <
    LESS_EQUAL,      // <=
    GREATER,         // >
    GREATER_EQUAL,   // >=

    // Literals
    IDENTIFIER,
    NUMBER,
    STRING,

    // Keywords
    PRINT,

    // Special
    NEWLINE,
    END_OF_FILE,
    INVALID
};

struct Token
{
    TokenType type;
    std::string lexeme;
};

#endif
