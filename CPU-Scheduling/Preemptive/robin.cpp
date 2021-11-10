#include <iostream>
#include <vector>
#include <string>

using namespace std;

#define DEBUG
#define SIZE 10
#define QUANTA 2

typedef struct process {
    int ID, arrival, burst;
} process;

typedef struct {
    int ID[SIZE], 
        arrival[SIZE], 
        burst[SIZE],
        finish[SIZE],
        turn_around[SIZE],
        wait[SIZE];
} table;

table stats = {0};

bool prepare_ready(vector<process>&, vector<process>&, int);

void schedule(vector<process> pros){
    int time = 0, previous_process = -1;
    vector<process> ready;
    string bar = "|", timestamps = "0  ";
    char temp[40];

    while (pros.size() + ready.size() > 0) {
        prepare_ready(pros, ready, time);

        while (ready.size() == 0){
            bar += " ";
            timestamps += " ";
            time += 1;
            prepare_ready(pros, ready, time);
        }

        for (int i = 0; i < ready.size();) {
            // prepare_ready(pros, ready, time);
            int limit = (ready[i].burst < QUANTA) ? ready[i].burst : QUANTA;
            time += limit;
            ready[i].burst -= QUANTA;
            int pid = ready[i].ID - 1;

            if(ready[i].burst <= 0){
                stats.ID[pid] = pid + 1;
                stats.arrival[pid] = ready[i].arrival;
                stats.finish[pid] = time;
                stats.turn_around[pid] = stats.finish[pid] - stats.arrival[pid];
                stats.wait[pid] = stats.turn_around[pid] - stats.burst[pid];

                ready.erase(ready.begin() + i);
                i--;
            }

            // if(previous_process == pid){
            //     bar += "     ";
            //     timestamps += "     ";
            //     continue;
            // }

            for(int j = 0; j < limit; j++) {
                bar += " ";
                timestamps += " ";
            }

            bar += "P" + to_string(pid + 1);

            for(int j = 0; j < limit; j++) {
                bar += " ";
                timestamps += " ";
            }

            bar += "|";

            sprintf(temp, "%-3d", time);
            timestamps += temp;
            previous_process = pid;
            if(prepare_ready(pros, ready, time))
                i++;

        }
        
    }
    cout << "===== Gantt Chart =====\n";
    for (int i = 0; i < bar.size(); i++)
        cout << '-';
    
    cout << '\n' << bar << '\n';

    for (int i = 0; i < bar.size(); i++)
        cout << '-';
    cout << '\n' << timestamps;
}

void print_table(int len){
    printf("\n\n==== Data Chart ====");
    printf("\nID  AT BT FT  TT  WT\n");
    for (int i = 0; i < len; i++) {
        printf("%-2d  %-2d %-2d %-3d %-3d %-3d\n",
            stats.ID[i], 
            stats.arrival[i], 
            stats.burst[i], 
            stats.finish[i], 
            stats.turn_around[i], 
            stats.wait[i]);
    }   
}

int main(){
    #ifdef DEBUG
    vector<process> pros{
        {1, 0, 5},
        {2, 1, 3},
        {3, 2, 1},
        {4, 3, 2},
        {5, 4, 3},

        // {1, 10, 2},
        // {2, 15, 3},
    };
    // int len = sizeof(pros) / sizeof(process);
    int len = pros.size();

    #endif

    for (int i = 0; i < len; i++) {
        stats.ID[i] = pros[i].ID;
        stats.arrival[i] = pros[i].arrival;
        stats.burst[i] = pros[i].burst;
    }

    cout << "========== Round Robin ==========\n";
    schedule(pros);
    print_table(pros.size());
    return 0;
}

bool prepare_ready(vector<process> &pros, vector<process> &ready, int time){
    bool changed = false; // to track mutation of ready vector
    for (int i = 0; i < pros.size();)
    {
        if(pros[i].arrival > time){
            i++;
            continue;
        }

        changed = true;
        ready.push_back(pros[i]);
        pros.erase(pros.begin() + i);
    }
    return changed;
}