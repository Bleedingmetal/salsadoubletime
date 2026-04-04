
# About Salsa Double Time

We implemented a multithreaded version of Salsa20 that leverages POSIX threads to parallelize the keystream generation process. While Bernstein's original Salsa20 code is highly efficient for single-core execution, we realized that the independent nature of Salsa20 blocks allows for simultaneous computation across multiple CPU cores.

Thus, we decided to approach the problem by parallelizing the `ECRYPT_encrypt_bytes()` function.

Instead of generating the keystream sequentially for the entire buffer, we partition the data into segments. On an 8-core/16-thread machine, we found that using 16 threads maximized throughput by saturating the available logical processors.

### Serial Execution (Original)
```
Block 1 -> Block 2 -> Block 3 -> ... -> Block N  [Sequential]
```

### Parallel Execution (Salsa Double Time)
```
| Thread 1:  Block 1   |  Thread 2:  Block 2   |  Thread 3:  Block 3   | ... | Thread 16: Block N   |
| Keystream Gen & XOR  |  Keystream Gen & XOR  |  Keystream Gen & XOR  | ... |  Keystream Gen & XOR |
```

Each thread independently generates its cipher keystream segment and performs the XOR operation with its assigned portion of the plaintext, enabling true parallel encryption across available processor cores.

## Usage

### Compile

The project includes a `Makefile` with the following build targets:

```bash
make original   # Build serial reference implementation
make modified   # Build Salsa Double Time (multithreaded version)
make clean      # Remove build artifacts
```

The build system compiles using `gcc` and links all required ECRYPT header files and source modules.

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

### Timing Output

The program prints the total cumulative time in milliseconds spent performing all encryption and decryption operations across all iterations:

```
Total time: 12345.678900ms
```

This represents pure cryptographic processing time and does not include:
- Cipher context initialization
- Memory allocation
- Random number generation
- Verification loops
- I/O operations

### Expected Results

Comparing the `original` and `modified` executables reveals the performance advantage of parallelization:

- **Original**: Sequential block-by-block processing
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

## License

This project incorporates Salsa20 reference code by D. J. Bernstein, released into the public domain.


