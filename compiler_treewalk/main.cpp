#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "lexer.h"
#include "parser.h"
#include "treewalk.h"
#include "token.h"

// Debug helper (you already asked for this earlier)
//const char* TokenTypeToString(TokenType type);

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <source-file>\n";
        return 1;
    }

    // ---------- Read source file ----------
    std::ifstream file(argv[1]);
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();

    // ---------- Lexing ----------
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.Tokenize();

    // ---------- Token dump (VERY IMPORTANT for debugging) ----------
    //for (size_t i = 0; i < tokens.size(); ++i)
    //{
    //    std::cout << i << ": "
    //              << TokenTypeToString(tokens[i].type)
    //              << " [" << tokens[i].lexeme << "]\n";
    //}
    //std::cout << "---------------------\n";

    try
    {
        // ---------- Parsing ----------
        Parser parser(tokens);
        std::vector<std::unique_ptr<Stmt>> program = parser.ParseProgram();

        // ---------- Interpretation ----------
        Interpreter interpreter;
        interpreter.Execute(program);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}

