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
	  FUNCTION,
	  MACRO,
	  ERROR,
	  NIL);

  struct Function;
  struct Macro;
  
  struct Value {

    Type type;

    union {
      bool bool_val;
      char char_val;
      i32 int_val;
      f32 float_val;
      String str_val;
      Function* func_val;
      Macro* macro_val;
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
  
  struct Function {

    typedef Value (*InternalFunc)(Array<Value>);

    InternalFunc internal_func;

    Function(InternalFunc _internal_func)
      : internal_func(_internal_func) {}

    Value call(Array<Value> args) {
      return internal_func(args);
    }
    
  };

  struct Sexp;

  struct Macro {

    typedef Value (*InternalFunc)(Array<Sexp*>);

    InternalFunc internal_func;

    Macro(InternalFunc _internal_func)
      : internal_func(_internal_func) {}

    Value call(Array<Sexp*> args) {
      return internal_func(args);
    }

  };


}
