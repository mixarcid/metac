#pragma once

#include "Value.hpp"
#include "HashMap.hpp"

namespace mc {

  struct SymbolTable {

    SymbolTable* parent;

    HashMap<String, Value> symbol_map;

    SymbolTable(SymbolTable* _parent) : parent(_parent) {}
    void addSymbol(String name);
    bool hasSymbol(String name);
    Value* getValue(String name);

  };

}
