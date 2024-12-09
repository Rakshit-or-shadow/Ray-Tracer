all: FS_assg MS2_assg MS1_assg

# executables
MS1_assg: assg.o vector.o spheres.o color.o 
	gcc -Wall -std=c99 -O2 -DMS1 -o MS1_assg src/assg.c src/vector.c src/spheres.c src/color.c -lm
	rm -f *.o 

MS2_assg: assg.o vector.o spheres.o color.o 
	gcc -Wall -std=c99 -O2 -DMS2 -o MS2_assg src/assg.c src/vector.c src/spheres.c src/color.c -lm
	rm -f *.o 

FS_assg: assg.o vector.o spheres.o color.o
	gcc -Wall -std=c99 -O2 -DFS -o FS_assg src/assg.c src/vector.c src/spheres.c src/color.c -lm
	rm -f *.o 
	
# object files
assg.o: src/assg.c src/assg.h src/color.h src/vector.h src/spheres.h
	gcc -Wall -std=c99 -c src/assg.c

vector.o: src/vector.c src/vector.h 
	gcc -Wall -std=c99 -c src/vector.c 

spheres.o: src/spheres.c src/spheres.h src/vector.h
	gcc -Wall -std=c99 -c src/spheres.c 

color.o: src/color.c src/color.h src/vector.h
	gcc -Wall -std=c99 -c src/color.c 

# Clean rule
clean:
	rm -f *.o MS1_assg MS2_assg FS_assg