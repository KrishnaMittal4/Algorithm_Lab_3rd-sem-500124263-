#include<stdio.h>
#include<stdlib.h>
#include<time.h>

// Structure to represent an activity
struct Activity {
    int start;
    int finish;
};

int activityCompare(const void* , const void* );
void activitySelection(struct Activity activities[] , int );

int main(){
   
    // Example activities (start and finish times)
    struct Activity activities[] = {
    {0, 3}, 
    {2, 4}, 
    {1, 5}, 
    {6, 7}, 
    {5, 9}, 
    {8, 11}
    };

    int n = sizeof(activities) / sizeof(activities[0]);

    // Call the activity selection function
    printf("\nSelected activities:\n");
    activitySelection(activities, n);


    return 0;
}

// Function to compare two activities based on their finish times
int activityCompare(const void* a, const void* b) {
    struct Activity* activityA = (struct Activity*)a;
    struct Activity* activityB = (struct Activity*)b;
    return activityA->finish - activityB->finish;
}

// Function to select the maximum number of activities
void activitySelection(struct Activity activities[], int n) {
    // Sort activities based on their finish time
    qsort(activities, n, sizeof(activities[0]), activityCompare);

    printf("Selected activities based on indices: \n");

    // The first activity always gets selected
    int i = 0;
    printf("%d (start: %d, finish: %d)\n", i, activities[i].start, activities[i].finish);

    // Consider the rest of the activities
    for (int j = 1; j < n; j++) {
        // If this activity's start time is greater than or equal to
        // the finish time of the last selected activity, select it
        if (activities[j].start >= activities[i].finish) {
            printf("%d (start: %d, finish: %d)\n", j, activities[j].start, activities[j].finish);
            i = j; // Update the last selected activity
        }
    }
}

