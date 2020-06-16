trivial:
	mkdir -p bin
	gcc src/trivial.c -o bin/trivial

clean:
	rm bin/*