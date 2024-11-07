#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <utility> // for pair

using namespace std;

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

// Function to handle the test cases data from the numbers vector
vector<pair<int, vector<int>>> handleTestCasesData(vector<int> numbers) {
    int tc = numbers[0]; // The first number represents the number of test cases
    vector<pair<int, vector<int>>> testCases(tc, make_pair(0, vector<int>())); // Initialize the testCases vector with pairs
    int i = 1, j = 0; // Start processing from the second element (i=1) and first test case (j=0)

    // Process the numbers vector to fill testCases
    while (i < numbers.size()) {
        // Store the number of tasks (first element of each test case pair)
        testCases[j].first = numbers[i++];
        
        // Get the number of tasks for this test case
        int size = numbers[i++];

        // Vector to store execution times of tasks
        vector<int> excTimePerTask;

        // Read execution times for each task and store them in excTimePerTask
        while (size--) {
            excTimePerTask.push_back(numbers[i++]);
        }

        // Store the execution times in the second part of the pair
        testCases[j].second = excTimePerTask;

        // Move to the next test case
        j++;
    }

    return testCases; // Return the filled testCases vector
}

int main() {
    // Read numbers from file
    vector<int> numbers = readFromFile("./data.txt");

    // Check if there was an error reading the file
    if (numbers[0] == -100) {
        cout << "Error during reading the file" << endl;
        return 1; // Exit early due to file reading error
    }

    // Process the numbers to handle test cases data
    vector<pair<int, vector<int>>> testCases = handleTestCasesData(numbers);    
    
    // Example of printing the test cases
    for (int i = 0; i < testCases.size(); i++) {
        cout << "Test case " << i + 1 << ": " << endl;
        cout << "Number of tasks: " << testCases[i].first << endl;
        cout << "Execution times: ";
        for (int time : testCases[i].second) {
            cout << time << " ";
        }
        cout << endl;
    }

    return 0;
}
