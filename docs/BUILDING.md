# Building the docs

## Current Version

To build the current version of the docs, run the following commands from the repository root:

```bash
# Create a virtual environment and install the required packages
$ python3 -m venv venv
$ source venv/bin/activate
# Install the Python packages required to build the docs
$ python3 -m pip install -r docs/requirements.txt
# Run the make target to build the docs
$ make docs
```

Once the build is complete, you can view the docs by opening `docs/build/html/index.html` in your browser.
For example:

```bash
$ firefox docs/build/html/index.html
```

## Building Multiversion Docs

Multiversion docs include docs for the main branch and branches matching the pattern `v*.*`.
To build multiversion docs, run the following commands from the repository root:

```bash
# Create a virtual environment and install the required packages
$ python3 -m venv venv
$ source venv/bin/activate
# Install the Python packages required to build the docs
$ python3 -m pip install -r docs/requirements.txt
# Run the make target to build the multiversion docs
$ make docs-multiversion
```

Once the build is complete, you can view the docs by opening `docs/build/html/main/index.html` in your browser.
For example:

```bash
$ firefox docs/build/html/main/index.html
```
