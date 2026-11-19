#include <iostream>
#include <vector>
#include "ui/InputUtils.h"
#include "ui/Menu.h"

using namespace std;

int main() {
    bool running = true;

    while (running) {
        cout << "\n╔══════════════════════════════════════════════════════════════════════════════╗\n";
        cout << "║ CoreSim – CPU Scheduling Simulator                                           ║\n";
        cout << "╚══════════════════════════════════════════════════════════════════════════════╝\n";
        cout << "  1. Single-Core Scheduling\n";
        cout << "  2. Multi-Core Scheduling\n";
        cout << "  3. Algorithm Comparison\n";
        cout << "  4. Exit\n";

        int choice = InputUtils::readInt("Select [1-4]: ", 1, 4);
        if (choice == 1) Menu::handleSingleCore();
        else if (choice == 2) Menu::handleMultiCore();
        else if (choice == 3) Menu::handleComparison();
        else if (choice == 4) {
            cout << "\nExiting CoreSim. Goodbye!\n";
            running = false;
        }
    }
    return 0;
}
