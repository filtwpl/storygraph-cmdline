all: main.c storygraph.c storygraph.h
	gcc -Wall -Werror -g -o storygraph main.c storygraph.c -lm

clean:
	rm -f storygraph storygraph.exe