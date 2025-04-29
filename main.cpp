#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "MLFQ.h"
#include "SRTF.h"

int main() { //reads CSV files containing all test cases
    std::ifstream file("input.csv");
    if (!file.is_open()) {
        std::cerr << "Failed to open input.csv" << std::endl;
        return 1;
    }

    std::string line;
    int test_case = 1;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string token;

        std::getline(ss, token, ',');
        int num_processes = std::stoi(token);

        std::vector<MLFQ_Process> processes(num_processes);
        std::vector<SRTF_Process> processes_srtf(num_processes);

        for (int i = 0; i < num_processes; ++i) {
            if (!std::getline(ss, token, ',')) {
                std::cerr << "Error: Missing arrival/burst info for process " << i << " in test case " << test_case << std::endl;
                return 1;
            }

            std::stringstream ab(token);
            int arrival, burst;
            ab >> arrival >> burst;

            processes[i].pid = i;
            processes[i].arrival = arrival;
            processes[i].burst = burst;
            processes[i].remaining = burst;

            processes_srtf[i].pid = i;
            processes_srtf[i].arrival = arrival;
            processes_srtf[i].burst = burst;
            processes_srtf[i].remaining = burst;
        }

        //Run MLFQ
        MLFQ_Scheduler mlfq(processes.size());
        mlfq.processes = processes;
        mlfq.run("results_MLFQ_" + std::to_string(test_case) + ".csv");
        std::cout << "MLFQ Test " << test_case << " completed.\n";

        //Run SRTF
        SRTF_Scheduler srtf(processes_srtf.size());
        srtf.processes = processes_srtf;
        srtf.run("results_SRTF_" + std::to_string(test_case) + ".csv");
        std::cout << "SRTF Test " << test_case << " completed.\n";

        test_case++;
    }

    file.close();
    std::cout << "All test cases completed.\n";
    return 0;
}
