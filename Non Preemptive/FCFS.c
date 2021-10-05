#include <stdio.h>
#include <stdlib.h>

// #define DEBUG

typedef struct {
    int id, arrival, burst;
} process;


typedef struct {
    process *data;
    int len;
} queue;

queue* queue_init(int size);
void enqueue(queue*, process*);
process* dequeue(queue *q);
int process_arrival(const void*, const void*);


void schedule(process *pros, int len){
    int tick = 0;
    qsort(pros, len, sizeof(process), process_arrival);
    queue *ready = queue_init(len);

    for (int i = 0; i < len; i++)
        enqueue(ready, pros + i);

        
    while (ready->len > 0) {
        process *p = dequeue(ready);

        if (p->arrival <= tick){
            printf("Time %2d  Executing process %d\n", tick, p->id);
            tick += p->burst;
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

    free(ready);
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