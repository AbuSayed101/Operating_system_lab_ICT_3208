#include<algorithm>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>
    using namespace std;
vector<int> allocate(const vector<int> &originalBlocks, const vector<int> &processes, int method)
{
    vector<int> remaining = originalBlocks;
    vector<int> allocation(processes.size(), -1);
    for (size_t i = 0; i < processes.size(); ++i)
    {
        int chosen = -1;
        for (size_t j = 0; j < remaining.size(); ++j)
        {
            if (remaining[j] < processes[i])
                continue;
            if (method == 1)
            { // first fit
                chosen = static_cast<int>(j);
                break;
            }
            if (chosen == -1 ||
                (method == 2 && remaining[j] < remaining[chosen]) ||
                (method == 3 && remaining[j] > remaining[chosen]))
            {
                chosen = static_cast<int>(j);
            }
        }
        if (chosen != -1)
        {
            allocation[i] = chosen;
            remaining[chosen] -= processes[i];
        }
    }
    return allocation;
}
void report(const string &title, const vector<int> &blocks, const vector<int> &processes,
            const vector<int> &allocation)
{
    cout << "\n"
         << title << '\n';
    cout << left << setw(10) << "Process" << setw(12) << "Size" << setw(12) << "Block" << '\n';
    cout << string(34, '-') << '\n';
    for (size_t i = 0; i < processes.size(); ++i)
    {
        cout << left << setw(10) << ("P" + to_string(i + 1)) << setw(12) << processes[i];
        if (allocation[i] == -1)
            cout << setw(12) << "Not allocated" << '\n';
        else
            cout << setw(12) << ("B" + to_string(allocation[i] + 1)) << '\n';
    }
}
int main()
{
    int m, n;
    cout << "Number of memory blocks: ";
    cin >> m;
    vector<int> blocks(m);
    cout << "Enter block sizes:\n";
    for (int &x : blocks)
        cin >> x;
    cout << "Number of processes: ";
    cin >> n;
    vector<int> processes(n);
    cout << "Enter process sizes:\n";
    for (int &x : processes)
        cin >> x;
    report("First Fit", blocks, processes, allocate(blocks, processes, 1));
    report("Best Fit", blocks, processes, allocate(blocks, processes, 2));
    report("Worst Fit", blocks, processes, allocate(blocks, processes, 3));
    return 0;
}