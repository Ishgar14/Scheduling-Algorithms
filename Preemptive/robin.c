#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DEBUG
#define SIZE 10
#ifdef DEBUG
#define QUANTA 1
#else
int QUANTA;
#endif

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

// Ready queue to store process which HAVE arrived
process ready[SIZE] = {0};
int ready_len = 0;


void prepare_ready(process*, int*, int);
int cmp_arrival(const void*, const void*);
char* concat(char*, char*);

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

void schedule(process* pros, int len){
    int time = 0, limit, previous_process = 0, i = 0;
    char *bar = '\0';
    char *timestamps = '\0';
    qsort(pros, len, sizeof(process), cmp_arrival);
    char temp[10];
    process *p;

    while(1) {
        prepare_ready(pros, &len, time);

        if(i >= ready_len){
            if(ready_len == 0)
                i = 0;
            else 
                i %= ready_len;
        }

        p = (ready_len > 0) ? &ready[i] : NULL;

        if(p == NULL) {
            bar = concat(bar, "|");
            sprintf(temp, "%d", time);
            timestamps = concat(timestamps, temp);

            if(len == 0)
                break;

            while(ready_len == 0)
            {
                bar = concat(bar, "x");
                timestamps = concat(timestamps, " ");
                time += 1;
                prepare_ready(pros, &len, time);
            }
            
            continue;
        }

        if (previous_process == p->ID){
            bar = concat(bar, " ");
            timestamps = concat(timestamps, " ");
        }
        else {
            bar = concat(bar, "|");
            sprintf(temp, "%-3d", time);
            timestamps = concat(timestamps, temp);
        }

        limit = (p->burst < QUANTA) ? p->burst : QUANTA;
        for(int i = 0; i < limit; i++) {
            bar = concat(bar, " ");
            timestamps = concat(timestamps, " ");
        }

        if(previous_process != p->ID) {
            sprintf(temp, "P%d", p->ID);
            bar = concat(bar, temp);
        }
        

        for(int i = 0; i < limit; i++) {
            bar = concat(bar, " ");
            timestamps = concat(timestamps, " ");
        }

        time += limit;
        p->burst -= QUANTA;
        
        if(p->burst <= 0) {
            int pid = p->ID - 1;
            stats.finish[pid] = time;
            stats.turn_around[pid] = stats.finish[pid] - stats.arrival[pid];
            stats.wait[pid] = stats.turn_around[pid] - stats.burst[pid];
        }

        previous_process = p->ID;
        i++;        
    }
    
    printf("Gantt Chart\n");

    // decoration
    for (int i = 0; bar[i] != '\0'; i++) 
        printf("-");
    printf("\n");

    // Printing the process bars
    printf("%s\n", bar);

    // decoration
    for (int i = 0; bar[i] != '\0'; i++)
        printf("-");
    printf("\n");

    // printing the time stamps of process
    printf("%s", timestamps);
}

void main() {
    #ifdef DEBUG
    process pros[] = {
        {1, 0, 7},
        {2, 1, 5},
        {3, 2, 3},
        {4, 3, 1},

        // {1, 10, 2},
        // {2, 15, 3},
    };
    int len = sizeof(pros) / sizeof(process);

    #else
    int len = 0;
    printf("Enter number of pros: ");
    scanf("%d", &len);
    printf("\nEnter Quanta: ");
    scanf("%d", &QUANTA);
    printf("\n");
    process *pros = malloc(sizeof(process) * len);

    for (int i = 0; i < len; i++) {
        pros[i].ID = i + 1;
        printf("Enter arrival time of P%d: ", i + 1);
        scanf("%d", &pros[i].arrival);
        printf("Enter burst time of P%d: ", i + 1);
        scanf("%d", &pros[i].burst);
        printf("\n");
    }
    #endif

    for (int i = 0; i < len; i++) {
        stats.ID[i] = pros[i].ID;
        stats.arrival[i] = pros[i].arrival;
        stats.burst[i] = pros[i].burst;
    }

    printf("========== Round Robin ==========\n");
    schedule(pros, len);
    print_table(len);
}

int cmp_arrival(const void* p1, const void* p2){
    return ((process*)p1)->arrival > ((process*)p2)->arrival || 
            ((process*)p1)->ID > ((process*)p2)->ID;
}

void pro_copy(process *p1, process *p2) {
    p1->arrival = p2->arrival;
    p1->burst = p2->burst;
    p1->ID = p2->ID;
}

void kill(process *arr, int *len, int index_to_kill){
    int i;
    for (i = index_to_kill; i < *len - 1; i++)
        pro_copy(arr + i, arr + i + 1);
    if(i == *len - 1){
        process empty = {0, 0, 0};
        pro_copy(arr + i, &empty);
    }
    *len = *len - 1;
}

void prepare_ready(process *pros, int *len, int time){
    // Iterate over processes to check if given process has arrived
    while(1)
    {
        if(*len == 0 || pros[0].arrival > time)
            break;

        pro_copy(ready + ready_len, pros);
        kill(pros, len, 0);
        ready_len++;
    }
    
    // Iterate over ready queue to filter executed processes
    for (int i = 0; i < ready_len; i++)
        if(ready[i].burst <= 0)
            kill(ready, &ready_len, i);
}


char* concat(char *str1, char *str2) {
    int len = 0;
    int len1 = (str1 == 0 || *str1 == '\0') ? 0 : strlen(str1);
    int len2 = strlen(str2);
    char *result = malloc(len1 + len2 + 1);

    for (int i = 0; i < len1; i++)
        result[len++] = str1[i];
    for (int i = 0; i < len2; i++){
        result[len + i] = str2[i];
    }
    len += len2;

    result[len1 + len2] = '\0';
    free(str1);
    // free(str2);

    return result;
}