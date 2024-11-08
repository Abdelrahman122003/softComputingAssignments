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

using namespace std;

#define K 3
multimap<double, vector<int>> bestSols;

multimap<double, vector<int>> rouletteWheel(multimap<double, vector<int>>& winners,const multimap<double, vector<int>>& elements, const double& numOfTrials);
int fitness(vector<int> chromosome, int max_time_limit, vector<int> task_times);
multimap<double, vector<int>> _crossover(vector<int> &parent1, vector<int> &parent2, vector<int> task_times, int max_time);

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

// Adjusted Fitness , Chromosome 
multimap<double, vector<int>> initializePopulation(int population_size, int num_tasks, vector<int> task_times, int max_time)
{
  const int MAX_FITNESS = accumulate(task_times.begin(), task_times.end(), 0);

  vector<vector<int>> pop(population_size, vector<int>(num_tasks));
  multimap<double, vector<int>> population;

  srand(time(0));
  // Generate Chromosomes
  for (int i = 0; i < population_size; i++)
  {
      vector<int> chromosome(num_tasks);
      for (int j = 0; j < num_tasks; j++)
        chromosome[j] = (rand() % 2) ? 1 : 0;
      pop[i] = chromosome;
  }

  for (vector<int>& chromosome : pop){
    int actual_fitness = fitness(chromosome, max_time, task_times);

    if (actual_fitness > max_time){
        chromosome = vector<int>(num_tasks , 0);
        population.emplace(0, chromosome);
        continue;
    }

    // Adjust the fitness to be the bigger the fitness the better the solution
    int current_fitness = MAX_FITNESS - actual_fitness;
    population.emplace(current_fitness, chromosome);

    cout << "Fitness: " << current_fitness << " " << chromosome << endl;
  }

  return population;
}

// fitness function is applies for each chromosome
int fitness(vector<int> chromosome, int max_time_limit, vector<int> task_times)
{
    int core1_time = 0, core2_time = 0;
    for (int i = 0; i < chromosome.size(); i++)
    {
        core1_time += !chromosome[i]? task_times[i] : 0;
        core2_time += chromosome[i]? task_times[i] : 0;
    }
    // !!!!!!!!!!!!!! Think about how you will handle infeasible solutions.
    return max(core1_time, core2_time);
}


multimap<double, vector<int>> selection(multimap<double, vector<int>> population, int num_members, int num_tasks , vector<int> task_times, int max_time){
    multimap<double, vector<int>> selected_chromosomes;
    multimap<double, vector<int>> chromosomes;

    int totalFitness = 0;
    
    multimap<double, vector<int>> rest;
    for (auto it = population.rbegin(); it != population.rend(); ++it) {
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

    const int MAX_FITNESS = accumulate(task_times.begin(), task_times.end(), 0);
    multimap<double, vector<int>> result;
    
    for (auto& chromosome : selected_chromosomes){
      int actual_fitness = fitness(chromosome.second, max_time, task_times);

      if (actual_fitness > max_time){
          chromosome.second = vector<int>(num_tasks , 0);
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
      offsprings.merge(_crossover(it->second, next_it->second, task_times, max_time));
      ++it;
    }
  } 
  // Odd
  else {
    for (auto it = curr_generation.begin(); it != prev(curr_generation.end()); ++it){
      auto next_it = next(it);
      offsprings.merge(_crossover(it->second, next_it->second, task_times, max_time));
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

// rol wheel
multimap<double, vector<int>> _crossover(vector<int> &parent1, vector<int> &parent2, vector<int> task_times, int max_time)
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

    const int MAX_FITNESS = accumulate(task_times.begin(), task_times.end(), 0);

    for (vector<int>& chromosome : childs){
      int actual_fitness = fitness(chromosome, max_time, task_times);

      if (actual_fitness > max_time){
        chromosome = vector<int>(chromosome.size() , 0);
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

void flip_bit_mutation(pair<double ,vector<int>>& chromosome,vector<int> task_times, int max_time){
  const int MAX_FITNESS = accumulate(task_times.begin(), task_times.end(), 0);
  int mutation_point = rand() % chromosome.second.size();
  chromosome.second[mutation_point] = !chromosome.second[mutation_point];
  
  int actual_fitness = fitness(chromosome.second, max_time, task_times);

  if (actual_fitness > max_time){
    chromosome.first = 0;
    chromosome.second = vector<int>(chromosome.second.size() , 0);
  }

  // Adjust the fitness to be the bigger the fitness the better the solution
  int current_fitness = MAX_FITNESS - actual_fitness;
  chromosome.first = current_fitness;
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

int main()
{
    srand(time(0));
    random_device rd;  // Get a seed from the hardware
    mt19937 gen(rd()); // Use Mersenne Twister generator
    uniform_real_distribution<> dis(0.0, 1.0);
    // vector<string> vec;
    multimap<double, vector<int>> population = initializePopulation(10, 4, vector<int>{10, 20 , 30 , 6}, 66);
    for (int generation = 0; generation < 50; ++generation) {
      cout << "\nGeneration " << generation + 1 << ":\n";
      multimap<double, vector<int>> selected_chromosomes = selection(population, 5, 4,vector<int>{10, 20 , 30 , 6}, 66);
      cout << "\nSelected Chromosomes: \n";
      for(auto &chromosome : selected_chromosomes){
          cout << "Adjusted Fitness: " << chromosome.first << " " << chromosome.second << endl;
      }
      multimap<double, vector<int>> offsprings;
      double random_prop = dis(gen);
      double pc = 0.5;
      if (random_prop <= pc){
        offsprings = crossover(selected_chromosomes, vector<int>{10, 20, 30, 6}, 66);
      }else {
        offsprings = selected_chromosomes;
      }

      random_prop = dis(gen);
      double pm = 0.1;
      if (random_prop <= pm){
        for (auto& offspring : offsprings) {
          // pm <= 0.01  
          flip_bit_mutation((pair<double ,vector<int>>&) offspring , vector<int>{10, 20, 30, 6}, 66);
        }
        cout << "\nOffsprings after mutation: \n";
      }
      for(auto& ch : offsprings) {
        cout << "Offsprings Before Merge: " << (int) ch.first << " " << ch.second << endl;
      }
      for(auto& ch : bestSols) {
        cout << "bestSols Before Merge: " << (int) ch.first << " " << ch.second << endl;
      }
      population.clear();
      population.merge(offsprings);
      population.merge(bestSols);

      for(auto& ch : population){
        cout << "New Generation: " << (int) ch.first << " " << ch.second << endl;
      }
      
      for(auto& ch : offsprings) {
        cout << "Offsprings After Merge: " << (int) ch.first << " " << ch.second << endl;
      }
      for(auto& ch : bestSols) {
        cout << "bestSols After Merge: " << (int) ch.first << " " << ch.second << endl;
      }

      vector<int> task_times = vector<int>{10, 20, 30, 6};
      const int MAX_FITNESS = accumulate(task_times.begin(), task_times.end(), 0);

      auto bestSolution = population.rbegin();
      cout << "Best Solution: " << bestSolution->second << endl; 
      cout << "Actual Fitness: " << MAX_FITNESS - (int) bestSolution->first;
      cout << '\n';
      printCoreTimes(bestSolution->second, task_times);
    }

    // pair<vector<int>, vector<int>> newGeneration = _crossover(populations[0], populations[1]);


    // for (int i = 0; i < populations.size(); i++)
    // {
    //     for(int j = 0; j < populations[i].size(); j++){
    //         cout << populations[i][j] << " ";
    //     }
    //     cout << "\n";
    // }
    // cout << "populations 1\n";
    // cout  << populations[0] << "\n";
    // cout << "populations 2\n";
    // cout << populations[1] << "\n";

    // for(int i = 0; i < populations[0].size();i++)
    //     cout << populations[0][i] << " ";
    // pair<vector<int>, vector<int>> newGeneration = crossover(populations[0], populations[1]);


    // cout << "Generation 1 0\n";
    // vector<int> g1 = newGeneration.first;
    // cout  << g1 << "\n";
    // cout << "Generation 1 0\n";
    // vector<int> g2 = newGeneration.second;
    // cout  << g2 << "\n";

}
