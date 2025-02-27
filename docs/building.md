# Building the docs

To build the docs, run the following commands:

```bash
$ python3 -m venv venv
$ source venv/bin/activate
$ python3 -m pip install -r requirements.txt
$ make docs
```

Once the build is complete, you can view the docs by opening `docs/_build/html/index.html` in your browser.
For example:

```bash
$ firefox docs/_build/html/index.html
```
