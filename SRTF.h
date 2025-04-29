#ifndef SRTF_H
#define SRTF_H

#include <vector>
#include <fstream>
#include <algorithm>
#include <iostream>

struct SRTF_Process {
    int pid, arrival, burst, remaining, completion, waiting, turnaround, response;
    bool started = false;
};

class SRTF_Scheduler {
public:
    std::vector<SRTF_Process> processes;
    int n;

    SRTF_Scheduler(int num_processes) : n(num_processes) {
        processes.resize(n);
    }

    void inputProcesses() {
        for (int i = 0; i < n; ++i) {
            std::cout << "SRTF - Enter arrival time and burst time for process " << i << ": ";
            std::cin >> processes[i].arrival >> processes[i].burst;
            processes[i].pid = i;
            processes[i].remaining = processes[i].burst;
        }
    }

    void run(const std::string& filename) {
        std::sort(processes.begin(), processes.end(), [](SRTF_Process a, SRTF_Process b) {
            return a.arrival < b.arrival;
        });

        int completed = 0, time = 0, idle_time = 0;

        while (completed < n) {
            int idx = -1, min_remaining = 1e9;

            //Find ready shortest remaining time
            for (int i = 0; i < n; ++i) {
                if (processes[i].arrival <= time && processes[i].remaining > 0 && processes[i].remaining < min_remaining) {
                    min_remaining = processes[i].remaining;
                    idx = i;
                }
            }

            if (idx != -1) {
                if (!processes[idx].started) {
                    processes[idx].response = time - processes[idx].arrival;
                    processes[idx].started = true;
                }

                processes[idx].remaining--;
                time++;

                if (processes[idx].remaining == 0) {
                    completed++;
                    processes[idx].completion = time;
                    processes[idx].turnaround = processes[idx].completion - processes[idx].arrival;
                    processes[idx].waiting = processes[idx].turnaround - processes[idx].burst;
                }
            } else {//Add to idle time
                idle_time++;
                time++;
            }
        }

        //CPU utilization
        double cpu_utilization = ((time - idle_time) / (double)time) * 100;

        //Write results to CSV
        writeCSV(cpu_utilization, filename);
    }

    void writeCSV(double cpu_utilization, const std::string& filename) {
        std::ofstream file(filename);
        file << "PID,Arrival,Burst,Waiting,Turnaround,Response,CPU Utilization\n";
        for (auto &p : processes) {
            file << p.pid << "," << p.arrival << "," << p.burst << "," << p.waiting
                 << "," << p.turnaround << "," << p.response << "," << cpu_utilization << "\n";
        }
        file.close();
    }
};

#endif
