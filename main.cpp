#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <fstream>
#include <editline/readline.h>
#include <cstring>
#include "context.hpp"

using namespace std;



int main() {
     map<string, string> empty{};
     Context context(empty);
     char* code;
     read_history("/tmp/.lisp");
     while ((code = readline("> ")) != NULL) {
          try {
               while (!validate(code)) {
                    strcat(code, readline(">> "));
               }
               cout << context.evaluate(code) << endl;
          } catch (runtime_error e){
               cout << e.what() << " when parsing \n" << code << endl;;
          }
          add_history(code);
          free(code);
          write_history("/tmp/.lisp");
     }
  // string l = "(* (/ \"6\" 5) (eval \"(- 4 5)\"))";
  // map<string, function<string(vector<string>)>> m{{"+", add}, {"-", sub}, {"*", mul}, {"/", divis}, {"eval", eval}};
  // string::iterator begin(l.begin());
  // cout << execute(m, begin, l.end()) << endl;
}

// Repl related functions
