#include "StandardLib.hpp"
#include "SymbolTable.hpp"
#include "Sexp.hpp"

#define stdAssert(cond, msg)			\
  do {						\
    if (!(cond)) {				\
      err.str_val = msg;			\
    }						\
  } while (0)					\

namespace mc {

  Value add(Array<Value> values) {

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

      default:
	err.str_val = "+ function only takes ints and floats as arguments";
	return err;
	
      }
    }
    return ret;
  }

  Value mul(Array<Value> values) {

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

  Value bool_not(Array<Value> values) {
    Value err(Type::ERROR);
    stdAssert(values.size() == 1,
	      "not function only takes 1 argument");
    Value val = values.front();
    stdAssert(val.type == Type::BOOL,
	      "not function only takes boolean arguments");
    val.bool_val = !val.bool_val;
    return val;
  }

  Value bool_and(Array<Value> values) {
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
  
  Value bool_or(Array<Value> values) {
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

  Value equals(Array<Value> values) {
    Value err(Type::ERROR);
    stdAssert(values.size() > 1,
	      "equals function takes at least two arguments");
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

  Value print(Array<Value> values) {
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

  Value define(Array<Sexp*> args) {
    Value err(Type::ERROR);
    stdAssert(args.size() == 2,
	      "define macro only takes two arguments");
    Sexp* var = args[0];
    stdAssert(var->type == SexpType::VAR,
	      "first argument to define macro must be a variable");
    Value data(args[1]->eval());
    if (data.type == Type::ERROR) return data;
    ((VariableNode*)var)->setValue(data);
    return Value(Type::NIL);
  }

  Value _if(Array<Sexp*> args) {
    Value err(Type::ERROR);
    stdAssert(args.size() == 2 || args.size() == 3,
	      "if macro only takes two or three arguments");
    
    Value cond = args[0]->eval();
    if (cond.type == Type::ERROR) {
      return cond;
    }
    stdAssert(cond.type == Type::BOOL,
	      "first argument to if expression must evaluate to a bool");

    if (cond.bool_val) {
      return args[1]->eval();
    } else if (args.size() > 2) {
      return args[2]->eval();
    } else {
      return Value(Type::NIL);
    }
    
  }
  
  Function add_func(add);
  Function mul_func(mul);
  Function not_func(bool_not);
  Function and_func(bool_and);
  Function or_func(bool_or);
  Function equals_func(equals);
  Function print_func(print);
  Macro define_macro(define);
  Macro if_macro(_if);

  void initStandardLib() {
    
    SymbolTable::Id id;
    Value* val;

    id = SymbolTable::addSymbol("+");
    val = SymbolTable::getValue(id);
    val->type = Type::FUNCTION;
    val->func_val = &add_func;

    id = SymbolTable::addSymbol("*");
    val = SymbolTable::getValue(id);
    val->type = Type::FUNCTION;
    val->func_val = &mul_func;

    id = SymbolTable::addSymbol("not");
    val = SymbolTable::getValue(id);
    val->type = Type::FUNCTION;
    val->func_val = &not_func;

    id = SymbolTable::addSymbol("and");
    val = SymbolTable::getValue(id);
    val->type = Type::FUNCTION;
    val->func_val = &and_func;

    id = SymbolTable::addSymbol("or");
    val = SymbolTable::getValue(id);
    val->type = Type::FUNCTION;
    val->func_val = &or_func;

    id = SymbolTable::addSymbol("=");
    val = SymbolTable::getValue(id);
    val->type = Type::FUNCTION;
    val->func_val = &equals_func;

    id = SymbolTable::addSymbol("print");
    val = SymbolTable::getValue(id);
    val->type = Type::FUNCTION;
    val->func_val = &print_func;

    id = SymbolTable::addSymbol("define");
    val = SymbolTable::getValue(id);
    val->type = Type::MACRO;
    val->macro_val = &define_macro;

    id = SymbolTable::addSymbol("if");
    val = SymbolTable::getValue(id);
    val->type = Type::MACRO;
    val->macro_val = &if_macro;
      
  }
  
}
