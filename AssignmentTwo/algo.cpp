#include <iostream>
#include <random>
#include <utility>
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

// Function to read data from file and return it as a vector of integers
vector<float> readFromFile(string file) {
    ifstream inputFile(file); // Open file for reading
    vector<float> numbers;      // Vector to store the numbers
    float number;               // Variable to store each number

    // Check if the file opened successfully
    if (!inputFile) {
        numbers.push_back(-100); // Push error code if file can't be opened
        return numbers;          // Return error code
    }

    // Read numbers from the file and store them in the vector
    while (inputFile >> number) {
        numbers.push_back(number); // Store each number in the vector
    }

    // Close the file after reading
    inputFile.close();
    return numbers; // Return the vector of numbers
}

class TestCase{
    int num_chemical;
    float total_proportion;
    vector<pair<float, float>>ranges;
    vector<float>costs;
    public:
    TestCase(){}
    vector<float> getChemicalCosts(){
        return this->costs;
    }

    int getNumChemicals(){
        return this->num_chemical;
    }

    float getTotalProportion(){
        return this->total_proportion;
    }
    vector<pair<float, float>> getChemicalRanges(){
        return this->ranges;
    }
    

    void setChemicalCost(float cost){
        this->costs.push_back(cost);
    }
    void setChemicalRange(pair<float, float>range){
        this->ranges.push_back({range});
    }
    void setNumChemicals(int numChemicals){
        this->num_chemical = numChemicals;
    }
    void setTotalProportion(float totalPro){
        this->total_proportion = totalPro;
    }

    void displayTestCase(){
        cout << "Number of Chemicals is : " << this->getNumChemicals() << endl; 
        cout << "Total Proportion : " << this->getTotalProportion() << endl;
        cout << "Costs: ";
        for (float cost : this->getChemicalCosts()) {
            cout << cost << " ";
        }
        cout << "\n";
        for(auto k : this->getChemicalRanges()){
            cout << k.first << " " << k.second << endl;
        }
        // cout << endl;
    }
};

vector<TestCase> handleTestCasesData(vector<float> numbers) {
// Function to handle the test cases data from the numbers vector

    int tc = numbers[0]; // The first number represents the number of test cases
    int i = 1, j = 0; // Start processing from the second element (i=1) and first test case (j=0)
    vector<TestCase>testCases(tc);
    // Process the numbers vector to fill testCases
    while (i < numbers.size()) {
        TestCase testCase;
        int size = numbers[i++];
        testCase.setTotalProportion(numbers[i++]);
        testCase.setNumChemicals(size);
        int proportionSize = size;
        // Read execution times for each task and store them in excTimePerTask
        while (proportionSize--) {
            testCase.setChemicalRange({numbers[i++], numbers[i++]});
        }
        while(size--){
            testCase.setChemicalCost(numbers[i++]);
        }
        testCases[j]= (testCase);
        // Move to the next test case
        j++;
    }
    return testCases; 
}
class Algo{
    int popSize;
    TestCase testCase;
    vector<float>chemicalMix;
    vector<pair<vector<float>, float>>population;
    // store index of selected individual instead of store all data 
    vector<int>selectedAddress;
    
    public:
    Algo(){}
    Algo(TestCase test, int populationSize) {
        // Pass testCase object
        testCase = test;

        // Pass population size
        popSize = populationSize;

        // Initialize chemicalMix with the number of chemicals from the testCase
        chemicalMix = vector<float>(testCase.getNumChemicals());

        // Initialize population with default values
        population = vector<pair<vector<float>, float>>(
            popSize, 
            make_pair(vector<float>(testCase.getNumChemicals()), 0.0f)
        );
        selectedAddress = vector<int>(popSize / 2);
    }
    // function for randomization 
    float getproportion(pair<float, float>range){
        random_device rd; // Seed for random number generator
        mt19937 gen(rd()); // Mersenne Twister RNG
        uniform_real_distribution<float> dist(range.first, range.second);
        return dist(gen); // Generate a random float in the range
    }
    // function to initialize chemical 
    void makeChemicalMix(){
        for(int i = 0; i < testCase.getNumChemicals(); i++)
            chemicalMix[i] = getproportion(testCase.getChemicalRanges()[i]);
    }
    // function to make initial population
    void initalPopulation(){
        for(int i = 0; i < popSize; i++)
        {            
            makeChemicalMix();
            population[i].first = chemicalMix;
        }
    }
    // function to calculate fitness
    float calcFitness(){
        float fitness = 0.0f;
        // 0.2565 = *1000 = 256.5 => round(256.5) => 257 => /10 => 25.7
        for(int i  = 0;i < testCase.getNumChemicals();i++){
            // float calculatedValue = round(chemicalMix[i] * 10)/10; 
            // cout << "Original Value : " << chemicalMix[i] << "  Calculated Value : " << calculatedValue << endl;
            fitness += (round(chemicalMix[i] * 10)/10) * testCase.getChemicalCosts()[i];
        }
        return fitness;
    }
    void calcPopulationFitness(){
        for(int i = 0; i < popSize; i++)
        {
            chemicalMix = population[i].first;
            cout << chemicalMix;
            population[i].second = calcFitness();
            cout << population[i].second << endl;
            cout << "-------------------------------------------------------------\n";
        }
    }
    void tournmentSelection(){
        // i will compare two individual two two two two 
        // first two 
        // second two 
        // and so on ...
        // if size odd => so i make number of iteration before start the loop
        int iterations = popSize / 2;
        // in case 5 => will be make 2 iterations 
        // in case 6 => make 3 iterations
        int j = 0;
        for (int i = 0;i < iterations; i++)
            selectedAddress[i] = (population[j].second > population[j+1].second)?j+1: j, j += 2;
        cout << selectedAddress << endl;
    }
    void printInitalPopulation(){
        for(int i = 0 ; i < popSize; i++){
            for(int j = 0; j < testCase.getNumChemicals(); j++){
                cout << population[i].first[j] << " ";
            }
            cout << "\n--------------------------------------------------\n";
        }
    }

    // chromosome, int rangeIndx
    void chromosomeMutation(vector<float>& chromosome, int generationNum){
        int i = 0;
        for(auto& gene : chromosome){
            double pm = this->getproportion({0.0, 1.0});
            if (pm <= 0.05){
                gene = nonUniformGeneMutation(gene, generationNum, testCase.getChemicalRanges()[i]);
            }
            ++i;
        }
    }

    double nonUniformGeneMutation(double gene, int generationNum, pair<float, float> geneRange, const int depFactor = 2){
        const int MAX_GENERATION_NUMBER = 100;    
        double R1 = this->getproportion({0.0, 1.0});
        double y = (R1 <= 0.5)? (gene - geneRange.first) : (geneRange.second - gene);
        double R2 = this->getproportion({0.0, 1.0});
        y *= (1 - pow(R2, pow((1 - generationNum / MAX_GENERATION_NUMBER), depFactor)));
        return (R1 <= 0.5)? (gene - y) : (gene + y);
        // Design Two : 
        // double ch = (1 - pow(R2, pow((1 - generationNum / MAX_GENERATION_NUMBER), depFactor)));
        // if (R1 <= 0.5) {
        //     // Y = X - LB
        //     double y = (gene - geneRange.first);
        //     y *= ch;
        //     // X = X - Y => X become smaller
        //     gene -= y;
        // } else {
        //     double y = (geneRange.second - gene);
        //     y *= ch;
        //     // X = X + Y => X becomes bigger
        //     gene += y;
        // }
        // return gene;
    }
};

int main(){
    vector<float> numbers = readFromFile("./docs/tests.txt");

    // Check if there was an error reading the file
    if (numbers[0] == -100) {
        cout << "Error during reading the file" << endl;
        return 1; // Exit early due to file reading error
    }
    vector<TestCase> testCases = handleTestCasesData(numbers);
    // for (int i = 0; i < testCases.size(); i++)
    // {
    //     testCases[i].displayTestCase();
    //     cout << "__________________________________________________\n";
    // }
    Algo algo(testCases[0], 6);
    algo.initalPopulation();
    algo.calcPopulationFitness();
    algo.tournmentSelection();
}