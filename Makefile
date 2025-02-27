build:
	mkdir -p build
	cd build && cmake .. && make

clean:
	rm -rf build

build-demos:
	mkdir -p build
	cd build && cmake -DBUILD_DEMOS=ON .. && make

install-demos: build-demos
	cd build && make install

install: build
	cd build && make install

deb:
	mkdir -p build
	cd build && cmake ..
	cd build && make package

docker-build: clean
	docker build -f .docker/Dockerfile -t libtrossen_arm:latest .
.PHONY: docker-build

docker-run: docker-build
	docker run --net=host -it --rm --privileged libtrossen_arm:latest
