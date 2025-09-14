#include <iostream>
using namespace std;

struct Process {
    int id, arrival, burst, priority;
};

void calculateAndPrint(string name, int n, Process p[], int tq = 0) {
    int completion[n], turnaround[n], waiting[n], remaining[n];
    for (int i = 0; i < n; i++) remaining[i] = p[i].burst;

    int currentTime = 0, doneCount = 0;
    double totalTAT = 0, totalWT = 0;

    if (name == "FCFS") {
        for (int i = 0; i < n; i++) {
            if (currentTime < p[i].arrival) currentTime = p[i].arrival;
            completion[i] = currentTime + p[i].burst;
            turnaround[i] = completion[i] - p[i].arrival;
            waiting[i] = turnaround[i] - p[i].burst;
            currentTime = completion[i];
        }
    }
    else if (name == "SJF") { // Non-preemptive
        bool done[n] = {false};
        while (doneCount < n) {
            int idx = -1, minBurst = 1e9;
            for (int i = 0; i < n; i++) {
                if (!done[i] && p[i].arrival <= currentTime && p[i].burst < minBurst) {
                    minBurst = p[i].burst;
                    idx = i;
                }
            }
            if (idx == -1) { currentTime++; continue; }
            completion[idx] = currentTime + p[idx].burst;
            turnaround[idx] = completion[idx] - p[idx].arrival;
            waiting[idx] = turnaround[idx] - p[idx].burst;
            currentTime = completion[idx];
            done[idx] = true;
            doneCount++;
        }
    }
    else if (name == "Priority") { // Non-preemptive
        bool done[n] = {false};
        while (doneCount < n) {
            int idx = -1, bestPrio = 1e9;
            for (int i = 0; i < n; i++) {
                if (!done[i] && p[i].arrival <= currentTime && p[i].priority < bestPrio) {
                    bestPrio = p[i].priority;
                    idx = i;
                }
            }
            if (idx == -1) { currentTime++; continue; }
            completion[idx] = currentTime + p[idx].burst;
            turnaround[idx] = completion[idx] - p[idx].arrival;
            waiting[idx] = turnaround[idx] - p[idx].burst;
            currentTime = completion[idx];
            done[idx] = true;
            doneCount++;
        }
    }
    else if (name == "SJF Preemptive") {
        while (doneCount < n) {
            int idx = -1, minRem = 1e9;
            for (int i = 0; i < n; i++) {
                if (p[i].arrival <= currentTime && remaining[i] > 0 && remaining[i] < minRem) {
                    minRem = remaining[i];
                    idx = i;
                }
            }
            if (idx == -1) { currentTime++; continue; }
            remaining[idx]--;
            currentTime++;
            if (remaining[idx] == 0) {
                completion[idx] = currentTime;
                turnaround[idx] = completion[idx] - p[idx].arrival;
                waiting[idx] = turnaround[idx] - p[idx].burst;
                doneCount++;
            }
        }
    }
    else if (name == "Round Robin") {
        while (doneCount < n) {
            bool any = false;
            for (int i = 0; i < n; i++) {
                if (p[i].arrival <= currentTime && remaining[i] > 0) {
                    any = true;
                    int t = (remaining[i] < tq) ? remaining[i] : tq;
                    remaining[i] -= t;
                    currentTime += t;
                    if (remaining[i] == 0) {
                        completion[i] = currentTime;
                        turnaround[i] = completion[i] - p[i].arrival;
                        waiting[i] = turnaround[i] - p[i].burst;
                        doneCount++;
                    }
                }
            }
            if (!any) currentTime++;
        }
    }

    // Average calculation
    for (int i = 0; i < n; i++) {
        totalTAT += turnaround[i];
        totalWT += waiting[i];
    }

    cout << name << " → Avg TAT = " << (totalTAT / n)
         << ", Avg WT = " << (totalWT / n) << "\n";
}

int main() {
    int n;
    cout << "Enter number of processes: ";
    cin >> n;

    Process p[n];
    for (int i = 0; i < n; i++) {
        p[i].id = i+1;
        cout << "Enter arrival time of P" << i+1 << ": ";
        cin >> p[i].arrival;
        cout << "Enter burst time of P" << i+1 << ": ";
        cin >> p[i].burst;
        cout << "Enter priority of P" << i+1 << ": ";
        cin >> p[i].priority;
    }

    int tq;
    cout << "Enter Time Quantum for Round Robin: ";
    cin >> tq;

    cout << "\n--- Algorithm Comparison ---\n";
    calculateAndPrint("FCFS", n, p);
    calculateAndPrint("SJF", n, p);
    calculateAndPrint("Priority", n, p);
    calculateAndPrint("SJF Preemptive", n, p);
    calculateAndPrint("Round Robin", n, p, tq);

    return 0;
}
