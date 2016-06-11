#pragma once

#include "Macros.hpp"
#include "Types.hpp"
#include "String.hpp"
#include "Array.hpp"

namespace mc {

  MC_ENUM(Type,
	  BOOL,
	  CHAR,
	  INT,
	  FLOAT,
	  STRING,
	  ARRAY,
	  FUNCTION,
	  INTERNAL_FUNC,
	  INTERNAL_MACRO,
	  VAR_ARGS,
	  ERROR,
	  NIL);

  struct Sexp;
  struct Value;
  struct VariableNode;
  struct SymbolTable;
  
  typedef Value (InternalFunc)(Array<Value>, SymbolTable*);
  typedef Sexp* (InternalMacro)(Array<Sexp*>, SymbolTable*);
  
  struct Function;
  
  struct Value {

    Type type;

    union {
      bool bool_val;
      char char_val;
      i32 int_val;
      f32 float_val;
      String str_val;
      Array<Value> array_val;
      Function* func_val;
      InternalFunc* internal_func_val;
      InternalMacro* internal_macro_val;
    };

    Value(Type _type = Type::NIL);
    Value(const Value& b);
    Value(Value&& b);
    ~Value();
    void operator=(const Value& b);
    void operator=(Value&& b);
    bool operator==(Value& b);
    bool operator!=(Value& b);
  };

  String to_string(Value val);

}

