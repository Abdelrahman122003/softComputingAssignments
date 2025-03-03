#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <map>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <random>
#include <fstream>

using namespace std;

template <typename T>
ostream &operator<<(ostream &os, const vector<T> &vec)
{
    os << "[ ";
    for (auto it = vec.begin(); it != vec.end(); ++it)
    {
        os << *it;
        if (it + 1 != vec.end())
        {
            os << ", "; // Print a comma after each element, except the last one
        }
    }
    os << " ]";
    return os; // Return the ostream object to allow chaining
}
// Objects
class Variable
{
    string name;
    string type;
    pair<int, int> range;

public:
    Variable()
    {
    }
    void setName(string _name)
    {
        name = _name;
    }
    void setType(string _type)
    {
        type = _type;
    }
    void setLower(int _lower)
    {
        range.first = _lower;
    }
    void setUpper(int _upper)
    {
        range.second = _upper;
    }
    string getName()
    {
        return name;
    }
    string getType()
    {
        return type;
    }

    // int get
    int getLower()
    {
        return range.first;
    }
    int getUpper()
    {
        return range.second;
    }
};

class Fuzzy
{
    string name;
    string type;
    vector<double> values;

public:
    Fuzzy()
    {
    }
    void setName(string _name)
    {
        name = _name;
    }
    void setType(string _type)
    {
        type = _type;
    }
    // add value
    void addValue(double _value)
    {
        values.push_back(_value);
    }

    string getName()
    {
        return name;
    }
    string getType()
    {
        return type;
    }
    vector<double> getValues()
    {
        return values;
    }
};

class FuzzySet
{
    string name;
    vector<Fuzzy> fuzzySet;

public:
    FuzzySet()
    {
    }
    void setName(string _name)
    {
        name = _name;
    }
    void setFuzzySet(vector<Fuzzy> _fuzzy)
    {
        fuzzySet = _fuzzy;
    }
    string getName()
    {
        return name;
    }
    vector<Fuzzy> getFuzzySet()
    {
        return fuzzySet;
    }
};
class Rule
{
    string operation;
    vector<string> variables;
    vector<string> sets;

public:
    void setOperation(string _operation)
    {
        operation = _operation;
    }
    void setVariable(string _variable)
    {
        variables.push_back(_variable);
    }
    void setSet(string _set)
    {
        sets.push_back(_set);
    }

    string getOperation()
    {
        return operation;
    }
    vector<string> getVariables()
    {
        return variables;
    }
    vector<string> getSets()
    {
        return sets;
    }
};

string extractToken(string &str, char delimiter)
{
    size_t pos = str.find(delimiter);
    // To ensure that pos not out of bound
    if (pos == string::npos)
    {
        throw invalid_argument("Invalid input format");
    }
    string token = str.substr(0, pos);
    str = str.substr(pos + 1); // Remove the token and delimiter from the original string
    return token;
}

class Extraction
{
public:
    Extraction()
    {
    }
    Variable extractVariableData(string variable)
    {
        Variable v;
        // Example input: proj_funding IN [0, 100]
        // 1) Extract the Name
        v.setName(extractToken(variable, ' '));
        // Now, variable contains: IN [0, 100]
        // 2) Extract the Type
        v.setType(extractToken(variable, ' '));
        // Now, variable contains: [0, 100]
        // 3) Extract the Range
        if (variable.front() != '[' || variable.back() != ']')
        {
            throw invalid_argument("Range must be enclosed in square brackets");
        }
        // Extract the lower bound
        variable = variable.substr(1, variable.size() - 2); // Remove brackets
        string lower = extractToken(variable, ',');         // Get lower bound
        v.setLower(stoi(lower));
        v.setUpper(stoi(variable)); // Remaining part is the upper bound
        return v;
    }
    Fuzzy extractFuzzyData(string fuzzy)
    {
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
        if (f.getType() == "TRAP")
            f.addValue(stoi(extractToken(fuzzy, ' ')));
        f.addValue(stoi(fuzzy.substr(0)));
        return f;
    }
    // ===> CODE FOR STORE RULES FUZZY LOGIC ALGO
    Rule extractRuleData(string rule)
    {
        Rule r;
        // Example input: proj_funding high or exp_level expert => risk low
        // 1) Extract the IN_VAR
        r.setVariable(extractToken(rule, ' '));
        r.setSet(extractToken(rule, ' '));
        // 2) Extract the Operation
        r.setOperation(extractToken(rule, ' '));
        // 2) Extract the IN_VAR
        r.setVariable(extractToken(rule, ' '));
        r.setSet(extractToken(rule, ' '));
        extractToken(rule, ' '); // to remove =>
        r.setVariable(extractToken(rule, ' '));
        r.setSet(rule);
        return r;
    }
};

class InputData
{
    Extraction e;
public:
    InputData()
    {
    }
    InputData(Extraction _e)
    {
        e = _e;
    }
    int inputOption()
    {
        int option;
        cin >> option;
        return option;
    }
    string inputSystemTitle()
    {
        string title;
        // Input the title
        cout << "Enter the system's name: ";
        getline(cin, title);
        return title;
    }
    string inputSystemDesc()
    {
        string description;
        // Input the multi-line description
        cout << "Enter the system's brief description(press Enter twice to finish):\n";
        string line;
        while (true)
        {
            getline(cin, line);
            if (line.empty()) // Stop input on empty line
                break;
            description += line + "\n";
        }
        return description;
    }
    string inputVariableName()
    {
        string variableName;
        cout << "Enter the variable's name: ";
        cin >> variableName;
        return variableName;
    }
    // ===> CODE FOR STORE FUZZY SET IN FUZZY LOGIC ALGO
    vector<Fuzzy> fuzzySetInput()
    {
        cout << "Enter the fuzzy set name, type (TRI/TRAP) and values: (Press x to finish)\n";
        string line;
        vector<Fuzzy> fuzzySet;
        while (true)
        {
            getline(cin, line);
            if (line.empty() || line == "x") // Stop input on empty line
                break;
            Fuzzy f = e.extractFuzzyData(line);
            fuzzySet.push_back(f);
        }
        return fuzzySet;
    }
    // ===> CODE FOR STORE RULES FUZZY LOGIC ALGO
    void rulesInput(vector<Rule> &rules)
    {
        cout << "Enter the rules in this format: (Press x to finish)\nLike: IN_variable set operator IN_variable set => OUT_variable set\n";
        string line;
        while (true)
        {
            getline(cin, line);
            if (line.empty() || line == "x") // Stop input on empty line
                break;
            Rule r = e.extractRuleData(line);
            rules.push_back(r);
        }
    }
    // ===> CODE FOR STORE VARIABLES IN FUZZY LOGIC ALGO
    void variablesInput(vector<Variable> &variables)
    {
        cout << "Enter the variable's name, type (IN/OUT) and range ([lower, upper]): (Press x to finish)\n";
        string line;
        while (true)
        {
            getline(cin, line);
            if (line.empty() || line == "x") // Stop input on empty line
                break;
            Variable v = e.extractVariableData(line);
            variables.push_back(v);
        }
    }
};
class Ouput
{
public:
    Ouput()
    {
    }
    void mainMenu()
    {
        cout << "Fuzzy Logic Toolbox\n";
        cout << "===================\n";
        cout << "1- Create a new fuzzy system\n";
        cout << "2- Quit\n";
    }
    void optionsMenu()
    {
        cout << "Main Menu:\n";
        cout << "==========\n";
        cout << "1- Add variables.\n";
        cout << "2- Add fuzzy sets to an existing variable.\n";
        cout << "3- Add rules.\n";
        cout << "4- Run the simulation on crisp values.\n";
    }
};
// Function for fuzzification using linear interpolation// Function for fuzzification using linear interpolation for triangle
double triangleFuzzy(double x, double x1, double y1, double x2, double y2, double x3, double y3)
{
    if (x < x1 || x > x3)
    {
        return 0.0; // Outside the fuzzy set
    }
    else if (x >= x1 && x <= x2)
    {
        return (x - x1) * (y2 - y1) / (x2 - x1) + y1; // First interpolation
    }
    else if (x >= x2 && x <= x3)
    {
        return (x - x2) * (y3 - y2) / (x3 - x2) + y2; // Second interpolation
    }
    return 0.0;
}

// Function for fuzzification using linear interpolation for trapezoid
double trapezoidalFuzzy(double x, double x1, double x2, double x3, double x4, double y1, double y2, double y3, double y4)
{
    if (x < x1 || x > x4)
    {
        return 0.0; // Outside the fuzzy set range, membership is 0
    }
    else if (x >= x1 && x <= x2)
    {
        // Increasing linearly from y1 to y2 between x1 and x2
        return (x - x1) * (y2 - y1) / (x2 - x1) + y1;
    }
    else if (x >= x2 && x <= x3)
    {
        // Constant membership y2 between x2 and x3
        return y2;
    }
    else if (x >= x3 && x <= x4)
    {
        // Decreasing linearly from y2 to y3 between x3 and x4
        return (x4 - x) * (y3 - y2) / (x4 - x3) + y2;
    }
    return 0.0;
}

map<string, double> fuzzifyValue(double value, string variableName, vector<FuzzySet> &fuzzySets)
{
    map<string, double> fuzzifiedValues;

    for (auto &fuzzySet : fuzzySets)
    {
        if (fuzzySet.getName() == variableName)
        { // Ensure this fuzzy set belongs to the current variable
            for (auto &fuzzy : fuzzySet.getFuzzySet())
            {
                vector<double> setValues = fuzzy.getValues();
                string fuzzyType = fuzzy.getType();
                string key = variableName + " " + fuzzy.getName(); // Key = "variableName fuzzyName"

                double fuzzifiedValue = 0.0;

                try
                {
                    // Validate fuzzy set values
                    if (fuzzyType == "TRI" && setValues.size() == 3)
                    {
                        // Triangular fuzzy set
                        double x1 = setValues[0], x2 = setValues[1], x3 = setValues[2];
                        fuzzifiedValue = triangleFuzzy(value, x1, 0.0, x2, 1.0, x3, 0.0);
                    }
                    else if (fuzzyType == "TRAP" && setValues.size() == 4)
                    {
                        // Trapezoidal fuzzy set
                        double x1 = setValues[0], x2 = setValues[1], x3 = setValues[2], x4 = setValues[3];
                        fuzzifiedValue = trapezoidalFuzzy(value, x1, x2, x3, x4, 0.0, 1.0, 1.0, 0.0);
                    }
                    else
                    {
                        throw invalid_argument("Invalid fuzzy type or parameter count for: " + fuzzy.getName());
                    }

                    // Add the fuzzified value
                    fuzzifiedValues[key] = fuzzifiedValue;
                }
                catch (exception &e)
                {
                    cerr << "Error fuzzifying " << key << ": " << e.what() << '\n';
                    fuzzifiedValues[key] = 0.0; // Assign a default value of 0 for invalid entries
                    continue;                   // Skip this fuzzy set and proceed to the next
                }
            }
        }
    }
    return fuzzifiedValues;
}

double complement(double membershipValue)
{
    return 1.0 - membershipValue;
}

double fuzzyAnd(double membershipValueA, double membershipValueB)
{
    return min(membershipValueA, membershipValueB);
}

double fuzzyOr(double membershipValueA, double membershipValueB)
{
    return max(membershipValueA, membershipValueB);
}
map<string, double> applyFuzzyInference(const map<string, double> &fuzzyValuesA, const map<string, double> &fuzzyValuesB, const string &operation)
{
    map<string, double> result;
    for (const auto &pairA : fuzzyValuesA)
    {
        string keyA = pairA.first; // Key for fuzzyValuesA
        double membershipValueA = pairA.second;

        // Iterate through all fuzzy sets in fuzzyValuesB
        for (const auto &pairB : fuzzyValuesB)
        {
            string keyB = pairB.first; // Key for fuzzyValuesB
            double membershipValueB = pairB.second;

            string combinedKey = keyA + ' ' + operation + ' ' + keyB; // Combine keys for result

            // Perform fuzzy operations based on the specified operation
            if (operation == "and")
            {
                result[combinedKey] = fuzzyAnd(membershipValueA, membershipValueB);
            }
            else if (operation == "or")
            {
                result[combinedKey] = fuzzyOr(membershipValueA, membershipValueB);
            }
            else if (operation == "and_not")
            {
                result[combinedKey] = fuzzyAnd(membershipValueA, complement(membershipValueB));
            }
            else if (operation == "or_not")
            {
                result[combinedKey] = fuzzyOr(membershipValueA, complement(membershipValueB));
            }
            else
            {
                throw invalid_argument("Invalid operation: " + operation);
            }
        }
    }
    return result;
}

double defuzzify(map<string, double> &aggregatedOutputs, vector<Fuzzy> &outputFuzzySets)
{
    double numerator = 0.0, denominator = 0.0;

    // Iterate through each fuzzy set in the output fuzzy sets
    for (auto &fuzzySet : outputFuzzySets)
    {
        // Get the values of the fuzzy set (x1, x2, x3)
        vector<double> setValues = fuzzySet.getValues();

        // Debugging: Print out the fuzzy set values
        // cout << "Fuzzy Set: " << fuzzySet.getName() << " Values: ";
        // for (double value : setValues)
        // {
        //     cout << value << " ";
        // }
        // cout << '\n';

        if (setValues.size() == 3)
        {
            // Triangular fuzzy set (TRI)
            double centroid = (setValues[0] + setValues[1] + setValues[2]) / 3.0;
            // cout << "Calculated Centroid (TRI): " << centroid << '\n';

            // Get the membership value for this fuzzy set from the aggregated outputs map
            double membershipValue = aggregatedOutputs.at(fuzzySet.getName()); // Use FuzzySet's name
            // cout << "Membership Value: " << membershipValue << '\n';

            // Update numerator and denominator for defuzzification
            numerator += centroid * membershipValue;
            denominator += membershipValue;
        }
        else if (setValues.size() == 4)
        {
            // Trapezoidal fuzzy set (TRAP)
            double centroid = (setValues[0] + setValues[1] + setValues[2] + setValues[4]) / 4.0;
            // cout << "Calculated Centroid (TRAP): " << centroid << '\n';
            // Get the membership value for this fuzzy set from the aggregated outputs map
            double membershipValue = aggregatedOutputs.at(fuzzySet.getName());
            // cout << "Membership Value: " << membershipValue << '\n';
            // Update numerator and denominator for defuzzification
            numerator += centroid * membershipValue;
            denominator += membershipValue;
        }
        else
        {
            cout << "Error: Unsupported fuzzy set type with " << setValues.size() << " values." << '\n';
        }
    }
    // cout << "Numerator: " << numerator << " Denominator: " << denominator << '\n';
    // Avoid division by zero
    return (denominator == 0.0) ? 0.0 : numerator / denominator;
}

class Algo
{
    vector<Variable> variables;
    vector<FuzzySet> fuzzySets;
    vector<Rule> rules;
    Ouput output;
    Extraction extraction;
    InputData input;

public:
    Algo()
    {
    }
    Algo(Extraction extractionData) : input(extractionData) {}

    void clearVectors()
    {
        variables.clear();
        fuzzySets.clear();
        rules.clear();
    }
    void run()
    {
        bool varInit = 0;
        bool fuzzyInit = 0;
        bool ruleInit = 0;
        while (true)
        {
            // Display the main menu and take the first option
            output.mainMenu();
            int mainOption = input.inputOption();
            if (mainOption == 1)
            {
                string title = input.inputSystemTitle();
                cin.clear();
                cin.ignore();
                string brief = input.inputSystemDesc();
            }
            else if (mainOption == 2)
            {
                cout << "You are exiting.\n";
                break;
            }
            // Begin the second loop for multiple user interactions
            while (true)
            {
                output.optionsMenu();
                int option = input.inputOption();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                if (option == 1)
                {
                    input.variablesInput(variables);
                    varInit = 1;
                }
                else if (option == 2)
                {
                    FuzzySet fuzzySet;
                    fuzzySet.setName(input.inputVariableName());
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    fuzzySet.setFuzzySet(input.fuzzySetInput());
                    fuzzySets.push_back(fuzzySet);
                    fuzzyInit = 1;
                }
                else if (option == 3)
                {
                    input.rulesInput(rules);
                    ruleInit = 1;
                }
                else if (option == 4)
                {
                    if (!varInit || !fuzzyInit || !ruleInit)
                    {
                        cout << "CAN'T START THE SIMULATION!\n";
                        if (!varInit)
                            cout << "Please Add the variables.\n";
                        if (!fuzzyInit)
                            cout << "Please add the fuzzy sets.\n";
                        if (!ruleInit)
                            cout << "Please add the rules.\n";
                        continue;
                    }
                    map<string, double> fuzzifiedValues;
                    cout << "Enter the crisp values: \n";
                    for (auto &var : variables)
                    {
                        if (var.getType() == "IN")
                        {
                            cout << var.getName() << ": ";
                            int crispValue;

                            while (!(cin >> crispValue))
                            {
                                cin.clear();                                         // Clear the error state
                                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                                cout << "Invalid input. Please enter a number for " << var.getName() << ": ";
                            }
                            // Fuzzify the crisp value
                            auto currentFuzzifiedValues = fuzzifyValue(crispValue, var.getName(), fuzzySets);
                            fuzzifiedValues.insert(currentFuzzifiedValues.begin(), currentFuzzifiedValues.end());
                        }
                    }
                    // cout << "\nFuzzified Values:\n";
                    // for (auto c : fuzzifiedValues)
                    // {
                    //     cout << c.first << " " << c.second << '\n';
                    // }
                    map<string, double> aggregatedOutputs;
                    for (auto &rule : rules)
                    {
                        vector<string> ruleVariables = rule.getVariables();
                        vector<string> ruleSets = rule.getSets();
                        string operation = rule.getOperation();
                        map<string, double> fuzzyValuesA, fuzzyValuesB;
                        fuzzyValuesA[ruleVariables[0] + " " + ruleSets[0]] = fuzzifiedValues[ruleVariables[0] + " " + ruleSets[0]];
                        fuzzyValuesB[ruleVariables[1] + " " + ruleSets[1]] = fuzzifiedValues[ruleVariables[1] + " " + ruleSets[1]];
                        //  Apply the fuzzy operation
                        auto ruleResults = applyFuzzyInference(fuzzyValuesA, fuzzyValuesB, operation);
                        // cout << "\nRule Results:\n";
                        // for (auto c : ruleResults)
                        // {
                        //     cout << c.first << " " << c.second << '\n';
                        // }
                        // Store the results for the output variable
                        string outputSet = ruleSets.back();
                        aggregatedOutputs[outputSet] = max(aggregatedOutputs[outputSet], ruleResults.begin()->second);
                        // cout << "\nAggregated Outputs:\n";
                        // for (auto c : aggregatedOutputs)
                        // {
                        //     cout << c.first << " " << c.second << '\n';
                        // }
                    }
                    vector<Fuzzy> outputFuzzySets;
                    for (auto &var : variables)
                    {
                        if (var.getType() == "OUT")
                        {
                            for (auto &fuzzySet : fuzzySets)
                            {
                                for (auto fuzzy : fuzzySet.getFuzzySet())
                                {
                                    if (fuzzySet.getName() == var.getName())
                                        outputFuzzySets.push_back(fuzzy);
                                }
                            }
                        }
                    }
                    double crispOutput = defuzzify(aggregatedOutputs, outputFuzzySets);
                    double max = -1.0;
                    string maxStr;
                    for (auto c : aggregatedOutputs)
                    {
                        if (max <= c.second)
                        {
                            max = c.second;
                            maxStr = c.first;
                        }
                    }
                    string outVar;
                    for (auto c : variables)
                    {
                        if (c.getType() == "OUT")
                            outVar = c.getName();
                    }
                    cout << "Running the simulation...\n";
                    cout << "Fuzzification => done\n";
                    cout << "Inference => done\n";
                    cout << "Defuzzification => done\n";
                    cout << "\nThe predicted " << outVar << " is " << maxStr << " (" << crispOutput << ')' << "\n \n";
                    // Clear vectors and run any final processes
                    clearVectors();
                    // Reset Boolean Variables
                    varInit = 0;
                    fuzzyInit = 0;
                    ruleInit = 0;
                    break;
                }
                else
                {
                    cout << "Invalid option. Please try again.\n";
                }
            }
        }
    }
};
int main()
{
    Extraction extraction;
    Algo algo(extraction);
    algo.run();
}
