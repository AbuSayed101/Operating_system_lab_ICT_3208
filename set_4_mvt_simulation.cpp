// Part B - MVT Mutiple variable partition Simulation

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
struct Segment
{
    int start;
    int size;
    bool free;
    int pid;
};
class MVTMemory
{
    vector<Segment> segments;
    void mergeFreeHoles()
    {
        vector<Segment> merged;
        for (const auto &s : segments)
        {
            if (!merged.empty() && merged.back().free && s.free)
            {
                merged.back().size += s.size;
            }
            else
            {
                merged.push_back(s);
            }
        }
        segments = merged;
    }

public:
    explicit MVTMemory(int total) { segments.push_back({0, total, true, -1}); }
    bool allocate(int pid, int amount)
    {
        if (amount <= 0)
            return false;
        for (const auto &s : segments)
            if (!s.free && s.pid == pid)
                return false;
        for (size_t i = 0; i < segments.size(); ++i)
        {
            if (segments[i].free && segments[i].size >= amount)
            {
                Segment used{segments[i].start, amount, false, pid};
                int remaining = segments[i].size - amount;
                segments[i] = used;
                if (remaining > 0)
                {
                    segments.insert(segments.begin() + static_cast<long>(i) + 1,
                                    {used.start + amount, remaining, true, -1});
                }
                return true;
            }
        }
        return false;
    }
    bool release(int pid)
    {
        bool found = false;
        for (auto &s : segments)
        {
            if (!s.free && s.pid == pid)
            {
                s.free = true;
                s.pid = -1;
                found = true;
            }
        }
        if (found)
            mergeFreeHoles();
        return found;
    }
    void display() const
    {
        int totalFree = 0, largestHole = 0;
        cout << "\nMVT memory map\n";
        cout << left << setw(12) << "Start" << setw(12) << "End" << setw(12)
             << "Size" << setw(16) << "Status" << '\n';
        cout << string(52, '-') << '\n';
        for (const auto &s : segments)
        {
            cout << left << setw(12) << s.start << setw(12) << (s.start + s.size - 1)
                 << setw(12) << s.size;
            if (s.free)
            {
                cout << setw(16) << "Free" << '\n';
                totalFree += s.size;
                largestHole = max(largestHole, s.size);
            }
            else
            {
                cout << setw(16) << ("P" + to_string(s.pid)) << '\n';
            }
        }
        cout << "Total free memory: " << totalFree << '\n';
        cout << "Largest free hole : " << largestHole << '\n';
        cout << "External fragmentation indicator (total free - largest hole): "
             << totalFree - largestHole << '\n';
    }
};
int main()
{
    int total;
    cout << "Total memory size: ";
    cin >> total;
    if (total <= 0)
        return 1;
    MVTMemory memory(total);
    while (true)
    {
        cout << "\n1. Allocate process\n2. Terminate process\n3. Display memory\n0. Exit\nChoice: ";
        int choice;
        cin >> choice;
        if (choice == 0)
            break;
        if (choice == 1)
        {
            int pid, size;
            cout << "Process ID and required size: ";
            cin >> pid >> size;
            cout << (memory.allocate(pid, size) ? "Allocated.\n" : "Allocation failed.\n");
        }
        else if (choice == 2)
        {
            int pid;
            cout << "Process ID to terminate: ";
            cin >> pid;
            cout << (memory.release(pid) ? "Released.\n" : "Process not found.\n");
        }
        else if (choice == 3)
        {
            memory.display();
        }
        else
        {
            cout << "Invalid choice.\n";
        }
    }
    return 0;
}

/*
input

Total memory size: 1000
1. Allocate process
2. Terminate process
3. Display memory
0. Exit
Choice:

Choice: 1
Process ID and required size:200

1 200
2 300
3 100


Total memory size: 1000
1000

Choice: 1
1

Process ID and required size:
1 200

Choice: 1
1

Process ID and required size:
2 300

Choice: 1
1

Process ID and required size:
3 100

Choice: 3
3

Choice: 0
0
*/