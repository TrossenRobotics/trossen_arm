build:
	mkdir -p build
	cd build && cmake .. && make

clean:
	rm -rf build

build-demos:
	mkdir -p build
	cd build && cmake -DBUILD_DEMOS=ON .. && make

install: build
	cd build && make install

deb:
	mkdir -p build
	cd build && cmake ..
	cd build && make package
