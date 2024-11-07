#include <bits/stdc++.h>
using namespace std;

vector<string> initializePopulation(int population_size, int num_tasks)
{
    vector<string> population(population_size);
    for (int i = 0; i < population_size; i++)
    {
        string chromosome = "";
        for (int j = 0; j < num_tasks; j++)
        {
            chromosome += (rand() % 2) ? '1' : '0';
        }
        population[i] = chromosome;
    }
    return population;
}

// fitness function is applies for each chromosome
int fitness(vector<string> chromosome, int max_time_limit, vector<int> task_times)
{
    int core1_time = 0;
    int core2_time = 0;
    for (int i = 0; i < chromosome.size(); i++)
    {
        for (int j = 0; j < chromosome[i].size(); j++)
        {
            if (chromosome[i][j] == '0')
                core1_time += task_times[i];
            else
                core2_time += task_times[i];
        }
    }
    // !!!!!!!!!!!!!! Think about how you will handle infeasible solutions.
    return max(core1_time, core2_time);
}

// rol wheel

void crossover(vector<string> parent1, vector<string> parent2)
{
    int crossover_point = rand() % parent1.size();
    vector<string> child1 = parent1;
    vector<string> child2 = parent2;
    for (int i = crossover_point; i < parent1.size(); i++)
    {
        child1[i] = parent2[i];
        child2[i] = child1[i];
    }
}
void flip_bit_mutation(vector<string> &chromosome){
    int mutation_point = rand() % chromosome.size();
    chromosome[mutation_point] = to_string(1 - stoi(chromosome[mutation_point]));
}

int main()
{
    srand(time(0));
    vector<string> vec;
    vec = initializePopulation(3, 5);
    for (int i = 0; i < vec.size(); i++)
    {
        cout << vec[i] << " ";
    }
}
