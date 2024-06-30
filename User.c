/*
Name: Yash Kapoor
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <ctype.h>

#include <sys/msg.h>

#define MAX_TEXT 512

struct my_msg_st {
	long int my_msg_type;

	int number;
	char some_text[MAX_TEXT];

};

// Function signatures
void send_message(int msgid, struct my_msg_st some_data, int size);
void receive_message(int msgid, struct my_msg_st some_data, int buffer_size, long int msg_to_receive);
int handle_user_input();

// Function Definitions
void send_message(int msgid, struct my_msg_st some_data, int size) {
	if (msgsnd(msgid, (void *)&some_data, size, 0) == -1) {
		fprintf(stderr, "msgsnd failed\n");
		exit(EXIT_FAILURE);
	}
}

void receive_message(int msgid, struct my_msg_st some_data, int buffer_size, long int msg_to_receive) {
	if (msgrcv(msgid, (void *)&some_data, buffer_size,
		msg_to_receive, 0) == -1) {

		fprintf(stderr, "msgrcv failed with error: %d\n", errno);
		exit(EXIT_FAILURE);			
	}

	printf("%s", some_data.some_text);
}

int handle_user_input() {
	float val = 0;
	
	while(val >= 0) {
		// keep prompting user for input until they enter an integer value

		int store_return;

		printf("Enter a number: ");
		store_return = scanf(" %f", &val);

		int integer_value = (int) val;

		if(store_return == EOF) {
			// error occurs before any values get stored
			fputs("You must enter an integer.\n", stderr);
			break;
		}
		else if (store_return == 0) {
			fputs("You must enter an integer.\n", stderr);
			int c = getchar();

   			while (c != '\n' && c != EOF) {
				// removing characters that are stored in val
       				c = getchar();
			}
		}
		else {
			if (integer_value == val) {
				// valid integer value
				return val;
			} 
			else {
				// float entered rather than integer value
				fputs("You must enter an integer.\n", stderr);
			}
		}
	}

}
int main() {
	int running = 1;

	struct my_msg_st some_data;

	int user_message;
	int calculator_message;

	long int msg_to_receive = 0;

	char buffer[BUFSIZ];	
	int inserted_number = 0;
	int deleted_number = 0;

	// creating message queue
	user_message = msgget((key_t)1234, 0666 | IPC_CREAT);

	// obtaining message queue
	calculator_message = msgget((key_t)2222, 0666 | IPC_CREAT);

	if (user_message == -1) {
		fprintf(stderr, "msgget failed with error: %d\n", errno);
		exit(EXIT_FAILURE);
	}

	while(running) {

		printf("\n\nEnter command: "); 

		scanf(" %s", &buffer); 
		fflush(stdout);

		for(int lowercase = 0; buffer[lowercase]; lowercase++) 	{
			// converting user input to lowercase
			buffer[lowercase] = tolower(buffer[lowercase]);
		}

		strcpy(some_data.some_text, buffer);

		if (strncmp(buffer, "end", 3) == 0) {
			// ending the "user" process once the user has typed "end"
			running = 0;

			send_message(user_message, some_data, MAX_TEXT);
			
		}

		else if (strncmp(buffer, "time", 4) == 0) {
			// sending message to the calculator (server)
			// then receiving appropriate message back 

			send_message(user_message, some_data, MAX_TEXT);
			
			receive_message(calculator_message, some_data, BUFSIZ,
			msg_to_receive);

		}

		else if (strncmp(buffer, "insert", 6) == 0) {

			inserted_number = handle_user_input();

			some_data.number = inserted_number;

			send_message(user_message, some_data, MAX_TEXT);
			

			receive_message(calculator_message, some_data, BUFSIZ,
			msg_to_receive);
			
			
		}

		else if (strncmp(buffer, "delete", 6) == 0) {

			deleted_number = handle_user_input();
			
			some_data.number = deleted_number;

			send_message(user_message, some_data, MAX_TEXT);
			
			receive_message(calculator_message, some_data, BUFSIZ,
			msg_to_receive);
			

		}

		else if (strncmp(buffer, "sum", 3) == 0) {

			send_message(user_message, some_data, MAX_TEXT);
			
			receive_message(calculator_message, some_data, BUFSIZ,
			msg_to_receive);
		}

		else if (strncmp(buffer, "average", 7) == 0) {

			send_message(user_message, some_data, MAX_TEXT);
			
			receive_message(calculator_message, some_data, BUFSIZ,
			msg_to_receive);

		}

		else if (strncmp(buffer, "min", 3) == 0) {

			send_message(user_message, some_data, MAX_TEXT);
			
			receive_message(calculator_message, some_data, BUFSIZ,
			msg_to_receive);
		}

		else if (strncmp(buffer, "median", 6) == 0) {

			send_message(user_message, some_data, MAX_TEXT);
			
			receive_message(calculator_message, some_data, BUFSIZ,
			msg_to_receive);
		}

		else {
			printf("You must enter a valid command! Try again!");
		}

	}

	exit(EXIT_SUCCESS);
}
