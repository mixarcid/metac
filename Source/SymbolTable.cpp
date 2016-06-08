#include "SymbolTable.hpp"
#include <climits>

namespace mc {

  void SymbolTable::addSymbol(String name) {
    symbol_map[name] = Value(Type::NIL);
  }

  bool SymbolTable::hasSymbol(String name) {
    auto it = symbol_map.find(name);
    if (it == symbol_map.end()) {
      if (parent) {
	return parent->getValue(name);
      } else {
	return false;
      }
    } else {
      return true;
    }
  }
  
  Value* SymbolTable::getValue(String name) {
    auto it = symbol_map.find(name);
    if (it == symbol_map.end()) {
      if (parent) {
	return parent->getValue(name);
      } else {
	assert(false);
	return NULL;
      }
    } else {
      return &(it->second);
    }
  }

}
