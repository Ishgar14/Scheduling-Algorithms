#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// #define DEBUG
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

int cmp_arrival(const void*, const void*);
char* concat(char*, char*);
process* next(process[], int*, int);

void schedule(process *pros, int len){
    int time = 0, limit, previous_process = 0;
    char *bar = '\0';
    char *timestamps = '\0';
    qsort(pros, len, sizeof(process), cmp_arrival);
    char temp[10];

    while(len > 0){
        process *p = next(pros, &len, time);

        if(p == NULL) {
            bar = concat(bar, "|");
            sprintf(temp, "%d", time);
            timestamps = concat(timestamps, temp);

            while((p = next(pros, &len, time)) == NULL){
                bar = concat(bar, "x");
                timestamps = concat(timestamps, " ");
                time += 1;
            }
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
        free(p);
    }
    
    printf("Gantt Chart\n");
    bar = concat(bar, "|");

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
    sprintf(temp, "%d", time);
    timestamps = concat(timestamps, temp);
    printf("%s", timestamps);
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

    printf("========== Shorted Job First [Preemptive] ==========\n");
    schedule(pros, len);
    print_table(len);
}

int cmp_arrival(const void* p1, const void* p2){
    return ((process*)p1)->arrival > ((process*)p2)->arrival;
}

void pro_copy(process *p1, process *p2) {
    p1->arrival = p2->arrival;
    p1->burst = p2->burst;
    p1->ID = p2->ID;
}

// This function returns the next available process 
process* next(process* pros, int *len, int time) {
    process temp = {0};
    temp.burst = INT_MAX;
    int index = -1;
    process *ret = NULL;

    for (int i = 0; i < *len; i++) {
        if(pros[i].arrival > time)
            break;

        if(pros[i].burst < temp.burst){
            pro_copy(&temp, pros + i);
            index = i;
        }
    }

    // No process could be found within given time
    if(index == -1)
        return NULL;
    
    ret = malloc(sizeof(process));
    pro_copy(ret, pros + index);
    pros[index].burst -= QUANTA;

    if (ret->burst == 0 || ret->burst <= QUANTA) {
        while(index < *len - 1){
            pro_copy(pros + index, pros + index + 1);
            index++;
        }

        *len = *len - 1;
    }
    return ret;
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