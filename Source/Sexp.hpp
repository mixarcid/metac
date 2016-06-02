#pragma once

#include "Value.hpp"
#include "SymbolTable.hpp"

namespace mc {

  MC_ENUM(SexpType,
	  CONST,
	  VAR,
	  EXPR);

  struct Sexp {
    SexpType type;
    Sexp(SexpType _type) : type(_type) {}
    virtual Value eval() = 0;
    virtual String toString(String spaces) = 0;
    virtual ~Sexp() {}
  };

  String to_string(Sexp* sexp);

  struct ConstNode : Sexp {
    
    Value value;
    ConstNode(Type type)
      : Sexp(SexpType::CONST), value(type) {}
    virtual Value eval() { return value; }
    virtual String toString(String spaces);
  };

  struct VariableNode : Sexp {

    String name;
    SymbolTable::Id id;

    VariableNode(String _name, SymbolTable::Id _id)
      : Sexp(SexpType::VAR), name(_name), id(_id) {}
    virtual Value eval();
    virtual String toString(String spaces);
    void setValue(Value val);
  };

  struct ExprNode : Sexp {
    
    Array<Sexp*> sexps;

    ExprNode() : Sexp(SexpType::EXPR) {}
    virtual ~ExprNode();
    virtual Value eval();
    virtual String toString(String spaces);
  };

};
