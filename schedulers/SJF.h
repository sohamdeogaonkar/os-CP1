#ifndef SJF_H
#define SJF_H

#include "../core/SimulationResult.h"

using namespace std;

class SJF {
public:
    static SimulationResult run(vector<Process> procs) {
        for (auto& p : procs) p.reset();
        SimulationResult res;
        res.algorithm_name = "Shortest Job First (SJF)";
        res.num_cores = 1;

        vector<GanttBlock> tl;
        int n = static_cast<int>(procs.size()), done = 0, t = 0;
        vector<bool> finished(n, false);

        while (done < n) {
            int best = -1;
            for (int i = 0; i < n; ++i) {
                if (!finished[i] && procs[i].arrival_time <= t) {
                    if (best == -1 || procs[i].burst_time < procs[best].burst_time ||
                       (procs[i].burst_time == procs[best].burst_time && procs[i].arrival_time < procs[best].arrival_time)) {
                        best = i;
                    }
                }
            }

            if (best == -1) {
                int next_arr = -1;
                for (int i = 0; i < n; ++i) {
                    if (!finished[i] && (next_arr == -1 || procs[i].arrival_time < next_arr))
                        next_arr = procs[i].arrival_time;
                }
                addBlock(tl, "IDLE", 0, t, next_arr, true);
                t = next_arr;
                continue;
            }

            Process& p = procs[best];
            if (p.start_time == -1) p.start_time = t;
            addBlock(tl, p.id, 0, t, t + p.burst_time);
            t += p.burst_time;
            p.completion_time = t;
            p.remaining_time = 0;
            p.is_completed = true;
            finished[best] = true;
            done++;
        }

        res.processes = procs;
        res.core_timelines.push_back(tl);
        res.compute();
        return res;
    }
};

#endif // SJF_H
