#pragma once

#include "Sexp.hpp"

namespace mc {

  struct Compiler {

    Array<Sexp*> ast;

    Compiler();
    ~Compiler();
    void parseString(String code);
    Value eval();
    
  };

}
