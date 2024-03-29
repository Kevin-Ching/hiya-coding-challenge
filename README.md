# Hiya Coding Challenge

## Prerequisites

- Linux environment

As this project was made with WSL, the following instructions assume a Linux environment. 
If you don't already have them, you can install essential build tools and CMake as follows: 
```bash
sudo apt-get update
sudo apt-get install build-essential
sudo apt-get install cmake
```

## Getting Started

1. Clone the repository.
2. Navigate to the project directory.

## Configure and Compile

Configure and compile the project into the `build` directory as follows: 
```bash
cmake -S . -B build
cmake --build build
```

## Run Input Files

Input files can now be run with: 
```bash
./build/short_callers [input file]
```

The $-a$ flag can be set to see all callers' average call duration:
```bash
./build/short_callers -a [input file]
```

The `inputs` directory contains input files that can be run. For example: 
```bash
./build/short_callers inputs/example.txt
```
should output 
```bash
[Bob]
```
and 
```bash
./build/short_callers -a inputs/example.txt
```
should output 
```bash
Caller: Bob,    Average: 2.5
Caller: Carl,   Average: 17
Caller: Ed,     Average: 20

[Bob]
```
