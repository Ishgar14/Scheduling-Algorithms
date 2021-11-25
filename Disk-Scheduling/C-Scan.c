#include <stdio.h>
#include <stdlib.h>

int abs(int val){
    return (val > 0) ? val : -val;
}

void sort(int *numbers, int len){
    for (int i = 0; i < len; i++) {
        int j = i;
        while (j > 0 && numbers[j] < numbers[j - 1]) {
            int temp = numbers[j];
            numbers[j] = numbers[j - 1];
            numbers[j - 1] = temp;
            j--;
        }
    }
}

int shortest_schedule(int *numbers, int len, int track) {
    int total_distance_travelled = 0, dist = 0, i = 0;
    sort(numbers, len);
    printf("%-10s %-10s\n", "Location", "Distance");

    while (numbers[i] < track) {
        i++;
    }
    int copy = i;

    for (; i < len; i++) {
        dist = abs(numbers[i] - track);
        total_distance_travelled += dist;
        track = numbers[i];
        printf("%-10d %-4d\n", numbers[i], dist);
    }

    for(i = 0; i < copy; i++){
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
    printf("Enter number of locations: ");
    scanf("%d", &n);
    // int *numbers = calloc(n, sizeof(int));
    int numbers[10];

    for(int i = 0; i < n; i++){
        printf("Enter seek location: ");
        scanf("%d", numbers + i);
    }
    printf("\n");
    
    int dist = shortest_schedule(numbers, n, track);
    printf("\nTotal distance is %d\n", dist);
    // free(numbers);
}