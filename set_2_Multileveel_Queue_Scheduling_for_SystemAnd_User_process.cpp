#include<algorithm>
#include<iomanip>
#include <iostream>
#include <queue>
#include <string>
#include <vector>
using namespace std;
struct Process
{
    int id;
    int burst;
    int priority;
    int queueType; // 1 = system, 2 = user
};
struct Result
{
    int completion = 0;
    int turnaround = 0;
    int waiting = 0;
};
struct QueueRun
{
    vector<Result> result;
    vector<string> gantt;
    int finishTime = 0;
};
QueueRun runQueue(const vector<Process> &q, int algorithm, int quantum, int startTime)
{
    QueueRun out;
    out.result.resize(q.size());
    int time = startTime;
    if (q.empty())
    {
        out.finishTime = time;
        return out;
    }
    vector<int> order(q.size());
    for (size_t i = 0; i < q.size(); ++i)
        order[i] = static_cast<int>(i);
    if (algorithm == 2)
    {
        stable_sort(order.begin(), order.end(), [&](int a, int b)
                    {
            if (q[a].burst != q[b].burst) return q[a].burst < q[b].burst;
            return q[a].id < q[b].id; });
    }
    else if (algorithm == 4)
    {
        stable_sort(order.begin(), order.end(), [&](int a, int b)
                    {
            if (q[a].priority != q[b].priority) return q[a].priority < q[b].priority;
            return q[a].id < q[b].id; });
    }
    if (algorithm == 3)
    {
        vector<int> remaining(q.size());
        queue<int> ready;
        for (size_t i = 0; i < q.size(); ++i)
        {
            remaining[i] = q[i].burst;
            ready.push(static_cast<int>(i));
        }
        while (!ready.empty())
        {
            int idx = ready.front();
            ready.pop();
            int run = min(quantum, remaining[idx]);
            out.gantt.push_back("P" + to_string(q[idx].id) + "(" + to_string(run) + ")");
            time += run;
            remaining[idx] -= run;
            if (remaining[idx] > 0)
            {
                ready.push(idx);
            }
            else
            {
                out.result[idx].completion = time;
            }
        }
    }
    else
    {
        for (int idx : order)
        {
            out.gantt.push_back("P" + to_string(q[idx].id) + "(" + to_string(q[idx].burst) + ")");
            time += q[idx].burst;
            out.result[idx].completion = time;
        }
    }
    for (size_t i = 0; i < q.size(); ++i)
    {
        out.result[i].turnaround = out.result[i].completion; // all processes arrive at time 0
        out.result[i].waiting = out.result[i].turnaround - q[i].burst;
    }
    out.finishTime = time;
    return out;
}
string algorithmName(int choice)
{
    switch (choice)
    {
    case 1:
        return "FCFS";
    case 2:
        return "SJF (non-preemptive)";
    case 3:
        return "Round Robin";
    case 4:
        return "Priority (non-preemptive)";
    }
    return "Unknown";
}
void printQueueReport(const string &label, const vector<Process> &q, const QueueRun &run, int algorithm)
{
    cout << "\n"
         << label << " - " << algorithmName(algorithm) << '\n';
    cout << "Execution sequence: ";
    for (size_t i = 0; i < run.gantt.size(); ++i)
    {
        if (i)
            cout << " -> ";
        cout << run.gantt[i];
    }
    cout << "\n\n";
    cout << left << setw(8) << "PID" << setw(8) << "BT" << setw(10) << "PR"
         << setw(10) << "CT" << setw(10) << "TAT" << setw(10) << "WT" << '\n';
    cout << string(56, '-') << '\n';
    for (size_t i = 0; i < q.size(); ++i)
    {
        cout << left << setw(8) << ("P" + to_string(q[i].id))
             << setw(8) << q[i].burst << setw(10) << q[i].priority
             << setw(10) << run.result[i].completion << setw(10) << run.result[i].turnaround
             << setw(10) << run.result[i].waiting << '\n';
    }
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
    vector<Process> systemQueue, userQueue;
    cout << "All processes are assumed to arrive at time 0.\n";
    cout << "Enter burst time, priority and queue type (1=system, 2=user).\n";
    cout << "A smaller priority number means higher priority.\n";
    for (int i = 0; i < n; ++i)
    {
        Process p{};
        p.id = i + 1;
        cout << "P" << p.id << ": ";
        cin >> p.burst >> p.priority >> p.queueType;
        if (p.burst <= 0 || (p.queueType != 1 && p.queueType != 2))
        {
            cerr << "Invalid burst time or queue type.\n";
            return 1;
        }
        if (p.queueType == 1)
            systemQueue.push_back(p);
        else
            userQueue.push_back(p);
    }
    cout << "\nAlgorithm choices: 1=FCFS, 2=SJF, 3=Round Robin, 4=Priority\n";
    int systemAlgorithm, userAlgorithm;
    cout << "Algorithm for system queue: ";
    cin >> systemAlgorithm;
    cout << "Algorithm for user queue: ";
    cin >> userAlgorithm;
    if (systemAlgorithm < 1 || systemAlgorithm > 4 || userAlgorithm < 1 || userAlgorithm > 4)
    {
        cerr << "Invalid algorithm choice.\n";
        return 1;
    }
    int systemQuantum = 1, userQuantum = 1;
    if (systemAlgorithm == 3)
    {
        cout << "Time quantum for system queue: ";
        cin >> systemQuantum;
    }
    if (userAlgorithm == 3)
    {
        cout << "Time quantum for user queue: ";
        cin >> userQuantum;
    }
    if (systemQuantum <= 0 || userQuantum <= 0)
    {
        cerr << "Time quantum must be positive.\n";
        return 1;
    }
    // Strict queue priority: complete all system processes before user processes.
    QueueRun systemRun = runQueue(systemQueue, systemAlgorithm, systemQuantum, 0);
    QueueRun userRun = runQueue(userQueue, userAlgorithm, userQuantum, systemRun.finishTime);
    printQueueReport("System Queue (higher priority)", systemQueue, systemRun, systemAlgorithm);
    printQueueReport("User Queue", userQueue, userRun, userAlgorithm);
    cout << "\nOverall completion time: " << userRun.finishTime << '\n';
    return 0;
}