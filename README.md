# Oil Compiler

>[!WARNING]
> This is not an actual compressor to compete with other compilers

## About the Oil Compiler

Just a compiler based on old Pokemon sprite compression.
It is a learning-focused project for me to retouch on C++.

## Goals for me

- [x] Learn C++ again
- [x] Retouch on file manipulation
- [x] Practice C++ debugging
- [x] Realize C++ is hell
- [ ] Makes dad come back
- [ ] Compresses your mom
- [ ] Made to compete with other compressors

## How to build

Firstly run `CMake`, then put the Build output to `out`.

```bash
cmake -B out
```

Then go to the out directory.

```bash
cd out
```

Finally do `make` to get the binary.

```bash
make
```

## Usage

You can compress by default:

```bash
oilc /path/to/file
```

Or decompress using **-d/--decompress**:

```bash
oilc -d /path/to/oil/file
```

Some optional flags are:
- **-o/--output** Specify an output.
- **-D/--debug** For debugging.

## Review

I can't believe C++ can be this functional.
Like I guess, from it's phrase "Zero-cost abstractions" may be a standpoint.
But I guess I couldn't see that clearly.

After seeing the linux boot code I took inspiration from that and used it
in files.cc. I kind of like that style of coding, if the label doesn't lie
you can just move on. It's really so simple!

In this point of time (01/22/2026), I can appreciate what C++ has to offer,
but still, I believe Rust can implement that (Cope).
