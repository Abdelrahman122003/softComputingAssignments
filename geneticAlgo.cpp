#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>  // For rand() and srand()
#include <ctime>    // For time()
#include <map>
#include <algorithm>
#include <numeric>  // For std::accumulate
#include <cmath>  // for std::abs
#include <random>  // For better random number generation
#include <fstream>
using namespace std;

#define K 3

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
        this-> max_time = maxTime;
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
    vector<int>chromosome;
    TestCase testCase;
    int MAX_FITNESS;
    int population_size;


    void getMaxFitness(){
        this->MAX_FITNESS = accumulate(this->testCase.getTasksTimes().begin(), this->testCase.getTasksTimes().end(), 0);
    }
    void makeChromosome(){
        for (int j = 0; j < testCase.getNumTasks(); j++)
            this->chromosome[j] = (rand() % 2) ? 1 : 0;
    }

    public:

    GeneticAlgorithm(){
        
    }

    GeneticAlgorithm(int populationSize, TestCase test){
        this->population_size = populationSize,
        this->testCase = test;
        this->chromosome = vector<int>(this->testCase.getNumTasks());
        this->initialPopulation = vector<vector<int>>(this->population_size, vector<int>(testCase.getNumTasks()));
        getMaxFitness();
    }
    // Adjusted Fitness , Chromosome 
    void initializePopulation()
    {
        srand(time(0));
        // Generate Chromosomes
        for (int i = 0; i < population_size; i++)
        {
            this->makeChromosome();
            this->initialPopulation[i] = this->chromosome;
        }
        // firstTimeFunction
        for (vector<int>& chromosome : this->initialPopulation){
            this->chromosome = chromosome;
            int actual_fitness = fitness();

            if (actual_fitness > testCase.getMaxTime()){
                this->chromosome = vector<int>(testCase.getNumTasks() , 0);
                this->population.emplace(0, this->chromosome);
                continue;
            }

            // Adjust the fitness to be the bigger the fitness the better the solution
            int current_fitness = MAX_FITNESS - actual_fitness;
            population.emplace(current_fitness, this->chromosome);

            cout << "Fitness: " << current_fitness << " " << this->chromosome << endl;
        }
    }

    int fitness()
    {
        int core1_time = 0, core2_time = 0;
        for (int i = 0; i < this->chromosome.size(); i++)
        {
            core1_time += !this->chromosome[i]? this->testCase.getTasksTimes()[i] : 0;
            core2_time += this->chromosome[i]? this->testCase.getTasksTimes()[i] : 0;
        }
        // !!!!!!!!!!!!!! Think about how you will handle infeasible solutions.
        return max(core1_time, core2_time);
    }

    multimap<double, vector<int>> selection(int num_members){
    multimap<double, vector<int>> selected_chromosomes;
    multimap<double, vector<int>> chromosomes;

    int totalFitness = 0;
    
    multimap<double, vector<int>> rest;
    for (auto it = this->population.rbegin(); it != population.rend(); ++it) {
        if (bestSols.size() < K){
            bestSols.emplace(it->first, it->second);       
        }else {
            rest.emplace(it->first, it->second);
        }
    }

    for_each(rest.begin(), rest.end(), [&totalFitness](const pair<const double, vector<int>>& pair) {
      totalFitness += (int) pair.first;
    });

    double total_proportion = 0.0;

    for (auto& chromosome: rest){
      chromosomes.emplace(chromosome.first / totalFitness, chromosome.second);
      total_proportion += (chromosome.first / totalFitness);
    }

    cout << "Total Proportion: " << total_proportion << endl;

    for (auto& chromosome : chromosomes){
        cout << chromosome.first << ": \n";
        cout << chromosome.second << endl;
    }

    rouletteWheel(selected_chromosomes, chromosomes, num_members);
    
    for (auto& sol: bestSols){
        cout << "BEST SOLS: " << sol.first << " " << sol.second << endl;
    }
    multimap<double, vector<int>> result;
    

    // firstTimeFunction
    for (auto& chromosome : selected_chromosomes){
        this->chromosome = chromosome.second;
      int actual_fitness = fitness();

      if (actual_fitness > this->testCase.getMaxTime()){
          chromosome.second = vector<int>(this->testCase.getNumTasks() , 0);
          result.emplace(0, chromosome.second);
          continue;
      }

      // Adjust the fitness to be the bigger the fitness the better the solution
      int current_fitness = MAX_FITNESS - actual_fitness;
      result.emplace(current_fitness, chromosome.second);
    }

    return result;
}

multimap<double, vector<int>> rouletteWheel(multimap<double, vector<int>>& winners,const multimap<double, vector<int>>& elements, const double& numOfTrials) {
    for (int i = 0; i < numOfTrials; ++i){              
        double random_prop = static_cast<double>(rand()) / RAND_MAX;
        cout << "\nProportion: " << random_prop << endl;
        double cumulativeUpperBound = 0.0; 
        for (auto& element : elements){ 
            cumulativeUpperBound += element.first;
            cout << cumulativeUpperBound << endl;
            if (random_prop <= cumulativeUpperBound){
                cout << "Selected Chromosome: " << element.first << " " << element.second << endl;
                winners.emplace(element.first, element.second);
                break;
            }
        }
    }
    return winners;
}
multimap<double, vector<int>> crossover(multimap<double, vector<int>> curr_generation, vector<int> task_times, int max_time){ 
  multimap<double, vector<int>> offsprings;
  // Even 
  if (curr_generation.size() % 2 == 0){
    for (auto it = curr_generation.begin(); it != curr_generation.end(); ++it){
      auto next_it = next(it);
      offsprings.merge(_crossover(it->second, next_it->second));
      ++it;
    }
  } 
  // Odd
  else {
    for (auto it = curr_generation.begin(); it != prev(curr_generation.end()); ++it){
      auto next_it = next(it);
      offsprings.merge(_crossover(it->second, next_it->second));
    }

    for (auto offspring: offsprings){
      cout << "Offspring: " << offspring.first << " " << offspring.second << endl;
    }

    multimap<double, vector<int>> bestOffsprings;
    int i = 0;
    for (auto it = offsprings.rbegin(); it != offsprings.rend() && i < curr_generation.size(); ++it) {     
      bestOffsprings.emplace(it->first, it->second);          
      ++i;
    }

    for (auto offspring: bestOffsprings){
      cout << "Best Offspring: " << offspring.first << " " << offspring.second << endl;
    }
    return bestOffsprings;
  }
  return offsprings;
}

multimap<double, vector<int>> _crossover(vector<int> &parent1, vector<int> &parent2)
{
    int crossover_point = rand() % parent1.size();
    cout << "crossover point : " << crossover_point;
    vector<int> child1 = parent1;
    vector<int> child2 = parent2;

    for (int i = crossover_point; i < parent1.size(); i++)
    {
      child1[i] = parent2[i];
      child2[i] = parent1[i];
    }

    multimap<double, vector<int>> offsprings;
    vector<vector<int>> childs {child1, child2};
    // firstTimeFunction
    for (vector<int>& chrom : childs){
        this->chromosome = chrom;
      int actual_fitness = fitness();

      if (actual_fitness > testCase.getMaxTime()){
        this->chromosome = vector<int>(this->chromosome.size() , 0);
        offsprings.emplace(0, chromosome);
        continue;
      }
      // Adjust the fitness to be the bigger the fitness the better the solution
      int current_fitness = MAX_FITNESS - actual_fitness;
      offsprings.emplace(current_fitness, chromosome);

      cout << "Fitness: " << current_fitness << " " << chromosome << endl;
    }

    return offsprings;
}
void printCoreTimes(const vector<int>& chromosome, const vector<int>& task_times)
{
    int core1_time = 0, core2_time = 0;
    cout << "\nTasks assigned to Core 1: ";
    for (int i = 0; i < chromosome.size(); i++) {
        if (chromosome[i] == 0) {
            core1_time += task_times[i];
            cout << task_times[i] << " ";
        }
    }
    cout << ", Core1 Time: " << core1_time << endl;

    cout << "\nTasks assigned to Core 2: ";
    for (int i = 0; i < chromosome.size(); i++) {
        if (chromosome[i] == 1) {
            core2_time += task_times[i];
            cout << task_times[i] << " ";
        }
    }
    cout << ", Core2 Time: " << core2_time << endl;
}


  void runGeneticAlgo(){

  }

};

int main(){
    GeneticAlgorithm algo;

    vector<int> numbers = readFromFile("./docs/data.txt");

    // Check if there was an error reading the file
    if (numbers[0] == -100) {
        cout << "Error during reading the file" << endl;
        return 1; // Exit early due to file reading error
    }
    // All testCases as a object
    vector<TestCase> testCases = handleTestCasesData(numbers);
    for (int i = 0; i < testCases.size(); i++) {
        cout << "Test case " << i + 1 << ": " << endl;
        testCases[i].displayTestCase();
    }
}