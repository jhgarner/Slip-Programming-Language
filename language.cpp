#include <iostream>
#include "language.hpp"
#include "context.hpp"
#include <cstdlib>

// helper functions
void addNum(map<string, function<string(vector<string>)>>& m) {
     m["+"] = add;
     m["-"] = sub;
     m["*"] = mul;
     m["/"] = divis;
     m[">"] = more;
     m["<"] = ls;
     m["="] = eq;
     m["&&"] = andL;
     m["||"] = orL;
     m["!"] = notL;
     m["id"] = id;
     m["rand"] = randomL;
}
void addIO(map<string, function<string(vector<string>)>>& m) {
     m["print"] = print;
     m["input"] = input;
     m["define"] = define;
}
void addString(map<string, function<string(vector<string>)>>& m) {
     m["++"] = concat;
     m["at"] = at;
}

map<string, function<string(vector<string>)>> addAll() {
     srand(time(0));
     map<string, function<string(vector<string>)>> m{};
     addNum(m);
     addIO(m);
     addString(m);
     return m;
}

// Turn 4.0 into 4
string simplify(double x) {
     if (x == (int) x) {
          return to_string((int)x);
     }
     return to_string(x);
}

// Numeric functions
string add(vector<string> l) {
     auto acc = 0.0;
     for (auto n : l) {
          acc += stod(n);
     }
     return simplify(acc);
}
string sub(vector<string> l) {
     auto acc = stod(l.at(0));
     for (u_int i = 1; i < l.size(); i++) {
          acc -= stod(l.at(i));
     }
     return simplify(acc);
}
string mul(vector<string> l) {
     auto acc = 1.0;
     for (auto n : l) {
          acc *= stod(n);
     }
     return simplify(acc);
}
string divis(vector<string> l) {
     auto acc = stod(l.at(0));
     for (u_int i = 1; i < l.size(); i++) {
          acc /= stod(l.at(i));
     }
     return simplify(acc);
}

string randomL(vector<string> p) {
     if (p[0] == "0") {
          return "0";
     }
     return to_string(rand() % stoi(p[0]));
}

// "0" is false and "1" is true
string ls(vector<string> l) {
     for (u_int i = 0; i < l.size() - 1; i++) {
          auto fst = stod(l.at(i));
          auto snd = stod(l.at(i + 1));
          if (fst >= snd) {
               return "0";
          }
     }
     return "1";
}

string more(vector<string> l) {
     for (u_int i = 0; i < l.size() - 1; i++) {
          auto fst = stod(l.at(i));
          auto snd = stod(l.at(i + 1));
          if (fst <= snd) {
               return "0";
          }
     }
     return "1";
}

string eq(vector<string> l) {
     for (u_int i = 0; i < l.size() - 1; i++) {
          auto fst = stod(l.at(i));
          auto snd = stod(l.at(i + 1));
          if (fst != snd) {
               return "0";
          }
     }
     return "1";
}
string andL(vector<string> l) {
     for (u_int i = 0; i < l.size() - 1; i++) {
          auto fst = stoi(l.at(i));
          auto snd = stoi(l.at(i + 1));
          if (!(fst && snd)) {
               return "0";
          }
     }
     return "1";
}
string orL(vector<string> l) {
     for (u_int i = 0; i < l.size() - 1; i++) {
          auto fst = stoi(l.at(i));
          auto snd = stoi(l.at(i + 1));
          if (!(fst || snd)) {
               return "0";
          }
     }
     return "1";
}
string notL(vector<string> l) {
     return to_string(!stoi(l.at(0)));
}

// IO functions
string print(vector<string> l) {
     for (auto val : l) {
          cout << val << " ";
     }
     cout << endl;
     return "Printed";
}
string input(vector<string> l) {
     cout << l.at(0) << " ";
     string val = "";
     cin >> val;
     return val;
}

// String functions
string concat(vector<string> l) {
     string result = "";
     for (auto val: l) {
          result += val;
     }
     return result;
}

string at(vector<string> l) {
     return to_string(l.at(0).at(stoi(l.at(1))));
}

string define(vector<string> p) {
     for (u_int i = 0; i < p.size(); i += 2) {
          (Context::globals)[p.at(i)] = p.at(i + 1);
     }
     return "Defined";
}

// Returns the last value passed to it
string id(vector<string> p) {
     return p.at(p.size() - 1);

}
