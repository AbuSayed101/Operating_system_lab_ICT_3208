#include <algorithm> // Added for min() and max()
#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

using namespace std;

constexpr int N = 5;

mutex forks[N];
mutex outputMutex;

void printLine(const string &text)
{
    lock_guard<mutex> lock(outputMutex);
    cout << text << '\n';
}

void philosopher(int id)
{
    for (int round = 1; round <= 3; ++round)
    {
        printLine("Philosopher " + to_string(id) +
                  " is thinking (round " + to_string(round) + ").");

        this_thread::sleep_for(chrono::milliseconds(60 + id * 15));

        int leftFork = id;
        int rightFork = (id + 1) % N;

        // Always lock the lower-numbered fork first
        int first = min(leftFork, rightFork);
        int second = max(leftFork, rightFork);

        unique_lock<mutex> lock1(forks[first]);
        unique_lock<mutex> lock2(forks[second]);

        printLine("Philosopher " + to_string(id) +
                  " is eating using forks " +
                  to_string(leftFork) + " and " +
                  to_string(rightFork) + ".");

        this_thread::sleep_for(chrono::milliseconds(80));
    }

    printLine("Philosopher " + to_string(id) + " has finished.");
}

int main()
{
    vector<thread> philosophers;

    for (int i = 0; i < N; i++)
    {
        philosophers.emplace_back(philosopher, i);
    }

    for (auto &t : philosophers)
    {
        t.join();
    }

    cout << "All philosophers completed without deadlock." << endl;

    return 0;
}