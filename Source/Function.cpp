#include "Function.hpp"

namespace mc {

  Array<Function*> Function::all_functions;
  
  Function::Function(Function& b) {
    inputs = b.inputs;
    body = b.body;
    b.inputs.clear();
    b.body = NULL;
  }
  
  Function::Function(Function&& b) {
    inputs = b.inputs;
    body = b.body;
    b.inputs.clear();
    b.body = NULL;
  }
  
  Function::~Function() {
    for (auto& var : inputs) {
      delete var;
    }
    if (body) delete body;
  }
  
  void Function::operator=(Function& b) {
    for (auto& var : inputs) {
      delete var;
    }
    if (body) delete body;
    
    inputs = b.inputs;
    body = b.body;
    b.inputs.clear();
    b.body = NULL;
  }
  
  void Function::operator=(Function&& b) {
    for (auto& var : inputs) {
      delete var;
    }
    if (body) delete body;

    inputs = b.inputs;
    body = b.body;
    b.inputs.clear();
    b.body = NULL;
  }

  Value Function::call(Array<Value> values, SymbolTable* table) {

    assert(body);
    Value ret;

    if (!inputs.empty() && inputs.back()->name == "...") {
      if (values.size() < inputs.size() - 1) {
	ret = Value(Type::ERROR);
	ret.str_val = "function expects at least " + to_string(inputs.size() - 1) +
	  " arguments, but is called with " + to_string(values.size());
	return ret;
      }
    } else {
      if (values.size() < inputs.size()) {
	ret = Value(Type::ERROR);
	ret.str_val = "function expects " + to_string(inputs.size()) +
	  " arguments, but is called with " + to_string(values.size());
	return ret;
      } 
    }
    
    SymbolTable child_table(table);
    for (u32 n=0; n<inputs.size(); ++n) {
      child_table.addSymbol(inputs[n]->name);
      if (inputs[n]->name == "...") {
	if (n != inputs.size() - 1) {
	  ret = Value(Type::ERROR);
	  ret.str_val = "variable arguments symbol \"...\" can only be used as last argument to function";
	  return ret;
	} else {
	  Value rest(Type::ARRAY);
	  for (u32 m=n; m<values.size(); ++m) {
	    rest.array_val.push_back(values[m]);
	  }
	  inputs[n]->setValue(rest, &child_table);
	}
      } else {
	inputs[n]->setValue(values[n], &child_table);
      }
    }
    
    return body->eval(&child_table);
  }

  Function* Function::create() {
    all_functions.emplace_back(new Function());
    return all_functions.back();
  }

  void Function::cleanup() {
    for (auto& f : all_functions) {
      delete f;
    }
    all_functions.clear();
  }

}
