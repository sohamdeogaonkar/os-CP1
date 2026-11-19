#ifndef MENU_H
#define MENU_H

#include "Workload.h"
#include "GanttRenderer.h"
#include "TablePrinter.h"
#include "../schedulers/Scheduler.h"

using namespace std;

class Menu {
public:
    static vector<Process> selectWorkload() {
        cout << "\n── Workload Selection ──────────────────────────────────────────\n";
        cout << "  1. Load Preset\n  2. Enter Processes Manually\n  3. Generate Random Workload\n";
        int opt = InputUtils::readInt("Choose [1-3]: ", 1, 3);

        if (opt == 1) {
            cout << "\n  1. Preset 1\n  2. Preset 2\n  3. Preset 3\n  4. Preset 4\n  5. Preset 5\n";
            return Workload::getPreset(InputUtils::readInt("Select [1-5]: ", 1, 5));
        } else if (opt == 2) {
            return Workload::enterManual();
        } else {
            int cnt = InputUtils::readInt("Count (1-20): ", 1, 20);
            int arr = InputUtils::readInt("Max Arrival (0-50): ", 0, 50);
            int min_b = InputUtils::readInt("Min Burst (1-20): ", 1, 20);
            int max_b = InputUtils::readInt("Max Burst (" + to_string(min_b) + "-50): ", min_b, 50);
            int pri = InputUtils::readInt("Max Priority (1-10): ", 1, 10);
            return Workload::generateRandom(cnt, arr, min_b, max_b, pri);
        }
    }

    static void handleSingleCore() {
        vector<Process> workload = selectWorkload();
        Workload::printTable(workload);
        cout << "\nAlgorithms: 1. FCFS  2. SJF  3. SRTF  4. Priority (NP)  5. Priority (P)  6. RR\n";
        int algo = InputUtils::readInt("Choose [1-6]: ", 1, 6);
        int q = (algo == 6) ? InputUtils::readInt("Quantum (>= 1): ", 1, 100) : 2;

        SimulationResult res;
        if (algo == 1) res = Scheduler::runFCFS(workload);
        else if (algo == 2) res = Scheduler::runSJF(workload);
        else if (algo == 3) res = Scheduler::runSRTF(workload);
        else if (algo == 4) res = Scheduler::runPriority(workload);
        else if (algo == 5) res = Scheduler::runPriorityPreemptive(workload);
        else res = Scheduler::runRoundRobin(workload, q);

        cout << "\n── Gantt Chart ─────────────────────────────────────────────────\n";
        if (!res.core_timelines.empty()) GanttRenderer::printGantt(res.core_timelines[0]);
        TablePrinter::printTable(res);
        TablePrinter::printSummary(res);
        InputUtils::pause();
    }

    static void handleMultiCore() {
        vector<Process> workload = selectWorkload();
        Workload::printTable(workload);
        cout << "\nCores: 1. 2 Cores  2. 4 Cores  3. 8 Cores  4. Custom (1-16)\n";
        int c_opt = InputUtils::readInt("Choose [1-4]: ", 1, 4);
        int cores = (c_opt == 1) ? 2 : (c_opt == 2) ? 4 : (c_opt == 3) ? 8 : InputUtils::readInt("Cores (1-16): ", 1, 16);
        cout << "\nQueue: 1. Global Queue  2. Per-Core Queue (Work Stealing)\n";
        bool global_q = (InputUtils::readInt("Choose [1-2]: ", 1, 2) == 1);
        cout << "\nAlgorithms: 1. FCFS  2. SJF  3. SRTF  4. Priority (NP)  5. Priority (P)  6. RR\n";
        int a_idx = InputUtils::readInt("Choose [1-6]: ", 1, 6) - 1;
        int q = (a_idx == 5) ? InputUtils::readInt("Quantum (>= 1): ", 1, 100) : 2;

        SimulationResult res = Scheduler::runMultiCore(a_idx, cores, global_q, workload, q);
        GanttRenderer::printMultiCore(res.core_timelines);
        TablePrinter::printTable(res);
        TablePrinter::printSummary(res);
        InputUtils::pause();
    }

    static void handleComparison() {
        vector<Process> workload = selectWorkload();
        Workload::printTable(workload);
        int q = InputUtils::readInt("Round Robin Quantum (>= 1): ", 1, 50);
        vector<SimulationResult> results = {
            Scheduler::runFCFS(workload), Scheduler::runSJF(workload), Scheduler::runSRTF(workload),
            Scheduler::runPriority(workload), Scheduler::runPriorityPreemptive(workload), Scheduler::runRoundRobin(workload, q)
        };
        TablePrinter::printComparison(results);
        InputUtils::pause();
    }
};

#endif // MENU_H
