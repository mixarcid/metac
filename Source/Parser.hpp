#pragma once

#include "Lexer.hpp"
#include "Sexp.hpp"

namespace mc {

  struct Parser {

    Array<Token> tokens;
    Array<Sexp*> sexp_stack;
    Array<Sexp*> ast;
    
    Parser(Array<Token> _tokens)
      : tokens(_tokens) {}
    
    void addSexp(Sexp* sexp);
    void parse(SymbolTable* table);

  };

}
