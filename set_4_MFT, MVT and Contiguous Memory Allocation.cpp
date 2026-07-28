// Part A - MFT Simulation
#include <iomanip>
#include <iostream>
#include <vector>
using namespace std;
int main()
{
    int partitions, processes;
    cout << "Number of fixed partitions: ";
    cin >> partitions;
    if (partitions <= 0)
        return 1;
    vector<int> partSize(partitions), assigned(partitions, -1);
    cout << "Enter partition sizes:\n";
    for (int &size : partSize)
        cin >> size;
    cout << "Number of processes: ";
    cin >> processes;
    if (processes <= 0)
        return 1;
    vector<int> procSize(processes), allocation(processes, -1);
    cout << "Enter process sizes:\n";
    for (int &size : procSize)
        cin >> size;
    // First-fit assignment: one process per fixed partition.
    for (int i = 0; i < processes; ++i)
    {
        for (int j = 0; j < partitions; ++j)
        {
            if (assigned[j] == -1 && partSize[j] >= procSize[i])
            {
                assigned[j] = i;
                allocation[i] = j;
                break;
            }
        }
    }

    int totalInternal = 0;
    cout << "\nMFT allocation table\n";
    cout << left << setw(10) << "Process" << setw(12) << "Size" << setw(12)
         << "Partition" << setw(16) << "Part. size" << setw(18) << "Internal frag." << '\n';
    cout << string(68, '-') << '\n';
    for (int i = 0; i < processes; ++i)
    {
        cout << left << setw(10) << ("P" + to_string(i + 1)) << setw(12) << procSize[i];
        if (allocation[i] == -1)
        {
            cout << setw(12) << "Not allocated" << setw(16) << "-" << setw(18) << "-" << '\n';
        }
        else
        {
            int j = allocation[i];
            int internal = partSize[j] - procSize[i];
            totalInternal += internal;
            cout << setw(12) << ("M" + to_string(j + 1)) << setw(16) << partSize[j]
                 << setw(18) << internal << '\n';
        }
    }
    cout << "Total internal fragmentation: " << totalInternal << '\n';
    return 0;
}


//  Part C - First Fit, Best Fit and Worst Fit