#pragma once
#include <string>
#include <vector>
#include <map>
#include <functional>
#include "scope.hpp"
#include "language.hpp"

using namespace std;

class Context {
public:
     Context(map<string, string> scope);
     string evaluate(string code);
     static map<string, string> globals;
private:

     // Executes the code that needs to be evaluated
     vector<string> parse(string::iterator &s, string::iterator end);
     // Leaves a string literal un-evaluated
     void quoted(string::iterator &s, string::iterator end);
     void parens(string::iterator &s, string::iterator end);

     map<string, string> scope;
     static map<string, function<string(vector<string>)>> builtIn;

     // Execute a piece of code in a different context
     string eval(vector<string> l); // (eval "(+ 1 1)") returns 2
     string exec(vector<string> l); // (exec "(+ 1 1)") returns 2
     // define a variable
     string define(vector<string> l); // (define name value)
     string let(vector<string> l); // (define name value)
     string seq(vector<string> l); // (define name value)
     string isDef(vector<string> l); // (define name value)
     string load(vector<string> l); // (define name value)
};
bool validate(string s);
