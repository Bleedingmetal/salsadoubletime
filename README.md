
# About Salsa Double Time

This project accelerates Salsa20 by converting its single-threaded encryption path into a 16-way parallel pipeline. We began with Daniel J. Bernstein’s public-domain `salsa20.c` implementation and then created `salsa20_modified.c` to refactor `ECRYPT_encrypt_bytes()` around POSIX threads (`pthread.h`).

The original Salsa20 code is already compact and efficient, but it generates the keystream sequentially for the entire buffer. That sequential execution becomes the performance bottleneck on an 8-core/16-thread processor, because only one logical thread can produce keystream at a time.

Our approach partitions each plaintext buffer into 16 segments and assigns one thread per segment. Each thread computes a separate Salsa20 keystream for its segment and XORs that keystream with the plaintext. All threads are initialized with the same IV in this implementation, which is a known limitation of the current design.

### Original Execution
```
Stream cipher keystream generated sequentially for the full buffer
```

### Parallel Execution (Multithreaded Salsa Double Time)
```
| Thread 1: Seg 1 | Thread 2: Seg 2 | ... | Thread 16: Seg 16 |
| Keystream + XOR | Keystream + XOR | ... | Keystream + XOR |
```

Each thread runs a separate Salsa20 stream on its assigned buffer segment, increasing throughput by leveraging multiple threads.

## Usage

### Compile

The project includes a `Makefile` with the following build targets:

```bash
make original   # Build serial reference implementation
make modified   # Build Salsa Double Time (multithreaded version)
make clean      # Remove build artifacts
```

This code depends on POSIX threads, so it is intended for Linux systems with `pthread.h` support. The build system compiles using `gcc` and links all required ECRYPT header files and source modules.

### Configuration

To optimize for your specific hardware, modify the thread count macro in `salsa20_modified.c`:

```c
#define THREADS 16
```

Change this value to match the number of logical CPU cores available on your target system. The default value of 16 is optimized for 8-core/16-thread processors.

### Run the Benchmarks

Execute either implementation:

```bash
./original   # Run serial version
./modified   # Run multithreaded version
```

The program serves as both a testbench and a functional reference for initializing cryptographic contexts. It demonstrates the complete workflow: key setup, IV initialization, encryption, and decryption operations.

## Performance Analysis

### Testbench Design

The `main.c` testbench isolates the performance of the cipher by measuring only the time spent inside the encryption and decryption routines. Program overhead, initialization, and I/O are excluded from the reported timing.

| Parameter | Value |
|-----------|-------|
| Buffer Size | ~524 kB per iteration |
| Iterations | 1024 complete encrypt-decrypt cycles |
| Key Management | Unique randomized 256-bit key per iteration |
| Data | Unique randomized plaintext per iteration |
| Total Workload | ~536 MB encrypted and decrypted |


### Observed Performance

The reported results are based on our computers' performance. In two test cases on two separate machines, we observed the following example timings:

| Machine | Original | Modified |
|---------|----------|----------|
| Test Case 1 | 21054.673828 ms | 4830.660156 ms |
| Test Case 2 | 49295.230469 ms | 22293.113281 ms |

These values illustrate that the speedup varies by machine configuration and workload, but the multithreaded implementation consistently reduces overall cryptographic processing time significantly.

### Expected Results

Comparing the `original` and `modified` executables reveals the performance advantage of parallelization:

- **Original**: Sequential stream cipher processing
- **Modified**: Parallel multi-threaded processing across available cores

The degree of performance improvement depends on:
- Number of available CPU cores
- Processor frequency and cache hierarchy
- System load and thread scheduling
- Synchronization overhead

## Project Files

| File | Purpose |
|------|---------|
| `salsa20.c` | Original serial reference implementation by Daniel J. Bernstein |
| `salsa20_modified.c` | Multithreaded variant with POSIX thread parallelization |
| `main.c` | Performance testbench and functional example |
| `ecrypt-*.h` | ECRYPT interface definitions and machine-specific configurations |
| `Makefile` | Build automation |

## Authors 

- Team: Team E1 Project 1
- Course: CS 4801 Introduction To Cryptography And Communication Security
- Authors: Aditya Manoj Krishna, Jacob Boyle, Olivia Olsen, Alan Wang

## References

This project incorporates Salsa20 reference code by D. J. Bernstein, released into the public domain.


