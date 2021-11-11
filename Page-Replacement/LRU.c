#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 30
#define EMPTY -1

typedef struct {
    int *frame;
    char status;
} frame;

void show_history(frame *history, int len, int PAGE_FRAMES){
    for (int i = 0; i < PAGE_FRAMES; i++) {
        for (int j = 0; j < len; j++) {
            int val = history[j].frame[i];

            if(val == EMPTY)
                printf("  -");
            else 
                printf("%3d", val);
        }
        printf("\n");
    }
    
    for (int j = 0; j < len; j++) {
        printf("%3c", history[j].status);
    }
}

void frame_copy(frame *dst, frame *src, int PAGE_FRAMES){
    for (int i = 0; i < PAGE_FRAMES; i++)
        dst->frame[i] = src->frame[i];
}

char hit_or_miss(frame *f, int PAGE_FRAME, char page){
    for (int i = 0; i < PAGE_FRAME; i++) {
        if(f->frame[i] == page - '0')
            return 'H';
    }
    return 'P';
}

// returns age of `page` 
int age(char *seq, int len, char page){
    for (int i = len - 1; i > 0; i--)
        if(seq[i] == page + '0')
            return len - i;
    
    return 1 << 31 - 1;
}
void schedule(char* sequence, int PAGE_FRAMES, frame *history){
    int counter = 0; 

    for (int i = 0; sequence[i] && i < PAGE_FRAMES; i++) {
        if(i > 0)
            frame_copy(&history[i], &history[i - 1], PAGE_FRAMES);

        history[i].status = 'P';
        history[i].frame[counter] = sequence[i] - '0';
        counter++;
    }

    for(int i = PAGE_FRAMES; sequence[i]; i++) {
        int boomer = 0, *target = 0;

        if(i > 0)
            frame_copy(&history[i], &history[i - 1], PAGE_FRAMES);

        history[i].status = hit_or_miss(&history[i], PAGE_FRAMES, sequence[i]);

        if(history[i].status == 'P'){
            for (int j = 0; j < PAGE_FRAMES; j++) {
                int _age = age(sequence, i, history[i].frame[j]);
                if(_age > boomer){
                    boomer = _age;
                    target = &history[i].frame[j];
                }
            }
            if(target)
                *target = sequence[i] - '0';
        }
    }
}

void main() {
    char sequence_string[SIZE] = "012301401234";
    int PAGE_FRAMES = 3;

    // printf("Enter number of page frames: ");
    // scanf("%d", &PAGE_FRAMES);

    // printf("Enter sequence string: ");
    // scanf("%s", sequence_string);

    frame history[SIZE];

    for (int i = 0; i < SIZE; i++) {
        history[i].frame = malloc(sizeof(int) * PAGE_FRAMES);
        for (int j = 0; j < PAGE_FRAMES; j++) 
            history[i].frame[j] = EMPTY;
    }
    
    schedule(sequence_string, PAGE_FRAMES, history);

    printf("Sequence\n", sequence_string);
    for (int i = 0; sequence_string[i]; i++)
        printf("%3c", sequence_string[i]);
    printf("\n\n");
    
    show_history(history, strlen(sequence_string), PAGE_FRAMES);
}