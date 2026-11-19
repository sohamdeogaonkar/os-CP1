#ifndef PROCESS_H
#define PROCESS_H

#include <string>

using namespace std;

struct Process {
    string id;
    int arrival_time = 0;
    int burst_time = 0;
    int priority = 1;

    int remaining_time = 0;
    int start_time = -1;
    int completion_time = -1;
    int waiting_time = 0;
    int turnaround_time = 0;
    int response_time = -1;
    int assigned_core = -1;
    bool is_completed = false;

    Process() = default;
    Process(string pid, int arr, int burst, int prio = 1)
        : id(move(pid)), arrival_time(arr), burst_time(burst),
          priority(prio), remaining_time(burst) {}

    void reset() {
        remaining_time = burst_time;
        start_time = -1;
        completion_time = -1;
        waiting_time = 0;
        turnaround_time = 0;
        response_time = -1;
        assigned_core = -1;
        is_completed = false;
    }

    void calculate() {
        if (completion_time >= 0) {
            turnaround_time = completion_time - arrival_time;
            waiting_time = turnaround_time - burst_time;
            if (waiting_time < 0) waiting_time = 0;
            response_time = (start_time >= 0) ? (start_time - arrival_time) : 0;
            if (response_time < 0) response_time = 0;
        }
    }
};

#endif // PROCESS_H
