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
     // Generate the global scope
     map<string, string> empty{};
     Context context(empty);
     char* code;
     read_history("/tmp/.lisp"); // Stores the history in ram on most Linux computers
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
}
