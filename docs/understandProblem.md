# Explanation of the Genetic Algorithm's Fitness Function for Task Allocation

## Goal of the Fitness Function

The goal is **not to use only one core** or to make one core handle all tasks. Instead, the aim is to **balance the load** between the two cores so that neither core is overloaded compared to the other. The reason we measure fitness by the core with the longer time is that, since the cores work in parallel, the total execution time is determined by whichever core takes longer to finish.

### Key Points:

1. **Parallel Execution**:

   - Both cores are working at the same time, but each one is only responsible for the tasks assigned to it.
   - Imagine you have two workers sharing a job. The job is complete only when both workers finish their tasks. So, if one worker has much more work, the other might finish early, but you’ll still be waiting for the first one to finish.

2. **Why Use the Longest Core Time**:

   - By minimizing the time of the core that’s taking the longest, we ensure that no core is overloaded. This **balance** helps to minimize the overall time needed to complete all tasks.
   - The solution doesn’t put all tasks on one core; rather, it **tries to spread tasks between the two cores** as evenly as possible.

3. **Example**:

   - Let’s say we have tasks with times `[4, 2, 5, 3]`.
   - If all tasks were assigned to one core, that core would take `4 + 2 + 5 + 3 = 14` units of time, while the other core would do nothing. This would clearly be inefficient.

   Instead, by splitting the tasks intelligently between the two cores, we can make both cores finish around the same time, reducing the total execution time.

   For example:

   - Core 1: `[4, 3]` → Total time = 7
   - Core 2: `[5, 2]` → Total time = 7
   - Here, both cores finish in 7 units of time, so our total execution time is 7 instead of 14.

### Summary

This is why we measure fitness by the core with the longer time—it's a way to ensure that the cores are well-balanced and that the total execution time is minimized.

```js
// simulation for 4 tasks
tasks         = [0, 1, 2,  3]
excution time = [1, 4, 50, 30]

// core 1 ==> 0
// core 2 ==> 1
// A chromosome represents a series of genes, where each gene is either 0 or 1. A 1 indicates that the task is assigned to core 1, and a 0 indicates that the task is assigned to core 2.
```
