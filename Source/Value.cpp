#include "Value.hpp"
#include "Sexp.hpp"

namespace mc {

  Value::Value(Type _type) : type(_type) {
    switch (type) {
    case Type::STRING: case Type::ERROR:
      new (&str_val) String();
      break;
    case Type::ARRAY:
      new (&array_val) Array<Value>();
      break;
    default: break;
    }
  }
  
  Value::Value(const Value& b) : type(b.type) {
    switch (type) {
    case Type::BOOL:
      bool_val = b.bool_val;
      break;
    case Type::CHAR:
      char_val = b.char_val;
      break;
    case Type::INT:
      int_val = b.int_val;
      break;
    case Type::FLOAT:
      float_val = b.float_val;
      break;
    case Type::FUNCTION:
      func_val = b.func_val;
      break;
    case Type::INTERNAL_FUNC:
      internal_func_val = b.internal_func_val;
      break;
    case Type::INTERNAL_MACRO:
      internal_macro_val = b.internal_macro_val;
      break;
    case Type::STRING: case Type::ERROR:
      new (&str_val) String(b.str_val);
      break;
    case Type::ARRAY:
      new (&array_val) Array<Value>(b.array_val);
      break;
    default: break;
    }
  }
  
  Value::Value(Value&& b) : type(b.type) {
    switch (type) {
    case Type::BOOL:
      bool_val = b.bool_val;
      break;
    case Type::CHAR:
      char_val = b.char_val;
      break;
    case Type::INT:
      int_val = b.int_val;
      break;
    case Type::FLOAT:
      float_val = b.float_val;
      break;
    case Type::FUNCTION:
      func_val = b.func_val;
      break;
    case Type::INTERNAL_FUNC:
      internal_func_val = b.internal_func_val;
      break;
    case Type::INTERNAL_MACRO:
      internal_macro_val = b.internal_macro_val;
      break;
    case Type::STRING: case Type::ERROR:
      new (&str_val) String(b.str_val);
      break;
    case Type::ARRAY:
      new (&array_val) Array<Value>(b.array_val);
      break;
    default: break;
    }
  }
  
  Value::~Value() {
    switch (type) {
    case Type::STRING: case Type::ERROR:
      str_val.~String();
      break;
    case Type::ARRAY:
      array_val.~Array<Value>();
      break;
    default: break;
    }
  }
  
  void Value::operator=(const Value& b) {
    switch (type) {
    case Type::STRING: case Type::ERROR:
      str_val.~String();
      break;
    case Type::ARRAY:
      array_val.~Array<Value>();
      break;
    default: break;
    }
    new (this) Value(b);
  }
  
  void Value::operator=(Value&& b) {
    switch (type) {
    case Type::STRING: case Type::ERROR:
      str_val.~String();
      break;
    case Type::ARRAY:
      array_val.~Array<Value>();
      break;
    default: break;
    }
    new (this) Value(b);
  }

  bool Value::operator==(Value& b) {
    switch(type) {
    case Type::BOOL:
      if (b.type != Type::BOOL) return false;
      return (bool_val == b.bool_val);
    case Type::CHAR:
      if (b.type != Type::CHAR) return false;
      return (char_val == b.char_val);
    case Type::INT:
      if (b.type == Type::INT) {
	return (int_val == b.int_val);
      } else if (b.type == Type::FLOAT) {
	return (int_val == b.float_val);
      } else {
	return false;
      }
    case Type::FLOAT:
      if (b.type == Type::INT) {
	return (float_val == b.int_val);
      } else if (b.type == Type::FLOAT) {
	return (float_val == b.float_val);
      } else {
	return false;
      }
    case Type::STRING:
      if (b.type != Type::STRING) return false;
      return (str_val == b.str_val);
    case Type::ARRAY:
      {
	if (b.type != Type::ARRAY) return false;
	if (array_val.size() != b.array_val.size()) return false;
	for (u32 n=0; n<array_val.size(); ++n) {
	  if (array_val[n] != b.array_val[n]) return false;
	}
	return true;
      }
    case Type::FUNCTION:
      if (b.type != Type::FUNCTION) return false;
      return (func_val == b.func_val);
    case Type::INTERNAL_FUNC:
      if (b.type != Type::FUNCTION) return false;
      return (internal_func_val == b.internal_func_val);
    case Type::INTERNAL_MACRO:
      if (b.type != Type::INTERNAL_MACRO) return false;
      return (internal_macro_val == b.internal_macro_val);
    case Type::ERROR:
      if (b.type != Type::ERROR) return false;
      return (str_val == b.str_val);
    case Type::NIL:
      return (b.type == Type::NIL);
    }
  }

  bool Value::operator!=(Value& b) {
    return (!(*this == b));
  }

  String to_string(Value val) {
    switch(val.type) {
    case Type::BOOL:
      if (val.bool_val) {
	return "#t";
      } else {
	return "#f";
      }
    case Type::CHAR:
      {
	String ret;
	ret += val.char_val;
	return ret;
      }
    case Type::INT:
      return to_string(val.int_val);
    case Type::FLOAT:
      return to_string(val.float_val);
    case Type::STRING:
      return val.str_val;
    case Type::ARRAY:
      {
	String ret = "[ ";
	for (Value& elem : val.array_val) {
	  ret += to_string(elem) + " ";
	}
	return ret + "]";
      }
    case Type::FUNCTION:
      return "<function>";
    case Type::INTERNAL_FUNC:
      return "<built-in function>";
    case Type::INTERNAL_MACRO:
      return "<built-in macro>";
    case Type::ERROR:
      return "Error: " + val.str_val;
    case Type::NIL:
      return "nil";
    }
  }

}
