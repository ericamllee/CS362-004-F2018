CFLAGS= -Wall -fpic -coverage -lm -std=c99

testme: testme.c
	gcc -o testme testme.c -g $(CFLAGS)

test:
	echo "Running testme"
	make testme
	./testme

clean:
	rm -f *.o *.dSYM *.gcda *.gcno