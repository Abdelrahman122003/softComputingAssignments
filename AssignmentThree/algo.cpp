#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>  // For rand() and srand()
#include <ctime>    // For time()
#include <map>
#include <algorithm>
#include <numeric>  // For std::accumulate
#include <cmath>  // for std::abs
#include <random>  // For better random number generation
#include <fstream>
using namespace std;
template <typename T>
ostream& operator<<(ostream& os, const vector<T>& vec)
{
    os << "[ ";
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        os << *it;
        if (it + 1 != vec.end()) {
            os << ", ";  // Print a comma after each element, except the last one
        }
    }
    os << " ]";
    return os;  // Return the ostream object to allow chaining
}


// Objects 
class Variable{
    string name;
    string type;
    pair<int, int> range;


    public:
    Variable(){

    }
    void setName(string _name){
        name = _name;
    }
    void setType(string _type){
        type = _type;
    }
    void setLower(int _lower){
        range.first = _lower;
    }
    void setUpper(int _upper){
        range.second = _upper;
    }
    string getName(){
        return name;
    }
    string getType(){
        return type;
    }

    // int get
    int getLower(){
        return range.first;
    }
    int getUpper(){
        return range.second;
    }
};

class Fuzzy{
    string name;
    string type;
    vector<int>values;
    public:
    Fuzzy(){

    }
    void setName(string _name){
        name = _name;
    }
    void setType(string _type){
        type = _type;
    }
    // add value
    void addValue(int _value){
        values.push_back(_value);
    }

    string getName(){
        return name;
    }
    string getType(){
        return type;
    }
    vector<int> getValues(){
        return values;
    }
    
};

class FuzzySet{
    string name;
    vector<Fuzzy> fuzzySet;

    FuzzySet(){

    }
    void setName(string _name){
        name = _name;
    }
    void setFuzzy(Fuzzy _fuzzy){
        fuzzySet.push_back(_fuzzy);
    }
    string getName(){
       return name;
    }
    vector<Fuzzy> getFuzzySet(){
        return fuzzySet; 
    }
};

class Rule{
    string operation;
    vector<string> variables;
    vector<string> sets;

    public:
    void setOperation(string _operation){
        operation = _operation;
    }
    void setVariable(string _variable){
        variables.push_back(_variable);
    }
    void setSet(string _set){
        sets.push_back(_set);
    }

    string getOperation()
    {
        return operation;
    }
    vector<string> getVariables(){
        return variables;
    }
    vector<string> getSets(){
        return sets;
    }
};

string extractToken(string& str, char delimiter) {
    size_t pos = str.find(delimiter);
    // To ensure that pos not out of bound
    if (pos == string::npos) {
        throw invalid_argument("Invalid input format");
    }
    string token = str.substr(0, pos);
    str = str.substr(pos + 1); // Remove the token and delimiter from the original string
    return token;
}


// ===> CODE FOR STORE VARIABLES IN FUZZY LOGIC ALGO 
Variable extractVariableData(string variable) {
    Variable v;
    // Example input: proj_funding IN [0, 100]

    // 1) Extract the Name
    v.setName(extractToken(variable, ' '));
    // Now, variable contains: IN [0, 100]

    // 2) Extract the Type
   v.setType(extractToken(variable, ' '));
    // Now, variable contains: [0, 100]

    // 3) Extract the Range
    if (variable.front() != '[' || variable.back() != ']') {
        throw invalid_argument("Range must be enclosed in square brackets");
    }
    
    // Extract the lower bound
   
    variable = variable.substr(1, variable.size() - 2); // Remove brackets
    string lower = extractToken(variable, ',');   // Get lower bound
    v.setLower(stoi(lower));
    v.setUpper(stoi(variable)); // Remaining part is the upper bound
  
    cout << "Name: " << v.getName() << endl;
    cout << "Type: " << v.getType() << endl;
    cout << "Lower bound: " << v.getLower() << endl;
    cout << "Upper bound: " << v.getUpper() << endl;

    return v;
}
void handleVariablesInput(){
    cout << "Enter the variable's name, type (IN/OUT) and range ([lower, upper]): (Press x to finish)\n";
    string line;
    vector<Variable>variables;
    while (true) {
        getline(cin, line);
        if (line.empty() || line == "x")  // Stop input on empty line
            break;
        Variable v = extractVariableData(line);
        variables.push_back(v);
        cout << v.getName() << endl;
        cout << v.getType()<< endl;
        cout << v.getLower()<< endl;
        cout << v.getUpper()<< endl;
        cout << "-------------------------------------------------------------\n";
    }

    cout << "next step " << endl;
}



// ===> CODE FOR STORE FUZZY SET IN FUZZY LOGIC ALGO 
string inputFuzzyName(){
    string fuzzyName;
    cout << "Enter the variable's name: ";
    cin >> fuzzyName;
    return fuzzyName;
}

Fuzzy addFuzzy(string fuzzy){
    Fuzzy f;
    // Example input: beginner TRI 0 15 30

    // 1) Extract the Name
    f.setName(extractToken(fuzzy, ' '));
    // Now, variable contains: TRI 0 15 30

    // 2) Extract the Type(TRI/TRAP)
    f.setType(extractToken(fuzzy, ' '));
    // Now, variable contains: 0 15 30

    // 3) Extract the values
    f.addValue(stoi(extractToken(fuzzy, ' ')));
    f.addValue(stoi(extractToken(fuzzy, ' ')));
    if(f.getType() == "TRAP")
        f.addValue(stoi(extractToken(fuzzy, ' ')));
    f.addValue(stoi(fuzzy.substr(0)));
    cout << "Name: " << f.getName() << endl;
    cout << "Type: " << f.getType() << endl;
    cout << "Values: ";
    cout << f.getValues() << endl;
    return f;
}

void handleFuzzySetInput(){
    cout << "Enter the fuzzy set name, type (TRI/TRAP) and values: (Press x to finish)\n";
    string line;
    vector<Fuzzy>fuzzySet;
    while (true) {
        getline(cin, line);
        if (line.empty() || line == "x")  // Stop input on empty line
            break;
        Fuzzy f = addFuzzy(line);
        fuzzySet.push_back(f);
        f.getName();
        f.getType();
        f.getValues();
        cout << "-------------------------------------------------------------\n";
    }
}
// ===> CODE FOR STORE RULES FUZZY LOGIC ALGO

Rule addRule(string rule){
    Rule r;
    // Example input: proj_funding high or exp_level expert => risk low

    // 1) Extract the IN_VAR
    r.setVariable(extractToken(rule, ' '));
    r.setSet(extractToken(rule, ' '));
    
    //2) Extract the Operation
    r.setOperation(extractToken(rule, ' '));

    // 2) Extract the IN_VAR
    r.setVariable(extractToken(rule, ' '));
    r.setSet(extractToken(rule, ' '));


    extractToken(rule, ' ');//to remove => 
    r.setVariable(extractToken(rule, ' '));
    r.setSet(rule);
    return r;
}

void handleRulesInput(){
    cout << "Enter the rules in this format: (Press x to finish)\nLike: IN_variable set operator IN_variable set => OUT_variable set\n";
    string line;
    vector<Rule>rules;
    while (true) {
        getline(cin, line);
        if (line.empty() || line == "x")  // Stop input on empty line
            break;
        Rule r = addRule(line);
        rules.push_back(r);
        cout << r.getVariables() << endl;
        cout << r.getSets() << endl;
        cout << r.getOperation() << endl;
        cout << "-------------------------------------------------------------\n";
    }
}

class Algo{

};

int main(){
    // inputSystemDesc();

    // ===> Test Variables input
    // handleVariablesInput();
    // ===> Test Fuzzy Set input
    // handleFuzzySetInput();

    // ===> Test Rules input
    handleRulesInput();
}

// void mainMenu() {
//     cout << "Fuzzy Logic Toolbox\n";
//     cout << "===================\n";
//     cout << "1- Create a new fuzzy system\n";
//     cout << "2- Quit\n";
// }
// string inputSystemTitle(){
//     string title;

//     // Input the title
//     cout << "Enter the system's name: ";
//     getline(cin, title);
//     cout << "\nYou entered:\n";
//     cout << "Title: " << title << "\n";
//     return title;
// }
// string inputSystemDesc(){
//     string description;
//     // Input the multi-line description
//     cout << "Enter the system's brief description(press Enter twice to finish):\n";
//     string line;
//     while (true) {
//         getline(cin, line);
//         if (line.empty())  // Stop input on empty line
//             break;
//         description += line + "\n";
//     }

//     // Output the input for confirmation
//     cout << "\nYou entered:\n";
//     cout << "Description:\n" << description;
//     return  description;
// }

// void optionsMenu() {
//     cout << "Main Menu:\n";
//     cout << "==========\n";
//     cout << "1- Add variables.\n";
//     cout << "2- Add fuzzy sets to an existing variable.\n";
//     cout << "3- Add rules.\n";
//     cout << "4- Run the simulation on crisp values.\n";
// }
