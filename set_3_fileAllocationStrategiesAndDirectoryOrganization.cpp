#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

struct Allocation
{
    string name;
    string method;
    int indexBlock = -1;
    vector<int> dataBlocks;
};

class Disk
{
private:
    vector<int> owner; // -1 = Free block
    vector<Allocation> files;

    bool nameExists(const string &name) const
    {
        for (const auto &f : files)
        {
            if (f.name == name)
                return true;
        }
        return false;
    }

    vector<int> collectFreeBlocks(int count) const
    {
        vector<int> blocks;

        for (int i = 0; i < owner.size() && blocks.size() < count; i++)
        {
            if (owner[i] == -1)
                blocks.push_back(i);
        }

        return blocks;
    }

public:
    Disk(int blocks)
    {
        owner.assign(blocks, -1);
    }

    // Contiguous Allocation
    bool allocateContiguous(const string &name, int count)
    {
        if (nameExists(name) || count <= 0)
            return false;

        int start = -1;
        int run = 0;

        for (int i = 0; i < owner.size(); i++)
        {
            if (owner[i] == -1)
            {
                run++;

                if (run == count)
                {
                    start = i - count + 1;
                    break;
                }
            }
            else
            {
                run = 0;
            }
        }

        if (start == -1)
            return false;

        int fileId = files.size();

        Allocation a;
        a.name = name;
        a.method = "Contiguous";

        for (int i = start; i < start + count; i++)
        {
            owner[i] = fileId;
            a.dataBlocks.push_back(i);
        }

        files.push_back(a);
        return true;
    }

    // Linked Allocation
    bool allocateLinked(const string &name, int count)
    {
        if (nameExists(name) || count <= 0)
            return false;

        vector<int> blocks = collectFreeBlocks(count);

        if (blocks.size() != count)
            return false;

        int fileId = files.size();

        for (int block : blocks)
            owner[block] = fileId;

        Allocation a;
        a.name = name;
        a.method = "Linked";
        a.dataBlocks = blocks;

        files.push_back(a);

        return true;
    }

    // Indexed Allocation
    bool allocateIndexed(const string &name, int count)
    {
        if (nameExists(name) || count <= 0)
            return false;

        vector<int> blocks = collectFreeBlocks(count + 1);

        if (blocks.size() != count + 1)
            return false;

        int fileId = files.size();

        for (int block : blocks)
            owner[block] = fileId;

        Allocation a;
        a.name = name;
        a.method = "Indexed";
        a.indexBlock = blocks[0];

        for (int i = 1; i < blocks.size(); i++)
            a.dataBlocks.push_back(blocks[i]);

        files.push_back(a);

        return true;
    }

    // Display Allocation Table
    void displayAllocations() const
    {
        cout << "\n========== File Allocation Table ==========\n";

        if (files.empty())
        {
            cout << "No files allocated.\n";
            return;
        }

        for (const auto &f : files)
        {
            cout << "\nFile Name : " << f.name << endl;
            cout << "Method    : " << f.method << endl;

            if (f.method == "Contiguous")
            {
                cout << "Start Block : " << f.dataBlocks.front() << endl;
                cout << "Length      : " << f.dataBlocks.size() << endl;
            }
            else if (f.method == "Linked")
            {
                cout << "Chain : ";

                for (int i = 0; i < f.dataBlocks.size(); i++)
                {
                    cout << f.dataBlocks[i];

                    if (i != f.dataBlocks.size() - 1)
                        cout << " -> ";
                }

                cout << " -> NULL\n";
            }
            else
            {
                cout << "Index Block : " << f.indexBlock << endl;

                cout << "Data Blocks : ";

                for (int block : f.dataBlocks)
                    cout << block << " ";

                cout << endl;
            }
        }
    }

    // Display Disk Map
    void displayDiskMap() const
    {
        cout << "\n========== Disk Map ==========\n";

        for (int i = 0; i < owner.size(); i++)
        {
            cout << setw(3) << i << ":";

            if (owner[i] == -1)
                cout << setw(8) << "-";
            else
                cout << setw(8) << files[owner[i]].name;

            if ((i + 1) % 8 == 0)
                cout << endl;
        }

        if (owner.size() % 8 != 0)
            cout << endl;
    }
};

int main()
{
    int diskBlocks;

    cout << "Enter number of disk blocks: ";
    cin >> diskBlocks;

    if (diskBlocks <= 0)
    {
        cout << "Invalid disk size.\n";
        return 1;
    }

    Disk disk(diskBlocks);

    while (true)
    {
        cout << "\n========== MENU ==========\n";
        cout << "1. Contiguous Allocation\n";
        cout << "2. Linked Allocation\n";
        cout << "3. Indexed Allocation\n";
        cout << "4. Display Allocation Table\n";
        cout << "5. Display Disk Map\n";
        cout << "0. Exit\n";

        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        if (choice == 0)
        {
            cout << "\nProgram Finished.\n";
            break;
        }

        if (choice == 4)
        {
            disk.displayAllocations();
            continue;
        }

        if (choice == 5)
        {
            disk.displayDiskMap();
            continue;
        }

        if (choice < 1 || choice > 3)
        {
            cout << "Invalid choice.\n";
            continue;
        }

        string fileName;
        int blocks;

        cout << "Enter file name: ";
        cin >> fileName;

        cout << "Enter required data blocks: ";
        cin >> blocks;

        bool ok = false;

        if (choice == 1)
            ok = disk.allocateContiguous(fileName, blocks);
        else if (choice == 2)
            ok = disk.allocateLinked(fileName, blocks);
        else
            ok = disk.allocateIndexed(fileName, blocks);

        if (ok)
            cout << "Allocation Successful.\n";
        else
            cout << "Allocation Failed (Duplicate file name or insufficient space).\n";
    }

    return 0;
}