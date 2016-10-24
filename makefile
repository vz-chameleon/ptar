
ptar : src/ptar.h src/ptar.o src/display.o src/main.o
	gcc -g src/ptar.o src/display.o src/main.o -o ptar -lm
	 
ptar.o : src/ptar.h src/ptar.c
	gcc -g -c src/ptar.c 

display.o : src/ptar.h src/display.c
	gcc -g -c src/display.c

main.o : src/ptar.h src/main.c
	gcc -g -c src/main.c


clean:
	rm -rf *.o

mrproper: clean
	rm -rf ptar
