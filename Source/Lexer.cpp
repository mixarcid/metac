#include "Lexer.hpp"
#include <stdexcept>

namespace mc {

  const char COMMENT_CHAR = '#';

  inline bool isDigit(char c) {
    return (c >= '0' && c <= '9');
  }

  inline bool isWhiteSpace(char c) {
    return (c == ' ' ||
	    c == '\n' ||
	    c == '\t');
  }

  inline bool isEndMark(char c) {
    return (c == ')' ||
	    c == '(' ||
	    c == ']' ||
	    c == '[' ||
	    c == COMMENT_CHAR ||
	    isWhiteSpace(c));
  }

  Token::Token(TokenId _id) : id(_id) {
    switch (id) {
    case TokenId::SYMBOL:
    case TokenId::STRING:
    case TokenId::ERROR:
      new (&str_val) String();
      break;
    default: break;
    }
  }

  Token::Token(const Token& b) : id(b.id) {
    switch (id) {
    case TokenId::SYMBOL:
    case TokenId::STRING:
    case TokenId::ERROR:
      new (&str_val) String(b.str_val);
      break;
    case TokenId::BOOL:
      bool_val = b.bool_val;
      break;
    case TokenId::CHAR:
      char_val = b.char_val;
      break;
    case TokenId::INT:
      int_val = b.int_val;
      break;
    case TokenId::FLOAT:
      float_val = b.float_val;
      break;
    default: break;
    }
  }

  
  Token::Token(Token&& b) : id(b.id) {
    switch (id) {
    case TokenId::SYMBOL:
    case TokenId::STRING:
    case TokenId::ERROR:
      new (&str_val) String(b.str_val);
      break;
    case TokenId::BOOL:
      bool_val = b.bool_val;
      break;
    case TokenId::CHAR:
      char_val = b.char_val;
      break;
    case TokenId::INT:
      int_val = b.int_val;
      break;
    case TokenId::FLOAT:
      float_val = b.float_val;
      break;
    default: break;
    }
  }
  
  Token::~Token() {
    switch (id) {
    case TokenId::SYMBOL:
    case TokenId::STRING:
    case TokenId::ERROR:
      str_val.~String();
      break;
    default: break;
    }
  }

  void Token::operator=(const Token& b) {
    switch (id) {
    case TokenId::SYMBOL:
    case TokenId::STRING:
    case TokenId::ERROR:
      str_val.~String();
      break;
    default: break;
    }
    new (this) Token(b);
  }

  void Token::operator=(Token&& b) {
    switch (id) {
    case TokenId::SYMBOL:
    case TokenId::STRING:
    case TokenId::ERROR:
      str_val.~String();
      break;
    default: break;
    }
    new (this) Token(b);
  }

  String to_string(Token& tok) {

    String ret = "< ";
    ret += TokenId_names<i32>[(u32)tok.id];
    ret += " , ";

    switch (tok.id) {

    case TokenId::LPAREN:
    case TokenId::RPAREN:
    case TokenId::LBRACE:
    case TokenId::RBRACE:
    case TokenId::NO_TOKEN:
    case TokenId::NIL:
      ret += "nil";
      break;
    case TokenId::BOOL:
      if (tok.bool_val) {
	ret += "#t";
      } else {
	ret += "#f";
      }
    case TokenId::CHAR:
      ret += "\'";
      ret += tok.char_val;
      ret += "\'";
      break;
    case TokenId::INT:
      ret += to_string(tok.int_val);
      break;
    case TokenId::FLOAT:
      ret += to_string(tok.float_val);
      break;
    case TokenId::SYMBOL:
    case TokenId::ERROR:
    case TokenId::STRING:
      ret += "\"" + tok.str_val + "\"";
      break;
      
    }
    ret += " >";
    return ret;
  }

  char Lexer::curChar() {
    char ret = '\0';
    if (index > 0) {
      ret = str.at(index-1);
    }
    return ret;
  }

  char Lexer::peakChar() {
    char ret = '\0';
    if (index < str.size()) {
      ret = str.at(index);
    }
    return ret;
  }

  char Lexer::eatChar() {
    char ret = peakChar();
    ++index;
    return ret;
  }

  String Lexer::eatWord() {
    String ret;
    char cur_char = curChar();
    while (!isEndMark(cur_char) &&
	   index <= str.size()) {
      ret += cur_char;
      cur_char = eatChar();
    }
    if (index != 0) {
      --index;
    }
    return ret;
  }

  void Lexer::pushToken() {
    token_stream.push_back(cur_token);
    cur_token = Token(TokenId::NO_TOKEN);
  }

  bool Lexer::handleQuote() {

    if (curChar() != '\"') return false;

    cur_token = Token(TokenId::STRING);
    bool escape = false;
    
    while (index < str.size()) {
      
      char c = eatChar();

      switch (c) {
	
      case '\"':
	if (escape) {
	  cur_token.str_val += "\"";
	} else {
	  return true;
	}
	break;

      case 'n':
	if (escape) {
	  cur_token.str_val += "\n";
	} else {
	  cur_token.str_val += "n";
	}
	break;

      case '\\':
	if (escape) {
	  cur_token.str_val += "\\";
	}
	break;

      default:
	cur_token.str_val += c;
	break;
	
      }
      if (c == '\\') {
	escape = true;
      } else {
	escape = false;
      }
    }

    cur_token = Token(TokenId::ERROR);
    cur_token.str_val = "unterminated quote";
    return true;
  }

  bool Lexer::handleNumber() {
    
    char cur_char = curChar();
    if (cur_char == '+' || cur_char == '-') {
      char p = peakChar();
      if (isEndMark(p)) return false;
    } else if (!isDigit(cur_char)) {
      return false;
    }

    cur_token = Token(TokenId::INT);
    String word = eatWord();
    for (size_t n=1; n < word.size(); ++n) {
      char c = word.at(n);
      if (cur_token.id != TokenId::ERROR) {
	if (c == '.') {
	  switch (cur_token.id) {
	  case TokenId::INT:
	    cur_token = Token(TokenId::FLOAT);
	    break;
	  case TokenId::FLOAT:
	    cur_token = Token(TokenId::ERROR);
	    cur_token.str_val = "cannot parse number";
	    break;
	  default: break;
	  }
	} else if (!isDigit(c)) {
	  cur_token = Token(TokenId::ERROR);
	  cur_token.str_val = "cannot parse number";
	}
      }
    }

    switch (cur_token.id) {
    case TokenId::INT:
      try {
	cur_token.int_val = std::stoi(word, NULL, 0);
      } catch(std::invalid_argument&) {
	cur_token = Token(TokenId::ERROR);
	cur_token.str_val = "cannot parse number";
      }
      break;
    case TokenId::FLOAT:
      try {
	cur_token.float_val = std::stof(word);
      } catch(std::invalid_argument&) {
	cur_token = Token(TokenId::ERROR);
	cur_token.str_val = "cannot parse number";
      }
      break;
    default: break;
    }
    
    return true;
  }

  void Lexer::lex() {
    
    while (index < str.size()) {
      
      char c = eatChar();
      
      if (is_comment) {
	if (c == '\n') {
	  is_comment = false;
	}
	continue;
      } else if (isWhiteSpace(c)) {
	continue;
      }
      
      switch (c) {
	
      case '(':
	cur_token = Token(TokenId::LPAREN);
	pushToken();
	break;
	
      case ')':
	cur_token = Token(TokenId::RPAREN);
	pushToken();
	break;

      case '[':
	cur_token = Token(TokenId::LBRACE);
	pushToken();
	break;
	
      case ']':
	cur_token = Token(TokenId::RBRACE);
	pushToken();
	break;

      case COMMENT_CHAR:
	is_comment = true;
	break;

      default:

	if (!handleNumber() &&
	    !handleQuote()) {
	  String word = eatWord();

	  if (word == "nil") {
	    cur_token = Token(TokenId::NIL);
	  } else if (word == "#t") {
	    cur_token = Token(TokenId::BOOL);
	    cur_token.bool_val = true;
	  } else if (word == "#f") {
	    cur_token = Token(TokenId::BOOL);
	    cur_token.bool_val = false;
	  } else {
	  
	    cur_token = Token(TokenId::SYMBOL);
	    cur_token.str_val = word;
	    for (size_t n=1; n < word.size(); ++n) {
	      char c = word.at(n);
	      if (c == '\'' || c == '\"') {
		cur_token = Token(TokenId::ERROR);
		cur_token.str_val = "unexpected quotation mark in symbol";
	      }
	    }
	  }
	}
	pushToken();
      }

    }
  }
  
}
