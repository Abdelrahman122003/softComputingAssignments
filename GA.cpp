#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>  // For rand() and srand()
#include <ctime>    // For time()


using namespace std;

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

// rol wheel
pair<vector<int>, vector<int>> crossover(vector<int> &parent1, vector<int> &parent2)
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
    vector<vector<int>>populations = initializePopulation(3, 5);


    // for (int i = 0; i < populations.size(); i++)
    // {
    //     for(int j = 0; j < populations[i].size(); j++){
    //         cout << populations[i][j] << " ";
    //     }
    //     cout << "\n";
    // }
    cout << "populations 1\n";
    cout  << populations[0] << "\n";
    cout << "populations 2\n";
    cout << populations[1] << "\n";

    for(int i = 0; i < populations[0].size();i++)
        cout << populations[0][i] << " ";
    pair<vector<int>, vector<int>> newGeneration = crossover(populations[0], populations[1]);
   

    cout << "Generation 1 0\n";
    vector<int> g1 = newGeneration.first;
    cout  << g1 << "\n";
    cout << "Generation 1 0\n";
    vector<int> g2 = newGeneration.second;
    cout  << g2 << "\n";

}
