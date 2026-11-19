#ifndef ROUNDROBIN_H
#define ROUNDROBIN_H

#include "../core/SimulationResult.h"
#include <deque>
#include <algorithm>

using namespace std;

class RoundRobin {
public:
    static SimulationResult run(vector<Process> procs, int quantum) {
        if (quantum <= 0) quantum = 1;
        for (auto& p : procs) p.reset();

        SimulationResult res;
        res.algorithm_name = "Round Robin (RR)";
        res.num_cores = 1;
        res.time_quantum = quantum;

        vector<GanttBlock> tl;
        int n = static_cast<int>(procs.size()), done = 0, t = 0;
        deque<int> q;
        vector<bool> enqueued(n, false);

        auto check_arrivals = [&](int cur_t) {
            for (int i = 0; i < n; ++i) {
                if (!enqueued[i] && procs[i].arrival_time <= cur_t) {
                    q.push_back(i);
                    enqueued[i] = true;
                }
            }
        };

        check_arrivals(t);

        while (done < n) {
            if (q.empty()) {
                int next_arr = -1;
                for (int i = 0; i < n; ++i) {
                    if (!procs[i].is_completed && (next_arr == -1 || procs[i].arrival_time < next_arr))
                        next_arr = procs[i].arrival_time;
                }
                if (next_arr > t) {
                    addBlock(tl, "IDLE", 0, t, next_arr, true);
                    t = next_arr;
                }
                check_arrivals(t);
                continue;
            }

            int idx = q.front();
            q.pop_front();
            Process& p = procs[idx];

            if (p.start_time == -1) p.start_time = t;
            int slice = min(quantum, p.remaining_time);
            addBlock(tl, p.id, 0, t, t + slice);
            t += slice;
            p.remaining_time -= slice;

            check_arrivals(t);

            if (p.remaining_time == 0) {
                p.completion_time = t;
                p.is_completed = true;
                done++;
            } else {
                q.push_back(idx);
            }
        }

        res.processes = procs;
        res.core_timelines.push_back(tl);
        res.compute();
        return res;
    }
};

#endif // ROUNDROBIN_H
