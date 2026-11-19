#ifndef WORKLOAD_H
#define WORKLOAD_H

#include "../core/Process.h"
#include "InputUtils.h"
#include <iostream>
#include <iomanip>
#include <random>

using namespace std;

class Workload {
public:
    static vector<Process> getPreset(int id) {
        switch (id) {
            case 1:
                return {Process("P1", 0, 8, 3), Process("P2", 1, 4, 1), Process("P3", 2, 9, 4), Process("P4", 3, 5, 2), Process("P5", 4, 2, 5)};
            case 2:
                return {Process("P1", 0, 24, 3), Process("P2", 1, 3, 2), Process("P3", 2, 3, 1), Process("P4", 3, 2, 4), Process("P5", 4, 4, 2)};
            case 3:
                return {Process("P1", 0, 10, 3), Process("P2", 2, 3, 1), Process("P3", 4, 2, 4), Process("P4", 6, 4, 2), Process("P5", 8, 1, 1)};
            case 4:
                return {Process("P1", 0, 12, 2), Process("P2", 0, 8, 1), Process("P3", 1, 15, 3), Process("P4", 2, 6, 2),
                        Process("P5", 3, 9, 4), Process("P6", 4, 4, 1), Process("P7", 5, 10, 3), Process("P8", 6, 5, 2)};
            case 5:
                return {Process("P1", 0, 6, 2), Process("P2", 0, 8, 4), Process("P3", 0, 3, 1), Process("P4", 0, 7, 3), Process("P5", 0, 4, 5)};
            default:
                return getPreset(1);
        }
    }

    static vector<Process> generateRandom(int count, int max_arr, int min_bst, int max_bst, int max_pri) {
        vector<Process> procs;
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> d_arr(0, max_arr);
        uniform_int_distribution<> d_bst(min_bst, max_bst);
        uniform_int_distribution<> d_pri(1, max_pri);

        for (int i = 1; i <= count; ++i) {
            procs.emplace_back("P" + to_string(i), d_arr(gen), d_bst(gen), d_pri(gen));
        }
        return procs;
    }

    static vector<Process> enterManual() {
        int n = InputUtils::readInt("Enter process count (1-20): ", 1, 20);
        vector<Process> procs;

        for (int i = 1; i <= n; ++i) {
            cout << "\n[ Process " << i << " ]\n";
            cout << "  Process ID (default P" << i << "): ";
            string pid;
            getline(cin, pid);
            if (pid.empty()) pid = "P" + to_string(i);

            int arr = InputUtils::readInt("  Arrival Time (>= 0): ", 0, 1000);
            int bst = InputUtils::readInt("  Burst Time   (>= 1): ", 1, 1000);
            int pri = InputUtils::readInt("  Priority     (1-99): ", 1, 99);
            procs.emplace_back(pid, arr, bst, pri);
        }
        return procs;
    }

    static void printTable(const vector<Process>& procs) {
        cout << "\n  ┌────────────┬──────────────┬────────────┬──────────┐\n";
        cout << "  │ Process ID │ Arrival Time │ Burst Time │ Priority │\n";
        cout << "  ├────────────┼──────────────┼────────────┼──────────┤\n";
        for (const auto& p : procs) {
            cout << " │ " << left << setw(10) << p.id
                 << " │ " << right << setw(12) << p.arrival_time
                 << " │ " << right << setw(10) << p.burst_time
                 << " │ " << right << setw(8) << p.priority << " │\n";
        }
        cout << "  └────────────┴──────────────┴────────────┴──────────┘\n";
        cout << "  Total: " << procs.size() << " processes | Priority: Lower number = Higher priority\n";
    }
};

#endif // WORKLOAD_H
