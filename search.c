#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
// create a fixed number of searcher threads
#define NUM_THREADS 100

void *searcher(void *); 

/* add global mailboxes for thread communication */

// location of beginning of pattern -- all threads quit when non-negative
int location = -1;
// thread counter
int threadNext = 0; 
// positions searched counter
int visited = 0; 
// the text array
char *text;
// the pattern
char *pattern;
// length of text string
int textSize;
// length of pattern string
int patternSize;
// init lock variable to prevent race conditions
pthread_mutex_t lock;

// suppress "no return" warning
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
int main(int argc, char *argv[]) {
	// first argument is the pattern string
	pattern = argv[1];
	patternSize = strlen(pattern);
	// second argument is the size of the array that will hold the text string
	textSize = atoi(argv[2]);

	// allocate space for the text array and read the text characters into it
	text = (char *)malloc(textSize + 1);
	int n = 0;
	// then read characters from the standard input, storing them in the text array 
	// until either an EOF is found or the array has been filled
	while (n < textSize) {
		int status = scanf("%c", &text[n]);
		n++;
		if (status == EOF) {
			textSize = n;
			break;
		}
	}
	// add end line char at end
	text[n] = '\0';

	// lock critical section
    pthread_mutex_init(&lock, NULL);
	// create thread id
    pthread_t tid[NUM_THREADS];
    
	// thread increment
	int t;
    for (t = 0; t < NUM_THREADS; t++) {
        int status = pthread_create(&tid[t], NULL, searcher, NULL);
	}

	// wait for the searcher threads to finish
	for (t=0; t < NUM_THREADS; t++)
        pthread_join(tid[t], NULL);

	// location of pattern found in text
	if (location >= 0)
		// print the result
		printf("\nPattern begins at character %d\n", location);
	else
		printf("Pattern not found\n");
}

// behavior of a searcher thread
void *searcher(void *arg) {
	while (1) {
		// lock critical section
		pthread_mutex_lock(&lock);

		// pick an unexamined starting position
        int h = threadNext;
		// repeat until either the pattern is found (by this thread or another one),
		// or there are no more unexamined starting positions
		if (location >= 0 || threadNext > textSize-patternSize) {
				pthread_mutex_unlock(&lock);
			    break;
		}
		threadNext++;

		pthread_mutex_unlock(&lock);  
		// end of critical section
		// found boolean val
		int found = 1;
		// counter
		int k;
		for (k = 0; k < patternSize; k++) {
			if (text[h + k] != pattern[k])
				found = 0;
		}

		// second critical section
 	    pthread_mutex_lock(&lock);  

	    visited++;
        if (found)
   	 	   location = h+1;

	   // end section, unlock
	   pthread_mutex_unlock(&lock); 
	}
}
#pragma GCC diagnostic pop
