#ifndef SIMULATIONRESULT_H
#define SIMULATIONRESULT_H

#include "Process.h"
#include "GanttBlock.h"
#include <vector>

using namespace std;

struct SimulationResult {
    string algorithm_name;
    int num_cores = 1;
    int time_quantum = 0;

    vector<Process> processes;
    vector<vector<GanttBlock>> core_timelines;

    double avg_waiting_time = 0.0;
    double avg_turnaround_time = 0.0;
    double avg_response_time = 0.0;
    int total_time = 0;

    void compute() {
        if (processes.empty()) return;

        double sum_wt = 0.0, sum_tat = 0.0, sum_rt = 0.0;
        int max_ct = 0;

        for (auto& p : processes) {
            p.calculate();
            sum_wt += p.waiting_time;
            sum_tat += p.turnaround_time;
            sum_rt += p.response_time;
            if (p.completion_time > max_ct) max_ct = p.completion_time;
        }

        avg_waiting_time = sum_wt / processes.size();
        avg_turnaround_time = sum_tat / processes.size();
        avg_response_time = sum_rt / processes.size();
        total_time = max_ct;
    }
};

#endif // SIMULATIONRESULT_H
