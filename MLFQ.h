#ifndef MLFQ_H
#define MLFQ_H

#include <vector>
#include <queue>
#include <fstream>
#include <iostream>

struct MLFQ_Process {
    int pid, arrival, burst, remaining, start_time = -1, completion_time, waiting_time, turnaround_time;
    int queue_level = 0;
    bool started = false;
};

class MLFQ_Scheduler {
public:
    std::vector<MLFQ_Process> processes;
    int n;

    MLFQ_Scheduler(int num_processes) : n(num_processes) {
        processes.resize(n);
    }

    void inputProcesses() {
        for (int i = 0; i < n; ++i) {
            std::cout << "MLFQ - Enter arrival time and burst time for process " << i << ": ";
            std::cin >> processes[i].arrival >> processes[i].burst;
            processes[i].pid = i;
            processes[i].remaining = processes[i].burst;
        }
    }

    void run(const std::string& filename) {
        std::vector<std::queue<int>> queues(3);
        std::vector<int> quantum = {4, 8, 12};
        int time = 0, completed = 0;
        int idle_time = 0;

        while (completed < n) {
            for (int i = 0; i < n; ++i) {
                if (processes[i].arrival == time) {
                    queues[0].push(i);
                }
            }

            bool found = false;
            for (int level = 0; level < 3; ++level) {
                if (!queues[level].empty()) {
                    int idx = queues[level].front();
                    queues[level].pop();

                    int exec_time = std::min(quantum[level], processes[idx].remaining);
                    if (processes[idx].start_time == -1)
                        processes[idx].start_time = time;

                    time += exec_time;
                    processes[idx].remaining -= exec_time;

                    for (int i = 0; i < n; ++i) {
                        if (i != idx && processes[i].arrival > time - exec_time && processes[i].arrival <= time) {
                            queues[0].push(i);
                        }
                    }

                    if (processes[idx].remaining == 0) {
                        processes[idx].completion_time = time;
                        completed++;
                        processes[idx].turnaround_time = processes[idx].completion_time - processes[idx].arrival;
                        processes[idx].waiting_time = processes[idx].turnaround_time - processes[idx].burst;
                    } else {
                        if (level < 2)
                            queues[level + 1].push(idx);
                        else
                            queues[level].push(idx);
                    }

                    found = true;
                    break;
                }
            } //add to idle time
            if (!found) {
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
        file << "PID,Arrival,Burst,Waiting,Turnaround,CPU Utilization\n";
        for (auto &p : processes) {
            file << p.pid << "," << p.arrival << "," << p.burst << "," << p.waiting_time
                 << "," << p.turnaround_time << "," << cpu_utilization << "\n";
        }
        file.close();
    }
};

#endif
