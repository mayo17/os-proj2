make: memsim.o

memsim: memsim.o
	gcc -o memsim memsim.o -lrt

memsim.o: memsim.c
	gcc -c memsim.c -lrt
	
clean:
	rm -f memsim
	rm -f memsim.o