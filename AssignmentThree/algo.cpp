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
    vector<double>values;
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
    void addValue(double _value){
        values.push_back(_value);
    }

    string getName(){
        return name;
    }
    string getType(){
        return type;
    }
    vector<double> getValues(){
        return values;
    }
    
};

class FuzzySet{
    string name;
    vector<Fuzzy> fuzzySet;
public:
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
class Rule1 {
    vector<string> inVariables, inSets, outVariables, outSets;
    string operation;

public:
    Rule1(const vector<string>& _inVars, const vector<string>& _inSets, const string& _op,
         const vector<string>& _outVars, const vector<string>& _outSets)
        : inVariables(_inVars), inSets(_inSets), operation(_op), outVariables(_outVars), outSets(_outSets) {}

    string getOperation() const { return operation; }
    const vector<string>& getInVariables() const { return inVariables; }
    const vector<string>& getInSets() const { return inSets; }
    const vector<string>& getOutVariables() const { return outVariables; }
    const vector<string>& getOutSets() const { return outSets; }
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
vector<Variable>variables;
void handleVariablesInput(){
    cout << "Enter the variable's name, type (IN/OUT) and range ([lower, upper]): (Press x to finish)\n";
    string line;
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

vector<Fuzzy>fuzzySet;
void handleFuzzySetInput(){
    string fuzzyName = inputFuzzyName();
    cout << "Enter the fuzzy set name, type (TRI/TRAP) and values: (Press x to finish)\n";
    cin.ignore();
    string line;
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

vector<Rule>rules;
void handleRulesInput(){
    cout << "Enter the rules in this format: (Press x to finish)\nIN_variable set operator IN_variable set => OUT_variable set\n";
    string line;
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
// Function for fuzzification using linear interpolation// Function for fuzzification using linear interpolation for triangle
double triangleFuzzy(double x, double x1, double y1, double x2, double y2, double x3, double y3) {
    if (x < x1 || x > x3) {
        return 0.0; // Outside the fuzzy set
    } else if (x >= x1 && x <= x2) {
        // First interpolation (increasing from y1 to y2 between x1 and x2)
        return (x - x1) * (y2 - y1) / (x2 - x1) + y1;
    } else if (x >= x2 && x <= x3) {
        // Second interpolation (decreasing from y2 to y3 between x2 and x3)
        return (x3 - x) * (y3 - y2) / (x3 - x2) + y2; // Corrected direction for decreasing slope
    }
    return 0.0; // Default case
}

// Function for fuzzification using linear interpolation for trapezoid
double trapezoidalFuzzy(double x, double x1, double x2, double x3, double x4, double y1, double y2, double y3, double y4) {
    if (x < x1 || x > x4) {
        return 0.0;  // Outside the fuzzy set range, membership is 0
    }
    else if (x >= x1 && x <= x2) {
        // Increasing linearly from y1 to y2 between x1 and x2
        return (x - x1) * (y2 - y1) / (x2 - x1) + y1;
    }
    else if (x >= x2 && x <= x3) {
        // Constant membership y2 between x2 and x3
        return y2;
    }
    else if (x >= x3 && x <= x4) {
        // Decreasing linearly from y2 to y3 between x3 and x4
        return (x4 - x) * (y3 - y2) / (x4 - x3) + y2;
    }
    return 0.0; // Default case
}



// Fuzzify a value for a specific variable using fuzzy sets
map<string, double> fuzzifyValue(int value, string variableName, vector<Fuzzy> fuzzySets) {
     map<string, double> fuzzifiedValues;

    // Iterate through each fuzzy set
    for (auto& fuzzy : fuzzySets) {
        vector<double> setValues = fuzzy.getValues();
        string fuzzyType = fuzzy.getType();
        string key = variableName + " " + fuzzy.getName();  // Key = (variableName fuzzyName)

        double fuzzifiedValue = 0.0;

        if (fuzzyType == "TRI" && setValues.size() == 3) {  // Triangular fuzzy set
            double x1 = setValues[0], x2 = setValues[1], x3 = setValues[2];
            double y1 = 0.0, y2 = 1.0, y3 = 0.0;  // Membership values for triangular set (low: 0, medium: 1, high: 0)

            // Fuzzify the value for the TRI fuzzy set
            fuzzifiedValue = triangleFuzzy(value, x1, y1, x2, y2, x3, y3); // Calculate fuzzified value (low to high)
        } else if (fuzzyType == "TRAP" && setValues.size() == 4) {  // Trapezoidal fuzzy set
            double x1 = setValues[0], x2 = setValues[1], x3 = setValues[2], x4 = setValues[3];
            double y1 = 0.0, y2 = 1.0, y3 = 1.0, y4 = 0.0;  // Membership values for trapezoidal set

            // Fuzzify the value for the TRAP fuzzy set
            fuzzifiedValue = trapezoidalFuzzy(value, x1, y1, x2, y2, x3, y3, x4, y4); // Calculate fuzzified value (flat middle)
        }

        // Add the fuzzified value for this fuzzy set
        fuzzifiedValues[key] = fuzzifiedValue;
    }
    for(auto c:fuzzifiedValues){
        cout << c.first << " " << c.second << endl;
    }
    // Return the fuzzified values
    return fuzzifiedValues;
}
double complement(double membershipValue) {
    return 1.0 - membershipValue;
}

double fuzzyAnd(double membershipValueA, double membershipValueB) {
    return min(membershipValueA, membershipValueB);
}

double fuzzyOr(double membershipValueA, double membershipValueB) {
    return max(membershipValueA, membershipValueB);
}
map<string, double> applyFuzzyInference(const map<string, double>& fuzzyValuesA,const map<string, double>& fuzzyValuesB,const string& operation) {
    map<string, double> result;
    for (const auto& pairA : fuzzyValuesA) {
        string keyA = pairA.first;  // Key for fuzzyValuesA
        double membershipValueA = pairA.second;

        // Iterate through all fuzzy sets in fuzzyValuesB
        for (const auto& pairB : fuzzyValuesB) {
            string keyB = pairB.first;  // Key for fuzzyValuesB
            double membershipValueB = pairB.second;

            string combinedKey = keyA + " and " + keyB;  // Combine keys for result

            // Perform fuzzy operations based on the specified operation
            if (operation == "and") {
                result[combinedKey] = fuzzyAnd(membershipValueA, membershipValueB);
            } else if (operation == "or") {
                result[combinedKey] = fuzzyOr(membershipValueA, membershipValueB);
            } else if (operation == "and_not") {
                result[combinedKey] = fuzzyAnd(membershipValueA,complement(membershipValueB));
            } else if(operation == "or_not"){
                result[combinedKey] = fuzzyOr(membershipValueA,complement(membershipValueB));
            } else {
                throw invalid_argument("Invalid operation: " + operation);
            }
        }
    }
    return result;
}
double defuzzify(map<string, double>& aggregatedOutputs, vector<Fuzzy>& outputFuzzySets) {
    double numerator = 0.0, denominator = 0.0;

    for (auto& fuzzy : outputFuzzySets) {
        vector<double> setValues = fuzzy.getValues();
        double centroid = (setValues[0] + setValues[1] + setValues[2]) / 3.0; // Centroid of TRI set
        double membershipValue = aggregatedOutputs.at(fuzzy.getName());

        numerator += centroid * membershipValue;
        denominator += membershipValue;
    }

    return (denominator == 0.0) ? 0.0 : numerator / denominator; // Avoid division by zero
}
// Run the fuzzy logic system
void runFuzzySystem() {
    // Step 1: Input crisp values for each input variable
    map<string, double> fuzzifiedValues;
    for ( auto& var : variables) {
        if (var.getType() == "IN") {
            cout << "Enter crisp value for " << var.getName() << " (range: [" << var.getLower() << ", " << var.getUpper() << "]): ";
            int crispValue;
            cin >> crispValue;

            // Get fuzzy sets associated with the variable
            vector<Fuzzy> associatedFuzzySets;
            for ( auto& fuzzy : fuzzySet) {
                if (fuzzy.getName().find(var.getName()) != string::npos) {
                    associatedFuzzySets.push_back(fuzzy);
                }
            }

            // Fuzzify the input value
            auto variableFuzzifiedValues = fuzzifyValue(crispValue, var.getName(), associatedFuzzySets);
            fuzzifiedValues.insert(variableFuzzifiedValues.begin(), variableFuzzifiedValues.end());
        }
    }

    // Step 2: Apply inference rules
    map<string, double> aggregatedOutputs;
    for (auto& rule : rules) {
    vector<string> ruleVariables = rule.getVariables();
    vector<string> ruleSets = rule.getSets();
    string operation = rule.getOperation();

    // Ensure we have at least two inputs
    if (ruleVariables.size() < 2 || ruleSets.size() < 2) {
        cerr << "Error: Rule is not properly defined.\n";
        continue;
    }

    // Extract the fuzzified values for the rule's variables
    map<string, double> fuzzyValuesA, fuzzyValuesB;

    if (fuzzifiedValues.find(ruleVariables[0] + " " + ruleSets[0]) != fuzzifiedValues.end()) {
        fuzzyValuesA[ruleVariables[0] + " " + ruleSets[0]] = fuzzifiedValues[ruleVariables[0] + " " + ruleSets[0]];
    }

    if (fuzzifiedValues.find(ruleVariables[1] + " " + ruleSets[1]) != fuzzifiedValues.end()) {
        fuzzyValuesB[ruleVariables[1] + " " + ruleSets[1]] = fuzzifiedValues[ruleVariables[1] + " " + ruleSets[1]];
    }

    if (fuzzyValuesA.empty() || fuzzyValuesB.empty()) {
        cerr << "Error: Could not find fuzzified values for rule variables.\n";
        continue;
    }

    // Apply the fuzzy operation
    auto ruleResults = applyFuzzyInference(fuzzyValuesA, fuzzyValuesB, operation);

    // Store the results for the output variable
    string outputSet = ruleSets.back();
    aggregatedOutputs[outputSet] = max(aggregatedOutputs[outputSet], ruleResults.begin()->second);
    }


    // Step 3: Defuzzify the output
    vector<Fuzzy> outputFuzzySets;
    for ( auto& fuzzy : fuzzySet) {
        if (fuzzy.getName().find("OUT") != string::npos) {
            outputFuzzySets.push_back(fuzzy);
        }
    }

    double crispOutput = defuzzify(aggregatedOutputs, outputFuzzySets);
    cout << "Final Crisp Output: " << crispOutput << endl;
}
int main(){
    // ===> Test Variables input
    handleVariablesInput();
    // ===> Test Fuzzy Set input
    handleFuzzySetInput();
    // ===> Test Rules input
    handleRulesInput();
    runFuzzySystem();
    return 0;
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
