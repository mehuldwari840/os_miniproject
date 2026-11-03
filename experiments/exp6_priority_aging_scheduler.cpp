#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

using namespace std;

// One SomaiyaSat downlink task = one OS process
struct Process {
    string pid;
    string name;
    int arrival;
    int burst;
    int priority; // lower number = higher priority
    int remaining = 0;
    int effPriority = 0; // priority after aging
    int waitedSinceAging = 0;
    int startTime = -1;
    int completionTime = 0;
    int waitingTime = 0;
    int turnaroundTime = 0;
    int responseTime = 0;
};

struct GanttBlock {
    string pid;
    int start;
    int end;
};

const int AGING_STEP = 3;  // every 3 s of waiting, priority improves by 1
const int AGING_FLOOR = 2; // aging never lifts a task above priority 2 (TT&C = 1 stays on top)

// Preemptive Priority Scheduling (optionally with Aging)
vector<GanttBlock> priorityScheduling(vector<Process>& p, bool aging) {
    int n = p.size(), time = 0, completed = 0;
    for (auto& x : p) {
        x.remaining = x.burst;
        x.effPriority = x.priority;
        x.waitedSinceAging = 0;
        x.startTime = -1;
        x.completionTime = 0;
        x.waitingTime = 0;
        x.turnaroundTime = 0;
        x.responseTime = 0;
    }

    vector<GanttBlock> gantt;
    string current = "";

    while (completed < n) {
        // Processes that have arrived and are not finished
        int sel = -1;
        for (int i = 0; i < n; i++) {
            if (p[i].arrival > time || p[i].remaining == 0)
                continue;
            // Highest priority first; tie -> earlier arrival
            if (sel == -1 || p[i].effPriority < p[sel].effPriority ||
               (p[i].effPriority == p[sel].effPriority && p[i].arrival < p[sel].arrival)) {
                sel = i;
            }
        }

        // CPU idle
        if (sel == -1) {
            if (current == "IDLE") {
                gantt.back().end++;
            } else {
                gantt.push_back({"IDLE", time, time + 1});
                current = "IDLE";
            }
            time++;
            continue;
        }

        // First time the process gets the CPU
        if (p[sel].startTime == -1) {
            p[sel].startTime = time;
            p[sel].responseTime = time - p[sel].arrival;
        }

        // Record Gantt execution
        if (current != p[sel].pid) {
            gantt.push_back({p[sel].pid, time, time + 1});
            current = p[sel].pid;
        } else {
            gantt.back().end++;
        }

        // Execute for one time unit
        p[sel].remaining--;
        time++;

        // Process completed
        if (p[sel].remaining == 0) {
            p[sel].completionTime = time;
            p[sel].turnaroundTime = p[sel].completionTime - p[sel].arrival;
            p[sel].waitingTime = p[sel].turnaroundTime - p[sel].burst;
            completed++;
        }

        // Aging: every waiting process gets closer to the CPU
        for (int i = 0; i < n; i++) {
            if (i == sel || p[i].arrival >= time || p[i].remaining == 0)
                continue;
            p[i].waitedSinceAging++;
            if (aging && p[i].waitedSinceAging % AGING_STEP == 0 && p[i].effPriority > AGING_FLOOR) {
                p[i].effPriority--;
            }
        }
    }
    return gantt;
}

void printReport(const string& title, vector<Process> p, bool aging) {
    vector<GanttBlock> gantt = priorityScheduling(p, aging);

    cout << "\n" << string(70, '=') << "\n";
    cout << "  " << title << " | AGING: " << (aging ? "ON" : "OFF") << "\n";
    cout << string(70, '=') << "\n";

    // Gantt chart
    cout << "\nGANTT CHART\n" << string(70, '-') << "\n";
    for (auto& g : gantt) cout << "| " << setw(4) << left << g.pid << " ";
    cout << "|\n";
    for (auto& g : gantt) cout << setw(7) << left << g.start;
    cout << gantt.back().end << "\n";

    // Process performance table
    cout << "\nPROCESS PERFORMANCE\n" << string(76, '-') << "\n";
    cout << left << setw(6) << "PID"
         << setw(18) << "Downlink Task"
         << setw(5) << "AT"
         << setw(5) << "BT"
         << setw(10) << "Priority"
         << setw(6) << "CT"
         << setw(6) << "WT"
         << setw(6) << "TAT"
         << setw(5) << "RT" << "\n";
    cout << string(76, '-') << "\n";

    double totalWT = 0, totalTAT = 0, totalRT = 0;
    int maxWT = 0;
    string maxPid;

    for (auto& x : p) {
        totalWT += x.waitingTime;
        totalTAT += x.turnaroundTime;
        totalRT += x.responseTime;
        if (x.waitingTime > maxWT) {
            maxWT = x.waitingTime;
            maxPid = x.pid;
        }
        cout << left << setw(6) << x.pid
             << setw(18) << x.name
             << setw(5) << x.arrival
             << setw(5) << x.burst
             << setw(10) << x.priority
             << setw(6) << x.completionTime
             << setw(6) << x.waitingTime
             << setw(6) << x.turnaroundTime
             << setw(5) << x.responseTime << "\n";
    }

    int n = p.size();
    cout << "\nPERFORMANCE EVALUATION\n" << string(40, '-') << "\n" << fixed << setprecision(2);
    cout << "Average Waiting Time    : " << totalWT / n << "\n";
    cout << "Average Turnaround Time : " << totalTAT / n << "\n";
    cout << "Average Response Time   : " << totalRT / n << "\n";
    cout << "Maximum Waiting Time    : " << maxWT << " (" << maxPid << ")\n";
}

int main() {
    // Scenario 1: normal downlink
    vector<Process> scenario1 = {
        {"P1", "TT&C Beacon",   0, 3, 1},
        {"P2", "SSTV Image",    1, 8, 4},
        {"P3", "Codec2 Voice",  2, 4, 2},
        {"P4", "M17 Data",      3, 5, 3},
        {"P5", "Housekeeping",  4, 2, 5}
    };

    // Scenario 2: busy ground-station pass (starvation test)
    vector<Process> scenario2 = {
        {"P1", "TT&C Beacon",   0,  3, 1},
        {"P2", "SSTV Image",    0,  6, 5},
        {"P3", "Codec2 Voice",  2,  4, 2},
        {"P4", "M17 Data",      5,  4, 2},
        {"P5", "TT&C Beacon",   8,  3, 1},
        {"P6", "Codec2 Voice", 11,  4, 2},
        {"P7", "TT&C Beacon",  15,  3, 1}
    };

    cout << string(70, '=') << "\n";
    cout << "       SOMAIYASAT SATELLITE DOWNLINK SCHEDULER\n";
    cout << "      PREEMPTIVE PRIORITY CPU SCHEDULING WITH AGING\n";
    cout << string(70, '=') << "\n";

    printReport("SCENARIO 1: NORMAL DOWNLINK", scenario1, false);
    printReport("SCENARIO 1: NORMAL DOWNLINK", scenario1, true);
    printReport("SCENARIO 2: BUSY PASS", scenario2, false);
    printReport("SCENARIO 2: BUSY PASS", scenario2, true);

    cout << "\n" << string(70, '=') << "\n";
    cout << "           SOMAIYASAT DOWNLINK INTERPRETATION\n";
    cout << string(70, '=') << "\n";
    cout << "TT&C Beacon (Priority 1) : telemetry, tracking & command - most urgent\n";
    cout << "Codec2 Voice (Priority 2): live digital voice relay\n";
    cout << "M17 Data (Priority 2-3)  : digital amateur packet transmission\n";
    cout << "SSTV Image (Priority 4-5): slow-scan TV earth observation, long & less urgent\n";
    cout << "Housekeeping (Priority 5): battery, temperature and health logs\n\n";
    cout << "Aging lets long-waiting SSTV images reach the downlink,\n";
    cout << "while TT&C beacons are never overtaken.\n";
    cout << string(70, '=') << "\n";

    return 0;
}
