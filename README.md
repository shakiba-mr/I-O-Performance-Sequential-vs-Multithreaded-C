# I/O Performance: Sequential vs Multithreaded (C)

A C program comparing sequential and multithreaded I/O performance, 
developed as part of an Operating Systems course at Eskişehir Technical 
University.

## Overview

The experiment measures execution time of repetitive file I/O operations 
under two execution models:
- **Sequential** — files written and read one after another
- **Multithreaded** — workload distributed across 4 POSIX threads

## Configuration

| Parameter | Value |
|---|---|
| Files | 200 |
| File size | 4096 bytes |
| Buffer size | 64 bytes |
| Threads | 4 |

## Results

| Method | Time |
|---|---|
| Sequential | 0.0487s |
| Multithreaded | 0.0526s |
| Speedup | 0.92x |

## Key Finding

Multithreaded I/O was ~8% **slower** than sequential — the expected result 
for a buffered local filesystem. Thread creation overhead and syscall 
contention outweigh any parallelism benefit when the bottleneck is I/O, 
not CPU.

## Course

Operating Systems — Eskişehir Technical University, 2026
