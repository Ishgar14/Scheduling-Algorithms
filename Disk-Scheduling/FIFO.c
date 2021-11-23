#include <stdio.h>
#include <stdlib.h>

int abs(int val){
    return (val > 0) ? val : -val;
}

int fifo_schedule(int *numbers, int len, int track) {
    int total_distance_travelled = 0, dist = 0;
    printf("%-10s %-10s\n", "Location", "Distance");

    for (int i = 0; i < len; i++) {
        dist = abs(numbers[i] - track);
        total_distance_travelled += dist;
        track = numbers[i];
        printf("%-10d %-4d\n", numbers[i], dist);
    }
    
    return total_distance_travelled;
}

void main() {
    int track, n;
    printf("Enter current track: ");
    scanf("%d", &track);
    printf("Enter number of x: ");
    scanf("%d", &n);
    // int *numbers = calloc(n, sizeof(int));
    int numbers[10];

    for(int i = 0; i < n; i++){
        printf("Enter seek location: ");
        scanf("%d", numbers + i);
    }
    printf("\n");
    
    int dist = fifo_schedule(numbers, n, track);
    printf("\nTotal distance is %d\n", dist);
    // free(numbers);
}