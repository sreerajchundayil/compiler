c# Tree-Walk Interpreter

### Design, Rationale, and Implementation Notes

------------------------------------------------------------------------

## 1. Context

This project implements a minimal programming language with:

-   Assignment statements\
-   Print statements\
-   Numeric expressions (`+ - * /`)\
-   Variables\
-   Parentheses\
-   Newline as statement delimiter

Pipeline:

    Source Code
      ↓
    Lexer        (tokens)
      ↓
    Parser       (AST)
      ↓
    Interpreter (execution)

The interpreter executes the **AST directly** using a **tree-walk
evaluation strategy**.

------------------------------------------------------------------------

## 2. What Is a Tree-Walk Interpreter?

A **tree-walk interpreter** executes a program by:

1.  Walking the Abstract Syntax Tree (AST)
2.  Visiting each node recursively
3.  Evaluating expressions and executing statements directly

There is **no intermediate bytecode**, **no VM**, and **no instruction
list**.

Execution follows the **structure of the AST** itself.

------------------------------------------------------------------------

## 3. Why Tree-Walk Was Chosen

### 3.1 Design Goals

The goals of this project at this stage are:

-   Clarity over performance
-   Minimal moving parts
-   Easy debugging and inspection
-   Faithful execution of grammar semantics
-   Educational correctness

A tree-walk interpreter satisfies all of these.

------------------------------------------------------------------------

### 3.2 Why Not Bytecode or VM (Yet)

  Approach      Reason Not Used
  ------------- ------------------------
  Stack VM      Adds complexity early
  Register VM   Overkill
  LLVM          Massive infrastructure
  JIT           Premature optimization

Tree-walk is the **simplest correct execution model**.

------------------------------------------------------------------------

## 4. Core Interpreter Responsibilities

The interpreter must:

1.  Execute statements **in order**
2.  Evaluate expressions **recursively**
3.  Maintain variable state
4.  Report runtime errors

Nothing else.

------------------------------------------------------------------------

## 5. Interpreter Architecture

### 5.1 Interpreter Class

The interpreter is a **stateful executor**.

``` cpp
class Interpreter
{
    std::unordered_map<std::string, double> variables;
};
```

#### Why store state in the class?

-   Variables must persist across statements
-   Execution context must be shared
-   Mirrors real language runtimes

------------------------------------------------------------------------

## 6. Execution Entry Point

``` cpp
void Execute(const std::vector<std::unique_ptr<Stmt>>& statements);
```

### Why this function exists

-   It defines the **execution boundary**
-   Separates parsing from execution
-   Allows future execution modes (debug, step, optimize)

Execution model:

    for each statement:
        execute statement

------------------------------------------------------------------------

## 7. Statement Execution

Statements **cause side effects**.

Supported statements: - Assignment - Print

``` cpp
void ExecuteStmt(const Stmt* stmt);
```

### Why split statement execution?

-   Statements do not return values
-   Keeps semantics clear
-   Enables easy extension (`if`, `while`, etc.)

------------------------------------------------------------------------

## 8. Expression Evaluation

Expressions **produce values**.

``` cpp
double EvaluateExpr(const Expr* expr);
```

### Why expressions return a value

This matches mathematical semantics:

    expression → value

------------------------------------------------------------------------

## 9. Performance Characteristics

### Advantages

✔ Extremely simple\
✔ Minimal code\
✔ Easy to debug

### Disadvantages

✘ Slow for large programs\
✘ Hard to optimize\
✘ Deep recursion risk

------------------------------------------------------------------------

## 10. Key Takeaway

> **Tree-walk interpreters execute structure, not instructions.**

------------------------------------------------------------------------

