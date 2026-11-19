# CoreSim – CPU Scheduling Simulator (C++17)

A modular, clean, and interactive terminal CPU scheduling simulator for Operating Systems course projects.

---

## Direct Compilation (No Makefile Needed!)

Compile and run directly with one standard `g++` command:

```bash
g++ -std=c++17 main.cpp -o coresim
./coresim
```

No Makefile, CMake, or external dependencies are required. Every single source file in the project is strictly **under 100 lines**!

---

## Modular Architecture & Folder Structure

```
CP1/
├── main.cpp                     # Clean entry point & controller loop (30 lines)
├── README.md                    # Project documentation
├── core/                        # Data models and structures
│   ├── Process.h                # Process model & metric calculations (50 lines)
│   ├── GanttBlock.h             # Timeline execution block & merger (33 lines)
│   └── SimulationResult.h       # Simulation outcome & summary metrics (44 lines)
├── schedulers/                  # Individual scheduling algorithms
│   ├── FCFS.h                   # First-Come First-Served (59 lines)
│   ├── SJF.h                    # Shortest Job First - Non-Preemptive (60 lines)
│   ├── SRTF.h                   # Shortest Remaining Time First (73 lines)
│   ├── PriorityNP.h             # Priority Non-Preemptive (60 lines)
│   ├── PriorityP.h              # Priority Preemptive (73 lines)
│   ├── RoundRobin.h             # Round Robin with quantum Q (80 lines)
│   ├── GlobalQueue.h            # Multi-Core Global Shared Queue (92 lines)
│   ├── PerCoreQueue.h           # Multi-Core Per-Core Queues with Work Stealing (91 lines)
│   └── Scheduler.h              # Unified scheduler facade (41 lines)
└── ui/                          # Presentation & User Interface
    ├── InputUtils.h             # Crash-proof input validation & pause (51 lines)
    ├── GanttRenderer.h          # Formatted ASCII Gantt charts with line wrapping (71 lines)
    ├── TablePrinter.h           # Per-process table, summary, & comparison matrix (68 lines)
    ├── Workload.h               # 5 presets, manual entry, & random generator (80 lines)
    └── Menu.h                   # Submenu routers for single, multi, and comparison (87 lines)
```

---

## File Overview

### Core Data Models (`core/`)
- **[Process.h](file:///home/soham/all-projects/clg/OS/CP1/core/Process.h)**: Defines `Process` struct (`id`, `arrival_time`, `burst_time`, `priority`, `remaining_time`, `start_time`, `completion_time`, `waiting_time`, `turnaround_time`, `response_time`).
- **[GanttBlock.h](file:///home/soham/all-projects/clg/OS/CP1/core/GanttBlock.h)**: Defines `GanttBlock` struct and `addBlock()` helper that merges adjacent continuous time slots.
- **[SimulationResult.h](file:///home/soham/all-projects/clg/OS/CP1/core/SimulationResult.h)**: Aggregates process outcomes, core timelines, makespan, throughput, utilization, and average metrics.

### Scheduling Algorithms (`schedulers/`)
- **[FCFS.h](file:///home/soham/all-projects/clg/OS/CP1/schedulers/FCFS.h)**: First-Come First-Served single-core algorithm.
- **[SJF.h](file:///home/soham/all-projects/clg/OS/CP1/schedulers/SJF.h)**: Shortest Job First (Non-Preemptive).
- **[SRTF.h](file:///home/soham/all-projects/clg/OS/CP1/schedulers/SRTF.h)**: Shortest Remaining Time First (Preemptive SJF).
- **[PriorityNP.h](file:///home/soham/all-projects/clg/OS/CP1/schedulers/PriorityNP.h)**: Priority scheduling (Non-Preemptive).
- **[PriorityP.h](file:///home/soham/all-projects/clg/OS/CP1/schedulers/PriorityP.h)**: Priority scheduling (Preemptive).
- **[RoundRobin.h](file:///home/soham/all-projects/clg/OS/CP1/schedulers/RoundRobin.h)**: Round Robin with configurable quantum $Q$.
- **[GlobalQueue.h](file:///home/soham/all-projects/clg/OS/CP1/schedulers/GlobalQueue.h)**: Multi-core single shared ready queue.
- **[PerCoreQueue.h](file:///home/soham/all-projects/clg/OS/CP1/schedulers/PerCoreQueue.h)**: Multi-core distributed per-core queues with dynamic Work Stealing (process migration).
- **[Scheduler.h](file:///home/soham/all-projects/clg/OS/CP1/schedulers/Scheduler.h)**: Unified facade dispatching calls to specific schedulers.

### Terminal UI & Rendering (`ui/`)
- **[InputUtils.h](file:///home/soham/all-projects/clg/OS/CP1/ui/InputUtils.h)**: Robust integer and boolean parsing with range enforcement.
- **[GanttRenderer.h](file:///home/soham/all-projects/clg/OS/CP1/ui/GanttRenderer.h)**: Clean ASCII Gantt charts with wrapping and multi-core stacked views.
- **[TablePrinter.h](file:///home/soham/all-projects/clg/OS/CP1/ui/TablePrinter.h)**: Detailed metrics breakdown table and side-by-side comparison matrix.
- **[Workload.h](file:///home/soham/all-projects/clg/OS/CP1/ui/Workload.h)**: Built-in benchmark presets, interactive manual input, and random workload generator.
- **[Menu.h](file:///home/soham/all-projects/clg/OS/CP1/ui/Menu.h)**: User menu routing for Single-Core, Multi-Core, and Algorithm Comparison.

### Entry Point
- **[main.cpp](file:///home/soham/all-projects/clg/OS/CP1/main.cpp)**: Clean 30-line application entry loop (Options 1–4).

---

## Supported Features

1. **Algorithms**:
   - First-Come First-Served (FCFS)
   - Shortest Job First (SJF – Non-Preemptive)
   - Shortest Remaining Time First (SRTF – Preemptive SJF)
   - Priority Scheduling (Non-Preemptive & Preemptive; lower number = higher priority)
   - Round Robin (configurable quantum $Q$)

2. **Single-Core & Multi-Core Execution**:
   - Single-core timeline execution with idle CPU detection.
   - Multi-core simulation on 2, 4, 8, or custom core counts (1–16).
   - Global Shared Queue vs. Per-Core Queues with dynamic Work Stealing.

3. **ASCII Gantt Charts**:
   - Clean terminal Gantt charts showing start/end markers and idle blocks (`IDLE`).
   - Stacked timelines for multi-core processors.

4. **Algorithm Comparison Matrix**:
   - Runs all algorithms on the same workload.
   - Displays side-by-side comparison of Average Waiting Time, Turnaround Time, Response Time, and Makespan.

5. **Workload Presets & Input Validation**:
   - 5 built-in presets (Classic Balanced, Convoy Effect, Preemption Showcase, Multi-Core Benchmark, Simultaneous Arrivals).
   - Interactive manual process entry.
   - Random workload generator.
   - Crash-proof input parsing.
