.default: all

all: pfind

pfind: pfind.c pfind.h
	gcc pfind.c -o pfind

run:
	./project3

val:
	valgrind --leak-check=full --tool=memcheck ./pfind

clean:
	$(RM) *~ *.o *.gch *.out pfind
