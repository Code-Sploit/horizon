all:
	make clean
	make compile
	make tests

clean:
	rm -rf build

compile:
	scripts/compile

tests:
	scripts/tests

install:
	cp build/horizon /usr/local/bin/