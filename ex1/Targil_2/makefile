all: ex2

# Make main programs
# Link the object file with the file contains the functions and math library:
ex2: pythagorean_triangle.o main_pt.o
	gcc -o ex2 pythagorean_triangle.o main_pt.o -lm

# Compile pythagorean_triangle.c into pythagorean_triangle.o
pythagorean_triangle.o: pythagorean_triangle.c pythagorean_triangle.h
	gcc -c pythagorean_triangle.c

# Compile main_pt.c into main_pt.o
main_pt.o: main_pt.c
	gcc -c main_pt.c

.PHONY: all clean

clean: 
	rm -f *.o ex2 
