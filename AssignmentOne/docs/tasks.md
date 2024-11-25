# Genetic Algorithm Task Allocation Problem Breakdown

## Task Breakdown:

### 1. Understand the Problem Requirements:

- Read and analyze the problem to understand the constraints (two cores, time limit, tasks, etc.).
- **[Time estimate: 30 minutes][Done]**

### 2. Parse Input Data:

- Read the input file that contains the test cases, each consisting of a max time limit, the number of tasks, and the execution time for each task.
- Store the data in a format suitable for processing, such as a list of dictionaries.
- **[Time estimate: 1 hour][Done]**

### 3. Representation of the Solution:

- Define a binary, one-dimensional chromosome: Each gene represents a task, where `1` means assigned to core 1 and `0` means assigned to core 2.
- **[Time estimate: 30 minutes][]**

### 4. Evaluate Fitness Function:

- Write a function to evaluate the fitness of a solution (chromosome). The fitness is determined by calculating the maximum execution time between both cores.
- If either core exceeds the max time limit, the solution is infeasible, and you should handle infeasibility appropriately.
- **[Time estimate: 1 hour][Done]**

### 5. Initialize Population:

- Generate an initial population with random chromosomes.
- Try different population sizes (50, 100, 250) and note how this affects the results.
- **[Time estimate: 1 hour][Done]**

### 6. Selection:

- Implement the roulette wheel selection method to choose chromosomes for the next generation.
- **[Time estimate: 1 hour][Ahmed]**

### 7. Crossover:

- Implement a one-point crossover method to combine two parent chromosomes into a child chromosome.
- **[Time estimate: 1 hour][Done]**

### 8. Mutation:

- Implement a bit-flip mutation method to randomly flip the gene of a chromosome with a small probability.
- **[Time estimate: 1 hour][Done]**

### 9. Elitism Replacement:

- Ensure that the best solution is carried over to the next generation (elitism).
- **[Time estimate: 30 minutes][Ahmed]**

### 10. Main Genetic Algorithm Loop:

- Implement the main loop of the genetic algorithm where:
  - The population is evolved for several generations (you decide the maximum number of generations).
  - Apply selection, crossover, mutation, and elitism to produce the next generation.
- **[Time estimate: 2-3 hours][]**

### 11. Handle Infeasible Solutions:

- Ensure infeasible solutions are handled correctly, either by discarding them or applying penalties to their fitness.
- **[Time estimate: 1 hour][]**

### 12. Output the Results:

- For each test case, output the index, the best solution found, its evaluation score, the chromosome representation, the tasks assigned to each core, and the total time taken for each core.
- **[Time estimate: 1 hour][]**
