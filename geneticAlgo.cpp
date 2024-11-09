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

#define NUM_GENERATIONS 100
#define POPULATION_SIZE 50

// Function to merge two multimaps
template <typename KeyType, typename ValueType>
std::multimap<KeyType, ValueType> mergeMultimaps(
    const std::multimap<KeyType, ValueType>& map1,
    const std::multimap<KeyType, ValueType>& map2) {

    // Create a result multimap that will hold the merged content
    std::multimap<KeyType, ValueType> mergedMap = map1;

    // Insert elements of map2 into the merged map
    mergedMap.insert(map2.begin(), map2.end());

    return mergedMap;
}

// Function to read data from file and return it as a vector of integers
vector<int> readFromFile(string file) {
    ifstream inputFile(file); // Open file for reading
    vector<int> numbers;      // Vector to store the numbers
    int number;               // Variable to store each number

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
    int num_tasks;
    int max_time;
    vector<int>tasks_times;
    public:
    TestCase(){

    }
    TestCase(int numTasks, int maxTime, vector<int>tasksTimes){
        this->num_tasks = numTasks;
        this->max_time = maxTime;
        this->tasks_times = tasksTimes;
    }

    vector<int> getTasksTimes(){
        return this->tasks_times;
    }

    int getNumTasks(){
        return this->num_tasks;
    }

    int getMaxTime(){
        return this->max_time;
    }

    void setTaskTime(int taskTime){
        this->tasks_times.push_back(taskTime);
    }
    void setNumTasks(int numTasks){
        this->num_tasks = numTasks;
    }
    void setMaxTime(int maxTime){
        this->max_time = maxTime;
    }

    void displayTestCase(){
        cout << "Max Time: " << this->getMaxTime() << endl;
        cout << "Execution times: ";
        for (int time : this->getTasksTimes()) {
            cout << time << " ";
        }
        cout << endl;
    }
};

vector<TestCase> handleTestCasesData(vector<int> numbers) {
// Function to handle the test cases data from the numbers vector

    int tc = numbers[0]; // The first number represents the number of test cases
    int i = 1, j = 0; // Start processing from the second element (i=1) and first test case (j=0)
    vector<TestCase>testCases(tc);
    // Process the numbers vector to fill testCases
    while (i < numbers.size()) {
        TestCase testCase;
        // Store the max time for tasks (first element of each test case pair)
        testCase.setMaxTime(numbers[i++]);
        // Get the number of tasks for this test case
        testCase.setNumTasks(numbers[i]);
        int size = numbers[i++];
        // Read execution times for each task and store them in excTimePerTask
        while (size--) {
            testCase.setTaskTime(numbers[i++]);
        }
        testCases[j]= (testCase);
        // Move to the next test case
        j++;
    }
    return testCases; 
}

ostream& operator<<(ostream& os, const vector<int>& vec)
{
    os << "[ ";
    for (int i = 0; i < vec.size(); ++i) {
        os << vec[i];
        if (i != vec.size() - 1) {
            os << ", ";  // Print a comma after each element, except the last one
        }
    }
    os << " ]";
    return os;  // Return the ostream object to allow chaining
}

class GeneticAlgorithm{
private:
    multimap<double, vector<int>> population;
    multimap<double, vector<int>> bestSols;
    vector<vector<int>>initialPopulation;
    TestCase testCase;
    int MAX_FITNESS;
    int population_size;


    void getMaxFitness(){
        vector<int> taskTimes = this->testCase.getTasksTimes();
        this->MAX_FITNESS = accumulate(taskTimes.begin(), taskTimes.end(), 0);
    }

    vector<int> makeChromosome(){
        int num_tasks = testCase.getNumTasks();
        vector<int> chromosome(num_tasks);
        for (int j = 0; j < num_tasks; j++)
            chromosome[j] = (rand() % 2) ? 1 : 0;
        return chromosome;
    }

    public:

    GeneticAlgorithm(){
        
    }

    GeneticAlgorithm(int populationSize, TestCase test){
        this->population_size = populationSize,
        this->testCase = test;
        this->initialPopulation = vector<vector<int>>(this->population_size, vector<int>(testCase.getNumTasks()));
        getMaxFitness();
    }
    // Adjusted Fitness , Chromosome 
    void initializePopulation()
    {
        // Generate Chromosomes
        for (int i = 0; i < population_size; i++)
        {
            this->initialPopulation[i] = this->makeChromosome();
        }

        // Attach Adjusted Fitness to the population Function
        for (vector<int>& chromosome : this->initialPopulation){
            int adjusted_fitness = getAdjustedFitness(chromosome);
            population.emplace(adjusted_fitness, chromosome);
        }
    }

    int fitness(vector<int>& chromosome)
    {
        int core1_time = 0, core2_time = 0;
        vector<int> taskTimes =  this->testCase.getTasksTimes();
        for (int i = 0; i < chromosome.size(); i++)
        {
            core1_time += !chromosome[i]? taskTimes[i] : 0;
            core2_time += chromosome[i]? taskTimes[i] : 0;
        }
        return max(core1_time, core2_time);
    }

    multimap<double, vector<int>> selection(int num_members){
        multimap<double, vector<int>> selected_chromosomes;
        multimap<double, vector<int>> chromosomes;

        int totalFitness = 0;
        
        multimap<double, vector<int>> rest;

        const int K = population.size() / 4; 

        for (auto it = this->population.rbegin(); it != this->population.rend(); ++it) {
            if (bestSols.size() < K){
                bestSols.emplace(it->first, it->second);       
            }else {
                rest.emplace(it->first, it->second);
            }
        }

        for_each(rest.begin(), rest.end(), [&totalFitness](const pair<const double, vector<int>>& pair) {
            totalFitness += (int) pair.first;
        });

        for (auto& chromosome: rest){
            chromosomes.emplace(chromosome.first / totalFitness, chromosome.second);
        }

        rouletteWheel(selected_chromosomes, chromosomes, num_members);
    
        multimap<double, vector<int>> result;
        
        for (auto& chromosome : selected_chromosomes){
            int adjusted_fitness = getAdjustedFitness(chromosome.second);
            result.emplace(adjusted_fitness, chromosome.second);
        }

        return result;
    }

    multimap<double, vector<int>> rouletteWheel(multimap<double, vector<int>>& winners,const multimap<double, vector<int>>& elements, const double& numOfTrials) {
        for (int i = 0; i < numOfTrials; ++i){              
            double random_prop = static_cast<double>(rand()) / RAND_MAX;
            double cumulativeUpperBound = 0.0; 
            for (auto& element : elements){ 
                cumulativeUpperBound += element.first;
                if (random_prop <= cumulativeUpperBound){
                    winners.emplace(element.first, element.second);
                    break;
                }
            }
        }
        return winners;
    }

    multimap<double, vector<int>> crossover(multimap<double, vector<int>> curr_generation){ 
        multimap<double, vector<int>> offsprings;
        // Even 
        if (curr_generation.size() % 2 == 0){
            for (auto it = curr_generation.begin(); it != curr_generation.end(); ++it){
                auto next_it = next(it);
                // offsprings = mergeMultimaps(offsprings, _crossover(it->second, next_it->second));
                offsprings.merge(_crossover(it->second, next_it->second));
                ++it;
            }
        } 
        // Odd
        else {
            for (auto it = curr_generation.begin(); it != prev(curr_generation.end()); ++it){
                auto next_it = next(it);
                // offsprings = mergeMultimaps(offsprings, _crossover(it->second, next_it->second));
                offsprings.merge(_crossover(it->second, next_it->second));
            }

            multimap<double, vector<int>> bestOffsprings;
            int i = 0;
            for (auto it = offsprings.rbegin(); it != offsprings.rend() && i < curr_generation.size(); ++it) {     
                bestOffsprings.emplace(it->first, it->second);          
                ++i;
            }

            return bestOffsprings;
        }
        return offsprings;
    }

    multimap<double, vector<int>> _crossover(vector<int> &parent1, vector<int> &parent2)
    {
        int crossover_point = rand() % parent1.size();
        vector<int> child1 = parent1;
        vector<int> child2 = parent2;

        for (int i = crossover_point; i < parent1.size(); ++i)
        {
            child1[i] = parent2[i];
            child2[i] = parent1[i];
        }

        multimap<double, vector<int>> offsprings;
        vector<vector<int>> childs {child1, child2};
        
        for (vector<int>& chrom : childs){
            int adjusted_fitness = getAdjustedFitness(chrom);
            offsprings.emplace(adjusted_fitness, chrom);
        }

        return offsprings;
    }

    void flip_bit_mutation(pair<double ,vector<int>>& chromosome){  
        int mutation_point = rand() % chromosome.second.size();
        chromosome.second[mutation_point] = !chromosome.second[mutation_point];
        chromosome.first = getAdjustedFitness(chromosome.second);
    }

    int getAdjustedFitness(vector<int>& chromosome){
        int actual_fitness = fitness(chromosome);

        if (actual_fitness > testCase.getMaxTime()){
            chromosome = vector<int>(testCase.getNumTasks() , 0);
            return 0;
        }
        // Adjust the fitness to be the bigger the fitness the better the solution
        return (MAX_FITNESS - actual_fitness);
    }

    void printCoreTimes(const vector<int>& chromosome, const vector<int>& task_times)
    {
        int core1_time = 0, core2_time = 0;
        cout << "Tasks Assigned to Core 1: ";
        for (int i = 0; i < chromosome.size(); i++) {
            if (chromosome[i] == 0) {
                core1_time += task_times[i];
                cout << task_times[i] << " ";
            }
        }
        cout << ", Core 1 Execution Time: " << core1_time << endl;

        cout << "Tasks Assigned to Core 2: ";
        for (int i = 0; i < chromosome.size(); i++) {
            if (chromosome[i] == 1) {
                core2_time += task_times[i];
                cout << task_times[i] << " ";
            }
        }
        cout << ", Core 2 Execution Time: " << core2_time << endl;
    }

    void runGeneticAlgo(){
        srand(time(0));
        random_device rd;  // Get a seed from the hardware
        mt19937 gen(rd()); // Use Mersenne Twister generator
        uniform_real_distribution<> dis(0.0, 1.0);
        
        initializePopulation();
        for (int generation = 0; generation < NUM_GENERATIONS; ++generation) {
            multimap<double, vector<int>> selected_chromosomes = selection(population.size() / 3);
            multimap<double, vector<int>> offsprings;
            double random_prop = dis(gen);
            double pc = 0.6;
            if (random_prop <= pc){
                offsprings = crossover(selected_chromosomes);
            }else {
                offsprings = selected_chromosomes;
            }

            random_prop = dis(gen);
            double pm = 0.08;
            if (random_prop <= pm){
                for (auto& offspring : offsprings) {
                    flip_bit_mutation((pair<double ,vector<int>>&) offspring);
                }
            }

            this->population.clear();
            // this->population.merge(offsprings);
            // this->population.merge(bestSols);
            this->population = mergeMultimaps(offsprings, bestSols);
        }

        // ZeroVector to represent infeasible solution
        const vector<int> zeroVector = vector<int>(testCase.getNumTasks(), 0);

        bool isInfeasibleSols = std::all_of(
            population.begin(), population.end(),
            [&zeroVector](const auto& chromosome) {
                return chromosome.second == zeroVector;
            }
        );

        if (isInfeasibleSols){
            int newMax = testCase.getMaxTime() * 1.5 + 10;
            if (newMax > MAX_FITNESS) newMax = MAX_FITNESS;
            cout << "We Cannot Reach a Solution Can Satisfy this Max Time ("<< testCase.getMaxTime() << ") Trying to Run Again with Max Time Equal to " << newMax << "...\n";
            testCase.setMaxTime(newMax);
            runGeneticAlgo();
            return;
        }

        auto bestSolution = population.rbegin();
        cout << "\nBest Solution: " << bestSolution->second << endl; 
        cout << "Actual Fitness(Total Execution Time): " << MAX_FITNESS - (int) bestSolution->first << '\n';
        printCoreTimes(bestSolution->second, testCase.getTasksTimes());
    }
};

int main(){
    vector<int> numbers = readFromFile("./docs/data.txt");

    // Check if there was an error reading the file
    if (numbers[0] == -100) {
        cout << "Error during reading the file" << endl;
        return 1; // Exit early due to file reading error
    }
    // All testCases as an object
    vector<TestCase> testCases = handleTestCasesData(numbers);
    for (int i = 0; i < testCases.size(); i++) {
        cout << "\n_______________________________ Test Case " << i << "___________________________________" << endl;
        testCases[i].displayTestCase();
        auto algo = new GeneticAlgorithm(POPULATION_SIZE, testCases[i]);
        algo->runGeneticAlgo();
        cout << "______________________________________________________________________________\n";
    }
}