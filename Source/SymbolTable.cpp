#include "SymbolTable.hpp"
#include <climits>

namespace mc {

  SymbolTable SymbolTable::table;
  const SymbolTable::Id SymbolTable::NO_SYMBOL = LONG_MAX;

  SymbolTable::Id SymbolTable::addSymbol(String name) {
    Id id = table.symbol_arr.size();
    table.symbol_arr.emplace_back(Type::NIL);
    table.symbol_map[name] = id;
    return id;
  }

  SymbolTable::Id SymbolTable::findSymbol(String name) {
    auto it = table.symbol_map.find(name);
    if (it == table.symbol_map.end()) {
      return NO_SYMBOL;
    } else {
      return it->second;
    }
  }
  
  Value* SymbolTable::getValue(SymbolTable::Id id) {
    return &(table.symbol_arr[id]);
  }

}
