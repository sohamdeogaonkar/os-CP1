#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "FCFS.h"
#include "SJF.h"
#include "SRTF.h"
#include "PriorityNP.h"
#include "PriorityP.h"
#include "RoundRobin.h"
#include "GlobalQueue.h"
#include "PerCoreQueue.h"

class Scheduler {
public:
    static SimulationResult runFCFS(vector<Process> p) { return FCFS::run(p); }
    static SimulationResult runSJF(vector<Process> p) { return SJF::run(p); }
    static SimulationResult runSRTF(vector<Process> p) { return SRTF::run(p); }
    static SimulationResult runPriority(vector<Process> p) { return PriorityNP::run(p); }
    static SimulationResult runPriorityPreemptive(vector<Process> p) { return PriorityP::run(p); }
    static SimulationResult runRoundRobin(vector<Process> p, int q) { return RoundRobin::run(p, q); }

    static SimulationResult runMultiCore(int algo_idx, int cores, bool global_q, vector<Process> procs, int quantum = 2) {
        if (cores <= 0) cores = 1;
        for (auto& p : procs) p.reset();

        SimulationResult res;
        string names[] = {"FCFS", "SJF", "SRTF", "Priority (NP)", "Priority (Preemptive)", "Round Robin"};
        string mode = global_q ? "Global Queue" : "Per-Core Queues";

        if (global_q) res = GlobalQueue::run(algo_idx, cores, procs, quantum);
        else res = PerCoreQueue::run(algo_idx, cores, procs, quantum);

        res.algorithm_name = names[algo_idx] + " (" + mode + ")";
        res.num_cores = cores;
        res.time_quantum = quantum;
        res.compute();
        return res;
    }
};

#endif // SCHEDULER_H
