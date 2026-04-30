#include <iostream>
#include <vector>

// This program is supposed to:
// 1. Compute the average of a list of numbers
// 2. Print all numbers greater than the average
//
// It compiles, but it contains several bugs.
// Use the VSCode debugger to find and fix them.

double computeAverage(std::vector<int>& nums) {
    int sum = 0;

    for (int i = 0; i < nums.size(); i++) {
        sum += nums[i];
    }

    return sum / nums.size();
}

std::vector<int> filterAboveAverage(std::vector<int>& nums, double avg) {
    std::vector<int> result;

    for (int i = 0; i < nums.size(); i++) {
        if (nums[i] > avg) {
            result.push_back(nums[i]);
        }
    }

    return result;
}

int main() {
    std::vector<int> values = {10, 20, 30, 40, 50};

    double avg = computeAverage(values);
    std::cout << "Average: " << avg << std::endl;

    std::vector<int> filtered = filterAboveAverage(values, avg);

    std::cout << "Values above average: ";
    for (int i = 0; i < filtered.size(); i++) {
        std::cout << filtered[i] << " ";
    }

    std::cout << std::endl;

    return 0;
}
