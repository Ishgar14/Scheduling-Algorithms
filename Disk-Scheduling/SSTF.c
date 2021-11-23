#include <stdio.h>
#include <stdlib.h>

int abs(int val){
    return (val > 0) ? val : -val;
}

void sort(int *diff, int *original, int len){
    for (int i = 0; i < len; i++) {
        int j = i;
        while (j > 0 && diff[j] < diff[j - 1]) {
            int temp = diff[j];
            diff[j] = diff[j - 1];
            diff[j - 1] = temp;
            
            temp = original[j];
            original[j] = original[j - 1];
            original[j - 1] = temp;
            j--;
        }
    }
}

int shortest_schedule(int *numbers, int len, int track) {
    int total_distance_travelled = 0, dist = 0, i = 0;

    // create an array of differences then sort that array
    int *diff_array = malloc(sizeof(int) * len);
    // int diff_array[10];
    for (i = 0; i < len; i++) 
        diff_array[i] = abs(numbers[i] - track);
    sort(diff_array, numbers, len);
    

    printf("%-10s %-10s\n", "Location", "Distance");
    for (i = 0; i < len; i++) {
        dist = abs(numbers[i] - track);
        total_distance_travelled += dist;
        track = numbers[i];
        printf("%-10d %-4d\n", numbers[i], dist);
    }

    free(diff_array);
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
    
    int dist = shortest_schedule(numbers, n, track);
    printf("\nTotal distance is %d\n", dist);
    // free(numbers);
}