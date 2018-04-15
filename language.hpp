#include <vector>
#include <string>
#include <functional>
#include <map>

using namespace std;

// grouping functions
void addNum(map<string, function<string(vector<string>)>>& m);
void addIO(map<string, function<string(vector<string>)>>& m);
void addString(map<string, function<string(vector<string>)>>& m);
map<string, function<string(vector<string>)>> addAll();

// Functions built into the language
string add(vector<string>);
string sub(vector<string>);
string mul(vector<string>);
string divis(vector<string>);
string ls(vector<string>);
string more(vector<string>);
string eq(vector<string>);
string print(vector<string>);
string input(vector<string>);
string concat(vector<string>);
string at(vector<string>);
string andL(vector<string>);
string orL(vector<string>);
string notL(vector<string>);
string define(vector<string>);
string id(vector<string>);
