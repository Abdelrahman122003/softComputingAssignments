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
#include <algorithm>

using namespace std;

#define NUM_GENERATIONS 50
#define POPULATION_SIZE 50

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
vector<double> readFromFile(string file) {
    ifstream inputFile(file); // Open file for reading
    vector<double> numbers;      // Vector to store the numbers
    double number;               // Variable to store each number

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
    double total_proportion;
    vector<pair<double, double>>ranges;
    vector<double>costs;
    public:
    TestCase(){}
    vector<double> getChemicalCosts(){
        return this->costs;
    }

    int getNumChemicals(){
        return this->num_chemical;
    }

    double getTotalProportion(){
        return this->total_proportion;
    }
    vector<pair<double, double>> getChemicalRanges(){
        return this->ranges;
    }
    

    void setChemicalCost(double cost){
        this->costs.push_back(cost);
    }
    void setChemicalRange(pair<double, double>range){
        this->ranges.push_back({range});
    }
    void setNumChemicals(int numChemicals){
        this->num_chemical = numChemicals;
    }
    void setTotalProportion(double totalPro){
        this->total_proportion = totalPro;
    }

    void displayTestCase(){
        cout << "Number of Chemicals is : " << this->getNumChemicals() << endl; 
        cout << "Total Proportion : " << this->getTotalProportion() << endl;
        cout << "Costs: ";
        for (double cost : this->getChemicalCosts()) {
            cout << cost << " ";
        }
        cout << "\n";
        for(auto k : this->getChemicalRanges()){
            cout << k.first << " " << k.second << endl;
        }
        // cout << endl;
    }
};

vector<TestCase> handleTestCasesData(vector<double> numbers) {
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
    vector<double>chemicalMix;
    vector<pair<vector<double>, double>>population,best;
    vector<pair<vector<double>, double>>offSprings;
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
        chemicalMix = vector<double>(testCase.getNumChemicals());

        // Initialize population with default values
        population = vector<pair<vector<double>, double>>(
            popSize, 
            make_pair(vector<double>(testCase.getNumChemicals()), 0.0)
        );
        selectedAddress = vector<int>(popSize / 2);
    }
    // function for randomization 
    double getproportion(pair<double, double>range){
        random_device rd; // Seed for random number generator
        mt19937 gen(rd()); // Mersenne Twister RNG
        uniform_real_distribution<double> dist(range.first, range.second);
        // Generate a random double in the range
        return round((dist(gen)) * 100.0) / 100.0; // Round to two decimal places
    }
    // function to calcualte best of population
    void getBest(){
        int bestSize = population.size() * 1/4;
        best = vector<pair<vector<double>,  double>>(bestSize);

        sort(population.begin(), population.end(),  [](const auto& a, const auto& b) {
            return a.second < b.second;
        });

        for(int i = 0; i < bestSize; i++){
            best[i] = population[i];
        }

        cout << "best ===> \n";
        for(auto [k, v]: best){
            cout << k << " " << v <<endl;
        }
    }
    void getOffSprings(){
        int offSpringsSize = population.size() * 3/4, start = population.size() * 1/4;
        offSprings = vector<pair<vector<double>,  double>>(offSpringsSize,
        make_pair(vector<double>(testCase.getNumChemicals()), 0.0));
        // cout << "start : " << start << "    size : " << population.size() << endl;
        // cout << "population[2] :  "  << population[2].first << endl;
        int j = 0;
        for (int i = start;  i < population.size();i++)
            offSprings[j++] = population[i];
        
        cout << "offSprings ===> \n";
        for(auto [k, v]: offSprings){
            cout << k << " " <<  v << endl;
        }
    }
    // function to initialize chemical 
    void makeChemicalMix(){ 
// 3 100.0   
// 5.0 25.0 10.0 40.0 15.0 35.0   
// 8.5 6.2 7.8 
        for(int i = 0; i < testCase.getNumChemicals(); i++)
            chemicalMix[i] = getproportion(testCase.getChemicalRanges()[i]);

        double total = accumulate(chemicalMix.begin(), chemicalMix.end(), 0.0);
        if (testCase.getTotalProportion() > total) {
            double remaining = testCase.getTotalProportion() - total;

            while (remaining > 0) {
                double distributed = 0.0; // Track how much is distributed in this iteration

                for (int i = 0; i < testCase.getNumChemicals(); i++) {
                    auto range = testCase.getChemicalRanges()[i];
                    double maxAddition = range.second - chemicalMix[i]; // How much can this element increase without exceeding the max range

                    if (maxAddition > 0) {
                        double addition = min(maxAddition, remaining); // Add the smaller of the remaining amount or max addition
                        chemicalMix[i] += addition;
                        chemicalMix[i] = (round(chemicalMix[i]) * 100.0) / 100.0;
                        distributed += addition;
                        remaining -= addition;

                        if (remaining <= 0) break; // Stop if nothing remains to distribute
                    }
                }
                // If no distribution was possible in this iteration, break to avoid infinite loops
                if (distributed == 0) break;
            }
        } else if (testCase.getTotalProportion() < total) {
            double remaining = total - testCase.getTotalProportion();
            while (remaining > 0) {
                double distributed = 0.0; // Track how much is distributed in this iteration
                for (int i = 0; i < testCase.getNumChemicals(); i++) {
                    auto range = testCase.getChemicalRanges()[i];
                    // L => 5, i => 50, max substract 40 
                    double maxSubstract = chemicalMix[i] - range.first; // How much can this element increase without exceeding the max range

                    if (maxSubstract > 0) {
                        double substract = min(maxSubstract, remaining); // Add the smaller of the remaining amount or max addition
                        chemicalMix[i] -= substract;
                        chemicalMix[i] = (round(chemicalMix[i]) * 100.0) / 100.0;
                        distributed += substract;
                        remaining -= substract;
                        if (remaining <= 0) break; // Stop if nothing remains to distribute
                    }
                }
                // If no distribution was possible in this iteration, break to avoid infinite loops
                if (distributed == 0) break;
            }
        }
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
    double calcFitness(){
        double fitness = 0.0;
        // 0.2565 = *1000 = 256.5 => round(256.5) => 257 => /10 => 25.7
        for(int i  = 0;i < testCase.getNumChemicals();i++){
            // double calculatedValue = round(chemicalMix[i] * 10)/10; 
            // cout << "Original Value : " << chemicalMix[i] << "  Calculated Value : " << calculatedValue << endl;
            fitness += (round(chemicalMix[i] * 10.0)/10.0) * testCase.getChemicalCosts()[i];
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
        int iterations = offSprings.size() / 2;
        // in case 5 => will be make 2 iterations 
        // in case 6 => make 3 iterations
        int j = 0;
        for (int i = 0;i < iterations; i++){
            int notSelected = (offSprings[j].second > offSprings[j+1].second)? j : j + 1;
            j += 2;
            offSprings[notSelected].second = -1.0;
        }
        // Discard not selected chromosomes
        offSprings.erase(remove_if(offSprings.begin(), offSprings.end(), [](const pair<vector<double>, double>& p) {return roundf(p.second) == -1.0f;}),
        offSprings.end());

        cout << "selected chromosmes ===> \n";
        for(auto [k, v]: offSprings){
            cout << k << " " << v << endl;
        }
        // BEST + REST => SELECT => CROSSOVER => MUTATION => OFFSPRINGS
    }
    void printInitalPopulation(){
        for(int i = 0 ; i < popSize; i++){
            for(int j = 0; j < testCase.getNumChemicals(); j++){
                cout << population[i].first[j];
            }
            cout << "\n--------------------------------------------------\n";
        }
    }
    vector<pair<vector<double>, double>> crossover() {
        vector<pair<vector<double>, double>> offsprings;
        vector<pair<vector<double>, double>> childs;
        // Even 
        if (this->offSprings.size() % 2 == 0){
            for (auto it = this->offSprings.begin(); it != this->offSprings.end(); ++it){
                auto next_it = next(it);
                childs = _crossover(it->first, next_it->first);
                offsprings.insert(offsprings.end(), childs.begin(), childs.end());
                ++it;
            }
        } 
        // Odd
        else {            
            for (auto it = this->offSprings.begin(); it != prev(this->offSprings.end()); ++it){
                auto next_it = next(it);
                childs = _crossover(it->first, next_it->first);
                offsprings.insert(offsprings.end(), childs.begin(), childs.end());
            }

            vector<pair<vector<double>, double>> bestOffsprings;
            int i = 0;
            for (auto it = offsprings.begin(); it != offsprings.end() && i < this->offSprings.size(); ++it) { 
                bestOffsprings.emplace_back(it->first, it->second); // Add to the result
                ++i;
            }
            return bestOffsprings;
        }
        return offsprings;
    }

    vector<pair<vector<double>, double>> _crossover(vector<double> &parent1, vector<double> &parent2)
    {
        srand(time(0));
        int crossover_point1 = rand() % parent1.size();
        int crossover_point2 = rand() % parent1.size();
        if(crossover_point2 < crossover_point1) swap(crossover_point1, crossover_point2);
        vector<double> child1 = parent1;
        vector<double> child2 = parent2;

        for (int i = crossover_point1; i < crossover_point2; ++i)
        {
            child1[i] = parent2[i];
            child2[i] = parent1[i];
        }

        vector<pair<vector<double>, double>> offsprings;
        vector<vector<double>> childs {child1, child2};

        for (vector<double> chrom : childs){
            chemicalMix = chrom;
            offsprings.emplace_back(chrom, calcFitness());
        }

        return offsprings;
    }

   // chromosome, int rangeIndx
    void chromosomeMutation(pair<vector<double>, double>& chromosome, int generationNum){
        int i = 0;
        bool isChanged = false;
        for(auto& gene : chromosome.first){
            double pm = this->getproportion({0.0, 1.0});
            if (pm <= 0.05){
                cout << "MUTATIONNNN\n";
                isChanged = true;
                gene = nonUniformGeneMutation(gene, generationNum, testCase.getChemicalRanges()[i]);
            }
            ++i;
        }
        if (isChanged){
            chemicalMix = chromosome.first; chromosome.second = calcFitness();
        }
    }

    double nonUniformGeneMutation(double gene, int generationNum, pair<double, double> geneRange, const int depFactor = 2){
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

    void runAlgo(){
        this->initalPopulation();
        this->calcPopulationFitness();
        for (int i = 0; i < NUM_GENERATIONS; ++i){
            best.clear();
            this->getBest();
            offSprings.clear();
            this->getOffSprings();
            this->tournmentSelection();
            double pc = this->getproportion({0.0, 1.0});

            if (pc <= 0.5){
                cout << "CROOOS OVEEER";
                this->offSprings = this->crossover();
            } 

            for (auto& chromosome : this->offSprings){
                this->chromosomeMutation(chromosome, i);
            }
            this->population.clear();
            this->population = this->best; 
            this->population.insert(population.end(), this->offSprings.begin(), this->offSprings.end());

            cout << "New Generation: " << endl;
            for (auto& [chromosome, p] : this->population){
                cout << chromosome << " " << p << endl;
            }
        }
        pair<vector<double>, double> bestSolution = *min_element(population.begin(), population.end(),[](const auto& a, const auto& b) {
            return a.second <= b.second;
        });
        cout << "Best Solution: " << endl;
        cout << bestSolution.first << " " << bestSolution.second << endl;
        vector<double> vec = bestSolution.first;
        // Use double as the initial value to reduce precision issues
        double totalProportion = accumulate(vec.begin(), vec.end(), 0.0, [](double sum, double value) {
            return sum + value; // Use double for summation
        });
        // double totalProportion = accumulate(vec.begin(), vec.end(), 0);
        cout << "Total Proportion: " << totalProportion << endl;
    }
};

int main(){
    vector<double> numbers = readFromFile("./docs/tests.txt");

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
    Algo algo(testCases[0], 8);
    algo.runAlgo();
}