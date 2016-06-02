#include "Parser.hpp"

namespace mc {

  void Parser::addSexp(Sexp* sexp) {
    if (sexp_stack.empty()) {
      ast.push_back(sexp);
    } else {
      sexp_stack.back()->sexps.push_back(sexp);
    }
  }
  
  void Parser::parse() {
    for (Token tok : tokens) {

      switch (tok.id) {

      case TokenId::LPAREN:
	{
	  ExprNode* node = new ExprNode();
	  addSexp(node);
	  sexp_stack.push_back(node);
	}
	break;

      case TokenId::RPAREN:
	sexp_stack.pop_back();
	break;

      case TokenId::SYMBOL:
	{
	  SymbolTable::Id id = SymbolTable::findSymbol(tok.str_val);
	  if (id == SymbolTable::NO_SYMBOL) {
	    id = SymbolTable::addSymbol(tok.str_val);
	  }
	  VariableNode* node = new VariableNode(tok.str_val, id);
	  addSexp(node);
	}
	break;

      case TokenId::STRING:
	{
	  ConstNode* node = new ConstNode(Type::STRING);
	  addSexp(node);
	  node->value.str_val = tok.str_val;
	}
	break;

      case TokenId::BOOL:
	{
	  ConstNode* node = new ConstNode(Type::BOOL);
	  addSexp(node);
	  node->value.bool_val = tok.bool_val;
	}
	break;

      case TokenId::CHAR:
	{
	  ConstNode* node = new ConstNode(Type::CHAR);
	  addSexp(node);
	  node->value.char_val = tok.char_val;
	}
	break;

      case TokenId::INT:
	{
	  ConstNode* node = new ConstNode(Type::INT);
	  addSexp(node);
	  node->value.int_val = tok.int_val;
	}
	break;

      case TokenId::FLOAT:
	{
	  ConstNode* node = new ConstNode(Type::FLOAT);
	  addSexp(node);
	  node->value.float_val = tok.float_val;
	}
	break;

      case TokenId::NIL:
	{
	  ConstNode* node = new ConstNode(Type::NIL);
	  addSexp(node);
	}
	break;

      case TokenId::ERROR:
	{
	  ConstNode* node = new ConstNode(Type::ERROR);
	  addSexp(node);
	  node->value.str_val = tok.str_val;
	}
	break;

      case TokenId::NO_TOKEN:
	assert(false);
	break;
	    
      }
	
    }

    if (!sexp_stack.empty()) {
      ConstNode* node = new ConstNode(Type::ERROR);
      node->value.str_val = "unterminated expression";
      ast.push_back(node);
    }
    
    if (ast.empty()) {
      addSexp(new ConstNode(Type::NIL));
    }
  }

}
