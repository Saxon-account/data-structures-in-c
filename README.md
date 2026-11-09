# Data Structures in C

A set of data structure implementations in C, each in its own
directory with a small test program. These are reorganized
implementations from CSEN 19 (Data Structures) coursework at Santa
Clara University, cleaned up into a single repo with a working build.

## Modules

### sets-array/
A set of strings stored in a plain array. The same header (`set.h`)
has two implementations:

- `sorted.c` keeps the elements in sorted order and searches with
  binary search.
- `unsorted.c` appends new elements at the end and searches linearly.

### hash-table-strings/
A set of strings stored in a hash table (`table.c`, same `set.h`
API as above). Collisions are handled with open addressing, and
removed slots are marked so lookups still work.

### hash-table-generic/
The same hash table idea, but for any element type. The caller
passes a compare function and a hash function to `createSet`, and
elements are stored as `void *` pointers.

### linked-list/
A doubly linked list with operations at both ends (`addFirst`,
`addLast`, `removeFirst`, `removeLast`) plus search and removal by
value. The directory also contains `set.c`, a generic hash-table
set whose buckets are chains of these lists.

### priority-queue/
A priority queue stored as a binary heap. `removeEntry` returns the
highest-priority entry, as decided by the compare function passed
to `createQueue` (with the usual convention, the smallest element
comes out first).

### huffman/
A Huffman coding program. It counts how often each byte occurs in a
file, builds a Huffman tree using the priority queue, and writes a
compressed file. `huffman.c` has the `main` function; `pack.c`
writes the compressed output. This one is a standalone program, not
a library, and is not part of the default build.

### hash-table-set/
A second generic set implementation. The table uses open addressing
with linear probing: on a collision, the next free slot is tried,
and deleted slots are marked so probe chains stay intact.

### apps/
Small driver programs that were written to exercise the libraries
above (word counting, duplicate finding, a maze solver, a radix
sort, and others). They are kept for reference and are not part of
the default build.

## Building and testing

You need `gcc` and `make`.

    make          # build all the test programs
    make test     # build and run them; each test prints PASS
    make clean    # remove the built test programs

Each test in `tests/` covers one module: it inserts, looks up, and
removes real elements and checks the counts and results with
`assert`.

## License

MIT — see `LICENSE`.
