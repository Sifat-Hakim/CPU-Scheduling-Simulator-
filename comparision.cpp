#include <iostream>
#include <string>
using namespace std;

struct Process {
    int id, arrival, burst, priority;
};

struct Result {
    string name;
    double avgTAT;
    double avgWT;
};

void calculateAndStore(string name, int n, Process p[], Result &res, int tq = 0) {
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

    res.name = name;
    res.avgTAT = totalTAT / n;
    res.avgWT = totalWT / n;
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

    string algorithms[5] = {"FCFS", "SJF", "Priority", "SJF Preemptive", "Round Robin"};
    Result results[5];

    for (int i = 0; i < 5; i++) {
        if (algorithms[i] == "Round Robin")
            calculateAndStore(algorithms[i], n, p, results[i], tq);
        else
            calculateAndStore(algorithms[i], n, p, results[i]);
    }

    cout << "\n--- Algorithm Comparison ---\n";
    cout << "Algorithm\tAvg TAT\tAvg WT\n";

    double bestTAT = 1e9, bestWT = 1e9;
    string bestAlgoTAT, bestAlgoWT;

    for (int i = 0; i < 5; i++) {
        cout << results[i].name << "\t" << results[i].avgTAT << "\t" << results[i].avgWT << "\n";
        if (results[i].avgTAT < bestTAT) {
            bestTAT = results[i].avgTAT;
            bestAlgoTAT = results[i].name;
        }
        if (results[i].avgWT < bestWT) {
            bestWT = results[i].avgWT;
            bestAlgoWT = results[i].name;
        }
    }

    cout << "\nBest Algorithm based on lowest Avg TAT: " << bestAlgoTAT << "\n";
    cout << "Best Algorithm based on lowest Avg WT: " << bestAlgoWT << "\n";

    return 0;
}

