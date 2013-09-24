all:
	clang -c enc.c -o enc.o
	clang -c main.c -o main.o
	clang enc.o main.o -o enc
tst:
	clang -c enc.c -o enc.o
	clang -c tst.c -o tst.o
	clang tst.o enc.o -o tst
time:
	clang -c enc.c -o enc.o
	clang -c time.c -o time.o
	clang time.o enc.o -o time
c:
	rm -f time
