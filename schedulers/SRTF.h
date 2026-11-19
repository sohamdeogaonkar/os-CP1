#ifndef SRTF_H
#define SRTF_H

#include "../core/SimulationResult.h"

using namespace std;

class SRTF {
public:
    static SimulationResult run(vector<Process> procs) {
        for (auto& p : procs) p.reset();
        SimulationResult res;
        res.algorithm_name = "Shortest Remaining Time First (SRTF)";
        res.num_cores = 1;

        vector<GanttBlock> tl;
        int n = static_cast<int>(procs.size()), done = 0, t = 0;

        while (done < n) {
            int best = -1;
            for (int i = 0; i < n; ++i) {
                if (!procs[i].is_completed && procs[i].arrival_time <= t) {
                    if (best == -1 || procs[i].remaining_time < procs[best].remaining_time ||
                       (procs[i].remaining_time == procs[best].remaining_time && procs[i].arrival_time < procs[best].arrival_time)) {
                        best = i;
                    }
                }
            }

            if (best == -1) {
                int next_arr = -1;
                for (int i = 0; i < n; ++i) {
                    if (!procs[i].is_completed && (next_arr == -1 || procs[i].arrival_time < next_arr))
                        next_arr = procs[i].arrival_time;
                }
                addBlock(tl, "IDLE", 0, t, next_arr, true);
                t = next_arr;
                continue;
            }

            Process& p = procs[best];
            if (p.start_time == -1) p.start_time = t;

            int step = p.remaining_time;
            for (int i = 0; i < n; ++i) {
                if (!procs[i].is_completed && procs[i].arrival_time > t) {
                    int delta = procs[i].arrival_time - t;
                    if (delta < step && procs[i].remaining_time < (p.remaining_time - delta)) {
                        step = delta;
                    }
                }
            }
            if (step <= 0) step = 1;

            addBlock(tl, p.id, 0, t, t + step);
            t += step;
            p.remaining_time -= step;

            if (p.remaining_time == 0) {
                p.completion_time = t;
                p.is_completed = true;
                done++;
            }
        }

        res.processes = procs;
        res.core_timelines.push_back(tl);
        res.compute();
        return res;
    }
};

#endif // SRTF_H
