build:
	mkdir -p build
	cd build && cmake .. && make

clean:
	rm -rf build
	rm -rf _packages
	rm -rf docs/doxygen
	rm -rf docs/build
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

# Build the current version of the documentation
docs: clean
	mkdir -p build
	cd build && cmake -DBUILD_DOCS=ON ..
	cd build && make doc
	sphinx-build docs docs/build/html
.PHONY: docs

# Build all versions of the documentation
docs-multiversion: clean
	@echo "Fetching latest branches from remote..."
	@git fetch --all
	@branches=$$(git branch -r | grep -Eo 'origin/v[0-9]+\.[0-9]+' | sed 's/origin\///'); \
	if [ -z "$$branches" ]; then \
		echo "No matching branches found."; \
		exit 1; \
	fi; \
	for branch in $$branches; do \
		git checkout "$$branch"; \
	done
	git checkout main
	@echo "Building documentation for all branches..."
	sphinx-multiversion -D 'exhale_args.containmentFolder=$${sourcedir}/api' docs docs/build/html
	@echo "<html><head><meta http-equiv=\"refresh\" content=\"0; url=main/index.html\" /></head></html>" > docs/build/html/index.html
.PHONY: docs-multiversion
