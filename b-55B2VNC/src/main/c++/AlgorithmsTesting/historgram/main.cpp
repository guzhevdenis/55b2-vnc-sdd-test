#include <iostream>
#include <vector>
#include <algorithm>
#include <bits/stdc++.h>
void show(std::vector<double>& a);
void create_range(std::vector<int>& a, int min, int max, int range);
void count_numbers(std::vector<int>&a, std::vector<int>&b, std::vector<int>& c, int range);
void create_histogram (std::vector<double>&pulses , std::vector<double>& b, std::vector<double>& c, int range);

int main ()
{
    std::vector<double> pulses{10, 11, 11, 20, 21, 21, 21, 30, 30, 30, 42, 42, 50};
    show(pulses);
    std::vector<double> amplitudes; 
    std::vector<double> numbers;
    create_histogram (pulses , amplitudes,  numbers,  2);
    show(amplitudes);
    show(numbers);
    return 0;
}

void create_histogram (std::vector<double>&pulses , std::vector<double>& amplitudes, std::vector<double>& numbers, int range){

	std::map<int, int> histogram;

    // Create histogram bins
    for (int value : pulses) {
        int bin = (value / range) * range; // Determine the bin for the value
        ++histogram[bin]; // Increment the count for this bin
    }

    // Output the histogram
    for (const auto& bin : histogram) {
        std::cout << "[" << bin.first << " - " << bin.first + range << "]: ";
        for (int i = 0; i < bin.second; ++i) {
            std::cout << "*"; // Print asterisks for the count
        }
        std::cout << " (" << bin.second << ")\n"; // Print the count
    }

	for (const auto& x : histogram){
			amplitudes.push_back((x.first));
			numbers.push_back(x.second);					
	}
}

void show(std::vector<double>& a) {
    for (const double& element : a) {
        std::cout << element << " ";
    }
    std::cout << std::endl;
}