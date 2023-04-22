# TinyTP

![CI](https://github.com/delsner/tinytp/actions/workflows/build.yml/badge.svg)
[![Coverage Status](https://coveralls.io/repos/github/delsner/tinytp/badge.svg)](https://coveralls.io/github/delsner/tinytp)


TinyTP is a tiny program for test prioritization.

## Structure

The project has the following structure:

```
├── cmake           <- Internal cmake configuration files.
├── external        <- Third-party libraries.
├── include         <- TinyTP headers.
├── test            <- Unit tests.
└── tinytp          <- TinyTP sources. 
```

## Usage

```shell
$ tinytp --help
tinytp: help

Usage: tinytp [OPTIONS] COMMAND

A simple test prioritization tool.

Options:
	--db string 		Path to TinyTP database (default: tinytp.db)
	--output string		Directory where to put any output (except database) (default: current)
	--changes string	Path to file containing files in changeset (default: changeset.txt)
	--jenkins string	Path to Jenkins JSON report (default: test-report.json)
	--module		Prioritize at test module level

Commands:
	collect		Collect data and store into TinyTP database
	prio		Prioritize tests
```

## Example

TinyTP provides a simple command line program that has two commands, `tinytp collect` and `tinytp prio`.
These can be used to either (1) collect new test results and store them into the TinyTP database or (2) run a test
prioritization algorithm.

### Collecting and Storing Test Results

There might be different test report parsers in the future. As of now, TinyTP supports only the Jenkins JSON test report format.
For the sake of this example, we're going to create a dummy report first:
```shell
# Create sample Jenkins JSON test report
$ echo '{
    "suites": [
        {
            "name": "foo.bar.Suite1",
            "duration": 0.08,
            "cases": [
                {
                    "name": "Case1",
                    "duration": 0.01,
                    "status": "FAILED"
                },
                {
                    "name": "Case2",
                    "duration": 0.07,
                    "status": "PASSED"
                }
            ]
        },
        {
            "name": "foo.bar.Suite2",
            "duration": 0.1,
            "cases": [
                {
                    "name": "Case1",
                    "duration": 0.05,
                    "status": "FAILED"
                },
                {
                    "name": "Case2",
                    "duration": 0.05,
                    "status": "FAILED"
                }
            ]
        }
    ]
}' > test-report.json
```

Now we can run the `tinytp collect` command and store it into the database (default is `tinytp.db`):

```shell
$ tinytp collect --jenkins test-report.json --db tp.db
```

To check if the database content is correct, you can run the following command:
```shell
$ sqlite3 tp.db "select * from test_suite_execution;"
1|foo.bar.Suite1|foo.bar|1|2|1680716088707|0.08
2|foo.bar.Suite2|foo.bar|2|2|1680716088708|0.1
```

### Running the Prioritization

To run the prioritization, you can specify the algorithm as well as the granularity (i.e., test suite or module level):

```shell
$ tinytp prio --db tp.db [--module]
# Check created output file "tintytp.prio"
$ cat tinytp.prio
foo.bar.Suite2,0.666667
foo.bar.Suite1,0.416667
```

To also incorporate the changeset (e.g., in a pull request), pass the `--changes` option:

```shell
$ git diff --name-only HEAD^1 > changeset.txt
$ tinytp prio --db tp.db --changes changeset.txt
```

## Build

### CMake

Run the following commands in the root of the TinyTP repository to build all subprojects:

```shell
mkdir build
cd build
# (1) Generate build system using cmake
# Linux/Windows
cmake -DCMAKE_BUILD_TYPE=Debug ..
# [optional] Change generator and target arch on Windows
cmake -DCMAKE_BUILD_TYPE=Debug -G "Visual Studio 17 2022" -A x64 ..
# (2) Build BinaryRTS using generated build system (Win32: MSBuild, Linux/macOS: Unix Makefiles)
cmake --build . --config Debug
```

### Docker

There is a [`Dockerfile`](./Dockerfile) for dockerized builds.
For building the Docker image and creating a container with a new `bash` session, run:

```shell
# Build TinyTP docker image 
$ docker build -t tinytp:0.1 -f Dockerfile .
# Create and run container with new bash session and current working directory mounted into container
$ docker run -v $(pwd):/tinytp -it tinytp:0.1 bash
```

Then, you can run the `cmake` commands from above inside the `bash` session to build TinyTP.
