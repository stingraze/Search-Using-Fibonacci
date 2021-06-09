#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
//Last Updated on June 9th, 2021 by Tsubasa Kato
int arr_prime[] = {1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377};

int length1;
int length2;

double time_spent = 0.0;

int Compare(char* query , char* data, int threshold)
{
	double time_spent = 0.0;
	clock_t begin_time = clock();

	int found = 0;
	int currentIndex = 3, j = 0, k = 0;
	int misValue = 0 , prim = arr_prime[currentIndex] , i = arr_prime[currentIndex];
	length1 = strlen(query);
	length2 = strlen(data);
	if(strcmp(query,data) == 0)
	{
		//printf("two strings are completely matched\n");
		return 1;
	}
	while(1)
	{

		//printf("Current index: %d\n", arr_prime[currentIndex]);
		while(i > length2)
		{
			//printf("Current index2: %d\n", arr_prime[currentIndex]);
			//i -= arr_prime[currentIndex--];
			i -= arr_prime[currentIndex--];
			if(currentIndex < 0)
			{
				//printf("two strings are not matched\n");
				return 0;
			}
			i += arr_prime[currentIndex];
		}
		for(j = i , k = 0; j < length2; j++)
		{
			if(query[k] == data[j])
			{
				if(k == length1 - 1)
				{
					found = 1;
					k = 0;
					
				}
				else
					k++;
			}
			else
			{
				k = 0;
				misValue++;
				//printf("miss value: %d\n", misValue);

			}
		}
		//Original: if(found && misValue <= threshold)
		int threshold2 = threshold * 2;
		if(found && misValue <= threshold2)
		{
			printf(" two strings are partially matched within threshold.\n");
			printf("%s", data);
			//clock_t end_time = clock();
			//time_spent += (double)(end_time - begin_time) / CLOCKS_PER_SEC;

			//printf("Time elpased is %f seconds", time_spent);
			return 1;
		}
		if(found){
			printf(" two strings were partially matched\n");
			printf("%s", data);
			//clock_t end_time = clock();
			
			//time_spent += (double)(end_time - begin_time) / CLOCKS_PER_SEC;

			//printf("Time elpased is %f seconds\n", time_spent);
			return 1;
		}


		found = 0;
		i += arr_prime[currentIndex];
	}
	printf("%s\n", query);
	printf("%s\n", data);
	printf("two strings are not matched\n");
	
	return 0;
}


int main(void){
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

    //printf("Output: %s\n", query);
    //printf("Length: %zu\n", length);

	FILE *fp = fopen("database.txt", "r");
	if (fp == NULL) {
		perror("Unable to open file!");
		exit(1);
	}
	char chunk[128];
	double time_spent = 0.0;
	clock_t begin_time = clock();

	while(fgets(chunk, sizeof(chunk), fp) != NULL){
		Compare(query,chunk,20);
	}
	
	clock_t end_time = clock();
	time_spent += (double)(end_time - begin_time) / CLOCKS_PER_SEC;
	printf(" Time elapsed is %f seconds", time_spent);

}




