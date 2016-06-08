#include "Parser.hpp"

namespace mc {

  void Parser::addSexp(Sexp* sexp) {
    if (sexp_stack.empty()) {
      ast.push_back(sexp);
    } else {
      Sexp* back = sexp_stack.back();
      if (back->type == SexpType::EXPR) {
	((ExprNode*)back)->sexps.push_back(sexp);
      } else if (back->type == SexpType::ARRAY) {
	((ArrayNode*)back)->sexps.push_back(sexp);
      } else {
	assert(false);
      }
    }
  }
  
  void Parser::parse(SymbolTable* table) {
    
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
	if (sexp_stack.empty()) {
	  
	  ConstNode* node = new ConstNode(Type::ERROR);
	  node->value.str_val = "unexpected \')\'";
	  ast.push_back(node);
	  
	} else {
	  
	  Sexp* sexp = sexp_stack.back();
	  sexp_stack.pop_back();
	  assert(sexp->type == SexpType::EXPR);
	  ExprNode* expr = (ExprNode*) sexp;
	  
	  if (!expr->sexps.empty()) {

	    Sexp* front = expr->sexps.front();
	    if (front->type != SexpType::VAR) {
	      break;
	    }
	    
	    Value func(front->eval(table));
	    
	    if (func.type == Type::INTERNAL_MACRO) {
	      Array<Sexp*> args;
	      args.reserve(expr->sexps.size() - 1);
	      for (u32 n=1; n<expr->sexps.size(); ++n) {
		args.push_back(expr->sexps[n]);
	      }
	      Sexp* new_sexp = (*func.internal_macro_val)(args, table);

	      expr->sexps.clear();
	      delete expr;

	      if (sexp_stack.empty()) {
		
		ast.pop_back();
		ast.push_back(new_sexp);
		
	      } else {
		
		Sexp* sexp2 = sexp_stack.back();
		assert(sexp2->type == SexpType::EXPR);
		ExprNode* expr2 = (ExprNode*) sexp2;

		expr2->sexps.pop_back();
		expr2->sexps.push_back(new_sexp);
		
	      }
	    }
	  }
	}
	break;

      case TokenId::LBRACE:
	{
	  ArrayNode* node = new ArrayNode();
	  addSexp(node);
	  sexp_stack.push_back(node);
	}
	break;

      case TokenId::RBRACE:
	sexp_stack.pop_back();
	break;

      case TokenId::SYMBOL:
	{
	  if (!table->hasSymbol(tok.str_val)) {
	    table->addSymbol(tok.str_val);
	  }
	  VariableNode* node = new VariableNode(tok.str_val);
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
