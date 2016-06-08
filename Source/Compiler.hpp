#pragma once

#include "Sexp.hpp"

namespace mc {

  struct Compiler {

    SymbolTable global_table;
    Array<Sexp*> ast;

    Compiler();
    ~Compiler();
    void parseString(String code);
    Value eval();
    
  };

}
