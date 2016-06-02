#include "Sexp.hpp"

namespace mc {

  Value VariableNode::eval() {
    return *(SymbolTable::getValue(id));
  }

  void VariableNode::setValue(Value val) {
    *(SymbolTable::getValue(id)) = val;
  }

  ExprNode::~ExprNode() {
    for (Sexp* sexp : sexps) {
      delete sexp;
    }
  }

  Value ExprNode::eval() {
    
    Value ret;
    if (sexps.empty()) {
      ret = Value(Type::ERROR);
      ret.str_val = "expression is empty";
      return ret;
    }

    Value func(sexps.front()->eval());
    switch(func.type) {

    case Type::ERROR:
      return func;

    case Type::FUNCTION:
      {
	Array<Value> args;
	args.reserve(sexps.size() - 1);
	for (u32 n=1; n<sexps.size(); ++n) {
	  Value val(sexps[n]->eval());
	  if (val.type == Type::ERROR) {
	    return val;
	  }
	  args.push_back(val);
	}
    
	ret = func.func_val->call(args);
      }
      break;

    case Type::MACRO:
      {
	Array<Sexp*> args;
	args.reserve(sexps.size() - 1);
	for (u32 n=1; n<sexps.size(); ++n) {
	  args.push_back(sexps[n]);
	}
	ret = func.macro_val->call(args);
      }
      break;

    default:
      ret = Value(Type::ERROR);
      ret.str_val = "first element in expression is not callable";
      break;
    }
    return ret;
  }
  
  String ConstNode::toString(String spaces) {
    return spaces + "const { " + to_string(value) + " }";
  }

  String VariableNode::toString(String spaces) {
    return spaces + "var {\n" +
      spaces + " name: \"" + name + "\"\n" +
      spaces + " value: " + to_string(eval()) + "\n" +
      spaces + "}";
  }

  String ExprNode::toString(String spaces) {
    String ret = spaces + "expr {\n";
    for (Sexp* sexp : sexps) {
      ret += sexp->toString(spaces + " ") + "\n";
    }
    ret += spaces + "}";
    return ret;
  }

  String to_string(Sexp* sexp) {
    assert(sexp);
    return sexp->toString("");
  }
  
}
