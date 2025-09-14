#include <iostream>
using namespace std;

int main() {
    int n;
    cout << "Enter number of processes: ";
    cin >> n;

    int arrival[n], burst[n], priority[n], completion[n], turnaround[n], waiting[n];
    int burstCopy[n]; // keep original burst times
    int ganttProcess[500], ganttStart[500], ganttEnd[500], ganttCount = 0;

    // Input process details
    for (int i = 0; i < n; i++) {
        cout << "Enter arrival time of process " << i+1 << ": ";
        cin >> arrival[i];
        cout << "Enter burst time of process " << i+1 << ": ";
        cin >> burst[i];
        burstCopy[i] = burst[i];
        cout << "Enter priority of process " << i+1 << " (lower number = higher priority): ";
        cin >> priority[i];
    }

    int choice;
    cout << "\nSelect Scheduling Algorithm:\n";
    cout << "1. FCFS\n2. SJF Non-Preemptive\n3. Priority Non-Preemptive\n4. SJF Preemptive\n5. Round Robin\nEnter choice: ";
    cin >> choice;

    int currentTime = 0;

    if (choice == 1) {
        // FCFS
        for (int i = 0; i < n; i++) {
            if (currentTime < arrival[i])
                currentTime = arrival[i];

            ganttProcess[ganttCount] = i+1;
            ganttStart[ganttCount] = currentTime;
            ganttEnd[ganttCount] = currentTime + burst[i];
            ganttCount++;

            completion[i] = currentTime + burst[i];
            turnaround[i] = completion[i] - arrival[i];
            waiting[i] = turnaround[i] - burst[i];
            currentTime = completion[i];
        }
    } else if (choice == 2) {
        // SJF Non-Preemptive
        bool done[n] = {false};
        for (int count = 0; count < n; count++) {
            int idx = -1;
            int minBurst = 100000;
            for (int i = 0; i < n; i++) {
                if (!done[i] && arrival[i] <= currentTime && burst[i] < minBurst) {
                    minBurst = burst[i];
                    idx = i;
                }
            }
            if (idx == -1) {
                currentTime++;
                count--;
                continue;
            }

            ganttProcess[ganttCount] = idx+1;
            ganttStart[ganttCount] = currentTime;
            ganttEnd[ganttCount] = currentTime + burst[idx];
            ganttCount++;

            completion[idx] = currentTime + burst[idx];
            turnaround[idx] = completion[idx] - arrival[idx];
            waiting[idx] = turnaround[idx] - burst[idx];
            currentTime = completion[idx];
            done[idx] = true;
        }
    } else if (choice == 3) {
        // Priority Non-Preemptive
        bool done[n] = {false};
        for (int count = 0; count < n; count++) {
            int idx = -1;
            int highestPrio = 100000;
            for (int i = 0; i < n; i++) {
                if (!done[i] && arrival[i] <= currentTime && priority[i] < highestPrio) {
                    highestPrio = priority[i];
                    idx = i;
                }
            }
            if (idx == -1) {
                currentTime++;
                count--;
                continue;
            }

            ganttProcess[ganttCount] = idx+1;
            ganttStart[ganttCount] = currentTime;
            ganttEnd[ganttCount] = currentTime + burst[idx];
            ganttCount++;

            completion[idx] = currentTime + burst[idx];
            turnaround[idx] = completion[idx] - arrival[idx];
            waiting[idx] = turnaround[idx] - burst[idx];
            currentTime = completion[idx];
            done[idx] = true;
        }
    } else if (choice == 4) {
        // SJF Preemptive
        int remaining[n];
        for (int i = 0; i < n; i++) remaining[i] = burst[i];
        int doneCount = 0;
        while (doneCount < n) {
            int idx = -1;
            int minRem = 100000;
            for (int i = 0; i < n; i++) {
                if (arrival[i] <= currentTime && remaining[i] > 0 && remaining[i] < minRem) {
                    minRem = remaining[i];
                    idx = i;
                }
            }
            if (idx == -1) {
                currentTime++;
                continue;
            }

            ganttProcess[ganttCount] = idx+1;
            ganttStart[ganttCount] = currentTime;

            remaining[idx]--;
            currentTime++;

            ganttEnd[ganttCount] = currentTime;
            ganttCount++;

            if (remaining[idx] == 0) {
                completion[idx] = currentTime;
                turnaround[idx] = completion[idx] - arrival[idx];
                waiting[idx] = turnaround[idx] - burst[idx];
                doneCount++;
            }
        }
    } else if (choice == 5) {
        // Round Robin
        int tq;
        cout << "Enter Time Quantum: ";
        cin >> tq;
        int remaining[n];
        for (int i = 0; i < n; i++) remaining[i] = burst[i];
        int doneCount = 0;
        currentTime = 0;
        while (doneCount < n) {
            bool any = false;
            for (int i = 0; i < n; i++) {
                if (arrival[i] <= currentTime && remaining[i] > 0) {
                    any = true;

                    ganttProcess[ganttCount] = i+1;
                    ganttStart[ganttCount] = currentTime;

                    int t = (remaining[i] < tq) ? remaining[i] : tq;
                    remaining[i] -= t;
                    currentTime += t;

                    ganttEnd[ganttCount] = currentTime;
                    ganttCount++;

                    if (remaining[i] == 0) {
                        completion[i] = currentTime;
                        turnaround[i] = completion[i] - arrival[i];
                        waiting[i] = turnaround[i] - burst[i];
                        doneCount++;
                    }
                }
            }
            if (!any) currentTime++;
        }
    } else {
        cout << "Invalid choice!";
        return 0;
    }

    // Display results
    double totalTAT = 0, totalWT = 0;
    cout << "\nProcess\tArrival\tBurst\tPriority\tCompletion\tTAT\tWT\n";
    for (int i = 0; i < n; i++) {
        cout << "P" << i+1 << "\t" << arrival[i] << "\t" << burstCopy[i] << "\t" 
             << priority[i] << "\t\t" << completion[i] << "\t\t" 
             << turnaround[i] << "\t" << waiting[i] << "\n";

        totalTAT += turnaround[i];
        totalWT += waiting[i];
    }

    cout << "\nAverage Turnaround Time = " << (totalTAT / n);
    cout << "\nAverage Waiting Time = " << (totalWT / n) << "\n";

    // Print Gantt Chart
    cout << "\nGantt Chart:\n";
    for (int i = 0; i < ganttCount; i++) {
        cout << "|  P" << ganttProcess[i] << "  ";
    }
    cout << "|\n";

    for (int i = 0; i < ganttCount; i++) {
        cout << ganttStart[i] << "\t";
    }
    cout << ganttEnd[ganttCount-1] << "\n";

    return 0;
}

