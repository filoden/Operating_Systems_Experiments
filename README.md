# Operating Systems Experiments

A collection of C and shell-based operating systems experiments. The repository explores file parsing, multithreaded producer/consumer design, interprocess communication, and filesystem I/O benchmarking.

## Overview

This repository contains three main project areas:

| Directory                     | Description                                                                                                              |
| ----------------------------- | ------------------------------------------------------------------------------------------------------------------------ |
| `SingleThreaded_FIle_Parser/` | Single-threaded word-frequency parser that compares two text files and outputs common words above a frequency threshold. |
| `MultiThreaded_File_Parser/`  | Mapper/reducer-style parser using processes, pthreads, semaphores, and System V message queues.                          |
| `IO Experiments/`             | Filesystem and I/O experiments for creating files, mounting loopback filesystems, and measuring random-read latency.     |

## Topics Covered

* C programming on Linux
* File I/O using POSIX system calls
* Dynamic memory management
* Linked-list data structures
* Word-frequency counting
* Process creation with `fork()`
* Multithreading with `pthread`
* Producer/consumer synchronization
* POSIX semaphores
* System V message queues
* Shell scripting for automated tests
* Filesystem experiments with FAT32, ext4, XFS, and F2FS
* Random I/O latency measurement

## Requirements

These projects are intended to run in a Linux environment.

Recommended tools:

* `gcc`
* `make`
* `bash` / `sh`
* POSIX threads support
* Standard Linux development headers

Some I/O experiment scripts also require administrative privileges and filesystem utilities such as:

* `losetup`
* `mkfs.fat`
* `mkfs.ext4`
* `mkfs.xfs`
* `mkfs.f2fs`
* `mount`
* `umount`

> **Note:** Some filesystem experiment scripts use `sudo`, loopback devices, and local user/group names. Review and modify these scripts before running them on your own machine.

## Project 1: Single-Threaded File Parser

Location:

```bash
SingleThreaded_FIle_Parser/
```

This project builds a single-threaded parser that compares two text files and finds words that appear in both files at or above a specified threshold.

The program:

1. Reads two input text files.
2. Tokenizes each file by whitespace.
3. Builds sorted doubly linked lists of unique words.
4. Counts word frequency per file.
5. Compares the two files.
6. Outputs common words whose counts meet the threshold in both files.
7. Sorts the result by combined frequency.

### Build

```bash
cd SingleThreaded_FIle_Parser
make
```

### Usage

```bash
./common <file1> <file2> <threshold> <output_file>
```

Example:

```bash
./common files/THEGODFATHER.txt files/THEGODFATHER2.txt 50 student_out_50.txt
```

### Run Tests

```bash
./test.sh
```

The test script builds the program, runs several threshold cases, and compares the generated output against known-correct output files.

## Project 2: Multi-Threaded File Parser

Location:

```bash
MultiThreaded_File_Parser/
```

This project expands the parsing problem into a mapper/reducer-style workflow.

The system uses:

* A command file containing `map <directory>` commands
* A mapper process for each mapped directory
* A thread for each `.txt` file found in a directory
* A bounded buffer protected by semaphores
* System V message queues to pass mapper output to the reducer
* A reducer that combines duplicate words and sorts final frequencies

### Build

```bash
cd MultiThreaded_File_Parser
make
```

This builds:

```bash
mapper
reducer
```

### Basic Usage

Start the mapper first:

```bash
./mapper commandFile1.txt 10 &
```

Then start the reducer:

```bash
./reducer student_out1.txt 1
```

Arguments:

```bash
./mapper <command_file> <buffer_size>
./reducer <output_file> <minimum_frequency>
```

### Run Tests

```bash
./test.sh
```

The test script runs multiple mapper/reducer test cases and compares the output files against the expected results in `correct_output/`.

## Project 3: I/O Experiments

Location:

```bash
IO Experiments/
```

This project contains a small C utility, `myio`, and supporting shell scripts for experimenting with file creation, loopback filesystems, and random-read latency.

The `myio` program has two modes:

| Mode | Description                                                                      |
| ---- | -------------------------------------------------------------------------------- |
| `1`  | Create or truncate a file and write zeroed 4096-byte blocks.                     |
| `2`  | Perform random block-aligned reads and print the median latency in microseconds. |

### Build

```bash
cd "IO Experiments"
make
```

### File Creation Mode

```bash
./myio 1 <filename> <number_of_4096_byte_blocks>
```

Example:

```bash
./myio 1 testfile 1000
```

### Random I/O Timing Mode

```bash
./myio 2 <filename> <io_size_bytes> <samples>
```

Example:

```bash
./myio 2 testfile 4096 2000
```

The program seeks to random block-aligned locations, performs reads of the requested size, records latency, sorts the samples, and prints the median result.

### Experiment Scripts

The directory includes several scripts for creating and testing different filesystems:

| Script     | Purpose                                                        |
| ---------- | -------------------------------------------------------------- |
| `ex1.sh`   | Basic file creation and copy comparison using `dd` and `diff`. |
| `ex2.sh`   | FAT32 loopback filesystem experiment.                          |
| `ex3.sh`   | ext4 loopback filesystem experiment.                           |
| `ex4.sh`   | XFS loopback filesystem experiment.                            |
| `ex5.sh`   | F2FS loopback filesystem experiment.                           |
| `ex6_1.sh` | Random I/O timing tests over multiple I/O sizes.               |

> **Warning:** Review these scripts before running them. Some scripts create loop devices, mount filesystems, clear caches, and require root privileges.

## Cleaning Build Files

Each project directory includes a `Makefile` with a clean target.

Example:

```bash
make clean
```

## Repository Notes

This repository is primarily intended as a learning and reference collection for operating systems concepts. Some generated files, object files, binaries, and test outputs may be present from previous runs.

For a clean rebuild, run:

```bash
make clean
make
```

inside the relevant project directory.

## Academic Use

This repository is provided for reference and portfolio purposes. Do not submit this work as your own.

