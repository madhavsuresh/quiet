LDFLAGS=-lcrypto
CC=clang
all:
	$(CC) -c enc.c  -o  enc.o
	$(CC) -c main.c -o main.o
	$(CC) enc.o main.o $(LDFLAGS) -o enc
tst:
	$(CC) -c enc.c -o enc.o
	$(CC) -c tst.c -o tst.o
	$(CC) tst.o enc.o $(LDFLAGS) -o tst
time:
	$(CC) -g -c enc.c -o enc.o
	$(CC) -c -g time.c -o time.o
	$(CC) -g time.o enc.o $(LDFLAGS) -o time 
clean:
	rm -f time 
	rm -f enc
	rm *.o
