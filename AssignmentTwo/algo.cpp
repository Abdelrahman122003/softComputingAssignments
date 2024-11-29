#include <iostream>
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
#include <iomanip>
#include <sstream>

using namespace std;

#define NUM_GENERATIONS 75
#define POPULATION_SIZE 160

ostringstream infeasibleSolution;

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
    vector<pair<float, float>> ranges;
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
    vector<float> chemicalMix;
    vector<pair<vector<float>, float>> population,best;
    vector<pair<vector<float>, float>> offSprings;
    // store index of selected individual instead of store all data 
    vector<int> selectedAddress;
    
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
        // Generate a random float in the range
        return round((dist(gen)) * 100.0f) / 100.0f; // Round to two decimal places
    }
    // function to calculate best of population
    void getBest(){
        int bestSize = round(population.size() * 0.25);
        best = vector<pair<vector<float>,  float>>(bestSize);

        sort(population.begin(), population.end(),  [](const auto& a, const auto& b) {
            return a.second < b.second;
        });

        for(int i = 0; i < bestSize; ++i){
            best[i] = population[i];
        }
    }
    void getOffSprings(){
        int start = best.size();
        int offSpringsSize = population.size() - start;
        offSprings = vector<pair<vector<float>,  float>>(
            offSpringsSize,
            make_pair(vector<float>(testCase.getNumChemicals()), 0.0f)
        );
        int j = 0;
        for (int i = start;i < population.size();++i)
            offSprings[j++] = population[i];
    }
    // function to initialize chemical 
    void makeChemicalMix(){ 
        for(int i = 0; i < testCase.getNumChemicals(); i++)
            chemicalMix[i] = getproportion(testCase.getChemicalRanges()[i]);
        adjustChromosome(chemicalMix);
    }

    void adjustChromosome(vector<float>& chromosome){
        float total = accumulate(chromosome.begin(), chromosome.end(), 0.0f);
        if (testCase.getTotalProportion() > total) {
            float remaining = testCase.getTotalProportion() - total;

            while (remaining > 0) {
                float distributed = 0.0f; // Track how much is distributed in this iteration

                for (int i = 0; i < testCase.getNumChemicals(); i++) {
                    auto range = testCase.getChemicalRanges()[i];
                    float maxAddition = range.second - chromosome[i]; // How much can this element increase without exceeding the max range

                    if (maxAddition > 0) {
                        float addition = min(maxAddition, remaining); // Add the smaller of the remaining amount or max addition
                        chromosome[i] += addition;
                        // chromosome[i] = (round(chromosome[i]) * 100.0f) / 100.0f;
                        distributed += addition;
                        remaining -= addition;

                        if (remaining <= 0) break; // Stop if nothing remains to distribute
                    }
                }
                // If no distribution was possible in this iteration, break to avoid infinite loops
                if (distributed == 0) break;
            }
        } else if (testCase.getTotalProportion() < total) {
            float remaining = total - testCase.getTotalProportion();
            while (remaining > 0) {
                float distributed = 0.0f; // Track how much is distributed in this iteration
                for (int i = 0; i < testCase.getNumChemicals(); i++) {
                    auto range = testCase.getChemicalRanges()[i];
                    // L => 5, i => 50, max substract 40 
                    float maxSubstract = chromosome[i] - range.first; // How much can this element increase without exceeding the max range

                    if (maxSubstract > 0) {
                        float substract = min(maxSubstract, remaining); // Add the smaller of the remaining amount or max addition
                        chromosome[i] -= substract;
                        // chromosome[i] = (round(chromosome[i]) * 100.0f) / 100.0f;
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
    float calcFitness(){
        float fitness = 0.0f;
        // 25.65 = *10 = 256.5 => round(256.5) => 257 => /10 => 25.7
        for(int i  = 0; i < testCase.getNumChemicals(); i++){
            fitness += (round(chemicalMix[i] * 10.0f) / 10.0f) * testCase.getChemicalCosts()[i];
        }
        return fitness;
    }

    void calcPopulationFitness(){
        for(int i = 0; i < popSize; i++)
        {
            chemicalMix = population[i].first;
            population[i].second = calcFitness();
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
            offSprings[notSelected].second = -1.0f;
        }
        // Discard not selected chromosomes
        offSprings.erase(remove_if(offSprings.begin(), offSprings.end(), [](const pair<vector<float>, float>& p) {return roundf(p.second) == -1.0f;}),
        offSprings.end());
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
    vector<pair<vector<float>, float>> crossover() {
        vector<pair<vector<float>, float>> offsprings;
        vector<pair<vector<float>, float>> childs;
        // Even 
        if (this->offSprings.size() % 2 == 0){
            for (int i = 0; i < this->offSprings.size(); i += 2) {
                childs = _crossover(this->offSprings[i].first, this->offSprings[i + 1].first);
                offsprings.insert(offsprings.end(), childs.begin(), childs.end());
            }
        } 
        // Odd
        else {            
            for (auto it = this->offSprings.begin(); it != prev(this->offSprings.end()); ++it){
                auto next_it = next(it);
                childs = _crossover(it->first, next_it->first);
                offsprings.insert(offsprings.end(), childs.begin(), childs.end());
            }

            vector<pair<vector<float>, float>> bestOffsprings;
            int i = 0;
            for (auto it = offsprings.begin(); it != offsprings.end() && i < this->offSprings.size(); ++it) { 
                bestOffsprings.emplace_back(it->first, it->second); // Add to the result
                ++i;
            }
            return bestOffsprings;
        }
        return offsprings;
    }

    vector<pair<vector<float>, float>> _crossover(vector<float> &parent1, vector<float> &parent2)
    {
        srand(time(0));
        int crossover_point1 = rand() % parent1.size();
        int crossover_point2 = rand() % parent1.size();
        if(crossover_point2 < crossover_point1) swap(crossover_point1, crossover_point2);
        vector<float> child1 = parent1;
        vector<float> child2 = parent2;

        for (int i = crossover_point1; i < crossover_point2; ++i)
        {
            child1[i] = parent2[i];
            child2[i] = parent1[i];
        }

        vector<pair<vector<float>, float>> offsprings;
        vector<vector<float>> childs {child1, child2};

        for (vector<float>& chrom : childs){
            chemicalMix = chrom;
            adjustChromosome(chemicalMix);
            offsprings.emplace_back(chemicalMix, calcFitness());
        }

        return offsprings;
    }

   // chromosome, int rangeIndx
    void chromosomeMutation(pair<vector<float>, float>& chromosome, int generationNum){
        int i = 0;
        bool isChanged = false;
        for(auto& gene : chromosome.first){
            double pm = this->getproportion({0.0, 1.0});
            if (pm <= 0.05){
                isChanged = true;
                gene = nonUniformGeneMutation(gene, generationNum, testCase.getChemicalRanges()[i]);
            }
            ++i;
        }
        if (isChanged){
            adjustChromosome(chromosome.first);
            chemicalMix = chromosome.first; chromosome.second = calcFitness();
        }
    }

    float nonUniformGeneMutation(float gene, int generationNum, pair<float, float> geneRange, const int depFactor = 2){
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
    pair<vector<float>, float>* handleProportionConstraint(){
        double lowerBoundSum = 0.0, upperBoundSum = 0.0;
        for (const auto& range : testCase.getChemicalRanges()) {
            lowerBoundSum += range.first; upperBoundSum += range.second;
        }   
        double totalProportion = testCase.getTotalProportion();

        string message;
        double newProportion;
        bool useLowerBounds;

        // Check for infeasible solutions
        if (totalProportion < lowerBoundSum) {
            message = "Total Proportion less than Sum of Lower Bounds of chemicals.";
            newProportion = lowerBoundSum;
            useLowerBounds = true;
        } else if (totalProportion > upperBoundSum) {
            message = "Total Proportion higher than Sum of Upper Bounds of chemicals.";
            newProportion = upperBoundSum;
            useLowerBounds = false;
        } else {
            return nullptr;
        }

        infeasibleSolution << "Infeasible Solution Encountered:\n" << message << "\nAdjusting Total Proportion to " << fixed << setprecision(2) << newProportion << "\n\n";
        cout << infeasibleSolution.str();
        testCase.setTotalProportion(newProportion);

        // Extract bounds based on the scenario
        vector<float> adjustedRanges;
        const auto& chemicalRanges = testCase.getChemicalRanges();
        adjustedRanges.reserve(chemicalRanges.size());

        transform(chemicalRanges.begin(),chemicalRanges.end(), back_inserter(adjustedRanges),
            [useLowerBounds](const pair<float, float>& range) {
                return useLowerBounds ? range.first : range.second;
            }
        );
        chemicalMix = adjustedRanges;  
        return new pair<vector<float>, float>(adjustedRanges, calcFitness());
    } 

    pair<vector<float>, float> runAlgo(){ 
        auto sol = this->handleProportionConstraint();
        if (sol != nullptr) return *sol;
        
        this->initalPopulation();
        this->calcPopulationFitness();
        for (int i = 0; i < NUM_GENERATIONS; ++i){
            this->getBest();
            this->getOffSprings();
            this->tournmentSelection();
            double pc = this->getproportion({0.0, 1.0});

            if (pc <= 0.7){
                auto temp = this->crossover();
                this->offSprings.clear();
                this->offSprings = temp;
            } 

            for (auto& chromosome : this->offSprings){
                this->chromosomeMutation(chromosome, i);
            }
            this->population.clear();
            this->population.resize(this->best.size() + this->offSprings.size());
            this->population = this->best; 
            this->population.insert(population.end(), this->offSprings.begin(), this->offSprings.end());

            if (population.size() == 1) break;

            best.clear();
            offSprings.clear();
        }

        pair<vector<float>, float> bestSolution = *min_element(population.begin(), population.end(),[](const auto& a, const auto& b) {
            return a.second <= b.second;
        });

        return bestSolution;
    }
};
void clearOutputFile(){
    ofstream outFile("./docs/output.txt", ios::out);
    outFile.close();
}

void writeIntoFile(int datasetNum, pair<vector<float>, float> bestSolution) {
    // Open the file in output mode (overwrite the file's content)
    ofstream outFile("./docs/output.txt", ios::app);
    if (!outFile) {
        cerr << "Error opening file!" << endl;
        return;
    }

    // Write the dataset number
    outFile << "Dataset " << datasetNum << endl;
    outFile << infeasibleSolution.str();
    // Clear the content and reset the state
    infeasibleSolution.str("");   // Clear the content
    infeasibleSolution.clear();   // Reset the state flags
    // Write the chemical proportions
    outFile << "Chemical Proportions: ";
    for (auto i = 0; i < bestSolution.first.size(); ++i) {
        outFile << fixed << setprecision(1) << bestSolution.first[i];
        if (i < bestSolution.first.size() - 1) {
            outFile << " ";
        }
    }
    outFile << endl;

    // Write the total cost
    outFile << "Total Cost: " << fixed << setprecision(2) << bestSolution.second << endl;

    // Close the file
    outFile.close();
}

int main(){
    vector<float> numbers = readFromFile("./docs/tests.txt");

    // Check if there was an error reading the file
    if (numbers[0] == -100) {
        cout << "Error during reading the file" << endl;
        return 1; // Exit early due to file reading error
    }
    vector<TestCase> testCases = handleTestCasesData(numbers);
    // clear ouputFile
    clearOutputFile();
    for(int i = 0; i < testCases.size(); i++)
    {
        Algo algo(testCases[i], POPULATION_SIZE);
        cout << "__________________________ TEST CASE " << i << " ____________________________\n";
        pair<vector<float>, float> bestSolution = algo.runAlgo();
        writeIntoFile(i+1, bestSolution);
        cout << "Best Solution: " << endl;
        cout << bestSolution.first << " " << bestSolution.second << endl;
        vector<float> vec = bestSolution.first;
        double totalProportion = accumulate(vec.begin(), vec.end(), 0.0);
        cout << "Total Proportion: " << totalProportion << "\n\n";
    } 
}