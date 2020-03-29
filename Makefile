default: cachetime

CC=g++
CFLAGS=--std=c++11 -fopenmp

cachetime: cachetime.cpp
	$(CC) $(CFLAGS) cachetime.cpp -o cachetime.out

clean:
	rm -f cachetime.out
