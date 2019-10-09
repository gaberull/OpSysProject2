all: project2 server

project2.o: project2.c 
	gcc -c project2.c  

project2: project2.o storage_remote.o
	gcc project2.o storage_remote.o -o project2

storage_remote.o: storage_remote.c storage_remote.h
	gcc -c storage_remote.c

server.o: storage.h comm.h storage_common.h
	gcc -c server.o

server: server.o
	gcc server.o -o server

pipes: pipe_in pipe_out

pipe_in: 
	mkfifo pipe_in

pipe_out: 
	mkfifo pipe_out

clean:
	rm -f *.o project2
