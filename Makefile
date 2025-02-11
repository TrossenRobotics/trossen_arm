clean:
	rm -rf build

build:
	mkdir -p build
	cd build && cmake .. && make

build-demos:
	mkdir -p build
	cd build && cmake -DBUILD_DEMOS=ON .. && make

install: build
	cd build && make install
