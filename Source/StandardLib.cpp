#include "StandardLib.hpp"
#include "SymbolTable.hpp"
#include "Sexp.hpp"
#include "Function.hpp"

#define stdAssert(cond, msg)			\
  do {						\
    if (!(cond)) {				\
      err.str_val = msg;			\
      return err;				\
    }						\
  } while (0)					\

#define macroAssert(cond, msg, ...)				\
  do {							\
    if (!(cond)) {					\
      ConstNode* ret = new ConstNode(Type::ERROR);	\
      ret->value.str_val = msg;				\
      __VA_ARGS__;					\
      return ret;					\
    }							\
  } while (0)						\

namespace mc {

  Value add(Array<Value> values, SymbolTable* table) {

    Value err(Type::ERROR);
    stdAssert(values.size() > 0,
	      "+ function takes at least one argument");
    err.str_val = "Incompatible types in + function";
    Value ret;

    for (Value& val : values) {
      switch(val.type) {

      case Type::INT:
	switch(ret.type) {
	case Type::NIL:
	  ret.type = Type::INT;
	  ret.int_val = val.int_val;
	  break;
	case Type::INT:
	  ret.int_val += val.int_val;
	  break;
	case Type::FLOAT:
	  ret.float_val += val.int_val;
	  break;
	default:
	  return err;
	}
	break;

      case Type::FLOAT:
	switch(ret.type) {
	case Type::NIL:
	  ret.type = Type::FLOAT;
	  ret.float_val = val.float_val;
	  break;
	case Type::INT:
	  {
	    ret.type = Type::FLOAT;
	    f32 tmp = ret.int_val;
	    ret.float_val = tmp;
	  }
	case Type::FLOAT:
	  ret.float_val += val.float_val;
	  break;
	default:
	  return err;
	}
	break;

      case Type::STRING:
	switch(ret.type) {
	case Type::NIL:
	  ret = Value(Type::STRING);
	case Type::STRING:
	  ret.str_val += val.str_val;
	  break;
	default:
	  return err;
	}
	break;

      case Type::ARRAY:
	switch(ret.type) {
	case Type::NIL:
	  ret = Value(Type::ARRAY);
	case Type::ARRAY:
	  for (Value& elem : val.array_val) {
	    ret.array_val.push_back(elem);
	  }
	  break;
	default:
	  return err;
	}
	break;
	
      default:
	err.str_val = "+ function only takes ints, floats, strings, and arrays as arguments";
	return err;
	
      }
    }
    return ret;
  }

  Value sub(Array<Value> values, SymbolTable* table) {

    Value err(Type::ERROR);
    stdAssert(values.size() > 0,
	      "- function takes at least one argument");
    err.str_val = "Incompatible types in - function";
    Value ret;

    if (values.size() == 1) {
      
      if (values[0].type == Type::INT) {
	ret = Value(Type::INT);
	ret.int_val = -values[0].int_val;
	return ret;
      } else if (values[0].type == Type::FLOAT) {
	ret = Value(Type::FLOAT);
	ret.float_val = -values[0].float_val;
	return ret;
      } else {
	return err;
      }
      
    } else {

      ret = values[0];

      for (u32 n=1; n<values.size(); ++n) {

	Value val = values[n];

	switch(val.type) {

	case Type::INT:
	  switch(ret.type) {
	  case Type::NIL:
	    ret.type = Type::INT;
	    ret.int_val = val.int_val;
	    break;
	  case Type::INT:
	    ret.int_val -= val.int_val;
	    break;
	  case Type::FLOAT:
	    ret.float_val -= val.int_val;
	    break;
	  default:
	    return err;
	  }
	  break;

	case Type::FLOAT:
	  switch(ret.type) {
	  case Type::NIL:
	    ret.type = Type::FLOAT;
	    ret.float_val = val.float_val;
	    break;
	  case Type::INT:
	    {
	      ret.type = Type::FLOAT;
	      f32 tmp = ret.int_val;
	      ret.float_val = tmp;
	    }
	  case Type::FLOAT:
	    ret.float_val -= val.float_val;
	    break;
	  default:
	    return err;
	  }
	  break;

	default:
	  err.str_val = "- function only takes ints and floats as arguments";
	  return err;
	
	}
	
      }
      
    }
    return ret;
  }
  
  Value mul(Array<Value> values, SymbolTable* table) {

    Value err(Type::ERROR);
    stdAssert(values.size() > 0,
	      "* function takes at least one argument");
    err.str_val = "Incompatible types in * function";
    Value ret;

    for (Value& val : values) {
      switch(val.type) {

      case Type::INT:
	switch(ret.type) {
	case Type::NIL:
	  ret.type = Type::INT;
	  ret.int_val = val.int_val;
	  break;
	case Type::INT:
	  ret.int_val *= val.int_val;
	  break;
	case Type::FLOAT:
	  ret.float_val *= val.int_val;
	  break;
	default:
	  return err;
	}
	break;

      case Type::FLOAT:
	switch(ret.type) {
	case Type::NIL:
	  ret.type = Type::FLOAT;
	  ret.float_val = val.float_val;
	  break;
	case Type::INT:
	  {
	    ret.type = Type::FLOAT;
	    f32 tmp = ret.int_val;
	    ret.float_val = tmp;
	  }
	case Type::FLOAT:
	  ret.float_val *= val.float_val;
	  break;
	default:
	  return err;
	}
	break;

      default:
	err.str_val = "* function only takes ints and floats as arguments";
	return err;
	
      }
    }
    return ret;
  }

  Value bool_not(Array<Value> values, SymbolTable* table) {
    Value err(Type::ERROR);
    stdAssert(values.size() == 1,
	      "not function only takes 1 argument");
    Value val = values.front();
    stdAssert(val.type == Type::BOOL,
	      "not function only takes boolean arguments");
    val.bool_val = !val.bool_val;
    return val;
  }

  Value bool_and(Array<Value> values, SymbolTable* table) {
    Value err(Type::ERROR);
    stdAssert(values.size() > 0,
	      "and function takes at least one argument");
    Value ret(Type::BOOL);
    ret.bool_val = true;
    for (Value val : values) {
      stdAssert(val.type == Type::BOOL,
		"and function only takes boolean arguments");
      ret.bool_val = (ret.bool_val && val.bool_val);
    }
    return ret;
  }
  
  Value bool_or(Array<Value> values, SymbolTable* table) {
    Value err(Type::ERROR);
    stdAssert(values.size() > 0,
	      "or function takes at least one argument");
    Value ret(Type::BOOL);
    ret.bool_val = false;
    for (Value val : values) {
      stdAssert(val.type == Type::BOOL,
		"or function only takes boolean arguments");
      ret.bool_val = (ret.bool_val || val.bool_val);
    }
    return ret;
  }

  Value equals(Array<Value> values, SymbolTable* table) {
    Value err(Type::ERROR);
    stdAssert(values.size() > 1,
	      "= function takes at least two arguments");
    Value ret(Type::BOOL);
    ret.bool_val = true;
    Value first_val = values[0];
    for (u32 n=1; n<values.size(); ++n) {
      if (first_val != values[n]) {
	ret.bool_val = false;
	break;
      }
    }
    return ret;
  }

  Value greater(Array<Value> values, SymbolTable* table) {
    
    Value err(Type::ERROR);
    stdAssert(values.size() == 2,
	      "> function takes two arguments");
    Value ret(Type::BOOL);
    
    switch (values[0].type) {

    case Type::INT:
          
      switch (values[0].type) {

      case Type::INT:
	ret.bool_val = (values[0].int_val > values[1].int_val);
	break;

      case Type::FLOAT:
	ret.bool_val = (values[0].int_val > values[1].float_val);
	break;
      
      default:
	err.str_val = "> function only takes ints and floats as arguments";
	return err;
      }
      break;

    case Type::FLOAT:
      switch (values[0].type) {

      case Type::INT:
	ret.bool_val = (values[0].float_val > values[1].int_val);
	break;

      case Type::FLOAT:
	ret.bool_val = (values[0].float_val > values[1].float_val);
	break;
      
      default:
	err.str_val = "> function only takes ints and floats as arguments";
	return err;
      }
      break;
      
    default:
      err.str_val = "> function only takes ints and floats as arguments";
      return err;
    }

    return ret;
    
  }

  Value lesser(Array<Value> values, SymbolTable* table) {
    
    Value err(Type::ERROR);
    stdAssert(values.size() == 2,
	      "< function takes two arguments");
    Value ret(Type::BOOL);
    
    switch (values[0].type) {

    case Type::INT:
          
      switch (values[0].type) {

      case Type::INT:
	ret.bool_val = (values[0].int_val < values[1].int_val);
	break;

      case Type::FLOAT:
	ret.bool_val = (values[0].int_val < values[1].float_val);
	break;
      
      default:
	err.str_val = "< function only takes ints and floats as arguments";
	return err;
      }
      break;

    case Type::FLOAT:
      switch (values[0].type) {

      case Type::INT:
	ret.bool_val = (values[0].float_val < values[1].int_val);
	break;

      case Type::FLOAT:
	ret.bool_val = (values[0].float_val < values[1].float_val);
	break;
      
      default:
	err.str_val = "< function only takes ints and floats as arguments";
	return err;
      }
      break;
      
    default:
      err.str_val = "< function only takes ints and floats as arguments";
      return err;
    }

    return ret;
    
  }

  Value print(Array<Value> values, SymbolTable* table) {
    Value err(Type::ERROR);
    stdAssert(values.size() > 0,
	      "print function takes at least one argument");
    String out;
    for (Value val : values) {
      out += to_string(val);
    }
    printf("%s", out.c_str());
    return Value(Type::NIL);
  }

  Value _do(Array<Value> values, SymbolTable* table) {
    Value err(Type::ERROR);
    stdAssert(values.size() > 0,
	      "do function takes at least one argument");
    return values.back();
  }

  Sexp* define(Array<Sexp*> args, SymbolTable* table) {
    macroAssert(args.size() == 2,
	      "define macro only takes two arguments");
    macroAssert(args[0]->type == SexpType::VAR,
	      "first argument to define macro must be a variable");
    return new VarDeclNode((VariableNode*)args[0], args[1]);
  }

  Sexp* _if(Array<Sexp*> args, SymbolTable* table) {
    if (args.size() == 2) {
      return new IfStmtNode(args[0],
			    args[1],
			    new ConstNode(Type::NIL));
    } else if (args.size() == 3) {
      return new IfStmtNode(args[0],
			    args[1],
			    args[2]);
    } else {
      ConstNode* ret = new ConstNode(Type::ERROR);
      ret->value.str_val = "if macro only takes two or three arguments";
      return ret;
    }
  }

  Sexp* lambda(Array<Sexp*> args, SymbolTable* table) {
    macroAssert(args.size() == 2,
		"lambda macro only takes two arguments");
    Sexp* first = args[0];
    macroAssert(first->type == SexpType::EXPR,
		"first argument to lambda macro must be a list of arguments");
    Array<Sexp*>& inputs = ((ExprNode*)first)->sexps;
    Function* func = Function::create();

    for (Sexp* sexp : inputs) {
      macroAssert(sexp->type == SexpType::VAR,
		  "function arguments must be variables");
      func->inputs.push_back((VariableNode*) sexp);
    }
    
    func->body = args[1];
    
    ConstNode* ret = new ConstNode(Type::FUNCTION);
    ret->value.func_val = func;
    return ret;
  }
  
#define define_func(name, internal_name)	\
  table->addSymbol(name);			\
  val = table->getValue(name);			\
  val->type = Type::INTERNAL_FUNC;		\
  val->internal_func_val = &internal_name;

#define define_macro(name, internal_name)	\
  table->addSymbol(name);			\
  val = table->getValue(name);			\
  val->type = Type::INTERNAL_MACRO;		\
  val->internal_macro_val = &internal_name;
  
  void initStandardLib(SymbolTable* table) {
    
    Value* val;

    define_func("+", add);
    define_func("-", sub);
    define_func("*", mul);
    define_func("not", bool_not);
    define_func("and", bool_and);
    define_func("or", bool_or);
    define_func("=", equals);
    define_func(">", greater);
    define_func("<", lesser);
    define_func("print", print);
    define_func("do", _do);
    
    define_macro("define", define);
    define_macro("if", _if);
    define_macro("lambda", lambda);
  }
  
}
