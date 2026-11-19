#ifndef GANTTBLOCK_H
#define GANTTBLOCK_H

#include <string>
#include <vector>

using namespace std;

struct GanttBlock {
    string pid;
    int core_id = 0;
    int start_time = 0;
    int end_time = 0;
    bool is_idle = false;

    GanttBlock(string p, int core, int start, int end, bool idle = false)
        : pid(move(p)), core_id(core), start_time(start), end_time(end), is_idle(idle) {}

    int duration() const {
        return end_time - start_time;
    }
};

inline void addBlock(vector<GanttBlock>& tl, const string& pid, int core, int start, int end, bool idle = false) {
    if (start >= end) return;
    if (!tl.empty() && tl.back().pid == pid && tl.back().is_idle == idle && tl.back().end_time == start) {
        tl.back().end_time = end;
    } else {
        tl.emplace_back(pid, core, start, end, idle);
    }
}

#endif // GANTTBLOCK_H
