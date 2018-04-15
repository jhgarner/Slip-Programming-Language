#include <iostream>
#include <fstream>
#include <algorithm>
#include "context.hpp"
#include "language.hpp"

map<string, string> Context::globals{};
map<string, function<string(vector<string>)>> Context::builtIn{};

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

// string Context::seq(vector<string> l) {
//      string last = "";
//      for (u_int i = 0; i < l.size(); i++) {
//           auto val = l.at(i);
//           last = val;
//           this->scope["s" + to_string(i)] = [val](vector<string> _) {return val;};
//      }
//      return last;
// }

// string Context::isDef(vector<string> l) {
//      for (auto val : this->scope)
//           cout << "Got: " << val.first << endl;
//      if (this->scope.count(l.at(0)) != 0) {
//           return "1";
//      }
//      return "0";
// }

// // Modification to scope in eval will not propagate up
// string Context::eval(vector<string> l) {
     // for (auto v : scope) {
     //      cout << v.first << endl;
     //      if (i++ > 5) break;
     // }
//      auto newScope = this->scope;
//      for (u_int i = 0; i < l.size(); i++) {
//           auto val = l.at(i);
//           newScope["p" + to_string(i - 1)] = [val](vector<string> _) {return val;};
//      }
//      Context newContext = Context(newScope);
//      auto func = l.at(0);
//      auto funcB = func.begin();
//      cout << "string: " << string(funcB, func.end()) << endl;
//      string r = newContext.execute(funcB, func.end());
//      return r;
// }
// Exec has full power over the current scope
// string Context::exec(vector<string> l) {
//      for (u_int i = 0; i < l.size(); i++) {
//           auto val = l.at(i);
//           scope["p" + to_string(i)] = [val](vector<string> _) {return val;};
//      }
//      auto func = l.at(0);
//      auto funcB = func.begin();
//      return this->execute(funcB, func.end());
// }

// string Context::defineG(vector<string> l) {
//      for (u_int i = 0; i < l.size(); i += 2) {
//           string val = l.at(i + 1);
//           // This function ignores the parameter and always returns the same thing
//           cout << "Defining " << l.at(i) << " as " << val << endl;
//           (Context::global)[l.at(i)] = [val, this](vector<string> u) {
//                                             u.insert(u.begin(), val);
//                                             return this->exec(u);
//                                        };
//      }
//      return "Defined";
// }
// string Context::define(vector<string> l) {
//      for (u_int i = 0; i < l.size(); i += 2) {
//           string val = l.at(i + 1);
//           // This function ignores the parameter and always returns the same thing
//           cout << "Defining " << l.at(i) << " as " << val << endl;
//           (Context::global)[l.at(i)] = [val, this](vector<string> u) {
//                                             u.insert(u.begin(), val);
//                                             return this->eval(u);
//                                        };
//      }
//      return "Defined";
// }
string Context::let(vector<string> p) {
     for (u_int i = 0; i < p.size(); i += 2) {
          (scope)[p.at(i)] = p.at(i + 1);
     }
     return "Created";
}
string Context::load(vector<string> l) {
     for (auto file: l) {
          ifstream ifs(file);
          string line = "";
          string tmpLine = "";
          while (getline(ifs, tmpLine)) {
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
          builtIn = addAll();
          builtIn["let"] = bind(&Context::let, this, placeholders::_1);
          builtIn["load"] = bind(&Context::load, this, placeholders::_1);
     }

}

string Context::evaluate(string s) {
     auto begin{s.begin() + 1};
     auto end{s.end() - 1};
     if (s.at(0) == '`') {
          begin++;
          end--;
     }
     vector<string> result = execute(begin, end);
     auto newScope = scope;
     for (int i = 1; i < result.size(); i++) {
          auto& val = result[i];
          // Remove escape characters
          // v.erase(remove_if(v.begin(), v.end(), [](char x){return x == '\\';}), v.end());
          // Replace $vars with their values
          if (val.at(0) == '$') {
               try {
                    string varName = string(val.begin() + 1, val.end());
                    if (scope.count(varName) != 0)
                         val = scope.at(varName);
                    else
                         val = globals.at(varName);
               } catch (exception e) {
                    throw runtime_error("Unable to find: " + val);
               }
          } else if (val.at(0) == '(') {
               Context cont(scope);
               val = cont.evaluate(val);
          }
          newScope["p" + to_string(i)] = val;
     }
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
          cout << "The following were defined:";
          for (auto v: scope)
               cout << ", " << v.first << ": " << v.second;
          cout << endl;
          throw runtime_error("Unable to call: " + result.at(0));
     }
}


vector<string> Context::execute(string::iterator &s, string::iterator end) {
     vector<string> params;
     string::iterator last(s);
     bool clean;
     for (;s != end; s++) {
          switch (*s) {
          case '(':
               if (last != s) {
                    throw runtime_error("A word containing parenthesis must be wrapped in quotes");
               } else {
                    clean = parens(++s, end) || clean;
                    params.push_back(string(last, s + 1));
                    last = s + 1;
               }
               break;
          case '\t':
          case ' ':
               if (last != s) {
                    params.push_back(string(last, s));
               }
               last = s + 1;
               break;
          case '\\':
               clean = true;
               break;
          case '`':
               last = s;
               clean = quoted(++s, end) || clean;
               params.push_back(string(last, s + 1));
               last = s + 1;
               break;
          case ')':
               if (last != s) {
                    // Catches case "))"
                    params.push_back(string(last, s));
               } else if (params.size() == 0) {
                    throw runtime_error("Unable to find a function in expression");
               }
          }
     }
     if (last != s)
          params.push_back(string(last, s));
     return params;
}

bool Context::parens(string::iterator &s, string::iterator end) {
     bool clean = false;
     for (;s != end; s++) {
          if (*s == ')') {
               return clean;
          } else if (*s == '(') {
               parens(++s, end);
          } else if (*s == '\\') {
               s++;
               clean = true;
          }
     }
     throw runtime_error("No closing paren found");
}

bool Context::quoted(string::iterator &s, string::iterator end) {
     bool clean = false;
     for (;s != end; s++) {
          if (*s == '\'') {
               return clean;
          } else if (*s == '`') {
               quoted(++s, end);
          } else if (*s == '\\') {
               s++;
               clean = true;
          }
     }
     throw runtime_error("No closing quote found");
}
