#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

// Change database.txt with the filename you want to search.
// FIle Last updated on 5/1/2023 by Tsubasa Kato. Used ChatGPT (GPT-4) to change this C program
int arr_prime[] = {1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377};

typedef struct {
    char query[256];
    char data[128];
    int threshold;
} CompareArgs;

pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;

void *Compare(void *args) {
    CompareArgs *compareArgs = (CompareArgs *)args;
    char *query = compareArgs->query;
    char *data = compareArgs->data;
    int threshold = compareArgs->threshold;

    int found = 0;
    int currentIndex = 7, j = 0, k = 0;
    int misValue = 0, prim = arr_prime[currentIndex], i = arr_prime[currentIndex];
    int length1 = strlen(query);
    int length2 = strlen(data);

    if (strcmp(query, data) == 0) {
        pthread_mutex_lock(&print_mutex);
        printf("two strings are completely matched\n");
        pthread_mutex_unlock(&print_mutex);
        free(args);
        return 0;
    }

    while (1) {
        while (i > length2) {
            i -= arr_prime[currentIndex--];
            if (currentIndex < 0) {
                free(args);
                return 0;
            }
            i += arr_prime[currentIndex];
        }

        for (j = i, k = 0; j < length2; j++) {
            if (query[k] == data[j]) {
                if (k == length1 - 1) {
                    found = 1;
                    k = 0;
                } else {
                    k++;
                }
            } else {
                k = 0;
                misValue++;
            }
        }

        int threshold2 = threshold * 3;
        if (found && misValue <= threshold2) {
            pthread_mutex_lock(&print_mutex);
            printf("two strings are partially matched within threshold.\n");
            printf("%s", data);
            pthread_mutex_unlock(&print_mutex);
            free(args);
            return 0;
        }
        if (found) {
            pthread_mutex_lock(&print_mutex);
            printf("two strings were partially matched\n");
            printf("%s", data);
            pthread_mutex_unlock(&print_mutex);
            free(args);
            return 0;
        }

        found = 0;
        i += arr_prime[currentIndex];
    }
}

int main(void) {
    char query[256];
    size_t length;

    printf("Input: ");
    if (fgets(query, 256, stdin) == NULL || query[0] == '\n') {
        return 1;
    }
    length = strlen(query);
    if (query[length - 1] == '\n') {
        query[--length] = '\0';
    }

    FILE *fp = fopen("database.txt", "r");
    if (fp == NULL) {
        perror("Unable to open file!");
        exit(1);
    }

    char chunk[128];
    double time_spent = 0.0;
    clock_t begin_time = clock();

    const int num_threads = 4;
    pthread_t threads[num_threads];
    int active_threads = 0;

    while (fgets(chunk, sizeof(chunk), fp) != NULL) {
        CompareArgs *args = (CompareArgs *)malloc(sizeof(CompareArgs));
        strncpy(args->query, query, 256);
        strncpy(args->data, chunk, 128);
        args->threshold = 20;

        pthread_create(&threads[active_threads], NULL, Compare, args);
        active_threads++;

        if (active_threads >= num_threads) {
            for (int i = 0; i < active_threads; i++) {
                pthread_join(threads[i], NULL);
            }
            active_threads = 0;
        }
    }

    // Join any remaining threads
    for (int i = 0; i < active_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    fclose(fp);

    clock_t end_time = clock();
    time_spent += (double)(end_time - begin_time) / CLOCKS_PER_SEC;
    printf("Time elapsed is %f seconds", time_spent);

    return 0;
}
