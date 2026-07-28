#include <algorithm>
#include <iomanip>
#include <iostream>
#include <limits>
#include <queue>
#include <string>
#include <vector>
using namespace std;
struct Process
{
    int id;
    int arrival;
    int burst;
    int priority;
};
struct Result
{
    int completion = 0;
    int turnaround = 0;
    int waiting = 0;
};
void printResults(const string &title, const vector<Process> &p, const vector<Result> &r)
{
    double totalWT = 0.0, totalTAT = 0.0;
    cout << "\n"
         << title << "\n";
    cout << left << setw(8) << "PID" << setw(8) << "AT" << setw(8) << "BT"
         << setw(10) << "PR" << setw(10) << "CT" << setw(10) << "TAT"
         << setw(10) << "WT" << '\n';
    cout << string(64, '-') << '\n';
    for (size_t i = 0; i < p.size(); ++i)
    {
        cout << left << setw(8) << ("P" + to_string(p[i].id))
             << setw(8) << p[i].arrival << setw(8) << p[i].burst
             << setw(10) << p[i].priority << setw(10) << r[i].completion
             << setw(10) << r[i].turnaround << setw(10) << r[i].waiting << '\n';
        totalWT += r[i].waiting;
        totalTAT += r[i].turnaround;
    }
    cout << fixed << setprecision(2);
    cout << "Average waiting time    : " << totalWT / p.size() << '\n';
    cout << "Average turnaround time : " << totalTAT / p.size() << '\n';
}
vector<Result> finalize(const vector<Process> &p, const vector<int> &completion)
{
    vector<Result> r(p.size());
    for (size_t i = 0; i < p.size(); ++i)
    {
        r[i].completion = completion[i];
        r[i].turnaround = completion[i] - p[i].arrival;
        r[i].waiting = r[i].turnaround - p[i].burst;
    }
    return r;
}
vector<Result> fcfs(const vector<Process> &p)
{
    int n = static_cast<int>(p.size());
    vector<int> order(n), completion(n, 0);
    for (int i = 0; i < n; ++i)
        order[i] = i;
    stable_sort(order.begin(), order.end(), [&](int a, int b)
                {
        if (p[a].arrival != p[b].arrival) return p[a].arrival < p[b].arrival;
        return p[a].id < p[b].id; });
    int time = 0;
    for (int idx : order)
    {
        time = max(time, p[idx].arrival);
        time += p[idx].burst;
        completion[idx] = time;
    }
    return finalize(p, completion);
}
vector<Result> sjfNonPreemptive(const vector<Process> &p)
{
    int n = static_cast<int>(p.size());
    vector<bool> done(n, false);
    vector<int> completion(n, 0);
    int finished = 0, time = 0;
    while (finished < n)
    {
        int chosen = -1;
        for (int i = 0; i < n; ++i)
        {
            if (done[i] || p[i].arrival > time)
                continue;
            if (chosen == -1 || p[i].burst < p[chosen].burst ||
                (p[i].burst == p[chosen].burst && p[i].arrival < p[chosen].arrival) ||
                (p[i].burst == p[chosen].burst && p[i].arrival == p[chosen].arrival && p[i].id < p[chosen].id))
            {
                chosen = i;
            }
        }
        if (chosen == -1)
        {
            int nextArrival = numeric_limits<int>::max();
            for (int i = 0; i < n; ++i)
                if (!done[i])
                    nextArrival = min(nextArrival, p[i].arrival);
            time = max(time, nextArrival);
            continue;
        }
        time += p[chosen].burst;
        completion[chosen] = time;
        done[chosen] = true;
        ++finished;
    }
    return finalize(p, completion);
}
vector<Result> priorityNonPreemptive(const vector<Process> &p)
{
    int n = static_cast<int>(p.size());
    vector<bool> done(n, false);
    vector<int> completion(n, 0);
    int finished = 0, time = 0;
    while (finished < n)
    {
        int chosen = -1;
        for (int i = 0; i < n; ++i)
        {
            if (done[i] || p[i].arrival > time)
                continue;
            if (chosen == -1 || p[i].priority < p[chosen].priority ||
                (p[i].priority == p[chosen].priority && p[i].arrival < p[chosen].arrival) ||
                (p[i].priority == p[chosen].priority && p[i].arrival == p[chosen].arrival && p[i].id < p[chosen].id))
            {
                chosen = i;
            }
        }
        if (chosen == -1)
        {
            int nextArrival = numeric_limits<int>::max();
            for (int i = 0; i < n; ++i)
                if (!done[i])
                    nextArrival = min(nextArrival, p[i].arrival);
            time = max(time, nextArrival);
            continue;
        }
        time += p[chosen].burst;
        completion[chosen] = time;
        done[chosen] = true;
        ++finished;
    }
    return finalize(p, completion);
}
vector<Result> roundRobin(const vector<Process> &p, int quantum)
{
    int n = static_cast<int>(p.size());
    vector<int> remaining(n), completion(n, 0), order(n);
    for (int i = 0; i < n; ++i)
    {
        remaining[i] = p[i].burst;
        order[i] = i;
    }
    stable_sort(order.begin(), order.end(), [&](int a, int b)
                {
        if (p[a].arrival != p[b].arrival) return p[a].arrival < p[b].arrival;
        return p[a].id < p[b].id; });
    queue<int> ready;
    int next = 0, finished = 0, time = 0;
    while (finished < n)
    {
        if (ready.empty() && next < n && time < p[order[next]].arrival)
            time = p[order[next]].arrival;
        while (next < n && p[order[next]].arrival <= time)
        {
            ready.push(order[next]);
            ++next;
        }
        if (ready.empty())
            continue;
        int idx = ready.front();
        ready.pop();
        int run = min(quantum, remaining[idx]);
        time += run;
        remaining[idx] -= run;
        while (next < n && p[order[next]].arrival <= time)
        {
            ready.push(order[next]);
            ++next;
        }
        if (remaining[idx] > 0)
        {
            ready.push(idx);
        }
        else
        {
            completion[idx] = time;
            ++finished;
        }
    }
    return finalize(p, completion);
}
int main()
{
    int n;
    cout << "Number of processes: ";
    cin >> n;
    if (n <= 0)
    {
        cerr << "The number of processes must be positive.\n";
        return 1;
    }
    vector<Process> processes(n);
    cout << "Enter arrival time, burst time and priority for each process.\n";
    cout << "A smaller priority number means higher priority.\n";
    for (int i = 0; i < n; ++i)
    {
        processes[i].id = i + 1;
        cout << "P" << i + 1 << ": ";
        cin >> processes[i].arrival >> processes[i].burst >> processes[i].priority;
        if (processes[i].arrival < 0 || processes[i].burst <= 0)
        {
            cerr << "Arrival time must be non-negative and burst time must be positive.\n";
            return 1;
        }
    }
    int quantum;
    cout << "Time quantum for Round Robin: ";
    cin >> quantum;
    if (quantum <= 0)
    {
        cerr << "Time quantum must be positive.\n";
        return 1;
    }
    printResults("FCFS Scheduling", processes, fcfs(processes));
    printResults("SJF Scheduling (Non-preemptive)", processes, sjfNonPreemptive(processes));
    printResults("Priority Scheduling (Non-preemptive)", processes, priorityNonPreemptive(processes));
    printResults("Round Robin Scheduling", processes, roundRobin(processes, quantum));
    return 0;
}