//Name: Rawda Raafat Ramadan
//ID: 20231067
//Section: S8
//this sheet problem 5
//GitHub link: https://github.com/Marria-m/Diff.-Problems.
//Gmail: 20231067@stud.fci-cu.edu.eg

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <string>

using namespace std;

// Function to remove commas from memory usage string
int parse_memory_usage(const string& memory_usage_string) {
    string cleaned = memory_usage_string;
    cleaned.erase(remove(cleaned.begin(), cleaned.end(), ','), cleaned.end());  // Remove commas
    return stoi(cleaned);  // Convert to integer
}

class Process {
public:
    string name;
    int PID;
    string session_name;
    int session_number;
    int memory_usage;

    Process(const string& n, int p, const string& s, int sn, int m)
            : name(n), PID(p), session_name(s), session_number(sn), memory_usage(m) {}

    void display() const {
        cout << left << setw(20) << name
             << setw(10) << PID
             << setw(15) << session_name
             << setw(10) << session_number
             << setw(10) << memory_usage << " K\n";
    }
};

class ProcessList {
private:
    vector<Process> processes;

public:
    void load_from_file(const string& filename) {
        ifstream file(filename);
        string line;
        bool header_skipped = false;

        while (getline(file, line)) {
            if (!header_skipped) {
                // Skip the headers
                static int header_lines = 3;
                if (--header_lines <= 0) {
                    header_skipped = true;
                }
                continue;
            }

            istringstream iss(line);
            string name, session_name, memory_usage_string;
            int pid, session_number, memory_usage;

            if (!(iss >> name >> pid >> session_name >> session_number >> memory_usage_string))
                continue;

            // Remove "K" and convert memory usage to an integer
            if (!memory_usage_string.empty() && memory_usage_string.back() == 'K') {
                memory_usage_string.pop_back();
            }
            memory_usage = parse_memory_usage(memory_usage_string);

            processes.emplace_back(name, pid, session_name, session_number, memory_usage);
        }
    }

    void display() const {
        cout << left << setw(20) << "Image Name"
             << setw(10) << "PID"
             << setw(15) << "Session Name"
             << setw(10) << "Session#"
             << setw(10) << "Mem Usage\n";
        cout << string(65, '-') << "\n";

        for (const auto& process : processes) {
            process.display();
        }
    }

    void sort_by_name() {
        sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
            return a.name < b.name;
        });
    }

    void sort_by_PID() {
        sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
            return a.PID < b.PID;
        });
    }

    void sort_by_memory_usage() {
        sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
            return a.memory_usage < b.memory_usage;
        });
    }
};

void showMenu() {
    cout << "\n--- Task Manager ---\n";
    cout << "1. Display the original file content\n";
    cout << "2. Display processes sorted by Name\n";
    cout << "3. Display processes sorted by PID\n";
    cout << "4. Display processes sorted by Memory Usage\n";
    cout << "5. Exit\n";
    cout << "Enter your choice: ";
}

int main() {
    // Fetch the task list and save to a file
    system("tasklist > ./processes.txt");

    ProcessList processes_list;
    processes_list.load_from_file("processes.txt");

    int choice;
    do {
        showMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "\nOriginal File Content:\n";
                processes_list.display();
                break;
            case 2:
                processes_list.sort_by_name();
                cout << "\nProcesses Sorted by Name:\n";
                processes_list.display();
                break;
            case 3:
                processes_list.sort_by_PID();
                cout << "\nProcesses Sorted by PID:\n";
                processes_list.display();
                break;
            case 4:
                processes_list.sort_by_memory_usage();
                cout << "\nProcesses Sorted by Memory Usage:\n";
                processes_list.display();
                break;
            case 5:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 5);

    return 0;
}
