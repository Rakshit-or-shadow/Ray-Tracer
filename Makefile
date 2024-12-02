all: MS1 MS2 FS

# Executables
MS1: src/assg.o src/vector.o src/spheres.o 
	gcc -Wall -std=c99 -DMS1 -o MS1 src/assg.o src/vector.o src/spheres.o -lm

MS2: src/assg.o src/vector.o src/spheres.o src/color.o
	gcc -Wall -std=c99 -DMS2 -o MS2 src/assg.o src/vector.o src/spheres.o src/color.o -lm


# Object files
assg.o: src/assg.c src/assg.h
	gcc -Wall -std=c99 -DMS2 -c src/assg.c

vector.o: src/vector.c src/vector.h
	gcc -Wall -std=c99 -DMS2 -c src/vector.c

spheres.o: src/spheres.c src/sphere.h src/vector.h
	gcc -Wall -std=c99 -DMS2 -c src/spheres.c

color.o: src/color.c src/color.h src/vector.h
	gcc -Wall -std=c99 -DMS2 -c src/color.c

clean: rm -f *.o MS1 MS2 FS