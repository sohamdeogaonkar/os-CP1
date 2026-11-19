#ifndef INPUTUTILS_H
#define INPUTUTILS_H

#include <iostream>
#include <string>

using namespace std;

class InputUtils {
public:
    static int readInt(const string& prompt, int min_val, int max_val) {
        while (true) {
            cout << prompt;
            string line;
            if (!getline(cin, line)) return min_val;

            size_t first = line.find_first_not_of(" \t\r\n");
            if (first == string::npos) continue;
            size_t last = line.find_last_not_of(" \t\r\n");
            line = line.substr(first, last - first + 1);

            try {
                size_t idx = 0;
                int val = stoi(line, &idx);
                if (idx == line.size() && val >= min_val && val <= max_val) {
                    return val;
                }
            } catch (...) {}
            cout << "  [!] Please enter an integer between " << min_val << " and " << max_val << ".\n";
        }
    }

    static bool readBool(const string& prompt) {
        while (true) {
            cout << prompt << " (y/n): ";
            string line;
            if (!getline(cin, line)) return false;
            if (line == "y" || line == "Y" || line == "yes") return true;
            if (line == "n" || line == "N" || line == "no") return false;
            cout << "  [!] Please answer with 'y' or 'n'.\n";
        }
    }

    static void pause() {
        cout << "\nPress Enter to continue...";
        string line;
        getline(cin, line);
    }
};

#endif // INPUTUTILS_H
