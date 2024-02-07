all:
	make clean
	make prepare
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

prepare:
	chmod +x scripts/compile
	chmod +x scripts/tests
