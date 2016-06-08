#pragma once

#include "String.hpp"
#include "Types.hpp"
#include "Array.hpp"
#include "Macros.hpp"

namespace mc {

  MC_ENUM(TokenId,
	  LPAREN,
	  RPAREN,
	  LBRACE,
	  RBRACE,
	  SYMBOL,
	  STRING,
	  BOOL,
	  CHAR,
	  INT,
	  FLOAT,
	  NIL,
	  NO_TOKEN,
	  ERROR);

  struct Token {

    TokenId id;

    union {
      bool bool_val;
      char char_val;
      i32 int_val;
      f32 float_val;
      String str_val;
    };

    Token(TokenId _id);
    Token(const Token& b);
    Token(Token&& b);
    ~Token();
    void operator=(const Token& b);
    void operator=(Token&& b);
    
  };

  struct Lexer {

    String str;
    Array<Token> token_stream;
    Token cur_token;
    size_t index;
    bool is_comment;

    Lexer(String _str) :
      str(_str),
      cur_token(TokenId::NO_TOKEN),
      index(0),
      is_comment(false) {}

    char curChar();
    char peakChar();
    char eatChar();
    String eatWord();
    void pushToken();

    bool handleQuote();
    bool handleNumber();
    void lex();
  };

  String to_string(Token& tok);

}
