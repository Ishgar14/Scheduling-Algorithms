#include <stdio.h>
#include <stdlib.h>
#include "../gantt.h"

#define DEBUG

typedef struct {
    int id, arrival, burst;
} process;

typedef struct {
    process p[20];
    int finish_time[20];
    int waiting_time[20];
    int turn_around_time[20];
    int len;
} stats;

typedef struct {
    process *data;
    int len;
} queue;

queue* queue_init(int size);
void enqueue(queue*, process*);
process* dequeue(queue *q);

void print_stats(stats*);
int process_arrival(const void*, const void*);
void stat_sort(stats*);


void schedule(process *pros, int len){
    int tick = 0;
    gantt *chart = malloc(sizeof(gantt));
    stats statistics = {0};
    qsort(pros, len, sizeof(process), process_arrival);
    queue *ready = queue_init(len);

    for (int i = 0; i < len; i++)
        enqueue(ready, pros + i);


    while (ready->len > 0) {
        process *p = dequeue(ready);

        if (p->arrival <= tick){
            printf("Time %2d  Executing process %d\n", tick, p->id);
            tick += p->burst;
            int len = statistics.len;

            statistics.p[len].id = p->id;
            statistics.p[len].arrival = p->arrival;
            statistics.p[len].burst = p->burst;
            statistics.finish_time[len] = tick;
            statistics.turn_around_time[len] = statistics.finish_time[len] - p->arrival;
            statistics.waiting_time[len] = statistics.finish_time[len] - p->burst;
            statistics.len++;
        }
        else {
            /*
            int future = 0;
            for(int i = 0; i < ready->len; i++)
                if(ready->data[i].arrival <= tick){
                    future = 1;
                    break;
                }

            if(!future) {}
            */
            printf("Time %2d  IDLE\n", tick);
            tick += 1;
            enqueue(ready, p);
        }

        free(p);
    }

    stat_sort(&statistics);
    print_stats(&statistics);
    free(ready);
}

void print_stats(stats *statistics) {
    printf("ID AT  BT  FT  TT  WT\n");
    for (int i = 0; i < statistics->len; i++)
    {
        printf("%2d %2d  %2d  %2d  %2d  %2d\n", 
            statistics->p[i].id, 
            statistics->p[i].arrival, 
            statistics->p[i].burst, 
            statistics->finish_time[i], 
            statistics->turn_around_time[i],
            statistics->waiting_time[i]
        );
    }
    
}

void main() {
    #ifdef DEBUG
    process processes[] = {
        {1, 7, 15},
        {2, 0, 5},
        {3, 1, 7},
        {4, 5, 1},
        {5, 3, 2},
    };
    schedule(processes, sizeof(processes) / sizeof(process));

    #else
    int len = 0;
    printf("Enter number of processes: ");
    scanf("%d", &len);
    process *processes = malloc(sizeof(process) * len);

    for (int i = 0; i < len; i++) {
        processes[i].id = i + 1;
        printf("Enter arrival time of P%d: ", i + 1);
        scanf("%d", &processes[i].arrival);
        printf("Enter burst time of P%d: ", i + 1);
        scanf("%d", &processes[i].burst);
        printf("\n");
    }
    
    schedule(processes, len);
    
    #endif
}


queue* queue_init(int size) {
    queue *q = malloc(sizeof(queue));
    q->data = calloc(size, sizeof(process));
    q->len = 0;
    return q;
}

void enqueue(queue *q, process* p) {
    q->data[q->len].id = p->id;
    q->data[q->len].arrival = p->arrival;
    q->data[q->len].burst = p->burst;
    q->len++;
}

process* dequeue(queue *q){
    process *p = malloc(sizeof(process));
    p->id = q->data[0].id;
    p->arrival = q->data[0].arrival;
    p->burst = q->data[0].burst;

    for (int i = 1; i < q->len; i++) {
        q->data[i - 1].id = q->data[i].id;
        q->data[i - 1].arrival = q->data[i].arrival;
        q->data[i - 1].burst = q->data[i].burst;
    }
    q->len--;
    
    return p;
}

int process_arrival(const void *p1, const void *p2) {
    return ((process*) p1)->arrival > ((process*) p2)->arrival;
}

void swap(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}


void stat_sort(stats *statistic){
    for (int i = 0; i < statistic->len; i++) {
        int j = i;
        while (j > 0 && statistic->p[j].id < statistic->p[j-1].id){
            swap(&statistic->p[j].id, &statistic->p[j-1].id);
            swap(&statistic->p[j].arrival, &statistic->p[j-1].arrival);
            swap(&statistic->p[j].burst, &statistic->p[j-1].burst);

            swap(&statistic->finish_time[j], &statistic->finish_time[j-1]);
            swap(&statistic->turn_around_time[j], &statistic->turn_around_time[j-1]);
            swap(&statistic->waiting_time[j], &statistic->waiting_time[j-1]);
            j--;
        }
    }
}