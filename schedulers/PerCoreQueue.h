#ifndef PERCOREQUEUE_H
#define PERCOREQUEUE_H

#include "../core/SimulationResult.h"
#include <deque>
#include <algorithm>

using namespace std;

class PerCoreQueue {
public:
    static SimulationResult run(int algo_idx, int num_cores, vector<Process>& procs, int quantum) {
        SimulationResult res;
        res.core_timelines.resize(num_cores);
        int n = static_cast<int>(procs.size()), done = 0, t = 0;

        auto is_better = [&](int a, int b) {
            if (algo_idx == 1 && procs[a].burst_time != procs[b].burst_time) return procs[a].burst_time < procs[b].burst_time;
            if (algo_idx == 2 && procs[a].remaining_time != procs[b].remaining_time) return procs[a].remaining_time < procs[b].remaining_time;
            if ((algo_idx == 3 || algo_idx == 4) && procs[a].priority != procs[b].priority) return procs[a].priority < procs[b].priority;
            return procs[a].arrival_time < procs[b].arrival_time;
        };

        struct CoreQ { int running = -1, slice = 0; deque<int> local; };
        vector<CoreQ> cores(num_cores);
        vector<int> load(num_cores, 0);
        vector<bool> assigned(n, false);

        while (done < n) {
            for (int i = 0; i < n; ++i) {
                if (!assigned[i] && procs[i].arrival_time <= t) {
                    int min_c = 0;
                    for (int c = 1; c < num_cores; ++c) if (load[c] < load[min_c]) min_c = c;
                    cores[min_c].local.push_back(i); load[min_c] += procs[i].burst_time;
                    assigned[i] = true; procs[i].assigned_core = min_c;
                }
            }

            if (algo_idx != 5) {
                for (int c = 0; c < num_cores; ++c) {
                    if (cores[c].local.size() > 1) sort(cores[c].local.begin(), cores[c].local.end(), is_better);
                }
            }

            // Work Stealing
            for (int c = 0; c < num_cores; ++c) {
                if (cores[c].running == -1 && cores[c].local.empty()) {
                    int max_c = -1; size_t max_len = 1;
                    for (int o = 0; o < num_cores; ++o) {
                        if (o != c && cores[o].local.size() > max_len) { max_len = cores[o].local.size(); max_c = o; }
                    }
                    if (max_c != -1 && !cores[max_c].local.empty()) {
                        int stolen = cores[max_c].local.back(); cores[max_c].local.pop_back();
                        cores[c].local.push_back(stolen);
                        load[max_c] -= procs[stolen].burst_time; load[c] += procs[stolen].burst_time;
                        procs[stolen].assigned_core = c;
                    }
                }
            }

            for (int c = 0; c < num_cores; ++c) {
                if (cores[c].running == -1 && !cores[c].local.empty()) {
                    int nxt = cores[c].local.front(); cores[c].local.pop_front();
                    cores[c].running = nxt; cores[c].slice = 0;
                    if (procs[nxt].start_time == -1) procs[nxt].start_time = t;
                }
            }

            for (int c = 0; c < num_cores; ++c) {
                if (cores[c].running != -1) {
                    int idx = cores[c].running; Process& p = procs[idx];
                    addBlock(res.core_timelines[c], p.id, c, t, t + 1, false);
                    p.remaining_time--; cores[c].slice++;
                    if (p.remaining_time == 0) {
                        p.completion_time = t + 1; p.is_completed = true; done++;
                        load[c] -= p.burst_time; cores[c].running = -1;
                    } else if (algo_idx == 5 && cores[c].slice >= quantum) {
                        cores[c].running = -1; cores[c].slice = 0; cores[c].local.push_back(idx);
                    }
                } else {
                    addBlock(res.core_timelines[c], "IDLE", c, t, t + 1, true);
                }
            }
            t++;
        }
        res.processes = procs;
        return res;
    }
};

#endif // PERCOREQUEUE_H
