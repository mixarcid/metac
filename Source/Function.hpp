#include "Sexp.hpp"

namespace mc {

  struct Function {
      
    Array<VariableNode*> inputs;
    Sexp* body;

    Function() {}
    Function(Function& b);
    Function(Function&& b);
    virtual ~Function();
    void operator=(Function& b);
    void operator=(Function&& b);
    Value call(Array<Value> values, SymbolTable* table);

    static Array<Function*> all_functions;
    static Function* create();
    static void cleanup();
    
  };
  
}
