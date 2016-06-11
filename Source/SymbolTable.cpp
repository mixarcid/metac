#include "SymbolTable.hpp"
#include <climits>

namespace mc {

  void SymbolTable::addSymbol(String name) {
    auto it = symbol_map.find(name);
    if (it == symbol_map.end()) {
      Value* val = &(symbol_map[name]);
      *val = Value(Type::ERROR);
      val->str_val = "undefined variable \"" + name + "\"";
    }
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
