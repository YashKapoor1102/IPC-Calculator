all: Calculator User

Calculator: Calculator.o
	gcc Calculator.o -o calc

User: User.o
	gcc User.o -o user

Calculator.o: Calculator.c 
	gcc -c Calculator.c 

User.o: User.c 
	gcc -c User.c

clean:
	rm *.o user calc
