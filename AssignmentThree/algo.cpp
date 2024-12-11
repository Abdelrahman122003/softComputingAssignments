#include <iostream>
#include <vector>
#include <string>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()
#include <map>
#include <algorithm>
#include <numeric> // For std::accumulate
#include <cmath>   // for std::abs
#include <random>  // For better random number generation
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

        cout << "Name: " << v.getName() << endl;
        cout << "Type: " << v.getType() << endl;
        cout << "Lower bound: " << v.getLower() << endl;
        cout << "Upper bound: " << v.getUpper() << endl;
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
        cout << "Name: " << f.getName() << endl;
        cout << "Type: " << f.getType() << endl;
        cout << "Values: ";
        cout << f.getValues() << endl;
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
        // cout << "\nYou entered:\n";
        // cout << "Title: " << title << "\n";
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

        // Output the input for confirmation
        // cout << "\nYou entered:\n";
        // cout << "Description:\n" << description;
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
            f.getName();
            f.getType();
            f.getValues();
            cout << "-------------------------------------------------------------\n";
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
            cout << r.getVariables() << endl;
            cout << r.getSets() << endl;
            cout << r.getOperation() << endl;
            cout << "-------------------------------------------------------------\n";
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
            cout << v.getName() << endl;
            cout << v.getType() << endl;
            cout << v.getLower() << endl;
            cout << v.getUpper() << endl;
            cout << "-------------------------------------------------------------\n";
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
    return 0.0; // Default case
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
                    cerr << "Error fuzzifying " << key << ": " << e.what() << endl;
                    fuzzifiedValues[key] = 0.0; // Assign a default value of 0 for invalid entries
                    continue;                   // Skip this fuzzy set and proceed to the next
                }

                // Debugging: Output each calculated membership value (including zeroes)
                // cout << "Fuzzified: " << key << " -> " << fuzzifiedValue << endl;
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

            string combinedKey = keyA + " and " + keyB; // Combine keys for result

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
// Assuming FuzzySet has the methods getValues() and getName()
double defuzzify(map<string, double> &aggregatedOutputs, vector<Fuzzy> &outputFuzzySets)
{
    double numerator = 0.0, denominator = 0.0;

    // Iterate through each fuzzy set in the output fuzzy sets
    for (auto &fuzzySet : outputFuzzySets)
    {
        // Get the values of the fuzzy set (e.g., x1, x2, x3 for a TRI set)
        vector<double> setValues = fuzzySet.getValues();

        // Debugging: Print out the fuzzy set values
        cout << "Fuzzy Set: " << fuzzySet.getName() << " Values: ";
        for (double value : setValues)
        {
            cout << value << " ";
        }
        cout << endl;

        // Check if the set has the correct number of values (assuming TRI here, so it should have 3 values)
        if (setValues.size() != 3)
        {
            cout << "Error: Expected 3 values for TRI fuzzy set, found " << setValues.size() << endl;
            continue; // Skip this fuzzy set if it doesn't have 3 values
        }

        // Calculate the centroid of the triangular fuzzy set
        double centroid = (setValues[0] + setValues[1] + setValues[2]) / 3.0; // Assuming TRI set
        cout << "Calculated Centroid: " << centroid << endl;

        // Get the membership value for this fuzzy set from the aggregated outputs map
        double membershipValue = aggregatedOutputs.at(fuzzySet.getName()); // Use FuzzySet's name
        cout << "Membership Value: " << membershipValue << endl;

        // Update numerator and denominator for defuzzification
        numerator += centroid * membershipValue;
        denominator += membershipValue;
    }

    // Debugging: Print out the final numerator and denominator
    cout << "Numerator: " << numerator << " Denominator: " << denominator << endl;

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

                    for (auto var : variables)
                    {
                        cout << var.getName() << endl;
                        cout << var.getType() << endl;
                        cout << var.getLower() << endl;
                        cout << var.getUpper() << endl;
                        cout << "-----------------------------------------\n";
                    }
                    varInit = 1;
                }
                else if (option == 2)
                {
                    FuzzySet fuzzySet;
                    fuzzySet.setName(input.inputVariableName());
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    fuzzySet.setFuzzySet(input.fuzzySetInput());
                    fuzzySets.push_back(fuzzySet);
                    for (auto fuzzySet : fuzzySets)
                    {
                        cout << fuzzySet.getName() << endl;
                        for (auto fuzzy : fuzzySet.getFuzzySet())
                        {
                            cout << fuzzy.getName() << endl;
                            cout << fuzzy.getType() << endl;
                            cout << fuzzy.getValues() << endl;
                        }
                        cout << "------------------------------------------\n";
                    }
                    fuzzyInit = 1;
                }
                else if (option == 3)
                {
                    input.rulesInput(rules);

                    for (auto rule : rules)
                    {
                        cout << rule.getVariables() << endl;
                        cout << rule.getSets() << endl;
                        cout << rule.getOperation() << endl;
                        cout << "---------------------------------------------------------\n";
                    }
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
                    // Process the variables and perform fuzzy logic operations
                    map<string, double> fuzzifiedValues;

                    cout << "Enter the crisp values: \n";
                    for (auto &var : variables)
                    {
                        if (var.getType() == "IN")
                        {
                            cout << var.getName() << ": ";
                            int crispValue;

                            // Validate input to ensure it's an integer
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
                    cout << "\nFuzzified Values:\n";
                    for (auto c : fuzzifiedValues)
                    {
                        cout << c.first << " " << c.second << '\n';
                    }

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
                        cout << "\nRule Results:\n";
                        for (auto c : ruleResults)
                        {
                            cout << c.first << " " << c.second << '\n';
                        }
                        // Store the results for the output variable
                        string outputSet = ruleSets.back();
                        aggregatedOutputs[outputSet] = max(aggregatedOutputs[outputSet], ruleResults.begin()->second);
                        cout << "\nAggregated Outputs:\n";
                        for (auto c : aggregatedOutputs)
                        {
                            cout << c.first << " " << c.second << '\n';
                        }
                    }
                    vector<Fuzzy> outputFuzzySets;
                    for (auto &var : variables)
                    {
                        cout << "var.getType(): " << var.getType() << '\n';
                        if (var.getType() == "OUT")
                        {
                            for (auto &fuzzySet : fuzzySets)
                            {
                                for (auto fuzzy : fuzzySet.getFuzzySet())
                                {
                                    cout << "fuzzySet.getName(): " << fuzzySet.getName() << '\n';
                                    if (fuzzySet.getName() == var.getName())
                                        outputFuzzySets.push_back(fuzzy);
                                }
                            }
                        }
                    }

                    cout << "\nOutput Fuzzy\n";
                    for (auto c : outputFuzzySets)
                    {
                        cout << c.getName() << " " << c.getType() << " " << c.getValues() << '\n';
                    }
                    double crispOutput = defuzzify(aggregatedOutputs, outputFuzzySets);
                    cout << "Final Crisp Output: " << crispOutput << endl;
                    // Clear vectors and run any final processes
                    clearVectors();
                    varInit = 0;
                    fuzzyInit = 0;
                    ruleInit = 0;
                    break;
                }
                else if (option == 5)
                {
                    // Return to the main menu
                    break;
                }
                else
                {
                    std::cout << "Invalid option. Please try again.\n";
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
