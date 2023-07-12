
/*******************************************************************************
 * Name       : mtsieve.c
 * Author     : Julia Nelson
 * Date       : 7/1/2021
 * Description:  Multithreaded Primes Sieve
 * Pledge     : "I pledge my honor that I have abided by the Stevens Honor System."
 ******************************************************************************/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <getopt.h>
#include <ctype.h>
#include <sys/sysinfo.h>


int total_count = 0;
pthread_mutex_t lock;

 /* DESCRIPTION ------------

	This assignment requires you to implement the segmented sieve of Eratosthenes to find prime
	numbers having two or more digits that are ‘3’. 

	Unlike the standard sieve of Eratosthenes, that operates over the range [2, b], the segmented 
	version can start at an arbitrary lower bound a, working over the range [a, b]. 

	Since a wide range of values can be programmed in segments, this algorithm lends itself nicely 
	to a multithreaded approach.
 */


 /* VirtualBox SETTINGS ------------

	Before starting this assignment, increase the number of processors available to the virtual
	machine. Drag the slider up to the end of the green segment. The value shown at the end of the
	green segment indicates the number of true cores (not threads) available on your CPU
 */





/* ALGORITHM ------------

  -> Segmented Sieve of Eratosthenes...
	
	 Purpose: Used to find primes in range [a, b].
	 Steps:
		1. Use standard sieve of Eratosthenes to find all primes up to and including 
			sqrt(b), call them low_primes[]

		2. Create a boolean array high_primes[] with length = b - a + 1 and
			initialize each element to true

		3. for each prime p in low_primes[],
 				set i = ceil((double)a/p) * p - a
 				if a <= p
 					set i = i + p
 				starting at i, cross off all multiples of p in high_primes[]

		4. for each high_primes[i] that is true, print i + a
*/



typedef struct arg_struct {
	int start;
	int end; 
} thread_args;


// find primes from 2 to sqrt(high)
int fiindPrimes(int* prime, int b)
{
	int num = (int)(sqrt(b));
	bool *mark = (bool *) malloc((num+1) * sizeof(bool));
	memset(mark, true, (num+1) * sizeof(bool));
    for (int i = 2; i <= num; i++) {
        if (mark[i]) {
            for (int j = i * i; j <= num; j = j + i) {
                mark[j] = false;
            }
        }
    }
	int cnt = 0;
    for (int i = 2; i <= num; i++) {
        if (mark[i]) {
            prime[cnt++] = i;
        }
    }
	free(mark);
	return cnt;
}

bool check3digits(int value)
{
    int digit;
    int cnt = 0;
    while (value > 0) {
        digit = value % 10;
        if (digit == 3)
            cnt++;
        value /= 10;
        if (cnt == 2)
            return true;
    }
    return false;
}

// check prime from range [low, high]
void segmentedSieve(int a, int b)
{
 	int num = (int)(sqrt(b));
	int *low_primes = (int *) malloc(num * sizeof(int));
	bool *high_primes = (bool *) malloc((b - a + 1) * sizeof(bool));
    memset(high_primes, true, (b - a + 1) * sizeof(bool));
    int low_cnt = fiindPrimes(low_primes, b);
    
    for (int i = 0; i < low_cnt; i++) 
	{
		int p = low_primes[i];
        int lower = ceil((double)a / p) * p - a;
        if (a <= p)
            lower += p;
        
        for (int j = lower; j <= b-a; j = j + p) {
            high_primes[j] = false;
        }
    }
    for (int i = 0; i <= b-a; i++) {
        if (high_primes[i]) {
            //printf("%d ", i);
            if (check3digits(i + a)) {
                pthread_mutex_lock(&lock);
                total_count++;
                pthread_mutex_unlock(&lock);
            }
        }
    }
    free(high_primes);
    free(low_primes);
}

bool checkValid(char *str, char c, int* val)
{
	char *ptr;
	*val = strtol(str, &ptr, 10);
	if (*ptr != '\0')
	{
		printf("Error: Invalid input '%s' received for parameter '-%c'.\n", str, c);		
		return false;
	}
	int len = strlen(str);
	int ret;
	if (*val == 0)
		ret = 1;
	else if (*val > 0)
	 	ret = floor(log10(*val)) + 1;
	else
		ret = floor(log10(abs(*val))) + 2;
	if (len != ret)
	{
		printf("Error: Integer overflow for parameter '-%c'.\n", c);
		return false;
	}
	return true;
}


void *prime_routine(void * arg)
{
    thread_args* values = (thread_args*)arg;
    segmentedSieve(values->start, values->end);
    pthread_exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
	if (argc == 1)
	{
		printf("Usage: ./mtsieve -s <starting value> -e <ending value> -t <num threads>\n");
		return EXIT_FAILURE;
	}
	int opt, a, b, thread_num;
	bool aflag, bflag, tflag;
	bool ret;
	aflag = bflag = tflag = false;
	while((opt = getopt(argc, argv, "s:e:t:")) != -1) 
    { 
        switch(opt) 
        { 
            case 's':
				ret = checkValid(optarg, opt, &a);
				aflag = true;
				if (!ret)
					return EXIT_FAILURE;
				break;
			case 'e':
				ret = checkValid(optarg, opt, &b);
				bflag = true;
				if (!ret)
					return EXIT_FAILURE;
				break;
			case 't':
				ret = checkValid(optarg, opt, &thread_num);
				tflag = true;
				if (!ret)
					return EXIT_FAILURE;
				break;
            case '?':
				if (optopt == 'e' || optopt == 's' || optopt == 't') 
				{
					printf("Error: Option -%c requires an argument.\n", optopt);
					// no clue why its printing twice for this 
				} 
				else if (isprint(optopt)) 
				{         
					printf("Error: Unknown option '-%c'.\n", optopt);
				} 
				else 
				{
					printf("Error: Unknown option character '\\x%x'.\n", optopt);
				}
				return EXIT_FAILURE;
        }
    }
    if (!aflag)
    {
        printf("Error: Required argument <starting value> is missing.\n");
        return EXIT_FAILURE;
    }
    if (a < 2)
    {
        printf("Error: Starting value must be >= 2.\n");
        return EXIT_FAILURE;
    }
    if (!bflag)
    {
        printf("Error: Required argument <ending value> is missing.\n");
        return EXIT_FAILURE;
    }
    if (b < 2)
    {
        printf("Error: Ending value must be >= 2.\n");
        return EXIT_FAILURE;
    }
    if (b < a)
    {
        printf("Error: Ending value must be >= starting value.\n" );
        return EXIT_FAILURE;
    }
    if (!tflag)
    {
        printf("Error: Required argument <num threads> is missing.\n");
        return EXIT_FAILURE;
    }
    if (thread_num < 1)
    {
        printf("Error: Number of threads cannot be less than 1.\n");
        return EXIT_FAILURE;
    }
    int n_proc = get_nprocs();
    if (thread_num > 2 * n_proc)
    {
        printf("Error: Number of threads cannot exceed twice the number of processors(%d).\n", n_proc);
        return EXIT_FAILURE;
    }
    
	//printf("%d, %d, %d\n", a, b, thread_num);
	int total_numbers = b - a + 1;
	if (thread_num > total_numbers)
	    thread_num = total_numbers;
	
	int i, num_per_thread = total_numbers / thread_num;
	int remaind = total_numbers - num_per_thread * thread_num;
	int value = a;
	thread_args *thread_values = (thread_args *)malloc(thread_num * sizeof(thread_args));
	pthread_t *thread_ids = (pthread_t *)malloc(thread_num * sizeof(pthread_t));
	
	// initialize segments
	printf("Finding all prime numbers between %d and %d.\n", a, b);
	printf("%d segments:\n", thread_num);
	for (i = 0; i < remaind; i++)
	{
	    thread_values[i].start = value;
	    value += (num_per_thread + 1);
	    thread_values[i].end = value - 1;
	    printf("[%d, %d]\n", thread_values[i].start, thread_values[i].end);
	}
	for (; i < thread_num; i++)
	{
	    thread_values[i].start = value;
	    value += num_per_thread;
	    thread_values[i].end = value - 1;
	    printf("[%d, %d]\n", thread_values[i].start, thread_values[i].end);
	}
	
	for (i = 0; i < thread_num; i++)
	{
	    int rc = pthread_create(&thread_ids[i], NULL, prime_routine, (void*)&thread_values[i]);
	    if (rc) {
			printf("ERROR; return code from pthread_create() (consumer) is %d\n", rc);
			return EXIT_FAILURE;
		}
	}
	for (i = 0; i < thread_num; i++) 
    {
		int rc = pthread_join(thread_ids[i], NULL);
		if (rc) {
			printf("ERROR; return code from pthread_join() (consumer) is %d\n", rc);
			return EXIT_FAILURE;
		}
    }
    free(thread_ids);
	free(thread_values);
	int rc = pthread_mutex_destroy(&lock);
	if (rc) {
	    printf("ERROR; return code from pthread_mutex_destroy() (consumer) is %d\n", rc);
		return EXIT_FAILURE;
	}
	
	printf("Total primes between %d and %d with two or more '3' digits: %d\n", a, b, total_count);
	
	
	return 0;
}



















































