build:
	mkdir -p build
	cd build && cmake .. && make

clean:
	rm -rf build
	rm -rf _packages
	rm -rf docs/doxygen
	rm -rf docs/_build
	rm -rf docs/api
.PHONY: clean

build-demos:
	mkdir -p build
	cd build && cmake -DBUILD_DEMOS=ON .. && make

install: build
	cd build && make install

deb:
	mkdir -p build
	cd build && cmake ..
	cd build && make package

docs: clean
	mkdir -p build
	cd build && cmake -DBUILD_DOCS=ON ..
	cd build && make doc
	cd docs && make html
.PHONY: docs
