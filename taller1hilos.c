#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_NUM 10000

typedef struct {
    int size;
    int *numbers;
} Group;

typedef struct {
    int id;
    int num_buckets;
    int *buckets;
    Group *groups;
} ThreadArgs;

void *bucketSort(void *arg) {
    ThreadArgs *threadArgs = (ThreadArgs *)arg;
    int id = threadArgs->id;
    int num_buckets = threadArgs->num_buckets;
    int *buckets = threadArgs->buckets;
    Group *groups = threadArgs->groups;

    for (int i = 0; i < num_buckets; i++) {
        if (buckets[i] == id) {
            int size = groups[i].size;
            int *numbers = groups[i].numbers;

            // Bucket Sort
            int count[MAX_NUM + 1] = {0};
            for (int j = 0; j < size; j++) {
                count[numbers[j]]++;
            }

            for (int j = 0, k = 0; j <= MAX_NUM; j++) {
                for (int l = 0; l < count[j]; l++) {
                    numbers[k++] = j;
                }
            }

            // Print sorted numbers
            printf("Thread %d: ", id);
            for (int j = 0; j < size; j++) {
                printf("%d ", numbers[j]);
            }
            printf("\n");
        }
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <input_file> <num_buckets>\n", argv[0]);
        return 1;
    }

    char *input_file = argv[1];
    int num_buckets = atoi(argv[2]);

    FILE *file = fopen(input_file, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    int num_groups;
    fscanf(file, "%d", &num_groups);

    Group *groups = (Group *)malloc(num_groups * sizeof(Group));
    for (int i = 0; i < num_groups; i++) {
        int size;
        fscanf(file, "%d", &size);

        int *numbers = (int *)malloc(size * sizeof(int));
        for (int j = 0; j < size; j++) {
            fscanf(file, "%d", &numbers[j]);
        }

        groups[i].size = size;
        groups[i].numbers = numbers;
    }

    fclose(file);

    pthread_t *threads = (pthread_t *)malloc(num_buckets * sizeof(pthread_t));
    ThreadArgs *threadArgs = (ThreadArgs *)malloc(num_buckets * sizeof(ThreadArgs));

    int *buckets = (int *)malloc(num_groups * sizeof(int));
    for (int i = 0; i < num_groups; i++) {
        buckets[i] = i % num_buckets;
    }

    for (int i = 0; i < num_buckets; i++) {
        threadArgs[i].id = i;
        threadArgs[i].num_buckets = num_buckets;
        threadArgs[i].buckets = buckets;
        threadArgs[i].groups = groups;

        pthread_create(&threads[i], NULL, bucketSort, (void *)&threadArgs[i]);
    }

    for (int i = 0; i < num_buckets; i++) {
        pthread_join(threads[i], NULL);
    }

    free(groups);
    free(threads);
    free(threadArgs);
    free(buckets);

    return 0;
}
