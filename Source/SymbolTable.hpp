#pragma once

#include "Value.hpp"
#include "HashMap.hpp"

namespace mc {

  struct SymbolTable {

    typedef u64 Id;

    HashMap<String, Id> symbol_map;
    Array<Value> symbol_arr;

    static SymbolTable table;
    const static Id NO_SYMBOL;
    
    static Id addSymbol(String name);
    static Id findSymbol(String name);
    static Value* getValue(Id id);

  };

}
