#include "Parser.hpp"
#include "Compiler.hpp"

using namespace mc;

void eval_print(String input, Compiler* compiler) {
  compiler->parseString(input);
  Value ret = compiler->eval();
  const char* str = to_string(ret).c_str();
  if (ret.type == Type::ERROR) {
    fprintf(stderr, "%s\n", str);
  } else {
    printf("%s\n", str);
  }
}

int main(int argc, char** argv) {

  Compiler compiler;

  if (argc == 1) {
    //repl
    char buffer[5000];
    while (true) {
      printf("> ");
      fgets(buffer, 5000, stdin);
      String input(buffer);
      if (input == "quit\n") break;
      eval_print(input, &compiler);
    }

  } else {
    //execute file
    FILE* file = fopen(argv[1], "r");
    if (!file) {
      fprintf(stderr, "Unable to open %s\n", argv[1]);
      return -1;
    }

    String input;
    while(!feof(file)) {
      input += fgetc(file);
    }
    input.pop_back();
    compiler.parseString(input);
    Value ret = compiler.eval();
    const char* str = to_string(ret).c_str();
    if (ret.type == Type::ERROR) {
      fprintf(stderr, "%s\n", str);
    }
  }
  return 0;
}
