# Inter-process Communication Calculator

## Description

This program implements a calculator system using the client-server paradigm. It consists of two
processes: User and Calculator, which run on the same CPU. The user is responsible for sending
commands to the calculator. Then, the calculator checks which command the user entered and returns
the appropriate message to the user. Specifically, the calculator stores the set of integers, which
the user inputs, in an array. Based on the command the user sends to the calculator, the
calculator then performs operations on this set of numbers. Two message queues are used. One 
sends data from the user to the calculator and the other sends data from the calculator to 
the user.

There are a total of 8 commands that the user can enter, which the calculator deals with:

**Insert:** Inserts an integer in the array
**Delete:** Deletes an integer from the array
**Sum:** Adds all the integers in an array together and returns the result
**Average:** Finds the average of all the integers in the array and returns the result
**Min:** Finds and returns the minimum integer in the array
**Median:** Sorts the array in non-decreasing order and returns two values in the middle 
	if the number of values in the set are even. Otherwise, returns one value if 
	the number of values in the set are odd.
**Time:** You must type all those commands listed above first for you to use this command. This is 
      responsible for computing the average time required to perform the operations listed
      above. 
**End:** Deletes the message queues and exits the program. This must be entered after you are 
     done using this program or before you re-run it. 

  
This program is made up of two files:<br><br>
&nbsp;&nbsp;&nbsp;&nbsp;**User.c**: 		Client process: responsible for sending commands to the calculator<br>
&nbsp;&nbsp;&nbsp;&nbsp;**Calculator.c**: 	Server: responsible for storing and performing operations on a set of numbers<br><br>

&nbsp;&nbsp;&nbsp;&nbsp;**Makefile**:	Allows the user to compile and run the program

## Installation

Linux Operating System is required. 

If any other operating system (e.g., Windows, MAC OS) is being used, Oracle VM VirtualBox can be used to run Linux.

If Linux is not already installed on your operating system, use the following link to install it:

https://www.instructables.com/How-to-install-Linux-on-your-Windows/

## Usage
**Step 1:** Clone the repository to your local machine:
```
$ git clone https://github.com/YashKapoor1102/Interprocess-Communication-Calculator.git
```

**Step 2:** Ensure two "Terminal" windows are opened.

**Step 3:** Go to the folder where you cloned the repository by typing the following command:
```
$ cd Lexicographic-Sort-Concurrency
```

**Step 4:** Once you are in the folder, type the following command to compile all the source files:

	$ make

**Step 5:** To run the program, type the following command in the first terminal window:

	$ ./user

	It will then ask you to enter one out of the 7 commands listed above. However, before 
	entering a command, type the following in the second terminal window:

	$ ./calc

**Step 6:** To start using this program, type the "Insert" command to insert numbers one by one
	to your set of numbers. Simply enter a number you want to add (must be an integer)
	and the calculator shall do the rest for you! 

**Step 7:** After a few numbers are added to your set of numbers, you can begin using the rest
	of the commands that are listed above (e.g., Delete, Sum, Average). Using these 
	commands will give you more information about your set of numbers.

**Step 8:** After you have tested every operation (i.e., Insert, Delete, Average, Min, Median), you 
	can type the "Time" command to get the average time it takes to perform these
	operations.

**Step 9 (Very Important!):** After you are done using this program, make sure you type "end" 
			  as a command to ensure that both message queues get deleted. Otherwise, 
			  if you try to compile the program again, unusual behavior may occur. 
			  Hence, it is very important to type "end", rather than stopping the 
			  program forcefully. 

			  However, if you forget to type "end", simply re-compile the program
			  by repeating Step 5. Then, type "end" as a command once the user 
			  asks you to input a command. This will ensure all message queues 
			  are deleted successfully. 

			  Afterwards, if you want to run the program again, repeat steps 5-8. 


Once you are finished with this application, you can type the following command in the terminal to clean up your files:
```
$ make clean
```

## Credits
- Yash Kapoor 
