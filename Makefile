all: compile

compile:
	gcc -o main -I headers/ src/*.c

clean:
	rm -f main
