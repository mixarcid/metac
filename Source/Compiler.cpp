#include "Compiler.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "StandardLib.hpp"
#include "Function.hpp"

namespace mc {

  Compiler::Compiler() : global_table(NULL) {
    initStandardLib(&global_table);
  }

  Compiler::~Compiler() {
    for (Sexp* sexp : ast) {
      delete sexp;
    }
    Function::cleanup();
  }

  void Compiler::parseString(String code) {

    Lexer lexer(code);
    lexer.lex();
    Parser parser(lexer.token_stream);
    parser.parse(&global_table);

    //printf("%s\n", to_string(lexer.token_stream).c_str());
    for (Sexp* sexp : ast) {
      delete sexp;
    }
    ast = parser.ast;

  }

  Value Compiler::eval() {
    assert(!ast.empty());
    for (u32 n=0; n<ast.size()-1; ++n) {
      Value ret = ast[n]->eval(&global_table);
      if (ret.type == Type::ERROR) {
	return ret;
      }
    }
    return ast.back()->eval(&global_table);
  }

}
