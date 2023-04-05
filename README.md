# TinyTP

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

## Example

TinyTP provides a simple command line program that has two commands, `tinytp collect` and `tinytp prio`.
These can be used to either (1) collect new test results and store them into the TinyTP database or (2) run a test
prioritization algorithm.

```shell
$ ...
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
