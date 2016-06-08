#include "Sexp.hpp"
#include "Function.hpp"

namespace mc {

  Value VariableNode::eval(SymbolTable* table) {
    return *(table->getValue(name));
  }

  void VariableNode::setValue(Value val, SymbolTable* table) {
    *(table->getValue(name)) = val;
  }

  ExprNode::~ExprNode() {
    for (Sexp* sexp : sexps) {
      delete sexp;
    }
  }

  Value ExprNode::eval(SymbolTable* table) {
    
    Value ret;
    if (sexps.empty()) {
      ret = Value(Type::ERROR);
      ret.str_val = "expression is empty";
      return ret;
    }

    Value func(sexps.front()->eval(table));

    Array<Value> args;
    args.reserve(sexps.size() - 1);
    for (u32 n=1; n<sexps.size(); ++n) {
      Value val(sexps[n]->eval(table));
      if (val.type == Type::ERROR) {
	return val;
      }
      args.push_back(val);
    }
    
    switch(func.type) {

    case Type::ERROR:
      return func;

    case Type::FUNCTION:
      ret = func.func_val->call(args, table);
      break;

    case Type::INTERNAL_FUNC:
      ret = (*func.internal_func_val)(args, table);
      break;

    default:
      ret = Value(Type::ERROR);
      ret.str_val = "first element in expression is not callable";
      break;
    }
    return ret;
  }

  VarDeclNode::~VarDeclNode() {
    delete var;
    delete def;
  }

  Value VarDeclNode::eval(SymbolTable* table) {
    Value data(def->eval(table));
    if (data.type == Type::ERROR) return data;
    ((VariableNode*)var)->setValue(data, table);
    return Value(Type::NIL);
  }

  IfStmtNode::~IfStmtNode() {
    delete cond;
    delete if_clause;
    delete else_clause;
  }

  Value IfStmtNode::eval(SymbolTable* table) {
    Value cond_val = cond->eval(table);
    if (cond_val.type == Type::ERROR) return cond_val;
    if (cond_val.type != Type::BOOL)  {
      Value ret;
      ret = Value(Type::ERROR);
      ret.str_val = "first argument to if statment must be a boolean value";
      return ret;
    } else {
      if (cond_val.bool_val) {
	return if_clause->eval(table);
      } else {
	return else_clause->eval(table);
      }
    }
  }

  ArrayNode::~ArrayNode() {
    for (Sexp* sexp : sexps) {
      delete sexp;
    }
  }

  Value ArrayNode::eval(SymbolTable* table) {
    Value ret(Type::ARRAY);
    for (Sexp* sexp : sexps) {
      ret.array_val.push_back(sexp->eval(table));
    }
    return ret;
  }

  String ConstNode::toString(String spaces) {
    return spaces + "const { " + to_string(value) + " }";
  }

  String VariableNode::toString(String spaces) {
    return spaces + "var {\n" +
      spaces + " name: \"" + name + "\"\n" +
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

  String VarDeclNode::toString(String spaces) {
    String ret = spaces + "var decl {\n" +
      spaces + "var:\n" +
      var->toString(spaces + " ") + "\n" +
      spaces + "def:\n" +
      def->toString(spaces + " ") + "\n" +
      spaces + "}";
    return ret;
  }

  String IfStmtNode::toString(String spaces) {
    String ret = spaces + "if {\n" +
      spaces + "cond:\n" +
      cond->toString(spaces + " ") + "\n" +
      spaces + "if:\n" +
      if_clause->toString(spaces + " ") + "\n" +
      spaces + "else:\n" +
      else_clause->toString(spaces + " ") + "\n" +
      spaces + "}";
    return ret;
  }

  String ArrayNode::toString(String spaces) {
    String ret = spaces + "array {\n";
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
