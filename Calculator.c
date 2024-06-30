/*
Name: Yash Kapoor
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#include <sys/msg.h>

#define MAX_TEXT 512
#define MICRO_SEC_IN_SEC 1000000

// Function signatures
void print_array(int *numbers, int size);
int insert_val(int *numbers, int size, int number_inserted);
int delete_val(int *numbers, int size, int number_removed);
int sum_value(int *numbers, int size);
int min_value(int *numbers, int size);
void sort_array(int *numbers, int size);
struct median_holder find_median(int *numbers, int size);

struct my_msg_st {
	long int my_msg_type;

	int number;

	char some_text[BUFSIZ];
};

struct median_holder {
	int even_values;
	int odd_values;

	int median1;
	int median2;
};

// Function Definitions
void print_array(int *numbers, int size) {
	for(int i = 0; i < size; i++) {
		printf("%d ", numbers[i]); 
		fflush(stdout);
	}
}

int insert_val(int *numbers, int size, int number_inserted) {
	// value inserted after the current last number in the array

	numbers[size] = number_inserted;
	size++;

	return size;
}

int delete_val(int *numbers, int size, int number_removed) {

	int i = 0, total_size = 0;

	// all occurrences of the number are deleted after iterating through array
	for(i = 0; i < size; i++) {
		if(numbers[i] != number_removed) {
			// excluding the number from the array, so it gets removed 
			// adding all other elements besides the number to be removed
			numbers[total_size++] = numbers[i];
		}
	}

	return total_size;
}

int sum_value(int *numbers, int size) {
	int sum = 0;

	// Adding all the integers in the array together
	for(int add = 0; add < size; add++) {
		sum = sum + numbers[add];
	}

	return sum;
}

int min_value(int *numbers, int size) {
	int min = numbers[0];
		
	// finding the minimum value in the array
	for(int i = 1; i < size; i++) {
		if (min >  numbers[i]) {
			min = numbers[i];
		}
	}

	return min;
}

void sort_array(int *numbers, int size) {
	int i = 0, j = 0, temp = 0;

	// sorting the array in non-decreasing order
	for(i = 0; i < size; i++) {
		for(j = 0; j < (size - 1); j++) {
			if(numbers[j] > numbers[j + 1]) {
			// swap numbers if value at index j is greater than value at index j + 1
				temp = numbers[j];
				numbers[j] = numbers[j + 1];
				numbers[j + 1] = temp;
			}
		}
	}

}

struct median_holder find_median(int *numbers, int size) {

	struct median_holder mh;

	mh.even_values = 0;
	mh.odd_values = 0;

	if(size % 2 == 0) {
		mh.even_values = 1;
		// even number of values

		mh.median1 = numbers[(size - 1) / 2];
		mh.median2 = numbers[size / 2];
	}
	else {
		mh.odd_values = 1;
		// odd number of values

		mh.median1 = numbers[size / 2];
	}

	return mh;

}

int main() {
	int running = 1;

	int user_message;
	int calculator_message;

	struct my_msg_st some_data;
	struct timeval start, end;

	long int msg_to_receive = 0;

	int count = 0;
	// count is the current array size

	int MAX_SIZE = 15;
	int numbers[15];
	
	double time_insert, time_delete, time_sum, time_avg, time_min, time_median;

	// creating message queue
	calculator_message = msgget((key_t)2222, 0666 | IPC_CREAT);

	// obtaining message queue 
	user_message = msgget((key_t)1234, 0666 | IPC_CREAT);

	if (calculator_message == -1) {
		fprintf(stderr, "msgget failed with error: %d\n", errno);
		exit(EXIT_FAILURE);
	}
	
	int inserted = 0, deleted = 0, added = 0, averaged = 0, found_min = 0, found_median = 0;
	int done_all_operations = 0;

	while(running) {

		// messages retrieved from queue
		if (msgrcv(user_message, (void *)&some_data, BUFSIZ,
			msg_to_receive, 0) == -1) {

			fprintf(stderr, "msgrcv failed with error: %d\n", errno);
			exit(EXIT_FAILURE);
		}

		if (strncmp(some_data.some_text, "end", 3) == 0) {
			// ending the receiver process once the user has typed "end"
			running = 0;
		}

		else if (strncmp(some_data.some_text, "time", 4) == 0) {

			if(done_all_operations) {
				double TOTAL_NUM_OPERATIONS = 6.0;
				double average_time = (time_insert + time_delete + time_sum + time_avg + time_min + time_median) / TOTAL_NUM_OPERATIONS;

				printf("\nThe average time to perform the operations is:\n%lf micro sec", average_time);
				fflush(stdout);

				strcpy(some_data.some_text, "The average time to perform all the operations has been calculated!");
			}
			else {
				strcpy(some_data.some_text, "All operations must be done before attempting to use this command.");
			}
		}

		else if (strncmp(some_data.some_text, "insert", 6) == 0) {

			if(count != MAX_SIZE) {

				inserted = 1;
		
				int number_to_insert = some_data.number;

				gettimeofday(&start, NULL);
				count = insert_val(numbers, count, number_to_insert);
				gettimeofday(&end, NULL);

				printf("\nNumbers in your set are: \n");
				print_array(numbers, count);
			
				strcpy(some_data.some_text, "Your number was added to the set of numbers!");
			}
			else {
				strcpy(some_data.some_text, "Your number was not added to the set of numbers because your set is only allowed to have up to 15 numbers!");
			}

			// time it takes to insert an integer in the array
			time_insert = (end.tv_sec * MICRO_SEC_IN_SEC + end.tv_usec) - (start.tv_sec * MICRO_SEC_IN_SEC + start.tv_usec);

		}

		else if (strncmp(some_data.some_text, "delete", 6) == 0) {

			int number_to_delete = some_data.number;
			
			int temp = count;

			gettimeofday(&start, NULL);
			count = delete_val(numbers, temp, number_to_delete);
			gettimeofday(&end, NULL);

			// time it takes to delete an integer from the array
			time_delete = (end.tv_sec * MICRO_SEC_IN_SEC + end.tv_usec) - (start.tv_sec * MICRO_SEC_IN_SEC + start.tv_usec);

			if (count != 0) {
				printf("\nNumbers in your set are: \n");
				print_array(numbers, count);
			}
			else {
				printf("\nThe set of numbers is empty.\n");
			}

			if (temp == count) {
				// size has not changed, so element not found
				strcpy(some_data.some_text, "Your number was not found in the set of numbers!"); 	
			}
			else {
				// size changed, so element found and deleted
				deleted = 1;

				strcpy(some_data.some_text, "Your number was deleted from the set of numbers!");
			}
			
		}

		else if (strncmp(some_data.some_text, "sum", 3) == 0) {

			gettimeofday(&start, NULL);
			int sum = sum_value(numbers, count);
			gettimeofday(&end, NULL);

			// time it takes to sum all the integers in the array
			time_sum = (end.tv_sec * MICRO_SEC_IN_SEC + end.tv_usec) - (start.tv_sec * MICRO_SEC_IN_SEC + start.tv_usec);

			if(count != 0) {

				added = 1;

				printf("\nThe sum of all existing numbers in the set of numbers is: %d", sum); 
				fflush(stdout);

				strcpy(some_data.some_text, "The sum of all existing numbers in the stored set of numbers was computed!");
			}
			else {
				strcpy(some_data.some_text, "You have not added any numbers in your array!");
			}
	
		}

		else if (strncmp(some_data.some_text, "average", 7) == 0) {

			gettimeofday(&start, NULL);
			float avg = (float) sum_value(numbers, count) / (float) count;
			gettimeofday(&end, NULL);

			// time it takes to find the average of all integers in the array
			time_avg = (end.tv_sec * MICRO_SEC_IN_SEC + end.tv_usec) - (start.tv_sec * MICRO_SEC_IN_SEC + start.tv_usec);

			if(count != 0) {
				averaged = 1;

				printf("\nThe average of all existing numbers in the set of numbers is: %0.3f", avg); 
				fflush(stdout);

				strcpy(some_data.some_text, "The average of all existing numbers in the stored set of numbers was computed!");
			}

			else {
				strcpy(some_data.some_text, "You have not added any numbers in your array!");
			}

		}

		else if (strncmp(some_data.some_text, "min", 3) == 0) {
			
			gettimeofday(&start, NULL);
			int min = min_value(numbers, count);
			gettimeofday(&end, NULL);

			// time it takes to find the minimum value in the array
			time_min = (end.tv_sec * MICRO_SEC_IN_SEC + end.tv_usec) - (start.tv_sec * MICRO_SEC_IN_SEC + start.tv_usec);

			if(count != 0) {

				found_min = 1;

				printf("\nThe smallest number in the stored set of numbers is: %d", min); 
				fflush(stdout);
				

				strcpy(some_data.some_text, "The smallest number in the stored set of numbers was found!");
			}
			else {
				strcpy(some_data.some_text, "You have not added any numbers in your array!");
			}

		}

		else if (strncmp(some_data.some_text, "median", 6) == 0) {
			// calculate median
	
			gettimeofday(&start, NULL);
			sort_array(numbers, count);
			struct median_holder mh = find_median(numbers, count);
			gettimeofday(&end, NULL);
			
			time_median = (end.tv_sec * MICRO_SEC_IN_SEC + end.tv_usec) - (start.tv_sec * MICRO_SEC_IN_SEC + start.tv_usec);

			if(count != 0 && mh.even_values) {
				// array contains even number of elements and median found

				found_median = 1;	

				printf("\nSorted List is: \n");
				print_array(numbers, count);

				printf("\nThe median values in this set of numbers are: %d and %d", mh.median1, mh.median2);

				fflush(stdout);

				strcpy(some_data.some_text, "The median values in this set of numbers were found!");

			}
			else if (count != 0 && mh.odd_values) {
				// array contains odd number of elements and median found

				found_median = 1;

				printf("\nSorted List is: \n");
				print_array(numbers, count);

				printf("\nThe median value in this set of numbers is: %d", mh.median1);

				fflush(stdout);

				strcpy(some_data.some_text, "The median value in this set of numbers was found!");

			}
			else {
				strcpy(some_data.some_text, "You have not added any numbers in your array!");
			}
		}
		else {
			fputs("\nInvalid command\n", stderr);
		}

		// sending an appropriate message back to the user based on the
		// command they entered
		if (msgsnd(calculator_message, (void *)&some_data, MAX_TEXT, 0) == -1) {
			fprintf(stderr, "msgsnd failed\n");
			exit(EXIT_FAILURE);
		}

		if(inserted && deleted && added && averaged && found_min && found_median) {
			done_all_operations = 1;
		}
	
	}

	// deleting the message queues
	if (msgctl(user_message, IPC_RMID, 0) == -1) {
		fprintf(stderr, "msgctl(IPC_RMID) failed\n");
		exit(EXIT_FAILURE);
	}


	if (msgctl(calculator_message, IPC_RMID, 0) == -1) {
		fprintf(stderr, "msgctl(IPC_RMID) failed\n");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);

}
