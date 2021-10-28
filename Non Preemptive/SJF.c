#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DEBUG
#define SIZE 10

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
    int time = 0;
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

        bar = concat(bar, "|");
        sprintf(temp, "%-3d", time);
        timestamps = concat(timestamps, temp);

        for(int i = 0; i < p->burst; i++) {
            bar = concat(bar, " ");
            timestamps = concat(timestamps, " ");
        }

        sprintf(temp, "P%d", p->ID);
        bar = concat(bar, temp);
        
        for(int i = 0; i < p->burst; i++) {
            bar = concat(bar, " ");
            timestamps = concat(timestamps, " ");
        }

        time += p->burst;

        int pid = p->ID - 1;
        stats.ID[pid] = p->ID;
        stats.arrival[pid] = p->arrival;
        stats.burst[pid] = p->burst;
        stats.finish[pid] = time;
        stats.turn_around[pid] = stats.finish[pid] - stats.arrival[pid];
        stats.wait[pid] = stats.turn_around[pid] - stats.burst[pid];

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
        {1, 7, 9},
        {2, 0, 3},
        {3, 1, 7},
        {4, 5, 1},
        {5, 3, 3},

        // {1, 10, 2},
        // {2, 15, 3},
    };
    int len = sizeof(pros) / sizeof(process);

    #else
    int len = 0;
    printf("Enter number of pros: ");
    scanf("%d", &len);
    process *pros = malloc(sizeof(process) * len);

    for (int i = 0; i < len; i++) {
        pros[i].id = i + 1;
        printf("Enter arrival time of P%d: ", i + 1);
        scanf("%d", &pros[i].arrival);
        printf("Enter burst time of P%d: ", i + 1);
        scanf("%d", &pros[i].burst);
        printf("\n");
    }
    #endif

    printf("========== Shorted Job First [Non Preemptive] ==========\n");
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
    process temp;
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

    while(index < *len - 1){
        pro_copy(pros + index, pros + index + 1);
        index++;
    }

    *len = *len - 1;
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