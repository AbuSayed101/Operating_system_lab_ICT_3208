#include <iomanip>
#include <iostream>
#include <vector>
using namespace std;
int main()
{
    int n, m;
    cout << "Number of processes and resource types: ";
    cin >> n >> m;
    if (n <= 0 || m <= 0)
    {
        cerr << "Both values must be positive.\n";
        return 1;
    }
    vector<vector<int>> allocation(n, vector<int>(m));
    vector<vector<int>> maximum(n, vector<int>(m));
    vector<vector<int>> need(n, vector<int>(m));
    vector<int> available(m);
    cout << "Enter the Allocation matrix (" << n << " x " << m << "):\n";
    for (auto &row : allocation)
        for (int &value : row)
            cin >> value;
    cout << "Enter the Maximum matrix (" << n << " x " << m << "):\n";
    for (auto &row : maximum)
        for (int &value : row)
            cin >> value;
    cout << "Enter the Available vector:\n";
    for (int &value : available)
        cin >> value;
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            need[i][j] = maximum[i][j] - allocation[i][j];
            if (need[i][j] < 0)
            {
                cerr << "Invalid input: allocation cannot exceed maximum demand.\n";
                return 1;
            }
        }
    }
    cout << "\nNeed matrix\n";
    for (int i = 0; i < n; ++i)
    {
        cout << "P" << i << ": ";
        for (int j = 0; j < m; ++j)
            cout << setw(4) << need[i][j];
        cout << '\n';
    }
    vector<int> work = available;
    vector<bool> finish(n, false);
    vector<int> safeSequence;
    while (static_cast<int>(safeSequence.size()) < n)
    {
        bool progress = false;
        for (int i = 0; i < n; ++i)
        {
            if (finish[i])
                continue;
            bool canRun = true;
            for (int j = 0; j < m; ++j)
            {
                if (need[i][j] > work[j])
                {
                    canRun = false;
                    break;
                }
            }
            if (canRun)
            {
                for (int j = 0; j < m; ++j)
                    work[j] += allocation[i][j];
                finish[i] = true;
                safeSequence.push_back(i);
                progress = true;
            }
        }
        if (!progress)
            break;
    }
    if (static_cast<int>(safeSequence.size()) == n)
    {
        cout << "\nThe system is in a SAFE state.\nSafe sequence: ";
        for (size_t i = 0; i < safeSequence.size(); ++i)
        {
            if (i)
                cout << " -> ";
            cout << "P" << safeSequence[i];
        }
        cout << '\n';
    }
    else
    {
        cout << "\nThe system is in an UNSAFE state; no complete safe sequence exists.\n";
    }
    return 0;
}