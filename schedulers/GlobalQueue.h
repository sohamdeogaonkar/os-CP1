#ifndef GLOBALQUEUE_H
#define GLOBALQUEUE_H

#include "../core/SimulationResult.h"
#include <deque>
#include <algorithm>

using namespace std;

class GlobalQueue {
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

        struct CoreState { int running = -1, slice = 0; };
        vector<CoreState> cores(num_cores);
        deque<int> ready;
        vector<bool> in_sys(n, false);

        while (done < n) {
            for (int i = 0; i < n; ++i) {
                if (!procs[i].is_completed && !in_sys[i] && procs[i].arrival_time <= t) {
                    ready.push_back(i); in_sys[i] = true;
                }
            }
            if (algo_idx != 5 && ready.size() > 1) sort(ready.begin(), ready.end(), is_better);

            if ((algo_idx == 2 || algo_idx == 4) && !ready.empty()) {
                for (int c = 0; c < num_cores && !ready.empty(); ++c) {
                    if (cores[c].running != -1) {
                        int top = ready.front(), cur = cores[c].running;
                        bool prem = (algo_idx == 2) ? (procs[top].remaining_time < procs[cur].remaining_time) : (procs[top].priority < procs[cur].priority);
                        if (prem) {
                            ready.pop_front(); ready.push_back(cur);
                            cores[c].running = top; cores[c].slice = 0;
                            if (procs[top].start_time == -1) procs[top].start_time = t;
                            procs[top].assigned_core = c;
                            sort(ready.begin(), ready.end(), is_better);
                        }
                    }
                }
            }

            for (int c = 0; c < num_cores; ++c) {
                if (cores[c].running == -1 && !ready.empty()) {
                    int nxt = ready.front(); ready.pop_front();
                    cores[c].running = nxt; cores[c].slice = 0;
                    if (procs[nxt].start_time == -1) procs[nxt].start_time = t;
                    procs[nxt].assigned_core = c;
                }
            }

            for (int c = 0; c < num_cores; ++c) {
                if (cores[c].running != -1) {
                    int idx = cores[c].running;
                    Process& p = procs[idx];
                    addBlock(res.core_timelines[c], p.id, c, t, t + 1, false);
                    p.remaining_time--; cores[c].slice++;

                    if (p.remaining_time == 0) {
                        p.completion_time = t + 1; p.is_completed = true; done++;
                        cores[c].running = -1;
                    } else if (algo_idx == 5 && cores[c].slice >= quantum) {
                        cores[c].running = -1; cores[c].slice = 0;
                        for (int i = 0; i < n; ++i) {
                            if (!procs[i].is_completed && !in_sys[i] && procs[i].arrival_time <= t + 1) {
                                ready.push_back(i); in_sys[i] = true;
                            }
                        }
                        ready.push_back(idx);
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

#endif // GLOBALQUEUE_H
