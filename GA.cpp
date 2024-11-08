#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>  // For rand() and srand()
#include <ctime>    // For time()
#include <map>
#include <algorithm>
#include <numeric>  // For std::accumulate
#include <cmath>  // for std::abs


using namespace std;

#define K 2
vector<vector<int>> bestSols;

multimap<double, vector<int>> rouletteWheel(multimap<double, vector<int>>& winners,const multimap<double, vector<int>>& elements, const double& numOfTrials);

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

vector<vector<int>> initializePopulation(int population_size, int num_tasks)
{
    vector<vector<int>> population(population_size, vector<int>(num_tasks));
    for (int i = 0; i < population_size; i++)
    {
        vector<int> chromosome(num_tasks);
        for (int j = 0; j < num_tasks; j++)
            chromosome[j] = (rand() % 2) ? 1 : 0;
        population[i] = chromosome;
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


multimap<double, vector<int>> selection(vector<vector<int>> population, int num_members, int max_time, vector<int> task_times){
    multimap<double, vector<int>> selected_chromosomes;
    multimap<double, vector<int>> chromosomes;
    
    const int MAX_FITNESS = accumulate(task_times.begin(), task_times.end(), 0);
    
    vector<int> fitnesses;
    fitnesses.reserve(population.size());

    int totalFitness = 0;
    for (vector<int>& chromosome : population){
        int actual_fitness = fitness(chromosome, max_time, task_times);

        if (actual_fitness > max_time){
            fitnesses.emplace_back(0);
            chromosome = vector<int>(population[0].size() , 0);
            continue;
        }

        // Adjust the fitness to be the bigger the fitness the better the solution
        int current_fitness = MAX_FITNESS - actual_fitness;
        totalFitness += current_fitness;
        fitnesses.emplace_back(current_fitness);

        cout << "Fitness: " << current_fitness << " " << chromosome << endl;
    }
    cout << "Total fitness: " << totalFitness << endl;

    double total_proportion = 0.0;
    for (int i = 0; i < fitnesses.size(); ++i){
        chromosomes.emplace(((double) fitnesses[i] / totalFitness), population[i]);
        total_proportion += ((double) fitnesses[i] / totalFitness);
    }

    cout << "Total Proportion: " << total_proportion << endl;

    for (auto& chromosome : chromosomes){
        cout << chromosome.first << ": \n";
        cout << chromosome.second << endl;
    }

    
    multimap<double, vector<int>> rest;
    for (auto it = chromosomes.rbegin(); it != chromosomes.rend(); ++it) {
        if (chromosomes.size() < K){
            bestSols.emplace_back(it->second);       
        }else {
            rest.emplace(it->first, it->second);
        }
    }

    rouletteWheel(selected_chromosomes, rest, num_members);
    
    for (auto& sol: bestSols){
        cout << sol << endl;
    }

    return selected_chromosomes;
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

multimap<double, vector<int>> crossover(multimap<double, vector<int>> curr_generation){
    
}

// rol wheel
pair<vector<int>, vector<int>> _crossover(vector<int> &parent1, vector<int> &parent2)
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

    return make_pair(child1, child2);
}

void flip_bit_mutation(vector<int> &chromosome){
    int mutation_point = rand() % chromosome.size();
    chromosome[mutation_point] = !chromosome[mutation_point];
}

int main()
{
    srand(time(0));
    // vector<string> vec;
    vector<vector<int>>populations = initializePopulation(8, 4);
    multimap<double, vector<int>> selected_chromosomes = selection(populations, 5, 66, vector<int>{10, 20 , 30 , 6});
    cout << "\nSelected Chromosomes: \n";
    for(auto &chromosome : selected_chromosomes){
        cout << "Adjusted Fitness: " << chromosome.first << " " << chromosome.second << endl;
    }


    pair<vector<int>, vector<int>> newGeneration = _crossover(populations[0], populations[1]);


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
