#include "Compiler.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "StandardLib.hpp"
#include <cstdio>

namespace mc {

  Compiler::Compiler() {
    initStandardLib();
  }

  Compiler::~Compiler() {
    for (Sexp* sexp : ast) {
      delete sexp;
    }
  }

  void Compiler::parseString(String code) {

    Lexer lexer(code);
    lexer.lex();
    //printf("Tokens: %s\n", to_string(lexer.token_stream).c_str());
    Parser parser(lexer.token_stream);
    parser.parse();
    ast = parser.ast;

  }

  Value Compiler::eval() {
    assert(!ast.empty());
    for (u32 n=0; n<ast.size()-1; ++n) {
      ast[n]->eval();
    }
    return ast.back()->eval();
  }

}
