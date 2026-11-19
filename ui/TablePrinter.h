#ifndef TABLEPRINTER_H
#define TABLEPRINTER_H

#include "../core/SimulationResult.h"
#include <iostream>
#include <iomanip>

using namespace std;

class TablePrinter {
public:
    static void printTable(const SimulationResult& res) {
        bool multi = (res.num_cores > 1);
        cout << "\n  ┌────────┬────────┬───────┬──────" << (multi ? "┬──────" : "") << "┬───────┬────────┬────────┬────────┬────────┐\n";
        cout << "  │  PID   │   AT   │  BT   │ Prio " << (multi ? "│ Core " : "") << "│ Start │ Compl. │  TAT   │   WT   │   RT   │\n";
        cout << "  ├────────┼────────┼───────┼──────" << (multi ? "┼──────" : "") << "┼───────┼────────┼────────┼────────┼────────┤\n";

        for (const auto& p : res.processes) {
            cout << "  │ " << left << setw(6) << p.id
                 << " │ " << right << setw(6) << p.arrival_time
                 << " │ " << right << setw(5) << p.burst_time
                 << " │ " << right << setw(4) << p.priority;
            if (multi) {
                string c_str = (p.assigned_core >= 0) ? ("C" + to_string(p.assigned_core)) : "-";
                cout << " │ " << right << setw(4) << c_str;
            }
            cout << " │ " << right << setw(5) << p.start_time
                 << " │ " << right << setw(6) << p.completion_time
                 << " │ " << right << setw(6) << p.turnaround_time
                 << " │ " << right << setw(6) << p.waiting_time
                 << " │ " << right << setw(6) << p.response_time << " │\n";
        }
        cout << "  └────────┴────────┴───────┴──────" << (multi ? "┴──────" : "") << "┴───────┴────────┴────────┴────────┴────────┘\n";
        cout << "  (AT=Arrival, BT=Burst, TAT=Turnaround, WT=Waiting, RT=Response Time)\n";
    }

    static void printSummary(const SimulationResult& res) {
        cout << "\n── Performance Summary ──────────────────────────────────────────\n";
        cout << fixed << setprecision(2);
        cout << "  • Algorithm:                  " << res.algorithm_name << "\n";
        if (res.time_quantum > 0) cout << "  • Time Quantum:               " << res.time_quantum << " units\n";
        cout << "  • Number of Cores:            " << res.num_cores << "\n";
        cout << "  • Total Makespan (Time):      " << res.total_time << " units\n";
        cout << "  • Average Turnaround Time:    " << res.avg_turnaround_time << " units\n";
        cout << "  • Average Waiting Time:       " << res.avg_waiting_time << " units\n";
        cout << "  • Average Response Time:      " << res.avg_response_time << " units\n";
    }

    static void printComparison(const vector<SimulationResult>& results) {
        cout << "\n╔══════════════════════════════════════════════════════════════════════════════╗\n";
        cout << "║ ALGORITHM COMPARISON MATRIX                                                  ║\n";
        cout << "╚══════════════════════════════════════════════════════════════════════════════╝\n";
        cout << "  ┌────────────────────────────────┬─────────┬─────────┬─────────┬──────────┐\n";
        cout << "  │ Algorithm                      │ Avg WT  │ Avg TAT │ Avg RT  │ Makespan │\n";
        cout << "  ├────────────────────────────────┼─────────┼─────────┼─────────┼──────────┤\n";

        for (const auto& r : results) {
            cout << "  │ " << left << setw(30) << r.algorithm_name.substr(0, 30)
                 << " │ " << right << fixed << setprecision(2) << setw(7) << r.avg_waiting_time
                 << " │ " << right << setw(7) << r.avg_turnaround_time
                 << " │ " << right << setw(7) << r.avg_response_time
                 << " │ " << right << setw(8) << r.total_time << " │\n";
        }
        cout << "  └────────────────────────────────┴─────────┴─────────┴─────────┴──────────┘\n";
    }
};

#endif // TABLEPRINTER_H
