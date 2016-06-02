#include "Parser.hpp"
#include "Compiler.hpp"

int main() {
  
  mc::String test("(define test (+ 1 2)) (define test2 3.01) (if (or (= test test2) #t) (print \"true\\n\") \"false\")");
  mc::Compiler compiler;
  printf("Input:\n%s\n\n", test.c_str());
  compiler.parseString(test); 
  printf("Output:\n%s\n", (mc::to_string(compiler.eval())).c_str());
  return 0;

}
