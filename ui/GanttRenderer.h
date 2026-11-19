#ifndef GANTTRENDERER_H
#define GANTTRENDERER_H

#include "../core/GanttBlock.h"
#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

class GanttRenderer {
public:
    static void renderSegment(const vector<GanttBlock>& blocks) {
        if (blocks.empty()) return;

        vector<int> widths;
        for (const auto& b : blocks) {
            int w = 6 + (b.duration() > 1 ? min(6, b.duration() * 2) : 0);
            widths.push_back(w);
        }

        cout << "  +";
        for (int w : widths) cout << string(w, '-') << "+";
        cout << "\n  |";

        for (size_t i = 0; i < blocks.size(); ++i) {
            string label = blocks[i].is_idle ? "IDLE" : blocks[i].pid;
            int w = widths[i];
            int pad_left = (w - static_cast<int>(label.length())) / 2;
            int pad_right = w - static_cast<int>(label.length()) - pad_left;
            cout << string(max(0, pad_left), ' ') << label << string(max(0, pad_right), ' ') << "|";
        }
        cout << "\n  +";
        for (int w : widths) cout << string(w, '-') << "+";
        cout << "\n  ";

        string s_start = to_string(blocks[0].start_time);
        cout << s_start;
        int print_pos = static_cast<int>(s_start.length());

        for (size_t i = 0; i < blocks.size(); ++i) {
            int target = 1;
            for (size_t j = 0; j <= i; ++j) target += widths[j] + 1;
            string s_end = to_string(blocks[i].end_time);
            int sp = max(1, (target - 1) - print_pos);
            cout << string(sp, ' ') << s_end;
            print_pos += sp + static_cast<int>(s_end.length());
        }
        cout << "\n";
    }

    static void printGantt(const vector<GanttBlock>& timeline) {
        if (timeline.empty()) return;
        const size_t PER_LINE = 8;
        for (size_t i = 0; i < timeline.size(); i += PER_LINE) {
            size_t end = min(timeline.size(), i + PER_LINE);
            vector<GanttBlock> seg(timeline.begin() + i, timeline.begin() + end);
            renderSegment(seg);
            if (end < timeline.size()) cout << "  ↓ (timeline continues below)\n";
        }
    }

    static void printMultiCore(const vector<vector<GanttBlock>>& timelines) {
        for (size_t c = 0; c < timelines.size(); ++c) {
            cout << "\n  [ Core " << c << " ]\n";
            printGantt(timelines[c]);
        }
    }
};

#endif // GANTTRENDERER_H
