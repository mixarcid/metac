#pragma once

#include "Value.hpp"
#include "SymbolTable.hpp"

namespace mc {

  MC_ENUM(SexpType,
	  CONST,
	  VAR,
	  EXPR,
	  VAR_DECL,
	  IF_STMT,
	  ARRAY);

  struct Sexp {
    SexpType type;
    Sexp(SexpType _type) : type(_type) {}
    virtual Value eval(SymbolTable* table) = 0;
    virtual String toString(String spaces) = 0;
    virtual ~Sexp() {}
  };

  String to_string(Sexp* sexp);

  struct ConstNode : Sexp {
    
    Value value;
    ConstNode(Type type)
      : Sexp(SexpType::CONST), value(type) {}
    virtual Value eval(SymbolTable* table) { return value; }
    virtual String toString(String spaces);
  };

  struct VariableNode : Sexp {

    String name;

    VariableNode(String _name)
      : Sexp(SexpType::VAR), name(_name) {}
    virtual Value eval(SymbolTable* table);
    virtual String toString(String spaces);
    void setValue(Value val, SymbolTable* table);
  };

  struct ExprNode : Sexp {
    
    Array<Sexp*> sexps;

    ExprNode() : Sexp(SexpType::EXPR) {}
    virtual ~ExprNode();
    virtual Value eval(SymbolTable* table);
    virtual String toString(String spaces);
  };

  struct VarDeclNode : Sexp {

    VariableNode* var;
    Sexp* def;

    VarDeclNode(VariableNode* _var, Sexp* _def)
      : Sexp(SexpType::VAR_DECL),
	var(_var), def(_def) {}
    virtual ~VarDeclNode();
    virtual Value eval(SymbolTable* table);
    virtual String toString(String spaces);
    
  };

  struct IfStmtNode : Sexp {

    Sexp* cond;
    Sexp* if_clause;
    Sexp* else_clause;

    IfStmtNode(Sexp* _cond,
	       Sexp* _if_clause,
	       Sexp* _else_clause)
      : Sexp(SexpType::IF_STMT),
	cond(_cond),
	if_clause(_if_clause),
	else_clause(_else_clause) {}
    virtual ~IfStmtNode();
    virtual Value eval(SymbolTable* table);
    virtual String toString(String spaces);
    
  };

  struct ArrayNode : Sexp {

    Array<Sexp*> sexps;

    ArrayNode() : Sexp(SexpType::ARRAY) {}
    virtual ~ArrayNode();
    virtual Value eval(SymbolTable* table);
    virtual String toString(String spaces);
    
  };
  
};
