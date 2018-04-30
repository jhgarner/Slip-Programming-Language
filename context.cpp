#include <iostream>
#include <fstream>
#include <algorithm>
#include "context.hpp"
#include "language.hpp"

// Global scope
map<string, string> Context::globals{};
// Global scope that is immutable
map<string, function<string(vector<string>)>> Context::builtIn{};

// Makes sure you have the right number of quotes and parenthesis
bool validate(string s) {
     int countP = 0;
     int countQ = 0;
     for (auto c : s) {
          if (c == '(')
               countP++;
          else if (c == ')')
               countP--;
          else if (c == '`')
               countQ++;
          else if (c == '\'')
               countQ--;
     }
     if (countP == 0 && countQ == 0) {
          return true;
     } else if (countP < 0) {
          throw runtime_error("Too many close Parentheses");
     } else if (countQ < 0) {
          throw runtime_error("Too many close quotes");
     }
     return false;
}

// Returns true if a name is defined
string Context::isDef(vector<string> l) {
     if (this->scope.count(l.at(0)) != 0 || globals.count(l.at(0)) != 0 || builtIn.count(l.at(0)) != 0) {
          return "1";
     }
     return "0";
}

// Runs all code in a file
string Context::load(vector<string> l) {
     for (auto file: l) {
          ifstream ifs(file);
          string line = "";
          string tmpLine = "";
          while (getline(ifs, tmpLine)) {
               // File comment
               if (tmpLine.at(0) == '#')
                    continue;
               line += tmpLine;
               if (validate(line)) {
                    this->evaluate(line);
                    line = "";
               }
          }
     }
     return "Loaded";
}

Context::Context(map<string, string> scope) {
     this->scope = scope;
     if (builtIn.empty()) {
          // Create the built in scope if it doesn't exist yet
          builtIn = addAll();
          builtIn["load"] = bind(&Context::load, this, placeholders::_1);
          builtIn["isDef"] = bind(&Context::isDef, this, placeholders::_1);
     }

}

string Context::evaluate(string s) {
     // Unwrap the main function for execution
     auto begin{s.begin() + 1};
     auto end{s.end() - 1};
     if (s.at(0) == '`') {
          begin++;
          end--;
     }

     // Parse the code
     vector<string> result = parse(begin, end);
     auto newScope = scope;

     // Loop over the parameters
     for (int i = 1; i < result.size(); i++) {
          auto& val = result[i];
          if (val.at(0) == '$') {
               // in this case we are refering to a parameter
               try {
                    string varName = string(val.begin() + 1, val.end());
                    if (scope.count(varName) != 0)
                         val = scope.at(varName);
                    else
                         val = globals.at(varName);
               } catch (exception e) {
                    throw runtime_error("Unable to find: " + val);
               }
          } else if (val.at(0) == '`') {
               // Unquote a string
               val = string(val.begin() + 1, val.end() - 1);
          } else if (val.at(0) == '(') {
               // Execute the nested block
               Context cont(scope);
               val = cont.evaluate(val);
          }
          // Add the value as a parameter
          newScope["p" + to_string(i)] = val;
     }

     // Recursively evaluate code until a builtin is reached
     if (result.at(0).at(0) == '(') {
          Context cont(newScope);
          string ret = cont.evaluate(result.at(0));
          return ret;
     } else if (scope.count(result.at(0)) != 0){
          Context cont(newScope);
          string ret = cont.evaluate(scope.at(result.at(0)));
          return ret;
     } else if (globals.count(result.at(0)) != 0){
          Context cont(newScope);
          string ret = cont.evaluate(globals.at(result.at(0)));
          return ret;
     } else if (builtIn.count(result.at(0)) != 0){
          Context cont(newScope);
          return builtIn.at(result.at(0))(vector<string>(result.begin() + 1, result.end()));
     } else {
          // The function does not exist
          cout << "The following had been defined:";
          for (auto v: scope)
               cout << ", " << v.first << ": " << v.second;
          cout << endl;
          throw runtime_error("Unable to call: " + result.at(0));
     }
}


vector<string> Context::parse(string::iterator &s, string::iterator end) {
     // Holds all words found
     vector<string> params;
     // Stores the beginning of the current word
     string::iterator last(s);
     for (;s != end; s++) {
          switch (*s) {
          case '(':
               if (last != s) {
                    // Catches W()rld
                    throw runtime_error("A word containing parenthesis must be wrapped in quotes");
               } else {
                    // catches (+ 1 1)
                    parens(++s, end);
                    params.push_back(string(last, s + 1));
                    last = s + 1;
               }
               break;
          case '\t':
          case ' ':
               // Catches whitespace
               if (last != s) {
                    params.push_back(string(last, s));
               }
               last = s + 1;
               break;
          case '`':
               // Catches `Hello World'
               last = s;
               quoted(++s, end);
               params.push_back(string(last, s + 1));
               last = s + 1;
               break;
          case ')':
               if (last != s) {
                    // Catches case "))"
                    params.push_back(string(last, s));
               } else if (params.size() == 0) {
                    // Catches ()
                    throw runtime_error("Unable to find a function in expression");
               }
          }
     }
     if (last != s)
          params.push_back(string(last, s));
     return params;
}

// Ignores everything in parenthesis
void Context::parens(string::iterator &s, string::iterator end) {
     for (;s != end; s++) {
          if (*s == ')') {
               return;
          } else if (*s == '(') {
               parens(++s, end);
          } else if (*s == '\\') {
               s++;
          }
     }
     throw runtime_error("No closing paren found");
}

// Ignores everything in quotes
void Context::quoted(string::iterator &s, string::iterator end) {
     for (;s != end; s++) {
          if (*s == '\'') {
               return;
          } else if (*s == '`') {
               quoted(++s, end);
          } else if (*s == '\\') {
               s++;
          }
     }
     throw runtime_error("No closing quote found");
}
