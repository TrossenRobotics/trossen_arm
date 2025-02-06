clean:
	rm -rf build

build:
	mkdir -p build
	cd build && cmake .. && make

install: build
	cd build && make install
